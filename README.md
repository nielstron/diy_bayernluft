DIY Humiditysensors for Bayernlüfter
------------------------------------

>Selbstgebaute Feuchtigkeitssensoren für den Bayernlüfter

This project aims to self-build [sensors for the Bayernluefter](https://www.hitl-shop.de/Bayernluefter-Typ-C-Zubehoer-Feuchtesensoren.htm) by 
- replacing them with cheap humidity and temperature sensors 
- translating the data via Arduino-like microchips 
- emulate the original ~~[HYT131](https://shop.bb-sensors.com/en/Measurement-by-branches/Building-automation/Digital-humidity-temperature-sensor-HYT131-bub.html)~~ [SHT31](https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/) temperature and humidity sensor 

Hence this project includes
- some code to read some typical sensors (DHT12, DHT22)
- some code to write data in the ~~[HYT131](https://shop.bb-sensors.com/out/media/Datasheet_long_incl_code_%20HYT131.pdf)~~ [SHT31](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/2_Humidity_Sensors/Datasheets/Sensirion_Humidity_Sensors_SHT3x_Datasheet_digital.pdf) format
- some basic code to plug the data from one sensor to the other

### How to set up?

Have a look at the configuration in `config.h`. Do your own sensors provide data in an integer format or double? Modify the values there accordingly.

Place your code which reads temperature and humdity values from a connected sensor in `main.ino`.
Make sure that the global values `cur_temperature` and `cur_humidity` are set on a regular interval.

Compile twice, once with `ITC_ADDRESS` set to the address for each sensor and upload the matching code.

### FAQ

#### The images on the website of Bayernluft show that HYT131 is used. Why do you implement SHT31?

We actually ordered the sensor-kit from Bayernluft and it includes the SHT31 and not the HYT131. Also emulated HYT131 code would not work, SHT31 emulation did though.