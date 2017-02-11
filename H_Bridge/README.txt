Can probably take out resistors R1 through R6. Don't think that they are needed since the L298 uses BJTs, which are based on the amount of current to the base (versus MOSFETs, which are based on the voltage applied to the gate).
The reason that they were originally there is to help and make sure that no "rogue" signals from the master triggered them to go high, but because the inputs to the L298 are logic gates, the input can already be thought of as high impedance.

R7 and R8 are the sense resistors. They are at only 1 Ohm because the processor's ADC converts voltages between 0 and 3.6V, and I'm assuming a peak current of 3 Amps (even though the stall current is 5 Amps, but I'm going to assume
for now that we wont run into that). So that means 3.6V/3A=1.2 which is close enough to 1A. And I found some 10 Watt resistors, so too much power shouldn't be an issue.

The LEDs are to indicate the state of the inputs into the H bridge.

The diodes are flyback diodes. They protect against back EMF and the effect of inductive loads (and a motor is basically a giant inductor).
The capacitors are kind of big because of the LEDs and diodes. They are meant to smooth out any noise and deal with power spikes.

The capacitors are at 0.22uF because of the other all the components in the design (LEDs, temp. sensor, etc...). These can be upped to a higher standard value if needed, at least on the Vdd line, since that's where all of the other "extra"
components are.

The resistor on the OS input of the is a pull-up resistor. According to the data sheet, a pull-up resistor (or at least a tie to high voltage) is needed in order to get an output from the temperature sensor's interrupt line. If it does not have this,
no high signal can be read from it (it's essentially useless, then). Also, on the temperature sensor, I left the inputs A2, A1, and A0 unconnected. This is because it looks like they are a way to address the sensor in a parallel method. However, 
this can be be done in a serial method over the I2C line (SD, which I named as the short for "serial data"). This is to reduce the amount of data lines needed.

Also, because we are doing PCB, I don't think that we need to worry about input/output pins. It just did it as though we were going to screw terminals. If we were doing it as pins that need to go to another circuit (such as for simulation), then
the screw terminals would be replaced with inputs/output ports.