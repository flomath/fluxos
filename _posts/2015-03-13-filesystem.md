---
layout: page
title: "File System"
category: doc
date: 2015-09-13 11:10:09
---

## The file system
For our operating system we are implementing a simple FAT16 file system using the built-in sdcard slot. 

### FAT structure
A fat partition is divided into different regions lsited below:

+ the boot sector
+ reserved sectors (opt.)
+ the FAT (file allocation table)
+ copy of FAT
+ root directory
+ data region

#### boot sector
The boot sector or boot block hold data to describe the file system. Below is a list of the most important data. 

|Offset from start|	Length|	Description|
|---|---|---|
|0x00	|3 bytes|	Part of the bootstrap program.|
|0x03	|8 bytes|	Optional manufacturer description.|
|0x0b	|2 bytes|	Number of bytes per block (almost always 512).|
|0x0d	|1 byte|	Number of blocks per allocation unit.|
|0x0e	|2 bytes|	Number of reserved blocks. This is the number of blocks on the disk that are not actually part of the file system; in most cases this is exactly 1, being the allowance for the boot block.|
|0x10	|1 byte	|Number of File Allocation Tables.|
|0x11|	2 bytes|	Number of root directory entries (including unused ones).|
|0x13|	2 bytes|Total number of blocks in the entire disk. If the disk size is larger than 65535 blocks (and thus will not fit in these two bytes), this value is set to zero, and the true size is stored at offset 0x20.|
|0x15	|1 byte|	Media Descriptor. This is rarely used, but still exists.|
|0x16|	2 bytes|	The number of blocks occupied by one copy of the File Allocation Table.|
|0x18	|2 bytes	|The number of blocks per track. This information is present primarily for the use of the bootstrap program, and need not concern us further here.|
|0x1a	|2 bytes	|The number of heads (disk surfaces). This information is present primarily for the use of the bootstrap program, and need not concern us further here.|
|0x1c|	4 bytes	|The number of hidden blocks. The use of this is largely historical, and it is nearly always set to 0; thus it can be ignored.|
|0x20|	4 bytes|	Total number of blocks in the entire disk (see also offset 0x13).|
|0x24	|2 bytes|	Physical drive number. This information is present primarily for the use of the bootstrap program, and need not concern us further here.|
|0x26|	1 byte|	Extended Boot Record Signature This information is present primarily for the use of the bootstrap program, and need not concern us further here.|
|0x27	|4 bytes|	Volume Serial Number. Unique number used for identification of a particular disk.|
|0x2b	|11 bytes|	Volume Label. This is a string of characters for human-readable identification of the disk (padded with spaces if shorter); it is selected when the disk is formatted.|
|0x36	|8 bytes|	File system identifier (padded at the end with spaces if shorter).|
|0x3e	|0x1c0 bytes|	The remainder of the bootstrap program.|
|0x1fe	|2 bytes|	Boot block 'signature' (0x55 followed by 0xaa).|

(Table from http://www.tavi.co.uk/phobos/fat.html#file_allocation_table)