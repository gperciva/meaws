#ifdef __cplusplus
extern "C" {
#endif 

#define MONOWAV_OK 0
#define MONOWAV_ERROR 1

typedef struct {
	short *buffer;
	int length;
	int status;
} monowav_sound;

int monowav_write(const char *filename, monowav_sound *info);
monowav_sound* monowav_read(const char *filename);

#ifdef __cplusplus
}
#endif 
