#include "header/win32.h"
#include "header/utils.h"
#include <time.h>
#include <Windows.h>

int is_cmd_path(const char* cmd)
{
    DEBUG_ASSERT(cmd);

    char command[256];
    snprintf(command, sizeof(command), "where \"%s\" >nul 2>&1", cmd);

    int result = system(command);
    return (result == 0);
}

char* popen(const char* command)
{
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
    HANDLE hRead, hWrite;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        return NULL;
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

    char cmd_line[1024];
    snprintf(cmd_line, sizeof(cmd_line), "cmd /c %s", command);

    if (!CreateProcessA(NULL, cmd_line, NULL, NULL, TRUE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        return NULL;
    }

    CloseHandle(hWrite);

    char buffer[4096];
    DWORD bytesRead;
    size_t total_size = 1;
    char* result = malloc(total_size);
    result[0] = '\0';

    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        size_t new_size = total_size + bytesRead;
        char* new_result = realloc(result, new_size);
        if (!new_result) {
            free(result);
            CloseHandle(hRead);
            return NULL;
        }
        result = new_result;
        strcat(result, buffer);
        total_size = new_size;
    }

    CloseHandle(hRead);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return result;
}

int zip_archive(const char* folder_path, const char* archive_name)
{
   char command[1024];

    snprintf(command, sizeof(command),
        "powershell -Command \"Compress-Archive -Path '%s' -DestinationPath '%s' -Force\"",
        folder_path, archive_name);

    /*
      int result = system(command);
      return (result == 0) ? 0 : -1;
    */

    popen(command);
    return 0;
}

const char* temp_is_time(const char* extension)
{
    if (!extension) return NULL;

    static char filename[256];  

    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);

    snprintf(filename, sizeof(filename),
        "%04d-%02d-%02d-%02d%s",
        timeinfo->tm_year + 1900,  
        timeinfo->tm_mon + 1,      
        timeinfo->tm_mday,        
        timeinfo->tm_hour,         
        extension);

    return filename;
}

int extract_zip(const char* archive_path, const char* output_dir) {
    char current_dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, current_dir);

    char seven_zip_path[MAX_PATH];
    snprintf(seven_zip_path, sizeof(seven_zip_path),
        "%s\\extensions\\7za.exe", current_dir);

    if (GetFileAttributesA(seven_zip_path) == INVALID_FILE_ATTRIBUTES) {
        printf("Error: 7za.exe not found at: %s\n", seven_zip_path);
        return -3;
    }

    char command[1024];
    snprintf(command, sizeof(command),
        "%s x \"%s\" -o\"%s\" -y",
        seven_zip_path, archive_path, output_dir);

    popen(command);
    return 0;
}