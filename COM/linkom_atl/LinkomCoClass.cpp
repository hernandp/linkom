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

#include "stdafx.h"
#include "LinkomCoClass.h"

STDMETHODIMP CLinkom::SetSyntax(BSTR syntaxSpec, BSTR * invalidToken, unsigned long * reasonCode) {

    if (!invalidToken || !reasonCode)
        return E_POINTER;

    wchar_t invToken[LK_MAX_TOKENARG_LEN];
    LK_RESULT r = LkSetSyntax(syntaxSpec, invToken, LK_MAX_TOKENARG_LEN);

    *reasonCode = r;
    *invalidToken = SysAllocString(invToken);
    return r == LK_R_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CLinkom::Parse(BSTR commandLine, BSTR * invalidArg, unsigned long * reasonCode) {
    if (!invalidArg || !reasonCode)
        return E_POINTER;

    wchar_t invArg[LK_MAX_TOKENARG_LEN];
    LK_RESULT r = LkParse2(commandLine, invArg, LK_MAX_TOKENARG_LEN);

    *reasonCode = r;
    *invalidArg = SysAllocString(invArg);
    return r == LK_R_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CLinkom::IsParamPresent(BSTR paramName, BOOL * bPresent) {
    if (!bPresent)
        return E_POINTER;

    *bPresent = LkIsParamPresent(paramName) == LK_R_OK ? TRUE : FALSE;
    return S_OK;
}

STDMETHODIMP CLinkom::GetParamValue(BSTR paramName, BSTR * paramValue, unsigned long * reasonCode) {
    if (!paramValue || !reasonCode)
        return E_POINTER;
    wchar_t* szParamValue;
    LK_RESULT r = LkGetParamValue(paramName, &szParamValue);
    *reasonCode = r;

    return r == LK_R_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CLinkom::GetExtErrorString(BSTR * errString) {
    if (!errString)
        return E_POINTER;
    *errString = SysAllocString(LkGetExtErrorString());
    return S_OK;
}