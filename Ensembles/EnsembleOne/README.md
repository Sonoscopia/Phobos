# Ensemble-ONE 

## Position 
Left Table

### Instruments 
a) cans struck by solenoids (x3)
b) woodblock struck by solenoids (x2)
c) shaker cans struck by solenoids (x3)
d) mousse metal coups struck by DC motors (x4)

## Electrical specifications

### Current drain 
a) 3 x 300mA = 900mA @12V
b) 2 x 300mA = 600mA @12V
c) 3 x 300mA = 900mA @12V
d) 4 x 500mA = 2A @5V
TOTAL(12V) = 0.9 + 0.6 + 0.9 = 2.4 Amps 
TOTAL(5V) = 2 Amps
TOTAL = 4.4 Amps  

### Current drain (Measured)
a) 
b) 
c) 
d) 
TOTAL(12V) =  
TOTAL(5V) = 
TOTAL =   



## BOM (BILL OF MATERIALS)

## CIRCUIT

1 x Arduino Mega 2560
8 x Solenoids - Gangbei 0530 (push/pull type, 12V, 5N, 300mA)
4 x DC motors (500mA)
8 x TIP120 (darlington transistor)
4 x IRF510 MOSFET
12 x 1N4007 Diode
12 x 1KΩ 1/4W Resistor
1 x LM7812 Voltage Regulator (or LM7815)
3? x LM7805 Voltage Regulator
4 x 0.22µF Ceramic Capacitor (224 - 220nF)
4 x 0.1µF Ceramic Capacitor (104 - 100nF)
4 x 220µF 25V Electrolytic Capacitor 

### Addiotional BOM (for LED's)
?12 x 2KΩ 1/4W (or 1/8W) 
?12 x 100Ω 1/4W (or 1/8W)
?12 x BC547 
12 x 220Ω 1/4W (or 1/8W)

### HUB (to centralize 4 instruments and send to Arduino)
- 8 x RJ45 female sockets for IC
- 4 x RJ45 Cables (ethernet)
- 1 x SCART or DP25 (also called sub-D or Nextron) female socket
- 1 x SCART or DP25 cable (male-male)

### Arduino case
- 1 x SCART or DP25 socket (female)
- 2 x female multipin socket (input + out thru power)
- 1 x female xlr socket (midi in)
- 1 x male xlr socket (midi out/thru)

### Cables
- 4 x RJ45
- 1 x multipin male-male
- 1 MIDI(male) to XLR(male)

