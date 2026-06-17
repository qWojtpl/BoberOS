#ifndef FS
#define FS

char fcreate(char* file);
void fwrite(char* file, char* content);
void fread(char* file, char* buffer);
void read_record(int record, char* data);
void fdelete(char* file);
char get_cluster(char* target, int curr);
char get_free_record();

#endif