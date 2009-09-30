#define MONOWAV_OK 0
#define MONOWAV_ERROR 1

int monowav_write(const char *filename, const short *buffer,
                  const unsigned long bufferLength);
int monowav_read(const char *filename, short *buffer,
                 unsigned long *bufferLength);

