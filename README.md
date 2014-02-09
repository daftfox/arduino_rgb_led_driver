arduino_rgb_led_driver
======================

Source for the RGB LED driver

So far it listens on the serial port for user generated commands.

The available commands are:
- 'color <color> <opt: side (l/r default by omision is both sides)>
    Set the color of the LEDs
- 'police'
    Whee hoo whee hoo
- 'pulse <color> <opt: speed (in ms. default is 500ms)>'
    Pulses the LEDs between two colors indefinitely or until the 'off' command is received
- 'fade <from> <to> <opt: step speed (in ms. default is 20ms)>'
    Fades the LEDs from one to the other color
- 'strobe <color> <opt: speed (in ms. default is 200ms)>'
    Strobes the LEDs indefinitely or until the 'off' command is received
- 'rainbow'
    Let out your inner gay self
- 'off'
    Turn off the LEDs/Cancel animation
- 'info'
    Display the list of available commands
- 'colors'
    Display the list of available colors
