<p align="center">
  <img src="https://us.123rf.com/450wm/vectomart/vectomart1808/vectomart180800065/107422713-lord-ganpati-background-for-ganesh-chaturthi-festival-of-india-with-message-meaning-my-lord-ganesha.jpg" width="125" height="125" />
</p>

# Table of Content
- [Table of Content](#table-of-content)
- [About The Project](#about-the-project)
- [Firmware](#firmware)
  - [Pinout of STM32F405RG](#pinout-of-stm32f405rg)
    - [Communication Buses](#communication-buses)
  - [How To Use The Firmware](#how-to-use-the-firmware)
    - [Install gcc-arm-none-eabi compiler tool suite](#install-gcc-arm-none-eabi-compiler-tool-suite)
    - [Install ST-Link Toolchain](#install-st-link-toolchain)
    - [Build the source code](#build-the-source-code)
    - [Flash on the STM32 device](#flash-on-the-stm32-device)
- [Resources](#resources)
- [Acknowledgements](#acknowledgements)
- [License](#license)


# About The Project

Mushak is a micromouse maze solving bot. The aim of this project is to make India's first and smallest half-size micromouse bot. "Mushak" in hindi means "Mouse".

This micromouse consist of a very small coreless motors (6 mm width and 10 mm length) and PCB as it's base . Since coreless motors doesn't provide enough torque for the movements of bot we used gear box to increase the torque. 

# Firmware

The firmware for this project is written using STM32-HAL. STM32 HAL consist of bare-metal register address and light weight peripheral libraries giving programmer the full control. For running multiple tasks we are using [FreeRTOS](https://www.freertos.org). We will be storing the maze in flash memory, since page size is 2 bytes, we need to access 2 bytes at a a time. 

The firmware is required to interact with the following external sensors/peripherals: 

- [SFH-4045N](https://dammedia.osram.info/media/resource/hires/osram-dam-2496064/SFH%204045N.pdf) : Infrared Emitter for detecting walls.
- [SFH-3015-FA](https://dammedia.osram.info/media/resource/hires/osram-dam-2495986/SFH%203015%20FA.pdf) : Infrared Receiver to receive the emitted 
- [AS5600-ASOT](https://ams.com/documents/20143/36005/AS5600_DS000365_5-00.pdf) : Encoders to calculate the movements of wheel.
- [DRV8833](https://www.ti.com/lit/ds/slvsar1e/slvsar1e.pdf?ts=1661677330657&ref_url=https%253A%252F%252Fwww.google.com%252F) : Dual H-bridge Motor driver required to control motor.
- [MPU6500](https://invensense.tdk.com/wp-content/uploads/2020/06/PS-MPU-6500A-01-v1.3.pdf) : Gyroscope and Accelerometer sensor.
- [SMT-0440-S-R](https://www.puiaudio.com/media/SpecSheet/SMT-0440-S-R.pdf) : buzzer to generate the victory sound.
- [VL6180x](https://www.st.com/resource/en/datasheet/vl6180x.pdf) : Time of flight sensor to measure the distance between wall and bot.
- [HC-08](http://www.hc01.com/downloads/HC-08A%20version%20english%20datasheet.pdf) : Bluetooth module primarily used for logging. It uses 9600 baud rate for communication.

## Pinout of STM32F405RG

The below image shows the pinout of STM32F405RG that we will be using for this project :

![](assets/stm32f405rg_pinout.png)

### Communication Buses

- I2C (Inter-Integrated Circuit) : We use two encoders,so we needed to use two different I2C bus. Vl6180x (ToF) sensor also uses I2C bus communication, so overall 3 I2C bus communication is used.
- SPI (serial peripheral interface) : MPU6500 supports both I2C as well as SPI, but since SPI offers faster communication , we decided to go with it.
- UART (Universal Asynchronous Receiver/Transmitter): HC-08 bluetooth module , which will be used for logging functions on UART transmission protocol.


## How To Use The Firmware

### Install gcc-arm-none-eabi compiler tool suite

* ### For Linux 

```
sudo apt-get update
sudo apt-get install -y gcc-arm-none-eabi
```

* ### For MacOS
```
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```
### Install ST-Link Toolchain

* ST-Link toolchain is required for flashing on STM32 chip.

* ### For Linux
```
sudo apt-get update
sudo apt-get -y install stlink-tools
```

* ### For MacOS

```
brew install stlink
```

### Build the source code

* ### Clone the reponsitory and change the directory to firmware

```
git clone https://github.com/gautam-dev-maker/mushak.git
cd mushak/firmware
```

* ### Build using make tool
```
make -j4  # based on no. of logical cores available in your system
```

### Flash on the STM32 device

```
make flash  # Currently only st-link support is added
```

# Resources
* [Project Futura](http://micromouseusa.com/?page_id=1342) is a tutorial for micromouse designing
* [Micromouse Tyre](http://micromouseusa.com/?page_id=1342) a blog related how to select the tyres for micromouse
* [Phase-lead controller](https://micromouseonline.com/2011/05/16/designing-a-phase-lead-controller/) an alternative to PID control systems for micromouse.
* [Slip Compensation](https://robotics.stackexchange.com/questions/1658/implementing-slip-compensation-into-a-half-size-micromouse) discussion on how to compensate slip in micromouse which becomes a serious issue
* [Kerise v5](https://github.com/kerikun11/micromouse-kerise-v5) Smallest micromouse ever made, contains good documentation of design,pcb and code.

# Acknowledgements
* [SRA VJTI](http://sravjti.in) for providing us with required resources and support.
* [Ryotaro Onuki](https://github.com/kerikun11) for helping with all the designs and technical aspect of micromouse. His willingness to share his experience and answering our question has led to the start of the project.

# License
The [License](https://github.com/gautam-dev-maker/mushak/blob/master/LICENSE) Used for this Project.
