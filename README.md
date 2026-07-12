# 🌉 Setu
### AI-Powered Environmental Awareness System for Deaf and Hard-of-Hearing Individuals

<p align="center">
  <img src=images/WhatsApp Image 2026-07-12 at 10.58.32(1).jpeg alt="SETU Logo" width="300">
</p>

> **Setu** (Sanskrit for *Bridge*) is an AI-powered assistive technology that bridges the gap between sound and awareness by converting critical environmental sounds into accessible visual and haptic alerts.

---

# 📖 Overview

People who are Deaf or Hard-of-Hearing often miss important environmental sounds such as ambulance sirens, vehicle horns, and attention calls. These missed auditory cues can compromise safety, independence, and everyday communication.

**Setu** continuously listens to the surrounding environment using an ESP32-based embedded system and an Edge Impulse TinyML model. When a critical sound is detected, it instantly converts it into accessible alerts through an OLED display, RGB LED, haptic feedback, and a companion web dashboard.

Rather than replacing hearing, Setu enhances environmental awareness, enabling users to respond confidently to important situations.

---

# ✨ Features

- 🎧 Real-time environmental sound recognition
- 🤖 TinyML inference using Edge Impulse
- 📟 OLED visual alerts
- 🔴 RGB LED severity indication
- 🤝 Haptic feedback (servo prototype)
- 🌐 Live web dashboard
- 📜 Alert history
- 📊 Confidence score display
- ♿ Accessibility-focused interface
- ⚡ Low-cost embedded implementation

---

# 🚨 Supported Sounds

| Sound | Alert |
|--------|-------|
| 🚑 Ambulance Siren | Emergency Alert |
| 🚗 Vehicle Horn | Caution Alert |
| 👤 Attention Call | Someone is Calling You |

---

# 🛠 Hardware

- ESP32 Dev Module
- Electret Microphone Module
- SSD1306 OLED Display (I2C)
- RGB LED
- Servo Motor (Haptic Prototype)
- Breadboard
- Jumper Wires
- USB Power / Li-ion Battery

---

# 💻 Software Stack

- Arduino IDE
- Edge Impulse
- Flask
- Python
- HTML
- CSS
- JavaScript

---

# 🧠 System Architecture

```
        Environmental Sound
                │
                ▼
      Electret Microphone
                │
                ▼
        ESP32 Microcontroller
                │
                ▼
    Edge Impulse TinyML Model
                │
        Sound Classification
                │
        ┌───────┴────────┐
        ▼                ▼
 OLED Display      Flask Dashboard
        │
        ▼
 RGB LED + Haptic Feedback
```

---

# 🔄 Workflow

```
Listen
   │
   ▼
Capture Audio
   │
   ▼
Run TinyML Inference
   │
   ▼
Classify Sound
   │
   ▼
Generate Alert
   │
 ┌─┴───────────────┐
 ▼                 ▼
OLED          Dashboard
 │
 ▼
RGB LED
 │
 ▼
Servo Feedback
```

---

# 📁 Project Structure

```
Setu/
│
├── firmware/
│   └── setu.ino
│
├── dashboard/
│   ├── app.py
│   ├── templates/
│   └── static/
│
├── hardware/
│   ├── wiring.png
│   ├── circuit.png
│   └── components.md
│
├── docs/
│   ├── architecture.png
│   ├── presentation.pdf
│   └── demo.mp4
│
├── images/
│   ├── dashboard.png
│   ├── prototype.jpg
│   └── logo.png
│
└── README.md
```

---

# 🚀 Installation

## Dashboard

Clone the repository

```bash
git clone https://github.com/<username>/Setu.git
cd Setu/dashboard
```

Install dependencies

```bash
pip install -r requirements.txt
```

Run

```bash
python app.py
```

Open

```
http://localhost:5000
```

---

## Firmware

1. Install ESP32 Board Package in Arduino IDE.
2. Install required Arduino libraries:
   - Adafruit GFX
   - Adafruit SSD1306
   - ESP32Servo
3. Add the exported Edge Impulse Arduino library.
4. Upload `firmware/setu.ino`.

---

# 📷 Demo

## Prototype

*(Insert prototype image)*

## Dashboard

*(Insert dashboard screenshot)*

## OLED Alerts

*(Insert OLED images)*

---

# 🔬 Machine Learning

- Platform: Edge Impulse
- Deployment: Arduino Library
- Inference: On-device (ESP32)
- Model Type: Audio Classification

---

# 🌍 Accessibility Impact

Setu improves:

- Personal Safety
- Environmental Awareness
- Independence
- Inclusive Communication

Instead of relying on others to interpret surrounding sounds, users receive immediate visual and haptic cues that enable them to make informed decisions.

---

# 🚀 Future Improvements

- Dedicated vibration motor
- Mobile application
- Battery monitoring
- Additional environmental sounds
- User-customizable sound profiles
- OTA firmware updates

---

# 👥 Team

| Member | Responsibility |
|----------|----------------|
| Member 1 | TinyML Model & Edge Impulse |
| Member 2 | Firmware & ESP32 Integration |
| Member 3 | Hardware & Embedded Design |
| Member 4 | Dashboard & Presentation |

---

# 🏆 Built For

**Starlet 5.0 Hackathon**

Theme:
> Accessibility • Inclusion • Independence

---

# 📄 License

This project is released under the MIT License.

---

# ❤️ Acknowledgements

- Edge Impulse
- ESP32 Community
- Flask
- Adafruit Libraries
- Starlet 5.0 Organizers

---

## ⭐ Our Vision

> **Setu transforms inaccessible auditory information into meaningful, actionable awareness—empowering Deaf and Hard-of-Hearing individuals to navigate the world with greater confidence, independence, and safety.**
