---
layout: page
title: "Filesystem"
category: doc
date: 2014-01-09 11:10:09
---

## Filesytem

### Card detection
For detecting multi media cards, the basic programming guide of the OMAP35x technical reference manual was implemented (MMC/SD/SDIO). It should be noticed here that time is a important factor. It can happen that a SD card cannot be identified immediately, sometimes a process has to be repeated. This model includes initialization, detection, status, read and write functionality of MMCs.

### FAT16
["FatFs"](http://elm-chan.org/fsw/ff/00index_e.html) was used, to integrate FAT16. It is a generic FAT file system module and is completely decoupled from our system. Therefore the following functions had to be implemented to connect the module to our system:

```
disk_status - Get device status
disk_initialize - Initialize device
disk_read - Read sector(s)
disk_write - Write sector(s)
disk_ioctl - Control device dependent features
get_fattime - Get current time
```

### Work in progress

+ Detection of not only SD Card

We only implemented the detection of a SD 1.x card. Other card types or versions like a SDIO card or a SD 2.x card have to be integrated into the process of detection.

+ Write SD

Currently only blocks of MMCs can be read, but not be written. Therefore the writing function `mmcsd_write_block_data` has to implemented.
