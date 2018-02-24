#ifndef MEMORY_THREAD
#define MEMORY_THREAD

extern char filename;

bool openfile(FIL *f);
void mem_thread(void const *args);

#endif