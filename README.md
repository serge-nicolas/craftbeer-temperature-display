## Synopsis
Arduino electronic wateproof thermometer with display.

Based on the work from https://www.carnetdumaker.net/articles/mesurer-une-temperature-avec-un-capteur-1-wire-ds18b20-et-une-carte-arduino-genuino/
And samples provided by U8glib.

## Motivation
Can be used to monitor beer fermentation temperature.

## Parts needed
- 4.7 ohm resistor
- KY34-B KUMAN Oled display screen 0.96"
- DS18b20 temperature sensor
- a breadboard
- Arduino platform
- power supply
- cables

## Doc
See /docs for libs, PDF and images

## Installation
Built with Atom + PlatformIO.

Install dependancies :
pio lib install OneWire
pio lib install U8glib (it will take the id=7)


## Platform
Tested on Leonardo.
