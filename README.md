SmartEVSE
=========

Smart Electric Vehicle Charge Controller

![Image of SmartEVSE](/pictures/SmartEVSEv2_small.jpg)

# What is it?

It's a Open Hardware and Software EVSE (Electric Vehicle Supply Equipment). It supports 1-3 phase charging, fixed charging cable or charging socket. Locking actuator support (3 different types). And it can directly drive a mains contactor for supplying power to the EV. It features a display from which all module parameters can be configured.<br>
Up to 4 four modules can be connected together to charge up to four EV's from one mains connection without overloading it.<br>
Smart Charging is done with the (optional) sensorbox and CT's (Current Transformers) which can be directly connected to the controller's RS485 pins.

The project consists of two parts:
- Smart EVSE controller
- Sensorbox with CT's

# Features

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

New V2.04 features are:
- Access control option added, allows for (RFID) locks to be connected, and usage of the Charging stating to be restricted.
- Status LED option, will indicate charging/stopped/ready to charge and error conditions to be visible.

# The Sensorbox

The Sensorbox should be placed where the Mains connection enters the building. Usually just after the kWh meter, this way it will be able to measure the total current per phase and send this information to the SmartEVSE.

In order to measure the current, Current transformers are used. Clip them around the L1,L2 and L3 wires, and plug the other end into the sensorbox.

![Image of Sensorbox](/pictures/sensorbox.jpg)

# Configuring the SmartEVSE

The SmartEVSE has a display, which shows the charging status, and if smart mode is used, also the measured current per phase.
It is possible to configure all settings using the built-in menu.

Three buttons below the display are used for navigating the menu.

Hold the center button for 2 seconds to enter the menu.
You can now use the left and right buttons to go to the different menu options. 

Pressing the center button, selects the option, and allows you change the value (for example change the charging current)

![Image of Menu](/pictures/SmartEVSEv2_mode_smart.jpg)

# Building the EVSE

In order to build a complete EVSE (charging station)
you will need:

- SmartEVSE.
- 4 pole NO Contactor rated for the max charging current. (for example Hager ESC440)
- Fixed charging cable or socket with locking solenoid.
- Enclosure with DIN rail. (for example Famatel type 3958)
- Terminal blocks (Wago TOPJOB S)

The EVSE needs to be protected with a circuit breaker and residual-current circuit breaker, usually located near or in the distribution board.

# Purchase the SmartEVSE

You can buy a complete assembled SmartEVSEv2 [here](http://www.stegen.com/en/ev-products/66-smart-evse-controller.html)<br>
Information on how to compile and setup the controller can be found on the [smartevse.nl](http://www.smartevse.nl) website.

**The assembled SmartEVSE v2 modules come pre-programmed with the latest firmware, and are fully tested!**

# Predefined electric meters

- Sensorbox v1 (1, 2)
- PHOENIX CONTACT EEM-350-D-MCB (3)
- Finder 7E.78.8.400.0212 (4)

# Modbus registers

## Register 0xC*: Configuration

Register | Description | Values
--- | --- | ---
0xC0 | Configuration | 0:Socket / 1:Fixed Cable
0xC1 | Load Balance | 0:Disable / 1:Master / 2-4:Slave
0xC2 | Minimal current the EV is happy with | (A)
0xC3 | Fixed Cable Current limit | (A)
0xC4 | Cable lock | 0:Disable / 1:Solenoid / 2:Motor
0xC5 | Surplus energy start Current | (A)
0xC6 | Stop solar charging at 6A after this time | (min)
0xC7 | External Start/Stop button | 0:Disable / 1:Enable
0xC8 | Residual Current Monitor | 0:Disable / 1:Enable

## Register 0xE*: Load balancing configuration (same on all SmartEVSE)

Register | Option
--- | --- | ---
0xE0 | Max Charge Current of the system | (A)
0xE1 | EVSE mode | 0:Normal / 1:Smart / 2:Solar
0xE2 | Max Mains Current | (A)
0xE3 | CT calibration value | Multiplier
0xE4 | Type of Mains electric meter | *
0xE5 | Address of Mains electric meter | 5-255
0xE6 | What does Mains electric meter measure | 0: Mains (Home+EVSE+PV) / 1: Home+EVSE
0xE7 | Type of PV electric meter | *
0xE8 | Address of PV electric meter | 5-255

* Number in brackets in section "Predefined electric meters"
