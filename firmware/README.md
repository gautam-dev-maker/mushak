# HOW TO USE THE FIRMWARE

## Install gcc-arm-none-eabi compiler tool suite

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
## Install ST-Link Toolchain

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

## Build the source code

* ### Clone the reponsitory and change the directory to firmware

```
git clone https://github.com/gautam-dev-maker/mushak.git
cd mushak/firmware
```

* ### Build using make tool
```
make -j4  # based on no. of logical cores available in your system
```

## Flash on the STM32 device

```
make flash  # Currently only st-link support is added
```
