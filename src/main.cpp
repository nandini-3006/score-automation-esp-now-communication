#include<Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

// Replace with your network credentials (STATION)
const char* ssid = "Piyusha";
const char* password = "123456789";

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message { 
  int id;
  int check_value;
  unsigned int readingId;
} struct_message;

struct_message incomingReadings;

// JSON document for board readings
ArduinoJson::StaticJsonDocument<200> doc;

AsyncWebServer server(80);
AsyncEventSource events("/events");

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len) { 
  // Copies the sender MAC address to a string

  Serial.println(incomingReadings.check_value);

  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

  // Populate JSON document
  doc["id"] = incomingReadings.id;
  doc["check"] = incomingReadings.check_value;
  doc["readingId"] = incomingReadings.readingId;
  String jsonString;
  serializeJson(doc, jsonString);
  events.send(jsonString.c_str(), "new_readings", millis());

}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP-NOW Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
</head>
<body>
    
    <style>
     body{
    background-image: url(https://wallpaper.forfun.com/fetch/7f/7fd7c66ecefecd3da76c4c9554a01f55.jpeg);
    background-repeat: no-repeat;}
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {font-size: 1.5rem;}
    body {margin: 0;}
    .topnav {overflow: hidden; color: white; padding: 10px;}
    .content {padding: 20px;}
    .card {background-color: #0ce2e2ac; box-shadow: 2px 2px 12px 1px rgba(255, 255, 255, 0.762); padding: 10px; border-radius: 25px;}
       .cards {font-family: Arial, Helvetica, sans-serif; display: grid; grid-gap: 3rem; grid-template-columns: repeat(3, 1fr);  /grid-template-columns: repeat(auto-fit, minmax(300px, 1fr))/; max-width: 1000px; margin: 0 auto; }
       .card:nth-child(1) { grid-column: 1; grid-row: 1; } /* Checkpoint 1 */
       .card:nth-child(2) { grid-column: 2; grid-row: 1; } /* Checkpoint 2 */
       .card:nth-child(3) { grid-column: 1; grid-row: 2; } /* Checkpoint 3 */
       .card:nth-child(4) { grid-column: 2; grid-row: 2; } /* Checkpoint 4 below Checkpoint 2 */
       .card:nth-child(5) { grid-column: 3; grid-row: 2; } /* Checkpoint 5 below Checkpoint 3 */
    .reading {font-size: 2rem; color: rgb(255, 255, 255);}
    .packet {color: #000000;}
    h4{font-size: 2rem; color:black;}
    @font-face {font-family:'road';src: url('SuperRoadDemoRegular.ttf')}
    h1{ font-family: 'road';background: linear-gradient(to bottom, #ffff00 , gold );
    -webkit-background-clip: text; 
    -webkit-text-fill-color: transparent; 
    background-clip: text; 
    -webkit-text-fill-color: transparent;
    font-size: 4.7rem;}

    /* particle */
    /* ---- reset ---- */

body {
  margin: 0;
  font:normal 75% Arial, Helvetica, sans-serif;
}

canvas {
  display: block;
  vertical-align: bottom;
}

/* ---- particles.js container ---- */

#particles-js {
  position: absolute;
  width: 100%;
  height: 100%;
  /* background-image: url(""); */
  background-repeat: no-repeat;
  background-size: cover;
  background-position: 50% 50%;
}

/* ---- stats.js ---- */


#stats,

#stats{
  border-radius: 3px 3px 0 0;
  overflow: hidden;
}

/* particle */
    </style>

  <div class="topnav">
    <!-- particle -->
  <div id="particles-js"></div>
  <script src='https://cdn.jsdelivr.net/particles.js/2.0.0/particles.min.js'></script>
  <script src='https://threejs.org/examples/js/libs/stats.min.js'></script><script  src="./script.js"></script>
  <!-- particle -->
    <h1>PATH OF PRIMES SCOREBOARD</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <h4>CHECKPOINT 1</h4>
        <p><b>Points</b>: <span class="reading" id="t1">--</span></p>
      </div>

      <div class="card">
        <h4>CHECKPOINT 2</h4>
        <p><b>Points</b>: <span class="reading" id="t2">--</span></p>
      </div>

      <div class="card">
        <h4>CHECKPOINT 3</h4>
        <p><b>Points</b>: <span class="reading" id="t3">--</span></p>
      </div>

      <div class="card">
        <h4>CHECKPOINT 4</h4>
        <p><b>Points</b>: <span class="reading" id="t4">--</span></p>
      </div>

      <div class="card">
        <h4>CHECKPOINT 5</h4>
        <p><b>Points</b>: <span class="reading" id="t5">--</span></p>
      </div>

    </div>
  </div>
  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');

      source.addEventListener('open', function(e) {
        console.log("Events Connected");
      }, false);

      source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);

      source.addEventListener('new_readings', function(e) {
        console.log("new_readings", e.data);
        var obj = JSON.parse(e.data);
        var checkpointId = obj.id;
        if (checkpointId >= 1 && checkpointId <= 5) {
          
          document.getElementById("t" + checkpointId).innerHTML = obj.check.toFixed(2);
          
        }
      }, false);
    }
    // particle
    /* ---- particles.js config ---- */

particlesJS("particles-js", {
  "particles": {
    "number": {
      "value": 120,
      "density": {
        "enable": true,
        "value_area": 800
      }
    },
    "color": {
      "value": "#ffffff"
    },
    "shape": {
      "type": "circle",
      "stroke": {
        "width": 0,
        "color": "#000000"
      },
      "polygon": {
        "nb_sides": 5
      },
      "image": {
        "src": "img/github.svg",
        "width": 100,
        "height": 100
      }
    },
    "opacity": {
      "value": 0.5,
      "random": false,
      "anim": {
        "enable": false,
        "speed": 1,
        "opacity_min": 0.1,
        "sync": false
      }
    },
    "size": {
      "value": 3,
      "random": true,
      "anim": {
        "enable": false,
        "speed": 40,
        "size_min": 0.1,
        "sync": false
      }
    },
    "line_linked": {
      "enable": true,
      "distance": 150,
      "color": "#ffffff",
      "opacity": 0.4,
      "width": 1
    },
    "move": {
      "enable": true,
      "speed": 6,
      "direction": "none",
      "random": false,
      "straight": false,
      "out_mode": "out",
      "bounce": false,
      "attract": {
        "enable": false,
        "rotateX": 600,
        "rotateY": 1200
      }
    }
  },
  "interactivity": {
    "detect_on": "canvas",
    "events": {
      "onhover": {
        "enable": true,
        "mode": "grab"
      },
      "onclick": {
        "enable": true,
        "mode": "push"
      },
      "resize": true
    },
    "modes": {
      "grab": {
        "distance": 140,
        "line_linked": {
          "opacity": 1
        }
      },
      "bubble": {
        "distance": 400,
        "size": 40,
        "duration": 2,
        "opacity": 8,
        "speed": 3
      },
      "repulse": {
        "distance": 200,
        "duration": 0.4
      },
      "push": {
        "particles_nb": 4
      },
      "remove": {
        "particles_nb": 2
      }
    }
  },
  "retina_detect": true
});


/* ---- stats.js config ---- */

var count_particles, stats, update;
stats = new Stats;
stats.setMode(0);
stats.domElement.style.position = 'absolute';
stats.domElement.style.left = '0px';
stats.domElement.style.top = '0px';
document.body.appendChild(stats.domElement);
count_particles = document.querySelector('.js-count-particles');
update = function() {
  stats.begin();
  stats.end();
  if (window.pJSDom[0].pJS.particles && window.pJSDom[0].pJS.particles.array) {
    count_particles.innerText = window.pJSDom[0].pJS.particles.array.length;
  }
  requestAnimationFrame(update);
};
requestAnimationFrame(update);
// particle
  </script>
</body>
</html>
)rawliteral";

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);

  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for receive callback
  esp_now_register_recv_cb(OnDataRecv);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  events.onConnect([](AsyncEventSourceClient* client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start the server
  server.begin();
}

void loop() {
  // Ping clients every 5 seconds
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping", NULL, millis());
    lastEventTime = millis();
  }
}