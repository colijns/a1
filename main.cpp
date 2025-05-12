
#include <iostream>
#include <string>
#include <Windows.h>
#include "HRUtil.h"
#include "GameApp.h"

int main()
{
    char szModulePath[512];
    memset(szModulePath, 0, sizeof(szModulePath));
    if (GetModuleFileNameA(NULL, szModulePath, sizeof(szModulePath) - 1) == 0) {
        std::cerr << "Failed to get module path" << std::endl;
        return 1;
    }

    szModulePath[sizeof(szModulePath) - 1] = '\0';
    
    int findPos = HRFindLastOf(szModulePath, "\\");
    if (findPos < 0 || findPos >= sizeof(szModulePath) - 1) {
        std::cerr << "Invalid path format" << std::endl;
        return 1;
    }
    
    szModulePath[findPos] = '\0';

    if (!SetCurrentDirectoryA(szModulePath)) {
        std::cerr << "Failed to set current directory" << std::endl;
        return 1;
    }

    RunGame();
    
    return 0;
}

