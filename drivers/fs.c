#include "fs.h"
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

/*

In the filesystem section, 1 record is inside 1 disk sector.
Record holds file path, first cluster identifier and metadata.
First 4 bytes in every cluster is next cluster identifier. 
Next 4 bytes in every cluster is current data size.

| 256 bytes | 4 bytes       | 252 bytes |
| file      | first cluster | metadata  |

*/

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
    if(record == -1) {
        return;
    }
    char data[512];
    int c = 0;
    unsigned long long g = 0;
    unsigned long long contentLength = strlen(content);
    bool end = false;
    while(!end) {
        int cluster = get_cluster(file, c);
        if(cluster == -2) { // create cluster
            char recordData[512];
            read_record(get_file_record(file), recordData);
            int freeCluster = get_free_cluster();
            for(int i = 0, j = 8 * (CLUSTER_BYTES - 1); i < CLUSTER_BYTES; i++, j-= 8) {
                recordData[FILE_SIZE + i] = (freeCluster >> j) & 0xFF;
            }
            cluster = freeCluster;
            write_record(record, recordData);
        } else {
            
        }
        for(int i = 0; i < CLUSTER_SIZE; i++) {
            char sectorData[512];
            for(int j = (i == 0 ? 8 : 0); j < 512; j++) {
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
        c++;
    }
}

void fappend(char* file, char* content) {

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