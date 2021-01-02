# m5stack 

This repository contains some examples for m5stack ESP32 Basic Development Kit with LTE module

## Examples

List of available examples

- ident.me

Performs HTTP GET request to  http://ident.me and returns device public IP

- mqtt

Connects to MQTT server and publishes events on button press.

## Hardware

Bellow is list of m5stack hardware used for examples.

- ESP32 Basic Core IoT Development Kit
- COM.LTE(4G) with SIMCom SIM7600G Module 

## Libraries

- [TinyGSM](https://www.arduino.cc/reference/en/libraries/tinygsm/)
- [ArduinoHttpClient](https://www.arduino.cc/reference/en/libraries/arduinohttpclient/)
- [PubSubClient](https://www.arduino.cc/reference/en/libraries/pubsubclient/)

## Documents

- [SIMCom AT commands](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/module/SIM7500_SIM7600%20Series_AT%20Command%20Manual%20_V1.10.pdf)
- [SIMCom AT MQTT commands](https://www.elementzonline.com/downloads/SIM7600/SIM7500_SIM7600%20Series_MQTT_ATC_V1.01.pdf)
