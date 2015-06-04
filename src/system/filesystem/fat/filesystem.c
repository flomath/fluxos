/**
 * © 2011 Paul Annesley MIT license; free open source.
 * https://github.com/pda/cfat16
 */

#include <stdlib.h>
#include <string.h>

#include "filesystem.h"
#include "dir.h"
#include "utils.h"

void fat_open_filesystem(struct fat16_filesystem *fs, char *path) {
    struct fat_bios_parameter_block *bp;

    if (!(fs->fd = fopen(path, "r"))) {
        fatal_error("unable to fopen filesystem");
    }

    if (!fread(&fs->boot_sector, sizeof(fs->boot_sector), 1, fs->fd)) {
        fatal_error("unable to read boot sector");
    }

    /* calculate and store commonly used parameters */
    bp = &fs->boot_sector.bios_params;
    fs->fat_offset = bp->bytes_per_sector * bp->reserved_sector_count;
    fs->fat_size = bp->bytes_per_sector * bp->sectors_per_fat;
    fs->root_dir_offset = fs->fat_offset + (fs->fat_size * bp->fat_count);
    fs->root_dir_size = bp->max_root_entries * sizeof(struct FAT_DIR);
    fs->root_dir_entries = bp->max_root_entries;
    fs->data_offset = fs->root_dir_offset + fs->root_dir_size;
    fs->cluster_size = bp->bytes_per_sector * bp->sectors_per_cluster;
}

void fat_close_filesystem(struct fat16_filesystem *fs) {
    if (fclose(fs->fd) != 0) fatal_error("unable to close filesystem");
}

int fat_seek_to_root_dir(struct fat16_filesystem *fs) {
    return fseek(fs->fd, fs->root_dir_offset, SEEK_SET);
}

int fat_seek_to_cluster(struct fat16_filesystem *fs, int cluster) {
    /* first cluster is #2 at position 0 */
    int offset = fs->data_offset + (cluster - 2) * fs->cluster_size;
    return fseek(fs->fd, offset, SEEK_SET);
}

FAT_FILE* fat_file_open(struct fat16_filesystem *fs, struct FAT_DIR *de) {
    FAT_FILE* file = malloc(sizeof(FAT_FILE));
    char *buffer;

    if (de->size > fs->cluster_size)
        fatal_error("TODO: read multi-cluster files");

    buffer = (char *) malloc(de->size + 1);
    if (buffer == 0) fatal_error("malloc failed");

    fat_seek_to_cluster(fs, de->start_cluster);
    fread(buffer, de->size, 1, fs->fd);
    buffer[de->size] = 0;

    file->buffer = buffer;

    return file;
}

int fat_file_close(FAT_FILE * file)
{
    free(file);

    return 0;
}

int fat_file_size(struct FAT_DIR *de)
{
    return de->size;
}

void fat_file_getname(char *output, struct FAT_DIR *de)
{
    int len;
    char name[FAT_FILENAME_LENGTH + 1], extension[FAT_EXTENSION_LENGTH + 1];
    char * p;

    /* name, null-terminated, whitespace stripped */
    len = FAT_FILENAME_LENGTH;
    while (de->name[len - 1] == 0x20 && len > 0) len--;
    memmove(name, de->name, len);
    name[len] = 0;

    /* extension, null-terminated, whitespace stripped */
    len = FAT_EXTENSION_LENGTH;
    while (de->extension[len - 1] == 0x20 && len > 0) len--;
    memmove(extension, de->extension, len);
    extension[len] = 0;

    /* write name */
    p = strcpy(output, name);

    /* write extension */
    if (*extension) {
        *p++ = '.';
        strcpy(p, extension);
    }
}

int fat_dir_entry_exists(struct FAT_DIR * de) {
    /* 0x00: null entry, 0xE5: marked as deleted */
    return !(*de->name == 0x00 || *de->name == 0xE5);
}

int fat_is_volume_label(struct FAT_DIR * de) {
    return (de->attributes & 0x08) == 0x08;
}

int fat_is_file(struct FAT_DIR * de) {
    /* not volume label, not directory */
    return (de->attributes & 0x18) == 0;
}

int fat_is_dir(struct FAT_DIR * de) {
    /* not volume label, is directory */
    return (de->attributes & 0x18) == 0x10;
}

FAT_DIR* fat_dir_open(FILE *fp)
{
    FAT_DIR *entry;
    fread(&entry, sizeof(entry), 1, fp);
    return entry;
}
