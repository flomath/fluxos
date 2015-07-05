/*
 * fstest.c
 *
 *  Created on: 04.07.2015
 *      Author: Philipp
 */
#include "ff.h"

void testFS(void){

	// Read binary file at once
	/*FRESULT result;
	FATFS fs;
	fs.drv = 0;
	f_mount(&fs, "", 1);

	FILINFO fno;

	DIR dp;
	result = f_opendir(&dp, "/\0");
	result = f_readdir(&dp, &fno);
	FIL file;//TODO: how to read a file
	f_open(&file, "/fluxy.txt", FA_READ);

	char buffa[928];

	unsigned br;

	br = f_tell(&file); // get read-pointer
	f_read(&file, buffa, 928, &br); // read 4 byte
	br = f_eof(&file); // check if eof is reached - br != 0, if it is so
	//zurück
	f_lseek(&file, 0); // set read-pointer to the beginning of the file
	br = f_tell(&file);

	f_close(&file);
	f_closedir(&dp);*/


	//http://elm-chan.org/fsw/ff/00index_e.html

//	FRESULT result;
//	FATFS fs;
//	fs.drv = 0;
//	f_mount(&fs, "asdf\0", 1);
//	//TCHAR* path = "/test.txt\0";
//	FILINFO fno;
//	//f_stat(path, &fno);
//	DIR dp;
//	result = f_opendir(&dp, "/\0");
//	result = f_readdir(&dp, &fno);
//	FIL file;//TODO: how to read a file
//	f_open(&file, "/led.bin\0", FA_READ);
//	// content of test.txt: "aaaabbbbcccc"
//	char buffa[4];
//	char buffb[4];
//	char buffc[4];
//	f_lseek(&file, 8); // set read-pointer to the first "c"
//	int br;
//	br = f_tell(&file); // get read-pointer
//	f_read(&file, buffc, 4, &br); // read 4 byte
//	br = f_eof(&file); // check if eof is reached - br != 0, if it is so
//	//zurück
//	f_lseek(&file, 0); // set read-pointer to the beginning of the file
//	br = f_tell(&file);
//	br = 0;
//	f_read(&file, buffa, 4, &br);
//	br = 0;
//	f_read(&file, buffb, 4, &br);
//	f_close(&file);
//	f_closedir(&dp);
}


