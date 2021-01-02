#include <HTTPClient.h>
#include <M5Stack.h>

#define SerialMon Serial
#define SerialAT Serial2

#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false

const char apn[]  = "internet.cxn";
const char gprsUser[] = "";
const char gprsPass[] = "";

TinyGsm modem(SerialAT);
TinyGsmClient lte(modem);
HttpClient http(lte, "ident.me", 80);

void debug(String msg) {
  SerialMon.print(msg);
  M5.Lcd.print(msg);
}

void debugln(String msg) {
  SerialMon.println(msg);
  M5.Lcd.println(msg);
}

void setup() {
  M5.begin();
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
  debugln("M5Stack + LTE Module");
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  // Set console baud rate
  debugln("Wait...");
  SerialMon.begin(115200);
  SerialAT.begin(115200, SERIAL_8N1, 5, 13);
  pinMode(2, OUTPUT);

  // Initializing modem.
  debugln("Initializing modem...");
  modem.restart();
  delay(10000);

  String modemInfo = modem.getModemInfo();
  debug("Modem Info:");
  debugln(modemInfo);
}

void loop() {
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

  // Make a HTTP GET request to ident.me.
  debugln("Performing HTTP GET request...");
  int err = http.get("/");
  if (err != 0) {
    debugln("failed to connect");
    delay(10000);
    return;
  }

  int status = http.responseStatusCode();
  debug("Response status code: ");
  debugln(String(status));
  if (!status) {
    delay(10000);
    return;
  }

  String body = http.responseBody();
  debugln("Response:");
  debugln(body);
  
  modem.gprsDisconnect();
  debugln("GPRS disconnected");

  debugln("");
  debug("ident.me IP: ");
  debugln(body);

  // Exit.
  while (true) {
    delay(1000);
  }
}
