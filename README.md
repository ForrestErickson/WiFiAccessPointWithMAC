# WiFiAccessPointWithMAC
An ESP32 Soft Access Point with SSID of ModelName and MAC, Serves a Web Page

## Motivation, Problem Description
If there are many devices of the same model name at a Wi-Fi location the MAC number will identify them as unique.

## This Solution
This program reads the ESP32 built in MAC and appends it to the model name to create the Soft AP SSID
Because the MAC is long, the Model Name is restricted in length.

To use this, the phisical device will need to have a lable or display with the SSID for a human to read.

## In action  
A screen shot of the found networks from an Android phone with two devices.  
![image](https://github.com/user-attachments/assets/75b8d6ca-2d95-4174-841d-5c83eae23363)  
In this case the Model Name was, "AbcdefgH".  
An underscore separates the model name from the MAC address.
