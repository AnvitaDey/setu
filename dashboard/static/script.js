const deviceId = document.body.dataset.deviceId;
const statusBadge = document.getElementById("status-badge");
const escalationBanner = document.getElementById("escalation-banner");
const cardText = document.getElementById("card-text");
const nextBtn = document.getElementById("next-btn");
const logList = document.getElementById("log-list");

async function fetchState() {
  try {
    const res = await fetch(`/api/state/${deviceId}`);
    const data = await res.json();
    render(data);
  } catch (e) {
    console.error("Failed to fetch state", e);
  }
}

function render(data) {
  statusBadge.textContent = data.state.charAt(0).toUpperCase() + data.state.slice(1);
  statusBadge.className = `badge ${data.state}`;

  escalationBanner.classList.toggle("hidden", !data.escalated);

  if (data.state === "idle") {
    cardText.textContent = "Waiting for session to begin…";
  } else {
    cardText.textContent = data.cards[data.card_index] || "";
  }

  logList.innerHTML = "";
  [...data.log].reverse().forEach((entry) => {
    const li = document.createElement("li");
    const time = entry.at ? new Date(entry.at).toLocaleTimeString() : "";
    li.textContent = `${time} — ${entry.event}`;
    logList.appendChild(li);
  });
}

nextBtn.addEventListener("click", async () => {
  const res = await fetch(`/api/next/${deviceId}`, { method: "POST" });
  const data = await res.json();
  render(data);
});

fetchState();
setInterval(fetchState, 1000);
