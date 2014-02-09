arduino_rgb_led_driver
======================

Source for the RGB LED driver

So far it listens on the serial port for user generated commands.
Next step is to implement the VirtualWire library for UDP like broadcast communication using 433Mhz RF transmitters/receivers. Once that is complete the idea is to create a central unit with an RF transmitter. This unit can receive commands from an app/webservice and translate them to slave modules using the RF transmitter.

The available commands are:
- <strong>color (color) (opt: side (l/r default by omision is both sides))</strong> Set the color of the LEDs
- <strong>police</strong> Whee hoo whee hoo
- <strong>pulse (color) (opt: speed (in ms. default is 500ms))</strong> Pulses the LEDs between two colors indefinitely or until the 'off' command is received
- <strong>fade (from) (to) (opt: step speed (in ms. default is 20ms))</strong> Fades the LEDs from one to the other color
- <strong>strobe (color) (opt: speed (in ms. default is 200ms))</strong> Strobes the LEDs indefinitely or until the 'off' command is received
- <strong>rainbow</strong> Let out your inner gay self
- <strong>off</strong> Turn off the LEDs/Cancel animation
- <strong>info</strong> Display the list of available commands
- <strong>colors</strong> Display the list of available colors
