# Iot-Project-on-water-quality-monitoring
In this project 4 water qualities are monitored-
- TDS(total dissolved solid)
- Turbidity
- pH value
- temperature
An arduino mega is used to collect all the data from the sensors. Then the values are sent to nodemcu(esp8266) through UART serial communication.
From the nodemcu values are sent to firebase with the help of a url link and api key written inside the nodemcu sketch. There are 2 sketches, one is for arduino
and another is for nodemcu.<br><br>
Components used-
- Arduino Mega
- Nodemcu
- TDS sensor
- Turbidity sensor
- pH sensor
- Temperature probe
- LM2596 voltage regulator
- 12v Power adapter
