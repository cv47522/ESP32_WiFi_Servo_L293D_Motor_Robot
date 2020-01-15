# ESP32 WiFi Servo L293D Motor Robot

- Arduino Physical Interactive Kinetic Robot Project
- Materials: ESP32 / DC Motors / Servos
- Size: 25(H)*25(W)*25(D)cm
- Language: Arduino
- ***More details of the project (Demo Video):*** *https://wantinghsieh.com/moving-instrumentbot/*

![image](https://wantinghsieh.com/wp-content/uploads/Moving-Instrumentbot_5_web.jpg)

---
[esp32]: https://cdn-learn.adafruit.com/downloads/pdf/adafruit-huzzah32-esp32-feather.pdf?timestamp=1576067516
[motor-pwm]: https://howtomechatronics.com/tutorials/arduino/arduino-dc-motor-control-tutorial-l298n-pwm-h-bridge/
[servo-pwm]: https://learn.sparkfun.com/tutorials/pulse-width-modulation/all

## What Does It Do?
This instrumentbot is controlled remotely via a webpage running on any portable devices with the same network as instrumentbot’s.

- ### The project combines these topics:

  - Uploading a customized webpage, which has multiple buttons with different colors, to the server on the robot for controlling its movement
  - Controlling DC Motors with the L293D H-Bridge IC by outputting PWM with a specific duty cycle to its ENABLE pins and digital high or low value to its INPUT pins

The instrumentbot is curious about everything around it and wants to start its adventurous trip. However, it doesn’t know the correct direction to adventure and thus rely heavily on audience’s instructions to help it find out a suitable way to move.

- ### The code works like this:

  - Displaying a webpage on a specific IP address (the server on the robot) which can be accessed by audience’s smartphone within the same SSID
  - The robot moves forward, backward, leftward or rightward according to the command received from the audience’s mobile browser
  - The robot turns around its eye ball which is controlled by two servo motors once it stops moving
 
## ESP32 Pin Connection ([ESP32 Diagram][esp32])

![image](https://wantinghsieh.com/wp-content/uploads/Moving-Instrumentbot_Esp32_1_web.jpg)

## Theory of Motor Control
* [Speed Control of DC Motor Using Pulse-Width Modulation][motor-pwm]
* [Angle Control of Servo Using Pulse-Width Modulation][servo-pwm]
