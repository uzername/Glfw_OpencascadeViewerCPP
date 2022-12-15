#ifndef WINAPIDIALOG2_DEF
#define WINAPIDIALOG2_DEF
#include <windows.h>      // For common windows data types and function headers
void basicFileOpen2(HWND hwnd) {
    OPENFILENAME ofn;       // common dialog box structure
    WCHAR szFile[260];       // buffer for file name
                  // owner window
    HANDLE hf;              // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = (szFile);
    // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"STEP FILE\0*.STP;*.STEP\0All files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box. 

    if (GetOpenFileName(&ofn) == TRUE) {
        
        }
}

#endif
