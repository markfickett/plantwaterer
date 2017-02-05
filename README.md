Automatic Plant Waterer
=======================

This plant waterer runs on an ATTiny85. It detects the moisture in the plant's
soil and detects whether the water reservoir is empty using a voltage divider
(more water means less resistance means higher voltage detected). It waters the
plant using a pump with a DC motor.

![Waterer, Bucket, and Plant](img/all.jpg?raw=true)

Circuit
-------

![Circuit Diagram](img/circuit_diagram.jpg?raw=true)

Parts and Construction
----------------------

*   ATTiny85 micro controller, 8-pin DIP socket (optional to make reprogramming
    easier)
*   3.3v regulator from Radio Shack
*   vcc smoothing capacitor (6.3v, 470 micro farads)
*   2x 150 ohm resistors for status LEDs
*   2x status LEDs (red for power, green for status as Morse code)
*   2x 220 kOhm resistors for voltage dividers
*   flyback diode (1N401 from Radio Shack) and smoothing capacitor (built in)
    for pump motor
*   MOSFET to control motor
*   12v wall wart from electronic scraps (a lower output voltage would waste
    less power as heat in the voltage regulator)
*   heat sink for voltage regulator (copper band from scrap electronics)
*   paristaltic pump from broken bubble gun toy
*   water sensors: speaker wire, nails, wood scrap
*   vinyl tubing
*   quarter-size perma-proto board from Adafruit (15 rows)

For the plant end, I cut a small wood scrap, which holds two conductive probes
to sense soil moisture and which keeps the vinyl tubing in place.

![Soil End](img/soil_end.jpg?raw=true)
