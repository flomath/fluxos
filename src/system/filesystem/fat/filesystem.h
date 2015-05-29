/**
 * © 2011 Paul Annesley MIT license; free open source.
 * https://github.com/pda/cfat16
 */

#pragma once

#include <stdio.h>
#include "boot.h"
#include "dir.h"

struct fat16_filesystem {
    FILE *fd;
    struct fat16_boot_sector boot_sector;
    int fat_offset;
    int fat_size;
    int root_dir_offset;
    int root_dir_size;
    int root_dir_entries;
    int data_offset;
    int cluster_size;
};

typedef struct {
    char* buffer;
} FAT_FILE;

/**
 * Filesystem
 */
void fat_open_filesystem(struct fat16_filesystem *, char *path);

void fat_close_filesystem(struct fat16_filesystem *);

int fat_seek_to_root_dir(struct fat16_filesystem *);

int fat_seek_to_cluster(struct fat16_filesystem *, int cluster);

/**
 * File Handling
 */
FAT_FILE* fat_file_open(struct fat16_filesystem *, struct FAT_DIR *);
int fat_file_close(FAT_FILE* file);
int fat_file_size(struct FAT_DIR *de);
void fat_file_getname(char *output, struct FAT_DIR *);

/**
 * Directory handling
 */
FAT_DIR* fat_dir_open(FILE *fp);
int fat_dir_entry_exists(struct FAT_DIR *);
int fat_is_volume_label(struct FAT_DIR *);
int fat_is_file(struct FAT_DIR *);
int fat_is_dir(struct FAT_DIR *);
