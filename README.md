# FakeUSB - Make your own BadUSB device!

## What is this?
This project was done for my university thesis about Cybersecurity. We needed a tool to test our USB Firewall 
(built using USBProxy and a BeagleBone Black). 

This tool is based on [USBProxy](https://github.com/dominicgs/USBProxy) (and needs a BeagleBone Black), but instead of waiting for a real device on the "slave port", it emulates an USB device, which is described by its Descriptors (located into the ``config`` folder).

If you use a keyboard or a mouse, you can also make a script with some actions which will be ran when the fake device is 
connected and recognized by the host.

## Basic example

```
fakeusb -a keyboard -d keyboard
```

Will run the ``keyboard`` script (located in the ``attack`` folder) using a fake keyboard device.

## Available Devices

Current available devices are:
- ``keyboard``: emulates a keyboard
- ``mouse``: emulates a mouse
- ``multimedia``: emulates keyboard + mouse
- ``mass-storage``: emulates a mass storage device

## Available Commands

Available commands for attack scripts are:
- **Keyboard**
  - ``WRITE(EP) "string"``: writes the string ``string`` as if it has beed typed on the keyboard
  - ``PRESS_KEYS(EP) KEY1+KEY2+KEY3``: simulates a keyboard shortcut (max 3 chars) (ex. ALT+F4 or CTRL+ALT+CANC)
- **Mouse**
  - ``CLICK(EP) KEY``: simulates a click of the key ``KEY`` on the mouse
  - ``MOVE(EP) X,Y``: moves the pointer to ``X`` pixels horizontal and ``Y`` pixels vertical
  - ``MOVE_AND_CLICK(EP) KEY X,Y``: simulates a drag & drop
- **Generic**
  - ``DELAY(EP) TIME``: waits ``TIME`` ms before sending the next command
  
``EP`` is the IN endpoint which we want to use to send our commands.

Those commands can be used only with keyboards or mice. You can find some examples in the ``attack`` folder.

## Other Info

- You can see this tool in action in [this video](https://youtu.be/hkkYLQ1sCco?t=1m23s), while it tries to bypass our USB Firewall.
- More info about Design and Implementations can be found in my thesis chapter (in Italian): [link](https://www.docdroid.net/UqovCEx/selection.pdf).
- No, I won't do a documentation (in English) for this because I'm lazy.
