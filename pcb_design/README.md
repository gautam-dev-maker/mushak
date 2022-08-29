# PCB Design

The Mushak PCB design is made on the latest version of KiCAD 6.

Mushak is designed over STM32F405RG microcontroller accomodated eith various sensors and drivers for the micromouse.

### Major components used in the PCB include -

- STM32F405RG - Microcontroller
- VL6180x - ToF sensor4
- DRV8833 - Motor Driver
- MPU6500 - IMU
- SFH-4045N
- SFH-w


---




## IR sensors


The IR sensors include majorly two components, IR emitter which emits the Infrared light of a perticular wavelength which is not visible with the human eye. 

This light gets reflected back from the obstacle present in fromt of the robot to the photo transistor, when the Infrared light of this perticular wavelenght enters the phototransistor device it allows the current to flow from the collecotr to the emmiter of device. This current flow creates a voltage drop across the resistor connected with the photo transistor and this voltage drop is read by th ADC of the microcontroller which varies as the intensity of light reflected changes. By this way we can determine how far is the obstacle from the robot.

For controlling the intensity of the transmitted light we used N-Channel Mosfet whose gate is controlled by PWM signal given by the microcontroller.
By varying the PWM duty cycle we can control the intensity of the transmitted light from IR emmiter by which we can control the distance sensitivity.

4 Sensor pairs are being used in Mushak


