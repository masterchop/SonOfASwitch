# SonOfASwitch

REST API for Chinese Sonoff light switches

I recently got [this Chinese light switch](https://www.banggood.com/DIY-Wi-Fi-Wireless-Switch-For-Smart-Home-With-ABS-Shell-p-1019971.html)
which has WiFi and contains an ESP8266 microcontroller. Using the pins inside, it is possible to flash your own code to it using the Arduino IDE.

The code in this repository was forked from [this tutorial](http://randomnerdtutorials.com/reprogram-sonoff-smart-switch-with-web-server/).
I modified it to serve a captive portal when unable to connect to a network, and to have an übersimple REST API once it's connected.
