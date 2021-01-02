#include <HTTPClient.h>
#include <M5Stack.h>

#define SerialMon Serial
#define SerialAT Serial2

#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>

#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

String imei = "unknown";
String imsi = "unknown";

// GPRS settings
const char apn[]  = "internet.cxn";
const char gprsUser[] = "";
const char gprsPass[] = "";

// MQTT details
const char* broker = "test.mosquitto.org";
const char* topic = "m5stack";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, "ident.me", 80);
PubSubClient mqtt(client);

void debug(String msg) {
  SerialMon.print(msg);
  M5.Lcd.print(msg);
}

void debugln(String msg) {
  SerialMon.println(msg);
  M5.Lcd.println(msg);
}

void mqttConnect() {
  while (!mqtt.connected()) {
    debug("Connecting to ");
    debug(broker);

    // Generate random client id
    String clientId = "m5stack-" + String(random(0xffff), HEX);

    // You can also connect with just client id
    // mqtt.connect(clientId.c_str());

    // Generate username with IMEI and IMSI as parameters
    String username = "dummy?imei=" + imei + "&imsi=" + imsi;
    String password = "dummy";
        
    // Attempt to connect
    if (!mqtt.connect(clientId.c_str(), username.c_str(), password.c_str())) {
      int state = mqtt.state();
      debug(" failed, rc=");
      debugln(String(state));
      delay(5000);
    }
    debugln(" success");
  }
}

void setup() {
  M5.begin();
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  debugln("M5Stack + LTE Module + MQTT");
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  // Set console baud rate
  debugln("Wait...");
  SerialMon.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, 5, 13);
  pinMode(2, OUTPUT);

  // Initalize modem.
  debugln("Initializing modem...");
  modem.restart();
  delay(10000);

  String modemInfo = modem.getModemInfo();
  debug("Modem Info:");
  debugln(modemInfo);

  debug("Waiting for network...");
  if (!modem.waitForNetwork()) {
    debugln(" fail");
    delay(10000);
    return;
  }
  debugln(" success");

  if (modem.isNetworkConnected()) {
    debugln("Network connected");
  }

  // Get IMEI
  imei = modem.getIMEI();
  debug("IMEI: ");
  debugln(imei);

  // Get IMSI
  imsi = modem.getIMSI();
  debug("IMSI: ");
  debugln(imsi);

  // Setup GPRS connection.
  debug("Connecting to ");
  debug(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    debugln(" fail");
    delay(10000);
    return;
  }
  debugln(" success");
  
  if (modem.isGprsConnected()) {
    debugln("GPRS connected");
  }

  // Make a HTTP GET request to ident.me
  debug("Get ident.me IP: ");
  int err = http.get("/");
  if (err != 0) {
    debugln("failed to connect");
    delay(10000);
    return;
  }

  int status = http.responseStatusCode();
  if (status != 200) {
    debugln("error");
    delay(10000);
    return;
  }

  String body = http.responseBody();
  debugln(body);

  // MQTT setup
  mqtt.setServer(broker, 1883);
  mqtt.setKeepAlive(60);
  
  delay(1000);
}

void loop() {
  if (!mqtt.connected()) {
    mqttConnect();
  }
  mqtt.loop();
  
  if (M5.BtnA.wasPressed()) {
    debugln("Publish event A");
    mqtt.publish(topic,"Event A");
  }

  if (M5.BtnB.wasPressed()) {
    debugln("Publish event B");
     mqtt.publish(topic,"Event B");
  }

  if (M5.BtnC.wasPressed()) {
    debugln("Publish event C");
     mqtt.publish(topic,"Event C");
  }
  
  M5.update();
  delay(100);
}
