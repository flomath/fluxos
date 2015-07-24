---
layout: page
title: "Overview"
category: doc
date: 2014-01-01 11:10:09
---

## Overview

### Requirements of the project

- Monolithic kernel
- Virtual memory management
  - provide MMU functionality
  - each process owns a seperate virtual area
  - each process acts the same way (as how it see's virtual memory)
- Simple process scheduling
- Single processing
  - no multithreading needed
  - only Processes, no Threads
- System calls
  - library with basic methods
- PCB (Process Control Block)
  - Information about process (esp. state)
  - own Stack for each process
- Driver architecture
  - define a architecture to use different modules of the board
- Interrupt-based UART
  - simple read and write functionality
- IPC
  - Semaphores
- Console application
  - a simple console to execute processes
- Memory card
  - use of external memory to load process/data
  
