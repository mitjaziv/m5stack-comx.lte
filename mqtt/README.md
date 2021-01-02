# MQTT Example

Simple MQTT connect and publish example running on m5stack hardware.

## Flow

- Initialize modem
- Get modem info
- Connect to cellular network
- Retrieve IMEI and IMSI
- Connect to GPRS via APN
- Get device public IP from http://ident.me
- Connect to MQTT server
- On keypress publish message to MQTT topic

## Serial output

```
17:47:51.819 -> M5Stack initializing...OK
17:47:52.461 -> M5Stack + LTE Module + MQTT
17:47:52.564 -> Initializing modem...
17:48:12.809 -> Modem Info:ATI  Manufacturer: SIMCOM INCORPORATED Model: SIMCOM_SIM7600G Revision: SIM7600M21-A_V2.0 IMEI: 123456789012345 +GCAP: +CGSM
17:48:12.809 -> Waiting for network... success
17:48:12.844 -> Network connected
17:48:12.844 -> IMEI: 123456789012345
17:48:12.877 -> IMSI: 543210987654321
17:48:12.877 -> Connecting to internet.cxn success
17:48:13.024 -> GPRS connected
17:48:13.024 -> Get ident.me IP: 93.81.26.26
17:48:15.796 -> Connecting to test.mosquitto.org success
17:48:22.861 -> Publish event A
17:48:30.896 -> Publish event B
17:48:32.086 -> Publish event C
```

## Display

![Display](display.png "Display")
