# RMInternalF103DevBoard
This is the repository showcasing the usage of ChibiOS on this cheap STM32F103C8T6 board equiped with a IMU, usb to serial converter and CAN bus transceiver. Implementation of common functions and peripherals used in the RoboMaster contest in the team ENTERPRIZE will be demoed here.

![alt text](https://github.com/alexacw/RMInternalF103DevBoard/blob/master/image.jpeg)

## Toolchain Setup (TBD)
### Common
1. Download and unzip ChibiOS 18.2.1 from https://sourceforge.net/projects/chibios/files/ChibiOS%20GPL3/Version%2018.2.1/

2. Move the ChibiOS folder to wherever you like, it also contains many demos showing the implementation of its RTOS features and supported HAL functions in different MCUs. 

### Linux (Ubuntu)
1. Install the arm-gcc compiler by running `sudo apt install gcc-arm-none-eabi` OR download and install the newest version from ARM's website, install by copying the containing files of the zipped folder to /usr/, sudo is required.

2. Create a symbolic link in the root directory of the cloned repository by `ln -s [path to your donwloaded Chibios] ./ChibiOS_18.2.1`

### Windows (TBD)

## Building and Debugging
1. Use `make -j4` in the root folder to compile, where "-j" specify the number of threads (4 here) used for compiling.

### Debugging with Ozone and jLink
1. Install SEGGER Ozone
2. Open the ozoneProj.jdebug project and connect the debugger via the SWD pins
3. Try it yourself
4. For new projects in Ozone, choose the correct MCU (STM32F103C8) and interface (SWD, USB, 1MHz), then open up the built .elf file (RMInternalDev.elf).


### Debugging with Openocd and STLink (TBD)

btw, VSCode is recommanded as the code editor and SEGGER Ozone with jLink as debugger.

## Demoed features (TBD)