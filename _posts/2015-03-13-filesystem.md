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

With these functions implemented, the whole application interface can be used, including `f_open`, `f_close`, `f_opendir` and all the rest.

To open and read a file following routine was used. Of course it has to improved in the way of handling directories and files, but at this time, it works to read a file.

```c
	// Fat file system object
	FATFS fs;
	fs.drv = 0;

	// Mount drive
	f_mount(&fs, "", 1);

	FILINFO fno;
	DIR dp;
	FRESULT res;
	unsigned ptr;
	
	// open and read root dir
	res = f_opendir(&dp, "/\0");
	if (res == FR_OK && f_readdir(&dp, &fno) == FR_OK) {
    	//TODO: file only in root path!
    	res = f_open(&file, fileName, FA_READ);
    	
    	if (!res) return;

    	ptr = f_tell(&file); // get read-pointer
    	f_read(&file, buffer, (uint32_t)bufferSize, &ptr); // read 4 byte
    	*bufferSize = ptr;
    	ptr = f_eof(&file); // check if eof is reached - if ptr != 0, go back
    	f_lseek(&file, 0); // set read-pointer to the beginning of the file

    	f_close(&file);
	}
	f_closedir(&dp);
```

### Loader
The loader copies data from a given address and size into a temporary buffer and creates a new process. The new process starts at a fixed address:

```c
#define CODE_START       0x00150000
#define CODE_MAIN_OFFSET 0xc
```

The buffered data is loaded into process space on the start address and its PC is set to the correct address. After this procedure the process state is set to `READY` and can be scheduled.  

### Work in progress

+ Detection of not only SD Card

We only implemented the detection of a SD 1.x card. Other card types or versions like a SDIO card or a SD 2.x card have to be integrated into the process of detection.

+ Write SD

Currently only blocks of MMCs can be read, but not be written. Therefore the writing function `mmcsd_write_block_data` has to implemented.

+ Time function

The function `get_fattime` has to be modified to fetch always the current time. At the moment it is using the same time.

+ Loader

Currently our loader can just handle one process, because only one buffer exists.