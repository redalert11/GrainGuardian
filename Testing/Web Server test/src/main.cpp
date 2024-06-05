#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <AsyncTCP.h>

// Global variables
bool client_mode = false;
String ssid_client;
String password_client;
String static_ip;
String gateway;
String subnet;
bool use_dhcp = true;

AsyncWebServer server(80);
DNSServer dnsServer;

// HTML template for the web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Wi-Fi Settings</title>
<style>
.switch {
  position: relative;
  display: inline-block;
  width: 60px;
  height: 34px;
}

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider {
  background-color: #2196F3;
}

input:focus + .slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked + .slider:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
</style>
</head>
<body>

<h2>ESP32 Wi-Fi Configuration</h2>
<form action="/get">
  <label class="switch">
    <input type="checkbox" id="clientMode" name="client_mode" onchange="updateClientMode()" %checked%>
    <span class="slider round"></span>
  </label>
  <label for="clientMode">Client Mode</label>
  <br><br>
  <div id="clientSettings" style="%style%">
    <label for="ssid">SSID:</label><br>
    <input type="text" id="ssid" name="ssid" value="%ssid%"><br>
    <label for="password">Password:</label><br>
    <input type="password" id="password" name="password" value="%password%"><br>
    <label for="dhcp">Use DHCP:</label>
    <input type="checkbox" id="dhcp" name="dhcp" %dhcp%><br>
    <div id="staticConfig" style="%staticStyle%">
      <label for="static_ip">Static IP:</label><br>
      <input type="text" id="static_ip" name="static_ip" value="%static_ip%"><br>
      <label for="gateway">Gateway:</label><br>
      <input type="text" id="gateway" name="gateway" value="%gateway%"><br>
      <label for="subnet">Subnet:</label><br>
      <input type="text" id="subnet" name="subnet" value="%subnet%"><br>
    </div>
  </div>
  <input type="submit" value="Save Settings">
</form>

<script>
function updateClientMode() {
  var x = document.getElementById('clientMode').checked;
  document.getElementById('clientSettings').style.display = x ? '' : 'none';
  document.getElementById('staticConfig').style.display = document.getElementById('dhcp').checked ? 'none' : '';
}
</script>

</body>
</html>
)rawliteral";

// Function to handle root requests
void handleRootRequest(AsyncWebServerRequest *request) {
  String html = String(index_html);
  
  // Replace placeholders with actual values
  html.replace("%checked%", client_mode ? "checked" : "");
  html.replace("%ssid%", ssid_client);
  html.replace("%password%", password_client);
  html.replace("%dhcp%", use_dhcp ? "checked" : "");
  html.replace("%static_ip%", static_ip);
  html.replace("%gateway%", gateway);
  html.replace("%subnet%", subnet);
  
  // Determine style for client settings based on client mode and DHCP checkbox
  String style = client_mode ? "" : "display:none;";
  String staticStyle = (client_mode && !use_dhcp) ? "" : "display:none;";
  html.replace("%style%", style);
  html.replace("%staticStyle%", staticStyle);
  
  request->send(200, "text/html", html);
}

// Function to handle form submissions
void handleGetRequest(AsyncWebServerRequest *request) {
  // Retrieve parameters from the form
  client_mode = request->getParam("client_mode")->value().toInt();
  ssid_client = request->getParam("ssid")->value();
  password_client = request->getParam("password")->value();
  use_dhcp = request->getParam("dhcp") ? true : false;
  static_ip = request->getParam("static_ip")->value();
  gateway = request->getParam("gateway")->value();
  subnet = request->getParam("subnet")->value();
  
  // Send response and restart ESP32
  request->send(200, "text/html", "Configuration Updated. Rebooting ESP32...");
  delay(1000);
  ESP.restart();
}

void setup() {
  Serial.begin(115200);
  
  // Set up preferences
  Preferences preferences;
  preferences.begin("wifi-config", false);
  
  // Load preferences
  client_mode = preferences.getBool("client_mode", false);
  ssid_client = preferences.getString("ssid", "");
  password_client = preferences.getString("password", "");
  use_dhcp = preferences.getBool("use_dhcp", true);
  static_ip = preferences.getString("static_ip", "");
  gateway = preferences.getString("gateway", "");
  subnet = preferences.getString("subnet", "");
  
  preferences.end();
  
  // Set up WiFi mode
  WiFi.mode(WIFI_AP_STA);
  
  // Set up DNS server
  dnsServer.start(53, "*", WiFi.softAPIP());
  
  // Set up web server
  server.on("/", HTTP_GET, handleRootRequest);
  server.on("/get", HTTP_GET, handleGetRequest);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
}
