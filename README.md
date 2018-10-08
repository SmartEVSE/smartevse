SmartEVSE
=========

Smart Electric Vehicle Charge Controller

![Image of SmartEVSE](/pictures/SmartEVSEv2white.jpg)

<h1>What is it?</h1>

It's a Open Hardware and Software EVSE (Electric Vehicle Supply Equipment). It supports 1-3 phase charging, fixed charging cable or charging socket. Locking actuator support (3 different types). And it can directly drive a mains contactor for supplying power to the EV. It features a display from which all module parameters can be configured.
Up to 4 four modules can be connected together to charge up to four EV's from one mains connection without overloading it.
Smart Charging is done with the (optional) sensorbox and CT's (Current Transformers) which can be directly connected to the controller's RS485 pins.

The project consists of two parts:
- Smart EVSE controller
- Sensorbox with CT's


New V2.04 features are:
- Access control option added, allows for (RFID) locks to be connected, and usage of the Charging stating to be restricted.
- 12V status LED, will indicate charging/stopped/ready to charge and error conditions to be visible.

<h1>Features</h1>:

- Fits into a standard DIN rail enclosure.
- Measures the current consumption of other appliances, and automatically lowers or increases the charging current to the EV. (sensorbox required)
- The load balancing feature let's you connect up to 4 SmartEVSE's to one mains supply.
- Switched 230VAC output, for contactor/relay.
- Powered RS485 communication bus for sensorbox.
- Can be used with fixed cable, or socket and charging cable.
- Automatically selects current capacity of the connected cable (13/16/32/63A)
- Locking actuator support, locks the charging cable in the socket, automatically unlocks on a power failure.
- Built-in temperature sensor.
- All module parameters can be configured using the display and buttons.
- Setup can also be done through serial CLI.
- Firmware upgradable through serial bootloader. 

<h1>The Sensorbox</h1>

The Sensorbox should be placed where the Mains connection enters the building. Usually just after the kWh meter, this way it will be able to measure the total current per phase and send this information to the SmartEVSE.
In order to measure the current, Current transformers are used. Clip them around the L1,L2 and L3 wires, and plug the other end into the sensorbox.

![Image of Sensorbox](/pictures/sensorbox.jpg)


The project folders are as follows:
/bootloader		microchip bootloader software
/cad			3D openscad files for the sensorbox
/eagle			schematics and pcb layouts for the EVSE controller, Sensorbox (3PCurrentTX), and optional LCD
/pictures		pictures of the boards, my single phase Roadster EVSE, and Three phase EVSE
/sourcecode		Microchip MPLAB sourcecode and HEX files.


