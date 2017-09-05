Arduino electronic wateproof thermometer with display.

Parts needed:
- 4.7 ohm resistor
- KY34-B KUMAN Oled display screen 0.96"
- DS18b20 temperature sensor
- a breadboard
- arduino platform
- power
- cables

See /docs for libs, PDF and images

Based on the work from https://www.carnetdumaker.net/articles/mesurer-une-temperature-avec-un-capteur-1-wire-ds18b20-et-une-carte-arduino-genuino/
And samples provided by U8glib.
Built with Atom + PlatformIO.
Tested on Leonardo.

Install depencies
pio lib install OneWire
pio lib install U8glib (it will take the id=7)
