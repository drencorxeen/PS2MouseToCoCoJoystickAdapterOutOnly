# PS2MouseToCoCoJoystickPortAdapterOutOnly
This project is design to take a PS/2 Mouse and convert it to be usable on a Tandy Radio Shack Color Computer's joystick port.

## Requirements
* 1x ATMega328P-PU chip DIP
* 1x Optional .300" wide 2.54mm pin spaced 28PIN Machine Socket Gold Plated
* 1x MCP4922 DIP
* 1x Optional .300" wide 2.54mm pin spaced 14PIN Machine Socket Gold Plated
* 1x 16MHz or 18.4323MHz crystal(If using 18.432MHz Crystal you will need MiniCore)
* 2x 1N4148 Diones
* 1x 10KΩ Resistor
* 2x 4.7KΩ Resistors
* 2x 470Ω Resistors
* 2x 3mm LEDs
* 2x 0.1µF Ceramic Capacitors
* 2x 22pF Ceramic Capacitors
* 1x MAB6H 6PIN DIN PCB Jack
* 1x mini B female USB connector THT PCB style (for receiving external +5V power)
* 1x 6PIN mini DIN female connector THT PCB style (for connecting PS/2 mouse)

You will also need the PS2Mouse Arduino library by Jacek Kunicki on Github:
https://github.com/rucek/arduino-ps2-mouse

You will also need the MCP49xx Arduino library by Thomas Backman on Github:
https://github.com/exscape/electronics/tree/master/Arduino/Libraries/DAC_MCP49xx

## Disclaimer

As with all DIY electronics projects, this project comes with absolutely no warranty and will not be held liable for any loss or damage caused to your Arduino or vintage PC you used this adapter on. Use at your own risk!