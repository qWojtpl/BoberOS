#include "fs.h"
#include "disk.h"
#include "print.h"
#include <stdbool.h>

#define FS_START        100
#define FS_END          200

#define FILE_SIZE       256
#define CLUSTER_BYTES   4
#define METADATA        252

#define CLUSTER_SIZE    8

/*

In the filesystem section, 1 record is inside 1 disk sector.
Record holds file path, first cluster identifier and metadata.
First 4 bytes in every cluster is next cluster identifier. 

| 256 bytes | 4 bytes       | 252 bytes |
| file      | first cluster | metadata  |

*/

int strlen(char* str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

char fcreate(char* file) {
    int cluster = get_cluster(file, 0);
    if(cluster != -1) { // check if file exists
        return 0;
    }
    char free = get_free_record();
    if(free == -1) {
        return 1;
    }
    char clusters[0];
    char metadata[0];
    write_record_d(free, file, clusters, metadata);
    return 2;
}

void fwrite(char* file, char* content) {
    char record = get_file_record(file);
    char data[512];
    int c = 0;
    int g = 0;
    long long contentLength = strlen(content);
    bool end = false;
    while(!end) {
        int cluster = get_cluster(file, c++);
        if(cluster == -1) {
            char recordData[512];
            read_record(get_file_record(file), recordData);
            int freeCluster = get_free_cluster();
            for(int i = 0, j = 8 * (CLUSTER_BYTES - 1); i < CLUSTER_BYTES; i++, j-= 8) {
                recordData[FILE_SIZE + i] = (freeCluster >> j) & 0xFF;
            }
            cluster = freeCluster;
            write_record(record, recordData);
        } else {
            //todo
        }
        for(int i = 0; i < CLUSTER_SIZE; i++) {
            char sectorData[512];
            for(int j = (i == 0 ? 4 : 0); j < 512; j++) {
                if(contentLength == 0) {
                    end = true;
                    sectorData[j] = '\0';
                } else {
                    sectorData[j] = content[g++];
                    contentLength--;
                }
            }
            short buffer[256];
            string_to_buffer(sectorData, buffer);
            write_sector(FILE_SIZE + cluster * 8 + i, buffer);
        }
    }
}

void fappend(char* file, char* content) {

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

void write_record(int record, char* data) {
    short buffer[256];
    string_to_buffer(data, buffer);
    write_sector(FS_START + record, buffer);
}

void read_record(int record, char* data) {
    short buffer[256];
    read_sector(FS_START + record, buffer);
    buffer_to_string(buffer, data);
}

int get_record_cluster(int record) {
    char recordData[512];
    read_record(record, recordData);
    char charId[CLUSTER_BYTES];
    for(int i = 0; i < CLUSTER_BYTES; i++) {
        charId[i] = recordData[FILE_SIZE + i];
    }
    return ((unsigned char) charId[0] << 24) | 
            ((unsigned char) charId[1] << 16) |
            ((unsigned char) charId[2] << 8) |
            ((unsigned char) charId[3]);
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

// returns cluster id by offset
int get_cluster(char* target, int offset) {
    char record = get_file_record(target);
    if(record == 0) {
        return -1;
    }
    int clusterId = get_record_cluster(record);
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

// get free record id
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

// get free cluster sector
int get_free_cluster() {
    int i = FS_END;
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
            return i;
        }
        i += 8;
    }
    return 0;
}

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
            return i;
        }
    }
    return 0;
}