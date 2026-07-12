/*
  Setu — wearable firmware
  Fuses three trigger paths (manual button, sustained loud sound, sudden tilt)
  into one session state machine, drives an LCD16x2 + buzzer/servo locally,
  and syncs session state to the dashboard over WiFi without blocking local output.

  Libraries needed (Library Manager):
    - LiquidCrystal_I2C (any maintained fork, e.g. by Frank de Brabander)
    - ESP32Servo (if using a servo as the haptic prototype)
  Board: ESP32 Dev Module
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// ---------- WiFi / backend config ----------
const char* WIFI_SSID     = "YOUR_HOTSPOT_SSID";
const char* WIFI_PASSWORD = "YOUR_HOTSPOT_PASSWORD";
const char* BACKEND_URL   = "http://192.168.x.x:5000/telemetry"; // Flask endpoint
const char* DEVICE_ID     = "setu-01";

// ---------- Pins ----------
const int PIN_BUTTON = 19;      // active-low, INPUT_PULLUP
const int PIN_MIC    = 34;      // ADC, input-only
const int PIN_TILT   = 33;      // digital
const int PIN_SERVO  = 18;      // haptic prototype (or swap for a buzzer + digitalWrite)

LiquidCrystal_I2C lcd(0x27, 16, 2); // adjust address if your backpack differs
Servo hapticServo;

// ---------- Thresholds — tune on-site ----------
const int   MIC_THRESHOLD        = 2200;  // raw ADC, calibrate against venue noise
const unsigned long MIC_SUSTAIN_MS = 2500; // how long loud sound must persist to auto-trigger
const unsigned long LONG_PRESS_MS  = 1200; // hold duration that counts as escalation
const unsigned long DEBOUNCE_MS    = 50;

// ---------- Cards shown on the wearer's own LCD ----------
const char* CARDS[] = {
  "I need a moment",
  "before I respond.",
  "I have autism.",
  "One Q at a time pls",
  "Avoid sudden touch",
  "or loud noise.",
  "Please wait ~10s",
  "after asking."
};
const int CARD_COUNT = 8; // shown two lines at a time, advances in pairs

// ---------- State ----------
enum SessionState { IDLE, ACTIVE, ESCALATED };
SessionState state = IDLE;
int cardPage = 0;

unsigned long buttonDownAt = 0;
bool buttonWasDown = false;

unsigned long micLoudSince = 0;
bool micCurrentlyLoud = false;

unsigned long lastTelemetryPost = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_TILT, INPUT);

  lcd.init();
  lcd.backlight();
  showIdle();

  hapticServo.attach(PIN_SERVO);
  hapticServo.write(0);

  connectWiFi();
}

void loop() {
  unsigned long now = millis();

  handleButton(now);
  handleMic(now);
  handleTilt(now);

  if (WiFi.status() != WL_CONNECTED) {
    static unsigned long lastReconnect = 0;
    if (now - lastReconnect > 5000) {
      lastReconnect = now;
      WiFi.reconnect();
    }
  }

  if (now - lastTelemetryPost >= TELEMETRY_INTERVAL_MS) {
    lastTelemetryPost = now;
    postTelemetry("heartbeat");
  }
}

// ---------- Trigger handlers ----------

void handleButton(unsigned long now) {
  bool isDown = digitalRead(PIN_BUTTON) == LOW;

  if (isDown && !buttonWasDown) {
    buttonDownAt = now;
    buttonWasDown = true;
  }

  if (!isDown && buttonWasDown) {
    unsigned long heldFor = now - buttonDownAt;
    buttonWasDown = false;
    if (heldFor >= DEBOUNCE_MS && heldFor < LONG_PRESS_MS) {
      onManualTrigger();
    }
  }

  // Long-hold escalation fires while still held, once threshold crossed
  if (isDown && buttonWasDown && (now - buttonDownAt >= LONG_PRESS_MS) && state != ESCALATED) {
    onEscalate("manual_hold");
  }
}

void handleMic(unsigned long now) {
  int level = analogRead(PIN_MIC);
  bool loud = level > MIC_THRESHOLD;

  if (loud && !micCurrentlyLoud) {
    micLoudSince = now;
    micCurrentlyLoud = true;
  } else if (!loud) {
    micCurrentlyLoud = false;
  }

  if (micCurrentlyLoud && (now - micLoudSince >= MIC_SUSTAIN_MS) && state == IDLE) {
    onAutoPrompt();
    micCurrentlyLoud = false; // avoid repeated re-fire
  }
}

void handleTilt(unsigned long now) {
  static bool lastTilt = false;
  static unsigned long tiltChangedAt = 0;
  bool tilted = digitalRead(PIN_TILT) == HIGH;

  if (tilted != lastTilt) {
    tiltChangedAt = now;
    lastTilt = tilted;
  }
  if (tilted && (now - tiltChangedAt > 150) && state != ESCALATED) {
    onEscalate("tilt");
  }
}

// ---------- State transitions ----------

void onManualTrigger() {
  state = ACTIVE;
  cardPage = 0;
  showCardPage();
  hapticPulse(200);
  postTelemetry("manual_trigger");
}

void onAutoPrompt() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Communication");
  lcd.setCursor(0, 1);
  lcd.print("mode available?");
  hapticPulse(120);
  postTelemetry("auto_prompt");
  // Wearer confirms with a short press within a few seconds to actually enter ACTIVE
}

void onEscalate(const char* source) {
  state = ESCALATED;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alert sent to");
  lcd.setCursor(0, 1);
  lcd.print("your contact.");
  hapticPulse(400);
  postTelemetry(source); // firmware sends the source string, backend maps it to "escalated"
}

void showIdle() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setu");
  lcd.setCursor(0, 1);
  lcd.print("Ready");
}

void showCardPage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(CARDS[cardPage]);
  lcd.setCursor(0, 1);
  if (cardPage + 1 < CARD_COUNT) lcd.print(CARDS[cardPage + 1]);
}

void hapticPulse(int durationMs) {
  hapticServo.write(90);
  delay(durationMs); // short, intentional — acceptable for a discrete confirm pulse
  hapticServo.write(0);
}

// ---------- Networking ----------

void postTelemetry(const char* eventType) {
  if (WiFi.status() != WL_CONNECTED) return; // local LCD/buzzer already fired regardless

  HTTPClient http;
  http.begin(BACKEND_URL);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(2000);

  String stateStr = state == IDLE ? "idle" : state == ACTIVE ? "active" : "escalated";
  String payload = String("{\"device_id\":\"") + DEVICE_ID +
                    "\",\"event\":\"" + eventType +
                    "\",\"state\":\"" + stateStr +
                    "\",\"uptime_ms\":" + millis() + "}";

  http.POST(payload);
  http.end();
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(250);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nWiFi connected" : "\nWiFi failed, will retry in loop()");
}
