// LinkomCoClass.cpp : Implementation of CLinkom

#include "stdafx.h"
#include "LinkomCoClass.h"


// CLinkom

STDMETHODIMP CLinkom::SetSyntax(BSTR syntaxSpec, BSTR * invalidToken, unsigned long * reasonCode) {
    return E_NOTIMPL;
}

STDMETHODIMP CLinkom::Parse(BSTR commandLine, BSTR * invalidArg, unsigned long * reasonCode) {
    return E_NOTIMPL;
}

STDMETHODIMP CLinkom::IsParamPresent(BSTR paramName, BOOL * bPresent) {
    return E_NOTIMPL;
}

STDMETHODIMP CLinkom::GetParamValue(BSTR paramName, BSTR * paramValue, unsigned long * reasonCode) {
    return E_NOTIMPL;
}

STDMETHODIMP CLinkom::GetExtErrorString(BSTR * errString) {
    return E_NOTIMPL;
}
