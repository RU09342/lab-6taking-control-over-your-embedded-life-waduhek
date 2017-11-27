# Lab 6: Percision Control
## PWM Part 2
In the folder "Lab6PercisionControlPWM" are the files which use a Pulse Modulated Signal to generate a constant voltage. A low pass filter was made and the output of this can be seen in the images bellow. ALl of these images are from the unloaded circuit.

![LowPass25](PWM_LowPass25_NL.png "PWM Low Pass 25% Duty Cycle")
![LowPass50](PWM_LowPass50_NL.png "PWM Low Pass 50% Duty Cycle")
![LowPass25](PWM_LowPass75_NL.png "PWM Low Pass 75% Duty Cycle")

When a load was applied the signal was pulled low so we needed to create an active low pass filter, after the active low pass filter was created the bellow images was taken with loads of 100, 1k, 10k, and 100k.

![LowPass75_100](PWM_LowPass75_100.png "PWM Low Pass 75% Duty Cycle 100 load")
![LowPass75_1000](PWM_LowPass75_1000.png "PWM Low Pass 75% Duty Cycle 1k load")
![LowPass75_10000](PWM_LowPass75_10000.png "PWM Low Pass 75% Duty Cycle 10k load")
![LowPass75_100000](PWM_LowPass75_100000.png "PWM Low Pass 75% Duty Cycle 100k load")

## R2R Ladder
An R2R ladder was made so that digital signals could be used to create an analog voltage. The voltage can be seen in a ramp and triange wave form both loaded and unloaded.

![R2R_Ramp_NL](R2R_Ramp_NL.png "R2R Ramp No Load")
![R2R_Ramp_100](R2R_Ramp_100.png "R2R Ramp 100 ohm Load")
![R2R_Triangle_NL](R2R_Triangle_NL.png "R2R Triangle No Load")
![R2R_Triangle_100](R2R_Triangle_100.png "R2R Triangle 100 ohm")


# Lab 6: Precision Control
Some applications require large amounts of voltage or current, so switching techniques must be used in order to provide the desired output. Other cases however require a finer control over the voltage or current going into them (some even require a control over resistance). So far you have looked at PWM to control the brightness of an LED, is there a way to use this to output a specified voltage or current, or even a specific waveform?

## PWM Part 2
Since you already have the code to perform PWM, then really, the software side of this part of the lab is fairly easy. You need to design a system which can take in a PWM duty cycle over something like UART (or you could have your system read in the position of a potentiometer), and produce that signal on a GPIO. The interesting part comes in when I want the output of your system to be an Analog voltage. In this case, a PWM with a 50% duty cycle should produce roughly Vcc/2 volts. This part of the lab should be done with the MSP430F5529 and the physical circuit should be constructed of an active Low-Pass Filter.

## R2R DAC
What if your system is noise sensitive or possibly needs more precision than just a PWM signal, you might need to look into using an actual Digital-to-Analog converter. One of the simplest DAC architectures is a R2R ladder. Using the MSP430F5529, you need to generate an 8-bit R2R ladder circuit that can produce "255" voltages between 0V and Vcc. Now how are you actually going to test this, cause I am sure you aren't going to measure 255 voltages on the DMM. You should set up your F5529 so it generates a staircase using a binary counter and then record on the oscilloscope the resulting waveform.

## Loading Effects
Obviously you are going to be making this type of circuitry to drive something. This introduces the idea of loading effect, wherein your circuit will perform differently based on what is going to be attached to it. For each of these implementations, try placing a variety of resistors from 100 ohms up to see what happens to your output signal and comment on what is happening.

## Deliverables
Along with what was asked in each of the parts above, for each implementation, you need to generate at least one triangle wave from your microntroller. This can be done by simply incrementing and decrementing values that are being sent to your circuit. You need to measure the output of each one of these along with taking the FFT on the scope of each one. The span on the FFT will need to go from 1kHz to about 50kHz if possible. You then need to compare the integrity of each signal by analyzing the difference in frequency components.

The README for this part is going to be mainly about the results of your measurement along with information on the implementation. You need to also talk about how you generated the triangle wave, but do not give me a dissertation on it. Since this is going to be talking about hardware, you need to place in the README a Bill Of Materials listing all hardware used as well as link to a Digikey cart which contains the parts needed in the right quantity. You do not need to include things like your F5529 or the breadboard or wires.