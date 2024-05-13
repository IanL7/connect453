# Connect453
*Capstone project for ECE 453 at UW Madison - Fall 2023*

Connect453 is a machine that accepts a standard Connect 4 game board, and allows for remote play from a laptop (over BLE) for one of the two players.

This repository contains the software and hardware designed for the project.

## Photos
![](images/connect453_coe_blog.jpeg)
- Taken from UW Madison COE Blog (https://engineering.wisc.edu/blog/ece-453-students-present-capstone-projects-at-open-house/)
![](images/connect453_back_view.jpeg)
- View of internals

## Team members and roles
- Ian Lodes
    - Software for the microcontroller board (game logic and BLE integration) 
    - Hardware design of the microcontroller board and the relay board
    - Contribution to the UI (BLE integration and testing)
- Christian VanIten
    - Software for the microcontroller board (I2C communication with the light sensor board)
    - Hardware design of the light sensor board
    - Fabrication of the machine front panel and game piece dropper
- Billy Begos
    - Software for the Rasberry Pi (image capture and UART communication)
    - Software for the UI
- Avery Dolinsek
    - Hardware design of the motor driver board
    - Construction of the machine frame base

## Software frameworks used
- FreeRTOS
- [Infineon 'bless' BLE stack](https://github.com/Infineon/bless)
- [Infineon HAL](https://github.com/Infineon/mtb-hal-cat1)

