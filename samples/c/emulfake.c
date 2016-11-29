/*

This file is part of Linkom Library

Copyright (c) 2016 Hernán Di Pietro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "..\..\linkom.h"
#include <stdio.h>

void showUsage() {
    puts("Usage:");
    puts("");
    puts("emulator [/?] /DISK0=mb /RAM=mb /CACHE[=nn] [/DD[=nn]] [/DISK1=mb] [/FAST]");
    puts("         [{/ALTCPU1|/ALTCPU2}] { video } <bios_rom_file> <firmware_rom_file> ");
    puts("");
    puts("/?                        Display help");
    puts("/DISK0=mb                 Specify disk 0 size in megabytes");
    puts("/DISK1=mb                 Specify (optional) disk 1 size in megabytes");
    puts("/RAM=mb                   Specify system RAM size in megabytes");
    puts("/CACHE[=nn]               Specify cache with optional size (default is 16k)");
    puts("/DD=nn                    Specify optional  floptical drive size. Default is 1M");
    puts("/FAST                     Use fast emulation mode");
    puts("/ALTCPUx                  Use alternate CPU emulation modes (1 or 2).");
    puts("video can be: /CGA, /EGA, /VGA or MCGA. ");
}

int wmain( int argc, wchar_t** argv ) {

    puts("A Fake Computer Emulator Program  v1.0.0");
    puts("This program is part of the Linkom Command Line Library Samples");
    puts("");

    LK_RESULT r = LkInit();

    wchar_t invTok[LK_MAX_TOKENARG_LEN];
    wchar_t invArg[LK_MAX_TOKENARG_LEN];

    r = LkSetSyntax(L"[/?] /DISK0= /RAM= /CACHE? [/DD?] [/DISK1=] [/FAST] [{/ALTCPU1|/ALTCPU2}] {/CGA|/EGA|/VGA|/MCGA} $biosrom $firmware0rom",
                    invTok, _countof(invTok));

    assert(r == LK_R_OK);

    r = LkParse(argc, argv, invArg, _countof(invArg));

    if (argc <= 1 || ( LkIsParamPresent(L"/?") == LK_R_OK) ) {
        showUsage();
        return 0;
    }

    if (r != LK_R_OK) {
        _putws(LkGetExtErrorString());
        return -1;
    }

    wchar_t* szBIOSROM, *szFirmwareROM;
    wchar_t* szd0, *szd1, *szdd;
    wchar_t* szRAM, *szCache;
    wchar_t szDisks[256] = { 0 }, szCacheInfo[256] = { 0 };
    wchar_t szVideo[256] = { 0 }, szCPU[256] = { 0 };

    LkGetParamValue(L"$biosrom", &szBIOSROM);
    LkGetParamValue(L"$firmware0rom", &szFirmwareROM);

    if (!szBIOSROM) {
        puts("WARNING: Please provide a BIOS ROM name.");
    }

    if (!szFirmwareROM) {
        puts("WARNING: Please provide a Firmware ROM BIOS name.");
    }

    LkGetParamValue(L"/disk0", &szd0);
    LkGetParamValue(L"/disk1", &szd1);
    r = LkGetParamValue(L"/dd", &szdd);
    swprintf_s(szDisks, 256, L"0:%s 1:%s dd:%s", szd0, (szd1 ? szd1 : L"None"),
               r == LK_E_NOTFOUND ? L"None" : (r == LK_E_UNDEFINED ? L"Default" : szdd ));
    if (LkIsParamPresent(L"/CGA") == LK_R_OK)
        wcscpy_s(szVideo, 256, L"Color Graphics Adapter (CGA)");
    if (LkIsParamPresent(L"/EGA") == LK_R_OK)
        wcscpy_s(szVideo, 256, L"Enhanced Graphics Adapter (EGA)");
    if (LkIsParamPresent(L"/VGA") == LK_R_OK)
        wcscpy_s(szVideo, 256, L"Video Graphics Array (VGA)");
    if (LkIsParamPresent(L"/MCGA") == LK_R_OK)
        wcscpy_s(szVideo, 256, L"Multicolor Graphics Adapter (MCGA)");
    LkGetParamValue(L"/RAM", &szRAM);
    r = LkGetParamValue(L"/CACHE", &szCache);
    if (szCache)
        wcscpy_s(szCacheInfo, 256, szCache);
    else {
        if (r == LK_E_UNDEFINED)
            wcscpy_s(szCacheInfo, 256, L"Default");
        else
            wcscpy_s(szCacheInfo, 256, L"None");
    }
    if (LkIsParamPresent(L"/ALTCPU1") == LK_R_OK)
        wcscat_s(szCPU, 256, L"CPU Type I");
    else if (LkIsParamPresent(L"/ALTCPU2") == LK_R_OK)
        wcscat_s(szCPU, 256, L"CPU Type II");

    if (LkIsParamPresent(L"/FAST") == LK_R_OK)
        wcscat_s(szCPU, 256, L" + Fast Mode");

    wprintf(L"Selected Arguments for emulator: \n");
    wprintf(L"==================================================================\n");
    wprintf(L"BIOS ROM:                       |  %s\n", szBIOSROM);
    wprintf(L"Firmware ROM:                   |  %s\n", szFirmwareROM);
    wprintf(L"Disk Configuration              |  %s\n", szDisks);
    wprintf(L"RAM                             |  %s\n", szRAM);
    wprintf(L"Cache                           |  %s\n", szCache);
    wprintf(L"Video system                    |  %s\n", szVideo);
    wprintf(L"CPU Configuration               |  %s\n", szCPU);
    wprintf(L"==================================================================\n");

    LkFree();

    return 0;
}

