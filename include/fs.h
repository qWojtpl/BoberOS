#ifndef FS
#define FS

char fcreate(char* file);
void fwrite(char* file, char* content);
void fread(char* file, char* buffer);
void fdelete(char* file);

#endif