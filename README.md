SmartEVSE
=========

Smart Electric Vehicle charging controller


What is it?

It's a Smart EVSE, initially build to charge a Tesla Roadster at home. But improved to now also work with three phase charging (Model S).
The smart bit is, that it actively measures the current consumption of all appliances at home, and automatically lowers or increases the charging current to the EV.

The project consists of two parts:
- Smart EVSE controller
- Sensorbox with CT's

The Smart EVSE controller connects to the EV, but also to the Sensorbox over a serial RS485 connection.
features:
- Fits into a standard DIN rail enclosure.
- switched 230VAC output, for contactor/relais.
- selects current capacity of the connected cable automatically (13/20/32/63A)
- locking solenoid support,locks the charging cable in the socket, automatically unlocks on a power failure.
- buildt-in temperature sensor. 
- optionally supports a 8x2 character LCD.
- setup through serial command line interface, things like MAX/MIN Current can be set.
- easy to upgrade through serial bootloader.

The Sensorbox, has inputs for Current Transformers(CT's). These CT's measure the current draw per phase, and send this information over a RS485 connection to the Smart EVSE controller.
The Sensorbox should be installed in the area where the MAINS connection enters your house. Just click the CT's on the (three) Phase wires, and run a serial wire (CAT5 works fine) to the EVSE controller.

The project folders are as follows:
/bootloader		microchip bootloader software
/cad			3D openscad files for the sensorbox
/eagle			schematics and pcb layouts for the EVSE controller, Sensorbox (3PCurrentTX), and optional LCD
/pictures		pictures of the boards, my single phase Roadster EVSE, and Three phase EVSE
/sourcecode		Microchip MPLAB sourcecode and HEX files.

For questions i can be reached on www.teslamotorsclub.com forum as "Fuzzylogic"

