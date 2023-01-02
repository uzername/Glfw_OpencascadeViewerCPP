#ifndef WINAPIDIALOG2_DEF
#define WINAPIDIALOG2_DEF
#include <windows.h>      // For common windows data types and function headers
#include <string>
#include "MyFileInfoStruct.h"

/// <summary>
/// get the filename from path
/// https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
/// </summary>
/// <typeparam name="T">wstring</typeparam>
/// <param name="path"></param>
/// <param name="delims"></param>
/// <returns></returns>
template<class T>
T base_name(T const& path, T const& delims = "/\\")
{
    return path.substr(path.find_last_of(delims) + 1);
}

/// <summary>
/// a simple subroutine to call file open dialog
/// </summary>
/// <param name="hwnd">hwnd of parent window</param>
/// <returns></returns>
bool basicFileOpen2(HWND hwnd, MyFileInfo* obtainedFile) {
    OPENFILENAME ofn;       // common dialog box structure
    WCHAR szFile[260];       // buffer for file name
                  // owner window
    HANDLE hf;              // file handle
    bool rslt = false;
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
        rslt = true;
        std::wstring ws(ofn.lpstrFile);
        obtainedFile->resultFileNameFull = std::string(ws.begin(), ws.end());
        obtainedFile->resultFileName = base_name<std::string> (obtainedFile->resultFileNameFull);
    }
    return rslt;
}



#endif
