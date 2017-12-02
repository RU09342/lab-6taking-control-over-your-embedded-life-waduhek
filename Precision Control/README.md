# Lab 6: Percision Control
## PWM Part 2
In the folder "Lab6PercisionControlPWM" are the files which use a Pulse Modulated Signal to generate a constant voltage. A low pass filter was made and the output of this can be seen in the images bellow. All of these images are from the unloaded circuit.

![LowPass25](PWM_LowPass25_NL.png "PWM Low Pass 25% Duty Cycle")
![LowPass50](PWM_LowPass50_NL.png "PWM Low Pass 50% Duty Cycle")
![LowPass25](PWM_LowPass75_NL.png "PWM Low Pass 75% Duty Cycle")

When a load was applied the signal was pulled low so we needed to create an active low pass filter, after the active low pass filter was created the bellow images was taken with loads of 100, 1k, 10k, and 100k.

![LowPass75_100](PWM_LowPass75_100.png "PWM Low Pass 75% Duty Cycle 100 load")
![LowPass75_1000](PWM_LowPass75_1000.png "PWM Low Pass 75% Duty Cycle 1k load")
![LowPass75_10000](PWM_LowPass75_10000.png "PWM Low Pass 75% Duty Cycle 10k load")
![LowPass75_100000](PWM_LowPass75_100000.png "PWM Low Pass 75% Duty Cycle 100k load")

## R2R Ladder
An R2R ladder was made so that digital signals could be used to create an analog voltage. The voltage can be seen in a ramp and triange wave form both loaded and unloaded. The triangle wave was created by incrementing and decrementing a port of outputs on the MSP430F5529.

![R2R_Ramp_NL](R2R_Ramp_NL.png "R2R Ramp No Load")
![R2R_Ramp_100](R2R_Ramp_100.png "R2R Ramp 100 ohm Load")
![R2R_Triangle_NL](R2R_Triangle_NL.png "R2R Triangle No Load")
![R2R_Triangle_100](R2R_Triangle_100.png "R2R Triangle 100 ohm")

