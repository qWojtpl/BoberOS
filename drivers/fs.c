#include "fs.h"
#include "disk.h"
#include "print.h"
#include <stdbool.h>

#define FS_START        100
#define FS_END          200

#define CLUSTER_SIZE    8

/*

In the filesystem section, 1 record is inside 1 disk sector.
Record holds file path, cluster identifiers and metadata.

| 128 bytes | 256 bytes | 128 bytes |
| file      | clusters  | metadata  |

*/

int strlen(char* str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

char fcreate(char* file) {
    char sector = get_cluster(file, 0);
    if(sector != 0) {
        return 0;
    }
    char free = get_free_record();
    if(free == 0) {
        return 1;
    }
    char str[512];
    int fileLen = strlen(file);
    for(int i = 0; i < 128; i++) {
        if(i >= strlen(file)) {
            str[i] = '\0';
        } else {
            str[i] = file[i];
        }
    }
    for(int i = 128; i < 384; i++) {
        str[i] = '\0';
    }
    for(int i = 384; i < 512; i++) {
        str[i] = '\0';
    }
    short buffer[256];
    string_to_buffer(str, buffer);
    write_sector(free, buffer);
    return 2;
}

void read_record(int record, char* data) {
    short buffer[256];
    read_sector(FS_START + record, buffer);
    buffer_to_string(buffer, data);
}

void fwrite(char* file, char* content) {

}

void fread(char* file, char* buffer) {
    int i = 0;
    char cluster_id;
    do {
        cluster_id = get_cluster(file, i++);

    } while(cluster_id != 0);
}

void fdelete(char* file) {

}

char get_cluster(char* target, int curr) {
    for(int i = FS_START; i < FS_END; i++) {
        short buffer[256];
        char  str_data[512];
        read_sector(i, buffer);
        buffer_to_string(buffer, str_data);
        bool found = true;
        for(int j = 0; j < 128; j++) {
            if(j >= strlen(target)) {
                if(str_data[j] != '\0') {
                    found = false;
                    break;
                }
                continue;
            }
            if(target[j] != str_data[j]) {
                found = false;
                break;
            }
        }
        if(found) {
            return str_data[128 + curr];
        }
    }
    return 0;
}

char get_free_record() {
    for(int i = FS_START; i < FS_END; i++) {
        short buffer[256];
        char  str_data[512];
        read_sector(i, buffer);
        buffer_to_string(buffer, str_data);
        bool found = true;
        for(int j = 0; j < 128; j++) {
            if(str_data[j] != '\0') {
                found = false;
                break;
            }
        }
        if(found) {
            return i;
        }
    }
    return 0;
}