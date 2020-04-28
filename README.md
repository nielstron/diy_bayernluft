DIY Humiditysensors for Bayernlüfter
------------------------------------

>Selbstgebaute Feuchtigkeitssensoren für den Bayernlüfter

This project aims to self-build [sensors for the Bayernlüfter](https://www.hitl-shop.de/Bayernluefter-Typ-C-Zubehoer-Feuchtesensoren.htm) by 
- replacing them with cheap humidity and temperature sensors 
- translating the data via Arduino-like microchips 
- emulate the [original HYT131](https://shop.bb-sensors.com/en/Measurement-by-branches/Building-automation/Digital-humidity-temperature-sensor-HYT131-bub.html) temperature and humidity sensor

Hence this project includes
- some code to read some typical sensors (DHT12, DHT22)
- some code to write data in [the HYT131 format](https://shop.bb-sensors.com/out/media/Datasheet_long_incl_code_%20HYT131.pdf)
- some basic code to plug the data from one sensor to the other