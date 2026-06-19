#ifndef FS_UTIL
#define FS_UTIL

// clusters

int get_cluster(char* target, int offset);
int get_free_cluster();

// records

char get_file_record(char* target);
char get_free_record();
int get_record_cluster(int record);
void read_record(int record, char* data);
void write_record(int record, char* data);
void write_record_d(int record, char* fileName, char* clusters, char* metadata);

#endif