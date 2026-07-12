"""
Setu dashboard backend.
Receives telemetry from the ESP32 wearable, holds session state in memory,
and serves the responder-facing dashboard the judge/responder reaches by
scanning the QR sticker on the wearable.

Run: python app.py
"""

from flask import Flask, request, jsonify, render_template
from datetime import datetime

app = Flask(__name__)

# Predefined communication protocol cards for the demo profile.
# In a real build this would be per-user, loaded from a profile store.
DEFAULT_CARDS = [
    "I need a moment before I respond.",
    "I have autism. Please ask one question at a time.",
    "Please avoid sudden touch or loud noise.",
    "Wait about 10 seconds after asking — I may need time to answer.",
]

# In-memory session store: { device_id: {...} }
sessions = {}


def get_session(device_id):
    if device_id not in sessions:
        sessions[device_id] = {
            "state": "idle",       # idle | active | escalated
            "card_index": 0,
            "cards": DEFAULT_CARDS,
            "escalated": False,
            "last_event": None,
            "last_update": None,
            "log": [],
        }
    return sessions[device_id]


@app.route("/telemetry", methods=["POST"])
def telemetry():
    data = request.get_json(force=True)
    device_id = data.get("device_id", "setu-01")
    event = data.get("event", "unknown")
    state = data.get("state", "idle")

    session = get_session(device_id)
    session["state"] = state
    session["last_event"] = event
    session["last_update"] = datetime.utcnow().isoformat()

    if event in ("manual_hold", "tilt"):
        session["escalated"] = True

    if event == "manual_trigger":
        session["card_index"] = 0

    if event not in ("heartbeat",):
        session["log"].append({"event": event, "at": session["last_update"]})

    return jsonify({"ok": True})


@app.route("/session/<device_id>")
def session_view(device_id):
    get_session(device_id)  # ensure it exists
    return render_template("index.html", device_id=device_id)


@app.route("/api/state/<device_id>")
def api_state(device_id):
    session = get_session(device_id)
    return jsonify(session)


@app.route("/api/next/<device_id>", methods=["POST"])
def api_next(device_id):
    session = get_session(device_id)
    if session["card_index"] + 1 < len(session["cards"]):
        session["card_index"] += 1
    return jsonify(session)


@app.route("/api/reset/<device_id>", methods=["POST"])
def api_reset(device_id):
    sessions[device_id] = {
        "state": "idle",
        "card_index": 0,
        "cards": DEFAULT_CARDS,
        "escalated": False,
        "last_event": None,
        "last_update": None,
        "log": [],
    }
    return jsonify(sessions[device_id])


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
