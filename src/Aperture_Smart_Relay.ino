#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "TVOJE_WIFI_NAZEV"; 
const char* password = "TVOJE_HESLO";

ESP8266WebServer server(80);

const int relayPins[3] = { D1, D2, D3 };
bool relayState[3] = { false, false, false };

String jsonState() {
  String s = "{";
  for (int i = 0; i < 3; ++i) {
    s += "\"relay";
    s += String(i+1);
    s += "\":";
    s += (relayState[i] ? "true" : "false");
    if (i < 2) s += ",";
  }
  s += "}";
  return s;
}

void handleRoot() {
  String html = R"rawliteral(
<!doctype html>
<html lang="cs">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Ovládání zásuvek</title>
<link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600;700&display=swap" rel="stylesheet">
<style>
:root{
  --bg-1:#07101a;
  --bg-2:#071a2a;
  --muted:#9aa6b2;
  --accent:#00b0ff;
  --ok:#1fbf6b;
  --err:#ff6b6b;
  --switch-w:120px;
  --switch-h:40px;
  --knob-size:34px;
  --pad:3px;
}
*{box-sizing:border-box}
html,body{height:100%}
body{margin:0;font-family:Inter,system-ui,Segoe UI,Roboto,Arial;background:linear-gradient(180deg,var(--bg-1) 0%, var(--bg-2) 100%);color:#e6f0f6;display:flex;align-items:center;justify-content:center;padding:20px}
.wrapper{width:100%;max-width:980px}
.header{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-bottom:16px}
.brand{display:flex;align-items:center;gap:12px}
.logo{width:48px;height:48px;border-radius:10px;background:linear-gradient(135deg,var(--accent),#6be0ff);display:flex;align-items:center;justify-content:center;font-weight:700;color:#04202a;box-shadow:0 8px 24px rgba(0,176,255,0.10)}
.title{font-size:18px;font-weight:600}
.subtitle{font-size:13px;color:var(--muted)}
.net{font-size:13px;color:var(--muted);text-align:right}
.grid{display:grid;grid-template-columns:repeat(3,1fr);gap:12px}
.card{background:linear-gradient(180deg,rgba(255,255,255,0.02),rgba(255,255,255,0.01));border-radius:12px;padding:14px;border:1px solid rgba(255,255,255,0.03);backdrop-filter:blur(6px);box-shadow:0 8px 24px rgba(2,8,18,0.6)}
.device-title{font-size:15px;margin:0 0 8px 0;color:#dff6ff}
.row{display:flex;align-items:center;justify-content:space-between;gap:12px}
.info{display:flex;flex-direction:column;min-width:0}
.small{font-size:12px;color:var(--muted)}
.state{font-weight:700;font-size:15px;margin-top:6px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.switch{
  width:var(--switch-w);
  height:var(--switch-h);
  position:relative;
  border-radius:999px;
  background:linear-gradient(90deg, rgba(255,255,255,0.02), rgba(255,255,255,0.01));
  padding:var(--pad);
  display:block;
  cursor:pointer;
  border:1px solid rgba(255,255,255,0.04);
  flex:0 0 auto;
  -webkit-tap-highlight-color: transparent;
}
.switch-track{
  position:absolute;
  inset:var(--pad);
  border-radius:999px;
  background:transparent;
  display:block;
}
.knob{
  position:absolute;
  top:50%;
  left:var(--pad);
  width:var(--knob-size);
  height:var(--knob-size);
  transform:translateY(-50%);
  border-radius:50%;
  background:linear-gradient(180deg,#ffffff,#dff6ff);
  box-shadow:0 6px 18px rgba(2,8,18,0.6);
  transition:left .20s cubic-bezier(.2,.9,.3,1), box-shadow .18s;
  z-index:2;
  border:1px solid rgba(0,0,0,0.08);
}
.switch.on{
  background:linear-gradient(90deg, rgba(0,176,255,0.12), rgba(31,191,107,0.06));
  box-shadow:0 10px 30px rgba(0,176,255,0.06);
}
.switch.on .knob{
  left: calc(100% - var(--knob-size) - var(--pad));
  box-shadow:0 12px 36px rgba(0,176,255,0.18);
}
.hint{font-size:12px;color:var(--muted);margin-top:8px}
.footer{margin-top:12px;color:var(--muted);font-size:13px;text-align:center}
@media (max-width:880px){.grid{grid-template-columns:repeat(2,1fr)}}
@media (max-width:560px){.grid{grid-template-columns:1fr}.header{flex-direction:column;align-items:flex-start;gap:8px}.net{text-align:left}}
</style>
</head>
<body>
<div class="wrapper">
  <div class="header">
    <div class="brand">
      <div class="logo">R</div>
      <div>
        <div class="title">Ovládání zásuvek</div>
        <div class="subtitle">Rychlé a bezpečné ovládání tří relé</div>
      </div>
    </div>
    <div class="net" id="net">—</div>
  </div>

  <div class="grid">
    <div class="card">
      <h3 class="device-title">Zásuvka 1</h3>
      <div class="row">
        <div class="info">
          <div class="small">Stav</div>
          <div id="label1" class="state">—</div>
        </div>
        <div id="sw1" class="switch" role="button" tabindex="0" aria-pressed="false">
          <span class="switch-track"></span>
          <span class="knob"></span>
        </div>
      </div>
    </div>

    <div class="card">
      <h3 class="device-title">Zásuvka 2</h3>
      <div class="row">
        <div class="info">
          <div class="small">Stav</div>
          <div id="label2" class="state">—</div>
        </div>
        <div id="sw2" class="switch" role="button" tabindex="0" aria-pressed="false">
          <span class="switch-track"></span>
          <span class="knob"></span>
        </div>
      </div>
    </div>

    <div class="card">
      <h3 class="device-title">Zásuvka 3</h3>
      <div class="row">
        <div class="info">
          <div class="small">Stav</div>
          <div id="label3" class="state">—</div>
        </div>
        <div id="sw3" class="switch" role="button" tabindex="0" aria-pressed="false">
          <span class="switch-track"></span>
          <span class="knob"></span>
        </div>
      </div>
      <div class="hint">Interval aktualizace: 3 s</div>
    </div>
  </div>

  <div class="footer">Verze rozhraní 1.0 • Připojte se k zařízení pro ovládání</div>
</div>

<script>
const sw = [document.getElementById('sw1'),document.getElementById('sw2'),document.getElementById('sw3')];
const lbl = [document.getElementById('label1'),document.getElementById('label2'),document.getElementById('label3')];
const net = document.getElementById('net');

function apply(i, state){
  if(state){
    sw[i].classList.add('on');
    sw[i].setAttribute('aria-pressed','true');
    lbl[i].textContent = '● Zapnuto';
    lbl[i].style.color = getComputedStyle(document.documentElement).getPropertyValue('--ok') || '#1fbf6b';
  } else {
    sw[i].classList.remove('on');
    sw[i].setAttribute('aria-pressed','false');
    lbl[i].textContent = '○ Vypnuto';
    lbl[i].style.color = getComputedStyle(document.documentElement).getPropertyValue('--err') || '#ff6b6b';
  }
}

async function fetchState(){
  try{
    const r = await fetch('/state');
    if(!r.ok) throw 0;
    const j = await r.json();
    apply(0, !!j.relay1);
    apply(1, !!j.relay2);
    apply(2, !!j.relay3);
    const ss = j.ssid || '';
    const ip = j.ip || '';
    net.textContent = ss ? ss + ' • ' + ip : ip || 'Nepřipojeno';
  }catch(e){
    net.textContent = 'Offline';
    apply(0,false); apply(1,false); apply(2,false);
  }
}

async function toggle(i){
  try{
    const res = await fetch('/toggle' + (i+1), { method:'POST' });
    if(!res.ok) return;
    const j = await res.json();
    apply(i, !!j['relay' + (i+1)]);
  }catch(e){}
}

sw.forEach((el, idx) => {
  el.addEventListener('click', ()=> toggle(idx));
  el.addEventListener('keydown', (e)=> { if(e.key==='Enter' || e.key===' ') { e.preventDefault(); toggle(idx); } });
});

fetchState();
setInterval(fetchState, 3000);
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleState() {
  String s = "{";
  s += "\"relay1\":"; s += (relayState[0] ? "true" : "false"); s += ",";
  s += "\"relay2\":"; s += (relayState[1] ? "true" : "false"); s += ",";
  s += "\"relay3\":"; s += (relayState[2] ? "true" : "false"); s += ",";
  s += "\"ssid\":\""; s += WiFi.SSID(); s += "\",";
  s += "\"ip\":\""; s += WiFi.localIP().toString(); s += "\"";
  s += "}";
  server.send(200, "application/json", s);
}

void handleToggle(int idx) {
  relayState[idx] = !relayState[idx];
  digitalWrite(relayPins[idx], relayState[idx] ? LOW : HIGH);
  server.send(200, "application/json", jsonState());
}

void handleToggle1() { handleToggle(0); }
void handleToggle2() { handleToggle(1); }
void handleToggle3() { handleToggle(2); }

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; ++i) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH);
  }

  WiFi.begin(ssid, password);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(300);
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/state", HTTP_GET, handleState);
  server.on("/toggle1", HTTP_POST, handleToggle1);
  server.on("/toggle2", HTTP_POST, handleToggle2);
  server.on("/toggle3", HTTP_POST, handleToggle3);
  server.on("/toggle1", HTTP_GET, handleToggle1);
  server.on("/toggle2", HTTP_GET, handleToggle2);
  server.on("/toggle3", HTTP_GET, handleToggle3);
  server.begin();
}

void loop() {
  server.handleClient();
}
