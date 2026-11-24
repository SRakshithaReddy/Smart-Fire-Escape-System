// ============================================================
// SMART FIRE ESCAPE SYSTEM
// ESP8266 + ESPAsyncWebServer + Live Web Dashboard
// ============================================================

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3wJhchs_s"
#define BLYNK_TEMPLATE_NAME "IOT Fire Escape Guidance System"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#define WIFI_SSID "YOUR_WIFI_HOTSPOT_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ---------------- WIFI ----------------
const char* ssid = "ESPTEST";
const char* password = "12345678";

// ---------------- BLYNK EVENT ----------------
// IMPORTANT: This must exactly match the Event Code shown in
// Blynk Console -> Template -> Events & Notifications.
// If your event code is different, change ONLY this line.
#define BLYNK_EVENT_CODE "smart_fire_escape"

// ---------------- SENSOR PINS ----------------
const int smokePin = A0;
const int flamePin = D7;
const int pirPin   = D6;

// ---------------- OUTPUT PINS ----------------
const int greenLED  = D5;
const int redLED    = D4;
const int buzzerPin = D2;

// ---------------- SETTINGS ----------------
const int smokeThreshold = 800;

// Create web server
AsyncWebServer server(80);

// Server-Sent Events for live updates
AsyncEventSource events("/events");

// Current sensor values
int smokeValue = 0;
bool smokeDetected = false;
bool flameDetected = false;
bool motionDetected = false;
bool fireDetected = false;

bool fireNotificationSent = false;
unsigned long lastBlynkConnectAttempt = 0;
const unsigned long blynkReconnectInterval = 10000;

// Timing
unsigned long previousSensorMillis = 0;
const unsigned long sensorInterval = 50;


// ============================================================
// WEB PAGE
// ============================================================

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="UTF-8">

<meta name="viewport"
content="width=device-width, initial-scale=1.0">

<title>Smart Fire Escape System</title>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Segoe UI,Arial,sans-serif;
}

body{

background:
linear-gradient(
135deg,
#08131f,
#0b2238,
#112d4e);

background-size:400% 400%;

animation:bgMove 18s ease infinite;

color:white;

min-height:100vh;

overflow-x:hidden;

}

@keyframes bgMove{

0%{background-position:0% 50%;}

50%{background-position:100% 50%;}

100%{background-position:0% 50%;}

}

header{

padding:25px;

background:rgba(0,0,0,.35);

backdrop-filter:blur(12px);

box-shadow:0 5px 20px rgba(0,0,0,.4);

}

header h1{

font-size:34px;

letter-spacing:1px;

}

header p{

margin-top:8px;

color:#9fb8d8;

}

.topBar{

display:flex;

justify-content:space-between;

margin-top:20px;

flex-wrap:wrap;

}

.statusBadge{

background:#0f5132;

padding:10px 18px;

border-radius:40px;

font-weight:bold;

box-shadow:0 0 18px #00ff6a55;

}

.clock{

font-size:18px;

font-weight:bold;

color:#8fd7ff;

}

.container{

width:95%;

max-width:1400px;

margin:25px auto;

}

.mainStatus{

padding:25px;

border-radius:18px;

background:#16a34a;

font-size:32px;

font-weight:bold;

text-align:center;

transition:.5s;

box-shadow:0 0 20px rgba(0,255,0,.25);

}

.cards{

margin-top:25px;

display:grid;

grid-template-columns:
repeat(auto-fit,minmax(220px,1fr));

gap:20px;

}

.card{

background:rgba(255,255,255,.08);

backdrop-filter:blur(12px);

border-radius:18px;

padding:25px;

transition:.35s;

box-shadow:
0 8px 25px rgba(0,0,0,.25);

}

.card:hover{

transform:translateY(-8px);

box-shadow:
0 15px 35px rgba(0,0,0,.4);

}

.card h2{

font-size:18px;

margin-bottom:18px;

color:#9ecfff;

}

.value{

font-size:34px;

font-weight:bold;

}
.statusSafe{

color:#7CFC00;

}

.statusDanger{

color:#ff5252;

animation:blink 1s infinite;

}

@keyframes blink{

0%{opacity:1;}

50%{opacity:.4;}

100%{opacity:1;}

}

.exitPanel{

margin-top:25px;

padding:25px;

border-radius:18px;

background:rgba(255,255,255,.08);

backdrop-filter:blur(12px);

box-shadow:0 8px 25px rgba(0,0,0,.25);

text-align:center;

}

.exitPanel h2{

color:#9ecfff;

margin-bottom:15px;

}

#recommendedExit{

font-size:34px;

font-weight:bold;

color:#7CFC00;

text-shadow:0 0 15px #00ff66;

}

.room{

margin-top:30px;

padding:25px;

border-radius:20px;

background:rgba(255,255,255,.06);

backdrop-filter:blur(15px);

position:relative;

overflow:hidden;

}

.roomTitle{

font-size:26px;

margin-bottom:25px;

color:#8fd7ff;

}

.floor{

position:relative;

height:420px;

border:5px solid white;

border-radius:10px;

}

.topWall{

position:absolute;

top:12px;

left:50%;

transform:translateX(-50%);

font-size:22px;

font-weight:bold;

color:#8fd7ff;

}

.leftSensors{

position:absolute;

left:25px;

top:120px;

font-size:22px;

line-height:45px;

}

.rightBuzzer{

position:absolute;

right:25px;

top:170px;

font-size:22px;

}

.person{

position:absolute;

left:50%;

top:58%;

transform:translate(-50%,-50%);

font-size:60px;

transition:.4s;

display:none;

}

.doors{

position:absolute;

bottom:-5px;

left:0;

width:100%;

display:flex;

justify-content:space-around;

}

.door{

width:180px;

padding:12px;

border-radius:12px 12px 0 0;

font-size:22px;

font-weight:bold;

text-align:center;

background:#16a34a;

box-shadow:0 0 20px rgba(0,255,0,.3);

transition:.4s;

}
.statusSafe{

color:#7CFC00;

}

.statusDanger{

color:#ff5252;

animation:blink 1s infinite;

}

@keyframes blink{

0%{opacity:1;}

50%{opacity:.4;}

100%{opacity:1;}

}

.exitPanel{

margin-top:25px;

padding:25px;

border-radius:18px;

background:rgba(255,255,255,.08);

backdrop-filter:blur(12px);

box-shadow:0 8px 25px rgba(0,0,0,.25);

text-align:center;

}

.exitPanel h2{

color:#9ecfff;

margin-bottom:15px;

}

#recommendedExit{

font-size:34px;

font-weight:bold;

color:#7CFC00;

text-shadow:0 0 15px #00ff66;

}

.room{

margin-top:30px;

padding:25px;

border-radius:20px;

background:rgba(255,255,255,.06);

backdrop-filter:blur(15px);

position:relative;

overflow:hidden;

}

.roomTitle{

font-size:26px;

margin-bottom:25px;

color:#8fd7ff;

}

.floor{

position:relative;

height:420px;

border:5px solid white;

border-radius:10px;

}

.topWall{

position:absolute;

top:12px;

left:50%;

transform:translateX(-50%);

font-size:22px;

font-weight:bold;

color:#8fd7ff;

}

.leftSensors{

position:absolute;

left:25px;

top:120px;

font-size:22px;

line-height:45px;

}

.rightBuzzer{

position:absolute;

right:25px;

top:170px;

font-size:22px;

}

.person{

position:absolute;

left:50%;

top:58%;

transform:translate(-50%,-50%);

font-size:60px;

transition:.4s;

display:none;

}

.doors{

position:absolute;

bottom:-5px;

left:0;

width:100%;

display:flex;

justify-content:space-around;

}

.door{

width:180px;

padding:12px;

border-radius:12px 12px 0 0;

font-size:22px;

font-weight:bold;

text-align:center;

background:#16a34a;

box-shadow:0 0 20px rgba(0,255,0,.3);

transition:.4s;

}
.fire{

position:absolute;

width:24px;

height:24px;

font-size:28px;

animation:flicker .35s infinite;

display:none;

}

@keyframes flicker{

0%{

transform:scale(1) rotate(-3deg);

}

25%{

transform:scale(1.12) rotate(2deg);

}

50%{

transform:scale(.95) rotate(-2deg);

}

75%{

transform:scale(1.08) rotate(3deg);

}

100%{

transform:scale(1);

}

}

.footer{

margin:35px 0;

text-align:center;

color:#9fb8d8;

font-size:14px;

}

@media(max-width:900px){

header h1{

font-size:26px;

}

.mainStatus{

font-size:24px;

}

.floor{

height:360px;

}

.person{

font-size:46px;

}

.door{

width:130px;

font-size:18px;

}

}

@media(max-width:600px){

.cards{

grid-template-columns:1fr;

}

.topBar{

flex-direction:column;

gap:12px;

}

.floor{

height:320px;

}

.leftSensors{

font-size:18px;

}

.rightBuzzer{

font-size:18px;

}

.person{

font-size:40px;

}

}

</style>

</head>

<body>

<header>

<h1>🔥 SMART FIRE ESCAPE SYSTEM</h1>

<p>Emergency Monitoring Dashboard</p>

<div class="topBar">

<div class="statusBadge">

🟢 ONLINE

</div>

<div class="clock" id="clock">

00:00:00

</div>

</div>

</header>

<div class="container">

<div id="mainStatus" class="mainStatus">

🟢 BUILDING SAFE

</div>

<div class="cards">
<div class="card">

<h2>🌫 Smoke Level</h2>

<div id="smokeValue" class="value statusSafe">
0 ppm
</div>

<div class="progress">
<div id="smokeBar" class="progressFill"></div>
</div>

<div id="smokeLevelText"
style="margin-top:12px;font-size:18px;color:#7CFC00;">
NORMAL
</div>

</div>

<div class="card">

<h2>🌫 Smoke Sensor</h2>

<div id="smokeStatus"
class="value statusSafe">

SAFE

</div>

</div>
<div class="card">

<h2>🔥 Flame Sensor</h2>

<div id="flameStatus" class="value statusSafe">

NO FLAME

</div>

</div>

<div class="card">

<h2>🚶 Motion Sensor</h2>

<div id="motionStatus" class="value">

NO MOTION

</div>

</div>

<div class="card">

<h2>🔊 Alarm</h2>

<div id="alarmStatus" class="value">

OFF

</div>

</div>

</div>

<div class="exitPanel">

<h2>

RECOMMENDED EXIT

</h2>

<div id="recommendedExit">

✅ USE EXIT A OR EXIT B

</div>

</div>

<div class="room">

<div class="roomTitle">

🏢 ROOM LAYOUT

</div>

<div class="floor">

<div class="topWall">

📡 PIR MOTION SENSOR

</div>

<div class="leftSensors">

🌫 MQ-2 SENSOR

<br><br>

🔥 FLAME SENSOR

</div>

<div class="person"

id="person">

🧍

</div>

<div class="rightBuzzer">

🔊 BUZZER

</div>

<div class="arrow"

id="exitArrow">

➡➡➡

</div>

<div class="fire"

id="fireIcon"

style="left:120px;top:185px;">

🔥

</div>

<div class="smoke"

id="smoke1"

style="left:90px;top:210px;animation-delay:0s;">

</div>

<div class="smoke"

id="smoke2"

style="left:120px;top:220px;animation-delay:1.2s;">

</div>

<div class="smoke"

id="smoke3"

style="left:145px;top:215px;animation-delay:2.4s;">

</div>

<div class="doors">

<div

class="door safe"

id="exitA">

🚪 EXIT A

</div>

<div

class="door safe"

id="exitB">

🚪 EXIT B

</div>

</div>

</div>

</div>

<div class="events">

<h2>

📜 EVENT LOG

</h2>

<ul id="eventLog">

<li>System Started...</li>

<li>Waiting for ESP8266...</li>

</ul>

</div>

<div class="footer">

ESP8266 Smart Fire Escape System

</div>

<script>
function addEvent(text){

const log=document.getElementById("eventLog");

const li=document.createElement("li");

const t=new Date();

li.innerHTML=

t.toLocaleTimeString()+" - "+text;

log.prepend(li);

while(log.children.length>8)

log.removeChild(log.lastChild);

}

function updateClock(){

const now=new Date();

document.getElementById("clock").innerHTML=

now.toLocaleTimeString();

}

setInterval(updateClock,1000);

updateClock();

function fireMode(){

document.getElementById("mainStatus").innerHTML=

"🚨 FIRE DETECTED";

document.getElementById("mainStatus").style.background="#c62828";

document.getElementById("recommendedExit").innerHTML=

"➡ USE EXIT B";

document.getElementById("recommendedExit").style.color="#00ff66";

document.getElementById("alarmStatus").innerHTML="ON";

document.getElementById("alarmStatus").className="value statusDanger";

const exitA=document.getElementById("exitA");

exitA.className="door blocked";

exitA.innerHTML="❌ EXIT A";
const exitB=document.getElementById("exitB");

exitB.className="door safe";

exitB.style.boxShadow="0 0 40px #00ff66";

exitB.style.transform="scale(1.12)";

if(document.getElementById("motionStatus").innerHTML=="🚶 PERSON DETECTED"){





document.getElementById("exitArrow").style.display="block";


}

document.getElementById("fireIcon").style.display="block";

document.getElementById("smoke1").style.display="block";

document.getElementById("smoke2").style.display="block";

document.getElementById("smoke3").style.display="block";

addEvent("🔥 Fire Detected");

}

function safeMode(){
  document.getElementById("exitA").innerHTML="🚪 EXIT A";

document.getElementById("exitB").style.boxShadow="";

document.getElementById("exitB").style.transform="scale(1)";

document.getElementById("mainStatus").innerHTML=

"🟢 BUILDING SAFE";

document.getElementById("mainStatus").style.background="#16a34a";

document.getElementById("recommendedExit").innerHTML=

"✅ USE EXIT A OR EXIT B";

document.getElementById("recommendedExit").style.color="#7CFC00";

document.getElementById("alarmStatus").innerHTML="OFF";

document.getElementById("alarmStatus").className="value statusSafe";

document.getElementById("exitA").className="door safe";

document.getElementById("exitB").className="door safe";

document.getElementById("exitArrow").style.display="none";

document.getElementById("fireIcon").style.display="none";

document.getElementById("smoke1").style.display="none";

document.getElementById("smoke2").style.display="none";

document.getElementById("smoke3").style.display="none";

addEvent("Building Safe");

}

if(!!window.EventSource){

const source=new EventSource("/events");

source.addEventListener("smokeValue",function(e){

document.getElementById("smokeValue").innerHTML=e.data+" ppm";

let p=parseInt(e.data);

if(isNaN(p)) p=0;

let percent=Math.min((p/1023)*100,100);

document.getElementById("smokeBar").style.width=percent+"%";

if(percent<40){

document.getElementById("smokeBar").style.background="#00ff66";

document.getElementById("smokeLevelText").innerHTML="NORMAL";

document.getElementById("smokeLevelText").style.color="#00ff66";

}
else if(percent<70){

document.getElementById("smokeBar").style.background="#ffcc00";

document.getElementById("smokeLevelText").innerHTML="MODERATE";

document.getElementById("smokeLevelText").style.color="#ffcc00";

}
else{

document.getElementById("smokeBar").style.background="#ff3333";

document.getElementById("smokeLevelText").innerHTML="DANGEROUS";

document.getElementById("smokeLevelText").style.color="#ff3333";

}

});

source.addEventListener("smoke",function(e){

if(e.data=="1"){

document.getElementById("smokeStatus").innerHTML="🌫 SMOKE";

document.getElementById("smokeStatus").className="value statusDanger";

fireMode();

}
else{

document.getElementById("smokeStatus").innerHTML="SAFE";

document.getElementById("smokeStatus").className="value statusSafe";

if(document.getElementById("flameStatus").innerHTML!="🔥 FLAME"){

safeMode();

}

}

});



source.addEventListener("flame",function(e){

if(e.data=="1"){

document.getElementById("flameStatus").innerHTML="🔥 FLAME";

document.getElementById("flameStatus").className="value statusDanger";

fireMode();

}
else{

document.getElementById("flameStatus").innerHTML="NO FLAME";

document.getElementById("flameStatus").className="value statusSafe";

}

});

source.addEventListener("motion",function(e){

if(e.data=="1"){

document.getElementById("motionStatus").innerHTML="🚶 PERSON DETECTED";

document.getElementById("person").style.display="block";

if(
    document.getElementById("smokeStatus").innerHTML=="🌫 SMOKE" ||
    document.getElementById("flameStatus").innerHTML=="🔥 FLAME"
){

    document.getElementById("exitArrow").style.display="block";

}

addEvent("Motion Detected");

}

else{

document.getElementById("motionStatus").innerHTML=

"NO MOTION";

document.getElementById("person").style.display="none";

}

});

}

</script>

</body>

</html>

)rawliteral";


// ============================================================
// SETUP
// ============================================================

void setup() {

    Serial.begin(115200);

    delay(1000);


    // ---------------- PIN MODES ----------------

    pinMode(smokePin, INPUT);
    pinMode(flamePin, INPUT);
    pinMode(pirPin, INPUT);

    pinMode(greenLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(buzzerPin, OUTPUT);


    // ---------------- SAFE STATE ----------------

    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(buzzerPin, LOW);


    Serial.println();
    Serial.println("======================================");
    Serial.println(" SMART FIRE ESCAPE SYSTEM");
    Serial.println("======================================");


    // ========================================================
    // CONNECT TO LAPTOP HOTSPOT
    // ========================================================

    Serial.println();
    Serial.print("Connecting to hotspot: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);


    while (WiFi.status() != WL_CONNECTED) {

        delay(500);

        Serial.print(".");

    }


    Serial.println();
    Serial.println();
    Serial.println("======================================");
    Serial.println(" WIFI CONNECTED");
    Serial.println("======================================");

    Serial.print("NodeMCU IP Address: ");

    Serial.println(WiFi.localIP());

    Serial.println();
    Serial.println("Open this IP address on your phone.");
    Serial.println("Phone must be connected to ESPTEST.");
    Serial.println();

    // ========================================================
    // BLYNK CLOUD
    // ========================================================

    Blynk.config(BLYNK_AUTH_TOKEN);

    Serial.println("Connecting to Blynk Cloud...");

    if (Blynk.connect(10000)) {
        Serial.println("BLYNK CONNECTED");
    }
    else {
        Serial.println("BLYNK NOT CONNECTED - will retry automatically.");
    }


    // ========================================================
    // WEB SERVER
    // ========================================================

    server.on("/", HTTP_GET,
    [](AsyncWebServerRequest *request) {

        request->send_P(
            200,
            "text/html",
            index_html
        );

    });


    // ========================================================
    // SERVER SENT EVENTS
    // ========================================================

    events.onConnect(
    [](AsyncEventSourceClient *client) {

        Serial.println(
            "Dashboard connected!"
        );

        client->send(
            "connected",
            NULL,
            millis(),
            100
        );

    });


    server.addHandler(&events);


    // ========================================================
    // START SERVER
    // ========================================================

    server.begin();

Serial.println();
Serial.println("==================================");
Serial.println("WEB SERVER STARTED");
Serial.println("Open this address in your phone:");
Serial.print("http://");
Serial.println(WiFi.localIP());
Serial.println("==================================");

    Serial.println(
        "Web server started successfully."
    );

    Serial.println(
        "======================================"
    );

}


// ============================================================
// LOOP
// ============================================================

void loop() {

    // Keep Blynk connection alive.
    Blynk.run();

    unsigned long currentMillis =
    millis();

    // Retry Blynk connection without blocking the web dashboard.
    if (!Blynk.connected() &&
        currentMillis - lastBlynkConnectAttempt >= blynkReconnectInterval) {

        lastBlynkConnectAttempt = currentMillis;

        Serial.println("Trying to reconnect to Blynk...");
        Blynk.connect(1000);
    }


    if (
        currentMillis - previousSensorMillis
        >= sensorInterval
    ) {

        previousSensorMillis =
        currentMillis;


        // ====================================================
        // READ SENSORS
        // ====================================================

        smokeValue =
        analogRead(smokePin);


        smokeDetected =
        (smokeValue >= smokeThreshold);


        flameDetected =
        (digitalRead(flamePin) == HIGH);


        motionDetected =
        (digitalRead(pirPin) == HIGH);


        fireDetected =
        smokeDetected || flameDetected;


        // ====================================================
        // BLYNK FIRE NOTIFICATION
        // ====================================================

        if (fireDetected) {

            if (!fireNotificationSent && Blynk.connected()) {

                String fireMessage;

                if (smokeDetected && flameDetected) {
                    fireMessage = "SMOKE AND FLAME DETECTED! Immediate attention required.";
                }
                else if (flameDetected) {
                    fireMessage = "FLAME DETECTED! Immediate attention required.";
                }
                else {
                    fireMessage = "SMOKE DETECTED! Immediate attention required.";
                }

                Blynk.logEvent(BLYNK_EVENT_CODE, fireMessage);
                fireNotificationSent = true;

                Serial.println("BLYNK ALERT SENT");
            }
        }
        else {
            // Allow a new notification after the fire condition clears.
            fireNotificationSent = false;
        }


        // ====================================================
        // FIRE ALARM
        // ====================================================

        if (fireDetected) {

            digitalWrite(
                greenLED,
                LOW
            );

            digitalWrite(
                redLED,
                HIGH
            );

            digitalWrite(
                buzzerPin,
                HIGH
            );

        }

        else {

            digitalWrite(
                greenLED,
                HIGH
            );

            digitalWrite(
                redLED,
                LOW
            );

            digitalWrite(
                buzzerPin,
                LOW
            );

        }


        // ====================================================
        // SERIAL MONITOR
        // ====================================================

        Serial.println(
            "--------------------------------"
        );


        Serial.print(
            "Smoke Value : "
        );

        Serial.println(
            smokeValue
        );


        Serial.print(
            "Smoke       : "
        );

        Serial.println(
            smokeDetected
            ? "YES"
            : "NO"
        );


        Serial.print(
            "Flame       : "
        );

        Serial.println(
            flameDetected
            ? "YES"
            : "NO"
        );


        Serial.print(
            "Motion      : "
        );

        Serial.println(
            motionDetected
            ? "DETECTED"
            : "NO MOTION"
        );


        Serial.print(
            "Status      : "
        );

        Serial.println(
            fireDetected
            ? "FIRE DETECTED"
            : "SAFE"
        );


        // ====================================================
        // SEND LIVE DATA TO WEBSITE
        // ====================================================

        String smokeString =
        String(smokeValue);


        events.send(
            smokeString.c_str(),
            "smokeValue",
            millis()
        );


        events.send(
            smokeDetected ? "1" : "0",
            "smoke",
            millis()
        );


        events.send(
            flameDetected ? "1" : "0",
            "flame",
            millis()
        );


        events.send(
            motionDetected ? "1" : "0",
            "motion",
            millis()
        );


     

    }

}
