---
layout: page
title: "Architecture"
category: doc
date: 2015-10-27 10:38:29
---

## Architecture

This chapter will give a brief overview of our suggested architecture for a monolithic operating system in an embedded oriented environment. Important modules will be discussed in more detail in the following chapters.

## Structure of Fluxos

The architecture is split into a user and kernel section. The figure below gives an overview of all modules involved. 

![seq_scheduler](../images/Architektur.png)

For a cleaner seperation of user applications and operating system we introduced systemcalls. These methods either uses kernel internal methods or an abstraction called driver to run actions in the supervisor mode instead of the default user mode. A driver publishes a standardized interface for various modules like Audio, UART or the serial bus. The driver usually offers isolated functionality for a specific HAL module, but can also act on different HAL or kernel modules. 

The HAL is the closest unit to the hardware and access registers and modules on the operated board. There are the base or common methods (acting as interface) and the hardware specific implementation (for this project the OMAP3530). For reasons of simplification not all methods and constants used in the OMAP3530-HAL are available in the common HAL and need to be added in a feature release.

