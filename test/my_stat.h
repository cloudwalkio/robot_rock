
#ifdef PAX
  #include <posapi.h>
  #include <posapi_all.h>
#endif

//int test_stat(char *file_name);
int create_file_and_directory(void);
int stat_info(void);
int stat_info2(void);
int stat_info3(void);
int stat_info5(FILE_INFO **st);
int stat_info6(void);
void stat_info4(void);
int file_basename(void);
int file_exists(void);
int file_rename(void);
int get_pax_key(void);
int get_pax_string(void);
int get_pax_hz_string(void);
int scr_print(void);

