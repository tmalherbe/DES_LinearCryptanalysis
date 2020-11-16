typedef struct{
  int size;
  BYTE *buffer;
}MAP;

MAP encode_file(char *src_path);

void save_file(BYTE *buffer, char *dst_path, int size);

void burnStack(void *stack, int length);
