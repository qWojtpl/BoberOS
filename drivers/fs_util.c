#include "fs_util.h"
#include "disk.h"
#include "util.h"
#include <stdbool.h>

#define FS_START        100
#define FS_END          200

#define FILE_SIZE       256
#define CLUSTER_BYTES   4
#define METADATA        252

#define CLUSTER_SIZE    8

// clusters

// Returns cluster id by offset
// Returns -1 if file doesn't exist, or -2 when record doesn't contain first cluster
int get_cluster(char* target, int offset) {
    char record = get_file_record(target);
    if(record == -1) {
        return -1;
    }
    int clusterId = get_record_cluster(record);
    if(clusterId == -1) {
        return -2;
    }
    while(offset != 0) {
        short buffer[256];
        char sectorData[512];
        read_sector(FS_END + clusterId, buffer);
        buffer_to_string(buffer, sectorData);
        if(sectorData[0] == '\0') {
            break;
        }
        clusterId = 
            ((unsigned char) sectorData[0] << 24) | 
            ((unsigned char) sectorData[1] << 16) |
            ((unsigned char) sectorData[2] << 8) |
            ((unsigned char) sectorData[3]);
        offset--;
    }
    return clusterId;
}


// Gets free cluster ID
int get_free_cluster() {
    int i = FS_END;
    int sectorId = 0;
    while(true) {
        bool empty = true;
        for(int j = 0; j < 8; j++) {
            short buffer[256];
            char content[512];
            read_sector(i + j, buffer);
            buffer_to_string(buffer, content);
            for(int k = 0; k < 512; k++) {
                if(content != '\0') {
                    empty = false;
                    break;
                }
            }
            if(!empty) {
                break;
            }
        }
        if(empty) {
            return sectorId;
        }
        i += 8;
        sectorId++;
    }
    return 0;
}

// records

char get_file_record(char* target) {
    for(int i = FS_START; i < FS_END; i++) {
        short buffer[256];
        char  str_data[512];
        read_sector(i, buffer);
        buffer_to_string(buffer, str_data);
        bool found = true;
        for(int j = 0; j < FILE_SIZE; j++) {
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
            return i - FS_START;
        }
    }
    return -1;
}

// Get free record ID
char get_free_record() {
    for(int i = FS_START; i < FS_END; i++) {
        short buffer[256];
        char  str_data[512];
        read_sector(i, buffer);
        buffer_to_string(buffer, str_data);
        bool found = true;
        for(int j = 0; j < FILE_SIZE; j++) {
            if(str_data[j] != '\0') {
                found = false;
                break;
            }
        }
        if(found) {
            return i - FS_START;
        }
    }
    return -1;
}

int get_record_cluster(int record) {
    char recordData[512];
    read_record(record, recordData);
    char charId[CLUSTER_BYTES];
    if(recordData[FILE_SIZE] == '\0') {
        return -1;
    }
    for(int i = 0; i < CLUSTER_BYTES; i++) {
        charId[i] = recordData[FILE_SIZE + i];
    }
    return ((unsigned char) charId[0] << 24) | 
            ((unsigned char) charId[1] << 16) |
            ((unsigned char) charId[2] << 8) |
            ((unsigned char) charId[3]);
}

void read_record(int record, char* data) {
    short buffer[256];
    read_sector(FS_START + record, buffer);
    buffer_to_string(buffer, data);
}

void write_record(int record, char* data) {
    short buffer[256];
    string_to_buffer(data, buffer);
    write_sector(FS_START + record, buffer);
}

void write_record_d(int record, char* fileName, char* clusters, char* metadata) {
    char str[512];
    for(int i = 0; i < FILE_SIZE; i++) {
        if(i >= strlen(fileName)) {
            str[i] = '\0';
        } else {
            str[i] = fileName[i];
        }
    }
    for(int i = FILE_SIZE; i < FILE_SIZE + CLUSTER_BYTES; i++) {
        if(i >= strlen(clusters)) {
            str[i] = '\0';
        } else {
            str[i] = clusters[i];
        }
    }
    for(int i = FILE_SIZE + CLUSTER_BYTES; i < FILE_SIZE + CLUSTER_BYTES + METADATA; i++) {
        if(i >= strlen(metadata)) {
            str[i] = '\0';
        } else {
            str[i] = metadata[i];
        }
    }
    write_record(record, str);
}

