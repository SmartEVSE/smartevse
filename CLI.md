CLI
====
SmartEVSE has a CLI available via a FTDI/UART/TTL interface.

To use the CLI you needa USB to TTL cable, a TTL-232R-3V3 would work. But a 5V cable also works.

The pin layout on the board (for version 1: underneath the LCD) is:
* Ground
* Unused
* Unused
* RXD
* TXD
* Unused

Version 2.x of the SmartEVSE has the 6 pin 'header' just behind the 12 pin pluggable conenctor.

In your terminal use these settings:
* 115200 8N1
* No hardware of software flow control

Hit enter a couple of times and you should get a menu.
