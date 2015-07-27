---
layout: page
title: "UART"
category: doc
date: 2014-01-04 11:10:09
---

## UART
Universal Asynchronous Receiver Transmitter is a serial port which allows the MC to communicate with other devices such as input sensors, LCDs or other computers. Sending the data is done bit by bit with a so called baud rate. This rate is the total amount of bits transmitted per second (bits/sec). The UART does not has any clock - thats why it is asynchronous.

+ General rule of thumb for baud rates: 9600
+ RxD (Receive)
+ TxD (Transmit)
+ Gnd

### Usage of UART
To use the UART (for example sending data via RS232) the `uart_driver_init()` method of the UartDriver must be called with the baudrate as parameter. The driver then initializes the UART1 module with a length of 8 bits for each received or sent packet. Currently it is not possible to change this setting.

The UART driver offers two methods to interact with devices connected to the UART interface: write and read. The console is based on these two methods to react to the user input and peform actions based on it. To do so, the console and the UART driver are using Semaphores for a proper communication manner. See Inter-Process Communication chapter for more details.

`uart_driver_write`  
This method will write the given data packet-wise to the UART registers. It is based on a simple loop iterating over the data and calling the uart_write method of our HAL.

`uart_driver_read`  
This method will read character by character from the UART device. There is also a check if all data was received in one go. If not, the uart_driver_interrupt will take care of the rest of the data later on.

### RS232
The RS232 communication sends either 1, called a *Mark* or 0, called *Space*. During idle state the communication continiously transmit 1.
