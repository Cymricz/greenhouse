My code for running a simple semi-automated greenhouse through the Adafruit Pro Trinket, currently it's being prototyped on an Arduino Uno, but once everything is buttoned up the project will move to it's permanent home on the Trinket.
The greenhouse is setup up to monitor the soil moisture on two pots, and take a measurement every day. The sensors are vulnerable to corrosion if left powered constantly, so they're hooked up to power through a transistor.
Every day, the sensors get power and then the analog signal is read to determine if the soil needs to be watered. If so, a red LED indicator is illuminated. Also incorporated into the greenhouse is a DHT11 temp/humidity sensor, as soon as I can nail down how to attach them, I intend to hook up a pair of servos to the lids of the greenhouse to open when the temperature reaches a certain threshold.

Questions and comments are always welcome, thanks for stopping by.
