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

- Sensorbox (1)
- PHOENIX CONTACT EEM-350-D-MCB (2)
- Finder 7E.78.8.400.0212 (3)
- Eastron SDM630 (4)
- Custom (5)

# Modbus registers

Baudrate is 9600bps 8N1 (8bit, no parity bit, one stop bit)<br>
With the SmartEVSE in normal mode (not load balancing master/slave) it will listen to slaveID = 0 (broadcast address) or slaveID=1<br>
All registers are 16 bit unsigned integers.<br>

## Register 0x0*: Broadcast Chargecurrent or Error

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x01 | W | Broadcast ChargeCurrent | 0.1 A | Charge Current (0 A = no current available)
0x02 | W | Broadcast Error | Bit | 1:LESS_6A / 2:NO_COMM / 4:TEMP_HIGH / 8:NO_CURRENT / 16:RCD / 32:NO_SUN

Register 0x01 is written to every 2 seconds by the Master, and holds the Charge Current per Slave EVSE. A total of 4 words (8 bytes) are written, One word per EVSE(0-3).<br>
Register 0x02 is written to only if an error occurred.<br>
Use function code 0x10 (Preset Multiple Registers), and broadcast address 0x00 to use this feature.<br>

## Register 0x008*: Ack State change

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x0082 | W | Ack State A->B | 0.1 A | Charge Current (0 A = no current available)
0x0083 | W | Ack State B->C | 0.1 A | Charge Current (0 A = no current available)

Register 0x82 and 0x83 are written by the Master to tell the Slave that it recognised a state change request.<br>
If ok, the value of the register contains the initial chargecurrent (usually 6.0 A)<br>
Use function code 0x06 (Preset Single Register) to write to these registers.<br>
The Slave addresses are Slave nr +1 (2-4).<br>

## Register 0x00A*: Current state

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x00A0 | R | State | | A-D (EVSE State), E-H (A-D Waiting for Balance Master)
0x00A1 | R | Error | Bit | 1:LESS_6A / 2:NO_COMM / 4:TEMP_HIGH / 8:NO_CURRENT / 16:RCD / 32:NO_SUN
0x00A2 | R | Maximum charging current | A |
0x00A3 | R | Minimum charging current | A |
0x00A4 | R | Number of used phases (Not implemented) | | 0:Undetected / 1 - 3
0x00A5 | R | Real charging current (Not implemented) | 0.1 A |
0x00A6 | R/W | Charging current | 0.1 A | 6 - 80
0x00A7 | R/W | Access bit | | 0:No Access / 1:Access
0x00A8 | R/W | EVSE mode (without saving) | | 0:Normal / 1:Smart / 2:Solar

## Register 0x00C*: Configuration

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x00C0 | R/W | Configuration | | 0:Socket / 1:Fixed Cable
0x00C1 | R/W | Load Balance (Also address of the device ) | | 0:Disable / 1:Master / 2-4:Slave
0x00C2 | R/W | Minimal current the EV is happy with | A | 6 - 16
0x00C3 | R/W | Cable Current limit | A | 13 - 80
0x00C4 | R/W | Cable lock | | 0:Disable / 1:Solenoid / 2:Motor
0x00C5 | R/W | Surplus energy start Current | A | 1 - 16
0x00C6 | R/W | Stop solar charging at 6A after this time | min | 0:Disable / 1 - 60
0x00C7 | R/W | External Switch on IO2 | | 0:Disable / 1:Access Push-Button / 2:Access Switch / 3:Smart-Solar Push-Button / 4:Smart-Solar Switch
0x00C8 | R/W | Residual Current Monitor on IO3 | | 0:Disable / 1:Enable

## Register 0x00E*: Load balancing configuration (same on all SmartEVSE)

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x00E0 | R/W | Max Charge Current of the system | A | 10 - 80
0x00E1 | R/W | EVSE mode | | 0:Normal / 1:Smart / 2:Solar
0x00E2 | R/W | Max Mains Current | A | 10 - 100
0x00E3 | R/W | CT calibration value | 0.01 | Multiplier
0x00E4 | R/W | Type of Mains electric meter | | *
0x00E5 | R/W | Address of Mains electric meter | | 5 - 255
0x00E6 | R/W | What does Mains electric meter measure | | 0:Mains (Home+EVSE+PV) / 1:Home+EVSE
0x00E7 | R/W | Type of PV electric meter | | *
0x00E8 | R/W | Address of PV electric meter | | 5 - 255
0x00E9 | R/W | Byte order of custom electric meter | | 0:LBF & LWF / 1:LBF &  HWF / 2:HBF & LWF / 3:HBF & HWF
0x00EA | R/W | Register for Current of custom electric meter | | 0 - 255
0x00EB | R/W | Divisor for Current of custom electric meter | 10<sup>x</sup> | 0 - 7 / 8:double

 * Number in brackets in section "Predefined electric meters"
