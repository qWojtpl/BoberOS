#ifndef FS
#define FS

char fcreate(char* file);
void fwrite(char* file, char* content);
void fread(char* file, char* buffer);
void read_record(int record, char* data);
void write_record(int record, char* data);
void write_record_d(int record, char* fileName, char* clusters, char* metadata);
void fdelete(char* file);
int get_cluster(char* target, int offset);
char get_file_record(char* target);
char get_free_record();
int get_free_cluster();

#endif