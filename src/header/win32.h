#ifndef WIN32_H
#define WIN32_H

int is_cmd_path(const char* cmd);
char* popen(const char* command);
int zip_archive(const char* folder_path, const char* archive_name);
const char* temp_is_time(const char* extension);
int extract_zip(const char* archive_path, const char* output_dir);

#endif