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
#ifndef __LINKOM_H__f82e85a35f914826a6af35aa02839625
#define __LINKOM_H__f82e85a35f914826a6af35aa02839625

/* You may keep  THIS COPYRIGHT NOTICE TO EMBED INTO YOUR BINARY IMAGE  */
/* (read License) */
volatile static char g_Copyright[] = "*** Linkom C Library (c) 2016 Hernan Di Pietro ***";

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__GNUC__) || defined(__WATCOMC__)
#define __STDC_WANT_LIB_EXT1__ 1
#include <wchar.h>
#endif


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef  LK_ENABLE_TRACE
#define _LKTRACE(...)    fprintf (stderr, __VA_ARGS__)
#define _LKTRACEW(...) fwprintf(stderr, __VA_ARGS__)
#else
#define _LKTRACE(...) 
#define _LKTRACEW(...)
#endif

#define LK_ARRAYSIZE(x)  ((sizeof(x) / sizeof(*(x))) / \
                         (size_t)(!(sizeof(x) % sizeof(*(x)))))
  
/* ---------------------------------------------------------------------------
   Error codes (inspired by MS HRESULTs)
   ---------------------------------------------------------------------------*/
typedef enum 
{
    LK_R_OK                 = 0x00000000,
    LK_R_FALSE              = 0x00000000,
    LK_R_TRUE               = 0x00000001,
    LK_E_MALLOC             = 0xEEEE0001,
    LK_E_NULLPOINTER        = 0xEEEE0002,
    LK_E_UNINITIALIZED      = 0xEEEE0003,
    LK_E_INVALIDARG         = 0xEEEE0004,
    LK_E_ALREADYINIT        = 0xEEEE0005,
    LK_E_LISTFULL           = 0xEEEE0006,
    LK_E_NOTFOUND           = 0xEEEE0007,
    LK_E_OUTOFBOUNDS        = 0xEEEE0008,
    LK_E_INVALIDSYNTAX      = 0xEEEE1000,
    LK_E_UNKNOWNARG         = 0xEEEE1001,
    LK_E_DUPLICATEARG       = 0xEEEE1002,
    LK_E_DUPLICATETOKEN     = 0xEEEE1003,
    LK_E_TYPEMISMATCH       = 0xEEEE1004,
    LK_E_MISSINGARG         = 0xEEEE1005,
    LK_E_MISSINGVALUE       = 0xEEEE1006,
    LK_E_MISSINGNAMARG      = 0xEEEE1007,
    LK_E_DUPLICATEGROUPITEM = 0xEEEE1008,
    LK_E_CONTRADICTARG      = 0xEEEE1009,
    LK_E_NOGROUPMEMBERCHOSEN= 0xEEEE100A,
    LK_E_TOKENLENGTH        = 0xEEEE100B,
    LK_E_TOKENCOUNT         = 0xEEEE100C,
    LK_E_GROUPCOUNT         = 0xEEEE100D,
    LK_E_GROUPMEMBERCOUNT   = 0xEEEE100E,
    LK_E_VALUELENGTH        = 0xEEEE100F,
    LK_E_UNDEFINED          = 0xEEEE1100,
    LK_E_INSUFFICIENTBUFFER = 0xEEEE1101
} LK_RESULT;
#define LK_SUCCESS(x)  ( ( (signed long)(x) ) >= 0)

/* ---------------------------------------------------------------------------
   Tunable limits, modify as you wish 
   ---------------------------------------------------------------------------*/
#ifndef LK_DEFINE_USER_LIMITS
#define LK_MAX_TOKENARG_LEN      256 /* Maximum argument and token length */
#define LK_MAX_TOKENARG_COUNT    256 /* Maximum argument and token count  */
#define LK_MAX_GROUP_COUNT       32  /* Maximum group count */
#define LK_MAX_PARAMVALUE_LEN    2048  /* Max str length for param. values*/
#endif

#define LK_MAX_ERRORBUFFER_LEN   1024

/* ---------------------------------------------------------------------------
   Token List types
   ---------------------------------------------------------------------------*/
typedef enum tagLK_TOKEN_TYPE
{
    LK_TOKEN_TYPE_NAMED_ARGUMENT,
    LK_TOKEN_TYPE_PARAMETER,
    LK_TOKEN_TYPE_PARAMETER_VAL,
    LK_TOKEN_TYPE_PARAMETER_VAL_OPT,
    LK_TOKEN_TYPE_PARAMETER_GROUP
} LK_TOKEN_TYPE;

typedef struct tagLK_TOKEN_ITEM
{
    wchar_t         name[LK_MAX_TOKENARG_LEN];
    LK_TOKEN_TYPE   type;
    int             memberOf;   // mutex-group member,if any
} LK_TOKEN_ITEM;

typedef struct tagLK_TOKEN_LIST_ENTRY
{
    bool                    isOptional;
    LK_TOKEN_ITEM           item;
} LK_TOKEN_LIST_ENTRY;

static wchar_t* g_lkTypeStr[] = 
{
    L"NAMED_ARGUMENT",    L"PARAMETER", L"PARAMETER_VAL",
    L"PARAMETER_VAL_OPT", L"GROUP"
};

/* This is a special name specifying a non-parameter argument 
   that's waiting to be matched against a syntax token */

#define LK_UNMAPPED_ARG_NAME    L"\0xFA\00\0xFB\00\0xFC\00\0xFD\00\00\00"

/* ----------------------------------------------------------------------------
   Argument List types
   ---------------------------------------------------------------------------*/
typedef struct tagARGUMENT
{
    wchar_t              name[LK_MAX_TOKENARG_LEN];
    wchar_t              value[LK_MAX_PARAMVALUE_LEN];
    bool                 hasValue;
    bool                 isParam;
    bool                 isMapped;
    bool                 isProcessed;
    int                  matchingGroup;
} LK_ARGUMENT;

typedef struct tagARGLIST
{
    LK_ARGUMENT          item[LK_MAX_TOKENARG_COUNT];
    size_t               count;
} LK_ARGLIST;   

/* ----------------------------------------------------------------------------
   Group data
   ---------------------------------------------------------------------------*/
   
#define LK_NOGROUP    -1
typedef struct tagLK_GROUPINFO
{
    wchar_t*            selectedParam;
    bool                isOptional;
} LK_GROUPINFO;

/* --------------------------------------------------------------------------
   Globals
   --------------------------------------------------------------------------*/
static LK_ARGLIST              g_argList;
static LK_TOKEN_LIST_ENTRY*    g_tokenList;
static size_t                  g_tokenCount, g_tokenMaxCount;
static bool                    g_syntaxValid;
static size_t                  g_groupCount;
static LK_GROUPINFO            g_groupInfo[LK_MAX_GROUP_COUNT];
static wchar_t                 g_exErrBuffer[LK_MAX_ERRORBUFFER_LEN];
/* --------------------------------------------------------------------------*/
LK_RESULT _Lk_TokenListInit(size_t maxCount)
{
   if (g_tokenList)
       return LK_E_ALREADYINIT;
   if (maxCount == 0)
       return LK_E_INVALIDARG;
   g_tokenList = (LK_TOKEN_LIST_ENTRY*)malloc(maxCount * sizeof (LK_TOKEN_LIST_ENTRY));
   if (!g_tokenList)
       return LK_E_MALLOC;    
   
   g_tokenCount = 0;
   g_tokenMaxCount = maxCount;
   g_groupCount = 0;
   return LK_R_OK;
}
/* --------------------------------------------------------------------------*/
LK_RESULT _Lk_TokenListClear()
{
    if (g_tokenMaxCount == 0)
        return LK_E_UNINITIALIZED;
    size_t i;
    for (i = 0 ; i < g_tokenMaxCount; ++i)
        memset (&g_tokenList[i], 0, sizeof(LK_TOKEN_LIST_ENTRY));
    return LK_R_OK;
}
/* --------------------------------------------------------------------------*/
LK_RESULT _Lk_TokenFindName(const wchar_t* name)
{
    for (size_t i = 0; i < g_tokenCount; ++i)
    {
        //wprintf(L"_Lk_TokenFindName (%s) == (%s)\n", g_tokenList[i].item.name, name);
        if (!_wcsicmp(g_tokenList[i].item.name, name))
            return LK_R_OK;
    }    
    return LK_E_NOTFOUND;
}
/* --------------------------------------------------------------------------*/
LK_RESULT _Lk_TokenAdd(LK_TOKEN_ITEM* item, bool isOptional, bool allowDuplicate)
{
    if (g_tokenCount == g_tokenMaxCount)
        return LK_E_LISTFULL;
    
    //wprintf(L">>_Lk_TokenAdd(%s)\n", item->name);
    
    if (!allowDuplicate && (_Lk_TokenFindName(item->name) == LK_R_OK))
        return LK_E_DUPLICATETOKEN;
    
    g_tokenList[g_tokenCount].isOptional    = isOptional;
    g_tokenList[g_tokenCount].item.type     = item->type;
    g_tokenList[g_tokenCount].item.memberOf = item->memberOf;
    wcscpy_s(g_tokenList[g_tokenCount].item.name, 
             LK_MAX_TOKENARG_LEN, item->name);
    g_tokenCount++;
    return LK_R_OK;
}

/* --------------------------------------------------------------------------*/
size_t _Lk_TokenGetCount() { return g_tokenCount; }

/* --------------------------------------------------------------------------*/
LK_RESULT _Lk_TokenGet(size_t idx, LK_TOKEN_LIST_ENTRY** pListEntry)
{
    if (idx >= g_tokenCount)
        return LK_E_OUTOFBOUNDS;
    
    *pListEntry = &g_tokenList[idx];
    return LK_R_OK;
}

void _Lk_TokenListFree()
{
    if (g_tokenList)
    {
        free (g_tokenList);
        g_tokenList = NULL;
        g_tokenCount = 0;
        g_tokenMaxCount=0;
    }
}

/* ---------------------------------------------------------------------------*/

LK_RESULT _Lk_ClassifyTokenR(wchar_t* token,  bool  isOptional)
{
    assert(token);
    _LKTRACEW(L"  %s (%s,%d)\n", __FUNCTIONW__, token, isOptional);
    
    const size_t len = wcslen(token);
    if (len > LK_MAX_TOKENARG_LEN)
        return LK_E_TOKENLENGTH;
    
    if ((token[0] == L'[') && (token[len-1] == L']') && !isOptional)
    {
        token[len-1] = L'\0';
        return _Lk_ClassifyTokenR(&token[1], true);
    }
    // minimal accepted form: {/x|/y} (length:7)
    else if ((token[0] == L'{') && (token[len-1] == L'}') && len >= 7)
    {      
        LK_RESULT r;
        if (g_groupCount == LK_MAX_GROUP_COUNT)
            return LK_E_GROUPCOUNT;

        g_groupInfo[g_groupCount].isOptional = isOptional;
        g_groupInfo[g_groupCount].selectedParam = NULL;
                
        LK_TOKEN_ITEM item;        
                
        _LKTRACEW(L"   Group-type token, decomposing...\n");    
        int i = 1, iArgStart = i;
        do
        {
            if(token[i] == L'|' || token[i] == L'}')
            {   
                if (token[iArgStart] != L'/')
                    return LK_E_INVALIDSYNTAX;
                
                memset (item.name, 0, LK_MAX_TOKENARG_LEN);
                wcsncpy_s(item.name, LK_MAX_TOKENARG_LEN, &token[iArgStart], i - iArgStart);
                
                if (item.name[wcslen(item.name)-1] == L'}' )
                    item.name[wcslen(item.name)-1] = 0;
                
                size_t namelen = wcslen(item.name);     
    
                _LKTRACEW(L"     item.name = %s\n", item.name);
                size_t k = namelen-1;                  
                if (item.name[k] == L':' || item.name[k] == L'=')
                {
                    _LKTRACEW(L"     item.type = LK_TOKEN_TYPE_PARAMETER_VAL\n");
                    item.type = LK_TOKEN_TYPE_PARAMETER_VAL; 
                    item.name[k] = L'\0';
                }
                else if (item.name[k] == L'?')
                {
                    if (namelen == 2) // Hack to accept '/?' which is fairly common
                    {
                        _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_PARAMETER\n");  
                        item.type = LK_TOKEN_TYPE_PARAMETER;   
                        item.name[k+1] = L'\0';                          
                    }
                    else
                    {            
                        _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_PARAMETER_VAL_OPT\n");
                        item.type = LK_TOKEN_TYPE_PARAMETER_VAL_OPT;  
                        item.name[k] = L'\0';                          
                    }                     
                }
                else 
                {
                    _LKTRACEW(L"     item.type = LK_TOKEN_TYPE_PARAMETER\n");
                    item.type = LK_TOKEN_TYPE_PARAMETER; 
                    item.name[k+1] = L'\0';
                }
                
                _LKTRACEW(L"     item.memberOf = %Iu\n", g_groupCount);
                item.memberOf = (int) g_groupCount;
                r = _Lk_TokenAdd(&item, isOptional, false);
                if (r == LK_R_OK)
                    iArgStart = i + 1;
                else
                {
                    //printf("TkListAdditem res=0x%08x \n", r);
                    if (r == LK_E_DUPLICATETOKEN)
                        return LK_E_DUPLICATEGROUPITEM;
                    break;
                }
            }
        } while (token[i++]);
        
        g_groupCount++;
        return LK_R_OK;       
    }
    else if (token[0] == L'$' && len > 1)
    {       
        LK_TOKEN_ITEM item;       
        item.memberOf = LK_NOGROUP;
        wcsncpy_s(item.name, LK_MAX_TOKENARG_LEN, token, LK_MAX_TOKENARG_LEN);     
        if (!isOptional) 
        {    
             _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_NAMED_ARGUMENT\n");    
            item.type = LK_TOKEN_TYPE_NAMED_ARGUMENT;
            return _Lk_TokenAdd(&item, isOptional, false);  
        }
    }
    else if (token[0] == L'/')
    {       
        LK_TOKEN_ITEM item; 
        item.memberOf = LK_NOGROUP;        
        wcsncpy_s(item.name, LK_MAX_TOKENARG_LEN, token, LK_MAX_TOKENARG_LEN);
        size_t namelen = wcslen(item.name);     
        
        // Check if it's a parameter with (optional) value associated

        size_t k = namelen-1;         
        if (token[k] == L':' || token[k] == L'=')
        {
            _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_PARAMETER_VAL\n");
            item.type = LK_TOKEN_TYPE_PARAMETER_VAL;            
            item.name[k] = L'\0';
        }
        else if (token[k] == L'?')
        {
            if (namelen == 2) // Hack to accept '/?' which is fairly common
            {
                _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_PARAMETER\n");  
                item.type = LK_TOKEN_TYPE_PARAMETER;   
                item.name[k+1] = L'\0';                          
            }
            else
            {            
                _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_PARAMETER_VAL_OPT\n");
                item.type = LK_TOKEN_TYPE_PARAMETER_VAL_OPT;  
                item.name[k] = L'\0';                          
            }                            
        }
        else 
        {
            _LKTRACEW(L"   item.type = LK_TOKEN_TYPE_PARAMETER\n");  
            item.type = LK_TOKEN_TYPE_PARAMETER;       
            item.name[k+1] = L'\0';
        }
        return _Lk_TokenAdd(&item, isOptional, false); 
    }
    return LK_E_INVALIDSYNTAX;
}

/* --------------------------------------------------------------------------*/
LK_RESULT _Lk_ClassifyToken(wchar_t* token)
{
    return _Lk_ClassifyTokenR(token, false);
}
/* ---------------------------------------------------------------------------*/
LK_RESULT _Lk_FindArgumentName(wchar_t* name, bool fMarkAsProcessed) {
    for (size_t i = 0; i < g_argList.count; i++) {
        if (!_wcsicmp(name, g_argList.item[i].name)) {
            if (fMarkAsProcessed)
                g_argList.item[i].isProcessed = true;
            return LK_R_TRUE;
        }
    }
    return LK_R_FALSE;
}
/*----------------------------------------------------------------------------*/

LK_RESULT _Lk_FindUnprocessedArg(wchar_t* outArg) {
    for (size_t i = 0; i < g_argList.count; i++) {
        if (!g_argList.item[i].isProcessed)
        {
            // If this an unprocessed named-argument, may be still is
            // unmapped to any token, so item[i].name is empty; in that
            // case we use the value instead, to return data.

            if (g_argList.item[i].name == NULL || wcslen(g_argList.item[i].name) == 0)
                wcscpy_s(outArg, LK_MAX_TOKENARG_LEN, g_argList.item[i].value);
            else
                wcscpy_s(outArg, LK_MAX_TOKENARG_LEN, g_argList.item[i].name);
            return LK_R_TRUE;
        }
    }
    return LK_R_FALSE;
}

/*----------------------------------------------------------------------------*/
LK_RESULT _Lk_EvalGroups(wchar_t* invalidArg)
{   
    size_t i;
    LK_TOKEN_LIST_ENTRY* pTok = NULL;  
    
    wchar_t pTokName[LK_MAX_TOKENARG_LEN];
    
    for (i = 0; i < g_groupCount; i++)
        g_groupInfo[i].selectedParam = NULL;
    
    for (i = 0; i < _Lk_TokenGetCount(); i++)
    {
        _Lk_TokenGet(i, &pTok);
        if ( pTok->item.memberOf != LK_NOGROUP )
        {      
            memset(pTokName, 0, sizeof(pTokName));
            _LKTRACEW(L"_Lk_EvalGroups: Evaluating token %s (group:%d)\n", pTok->item.name,pTok->item.memberOf);
            size_t j;
            for (j = 0; j < g_argList.count; j++)
            {   
                if (!_wcsicmp(g_argList.item[j].name, pTok->item.name))
                {
                    if (g_groupInfo[pTok->item.memberOf].selectedParam == NULL)
                    {        
                        if (pTok->item.type == LK_TOKEN_TYPE_PARAMETER_VAL && !g_argList.item[j].hasValue) 
                        {
                            _LKTRACEW(L"_Lk_EvalGroups: Argument %s potentially matched for group : %d, but missing value\n", 
                                      g_argList.item[j].name, pTok->item.memberOf);
                            wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, g_argList.item[j].name);
                            return LK_E_MISSINGVALUE;
                        }

                      _LKTRACEW(L"_Lk_EvalGroups: Argument %s matched for group : %d\n", g_argList.item[j].name,pTok->item.memberOf);
                      g_groupInfo[pTok->item.memberOf].selectedParam = g_argList.item[j].name;
                      g_argList.item[j].isProcessed = true;
                      break;
                    }
                    else
                    {
                        //Already got an argument from this  group
                        //Return the parameters we detected as contradictory
                        size_t cch = wcslen(g_groupInfo[pTok->item.memberOf].selectedParam)
                          + wcslen(g_argList.item[j].name) + 2;
                        wchar_t* invArgData = (wchar_t*) calloc (cch, sizeof(wchar_t));
                        if (!invArgData)
                            return LK_E_MALLOC;
                        wcscpy_s(invArgData,cch, g_groupInfo[pTok->item.memberOf].selectedParam);
                        wcscat_s(invArgData, cch, L" ");
                        wcscat_s(invArgData, cch, g_argList.item[j].name);
                        wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, invArgData);
                        free(invArgData);
                        return LK_E_CONTRADICTARG;
                    }
                }                 
            }
        }       
    }  

    for (i = 0; i < g_groupCount; i++)
    {
        if (!g_groupInfo[i].selectedParam && !g_groupInfo[i].isOptional)
        {
            wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, L"?");
            return LK_E_NOGROUPMEMBERCHOSEN;    
        }
    }
    return LK_R_OK;
}
/* ---------------------------------------------------------------------------*/
LK_RESULT _Lk_MapNamedArgs() {
    _LKTRACE("_Lk_MapNamedArgs\n");

    LK_TOKEN_LIST_ENTRY* pTok = NULL;
    size_t i = 0, j = 0;

    for (i = 0; i < _Lk_TokenGetCount(); i++) {
        _Lk_TokenGet(i, &pTok);

        _LKTRACEW(L"  Got token %Iu/%Iu name=%s \t[%s] (Opt=%d) \n", i + 1, _Lk_TokenGetCount(),
                  pTok->item.name, g_lkTypeStr[pTok->item.type], pTok->isOptional);

        if (pTok->item.type == LK_TOKEN_TYPE_NAMED_ARGUMENT) {

            // Search a command line argument to map with 
            // this named-argument token           

            for (j = 0; j < g_argList.count; j++) {
                if (!(g_argList.item[j].isParam || g_argList.item[j].isMapped)) {
                    g_argList.item[j].isMapped = true;
                    g_argList.item[j].isProcessed = true;
                    break;
                }
            }
            wcscpy_s(g_argList.item[j].name, LK_MAX_TOKENARG_LEN, pTok->item.name);
            _LKTRACEW(L"     Mapped named argument %s\n", pTok->item.name);
        }
    }

    return LK_R_OK;
}
/*---------------------------------------------------------------------------*/

LK_RESULT _Lk_GetParamValue(const wchar_t* paramName, wchar_t** paramValue) {

    if (!paramName | !paramValue | (paramName && wcslen(paramName) == 0))
        return LK_E_INVALIDARG;

    size_t i;
    for (i = 0; i < g_argList.count; ++i) {
        if (!_wcsicmp(paramName, g_argList.item[i].name)) {
            if (g_argList.item[i].hasValue) {
                *paramValue = g_argList.item[i].value;
                return LK_R_OK;
            } else {
                *paramValue = NULL;
                return LK_E_UNDEFINED;
            }
        }
    }
    *paramValue = NULL;
    return LK_E_NOTFOUND;
}

/*---------------------------------------------------------------------------*/

LK_RESULT _Lk_EvalArgs(wchar_t* invalidArg) {
    _LKTRACE("_Lk_EvalArgs\n");
    LK_RESULT r = LK_R_OK;

    if (LK_R_OK != (r = _Lk_EvalGroups(invalidArg)))
        return r;

    if (LK_R_OK != (r = _Lk_MapNamedArgs()))
        return r;

    size_t i;
    LK_TOKEN_LIST_ENTRY* pTok = NULL;

    for (i = 0; i < _Lk_TokenGetCount(); i++) 
    {
        _Lk_TokenGet(i, &pTok);

        if (pTok->item.memberOf != LK_NOGROUP && g_groupInfo[pTok->item.memberOf].selectedParam) 
        {
            _LKTRACEW(L"_Lk_EvalArgs: IGNORING token %s: Member of group %d, but already got a parameter mapped\n",
                      pTok->item.name, pTok->item.memberOf);
            continue;
        }
        
        switch (pTok->item.type) 
        {
            case LK_TOKEN_TYPE_PARAMETER:

                if (_Lk_FindArgumentName(pTok->item.name, true) == LK_R_FALSE && !pTok->isOptional) 
                {                       
                    _LKTRACEW(L"_Lk_EvalArgs: Required argument NOT FOUND: %s\n", pTok->item.name);
                    wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, pTok->item.name);
                    return LK_E_MISSINGARG;
                }

                break;

            case LK_TOKEN_TYPE_PARAMETER_VAL:

                if (_Lk_FindArgumentName(pTok->item.name, true) == LK_R_FALSE && !pTok->isOptional) 
                {
                    _LKTRACEW(L"_Lk_EvalArgs: Required argument NOT FOUND: %s\n", pTok->item.name);
                    wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, pTok->item.name);
                    return LK_E_MISSINGARG;
                }

                if (!pTok->isOptional) 
                {
                    wchar_t* paramVal;
                    if (_Lk_GetParamValue(pTok->item.name, &paramVal) != LK_R_OK) 
                    {
                        wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, pTok->item.name);
                        return LK_E_MISSINGVALUE;
                    }
                }
                break;

            case LK_TOKEN_TYPE_PARAMETER_VAL_OPT:
                if (_Lk_FindArgumentName(pTok->item.name, true) == LK_R_FALSE && !pTok->isOptional) {
                    _LKTRACEW(L"_Lk_EvalArgs: Required argument NOT FOUND: %s\n", pTok->item.name);
                    wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, pTok->item.name);
                    return LK_E_MISSINGARG;
                }

                break;

            case LK_TOKEN_TYPE_NAMED_ARGUMENT:
                break;

            default:
                assert(false);
                break;
        }
    }
    

    if (_Lk_FindUnprocessedArg(invalidArg) == LK_R_TRUE)
        return LK_E_INVALIDARG;
    
    return LK_R_OK;
}

/*---------------------------------------------------------------------------*/
LK_RESULT _Lk_VerifyParamNames(wchar_t* invalidArg) {

    LK_TOKEN_LIST_ENTRY* pTok;
    bool fOk;

    for (size_t i = 0; i < g_argList.count; i++) {
        if (g_argList.item[i].name[0] == L'/') {
            fOk = false;
            for (size_t j = 0; j < _Lk_TokenGetCount(); j++) {
                _Lk_TokenGet(j, &pTok);

                if (pTok->item.type == LK_TOKEN_TYPE_PARAMETER ||
                    pTok->item.type == LK_TOKEN_TYPE_PARAMETER_VAL ||
                    pTok->item.type == LK_TOKEN_TYPE_PARAMETER_VAL_OPT) {
                    if (! _wcsicmp(pTok->item.name, g_argList.item[i].name)) {
                        fOk = true;
                        break;
                    }
                }
            }

            if (!fOk) {
                wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, g_argList.item[i].name);
                return LK_E_INVALIDARG;
            }
        }
    }
    return LK_R_OK;
}
     
/*---------------------------------------------------------------------------*/
LK_RESULT _Lk_ValidateArgV(int argc, wchar_t** argv, wchar_t* invalidArg)
{
    _LKTRACE("_Lk_ValidateArgV\n");
    
    // Do first pass to arguments: verify duplicates, basic style, etc.
    //
    int    i;
    size_t j;
    for (i = 1; i < argc; i++)
    {
        // Check if there are duplicates
       
       _LKTRACE("  Verifying duplicates...\n");       
       for (j = 0; j < g_argList.count; ++j)
       {            
            if ( (_wcsicmp(argv[i], g_argList.item[j].name) == 0)
               &&(g_argList.item[j].name[0] == L'/'))
            {
                wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, argv[i]);
                _LKTRACEW(L"  Duplicate %s argument found\n", argv[i]);
                return LK_E_DUPLICATEARG;  
            }
       }
        
        // If argument includes '=' or ':', strip value and parameter name
        if (argv[i][0] == L'/')
        {
            _LKTRACEW(L"  %s: Parameter argument\n", argv[i]);            
            wchar_t* delim = NULL;
            g_argList.item[j].isParam = true;
            
            if ( (delim = wcschr(argv[i], L':')) || 
                 (delim = wcschr(argv[i], L'=')) )            
            {
                if ( *(delim+1) ==  L'\0') // delimiter present but no value!
                {
                    wcscpy_s(invalidArg, LK_MAX_TOKENARG_LEN, argv[i]);               
                    return LK_E_MISSINGVALUE;
                }
                else
                {                    
                    if (wcslen(delim + 1) > LK_MAX_PARAMVALUE_LEN)
                    {
                        wcsncpy_s(invalidArg, LK_MAX_TOKENARG_LEN, argv[i], delim - argv[i]);
                        return LK_E_VALUELENGTH;
                    }
                    
                    wcsncpy_s(g_argList.item[j].name, LK_MAX_TOKENARG_LEN, argv[i], 
                             delim - argv[i]);             
                    wcscpy_s(g_argList.item[j].value, LK_MAX_PARAMVALUE_LEN, 
                             delim + 1);
                    
                    g_argList.item[j].hasValue = true;
                }
            }
            else 
            {                
                wcscpy_s(g_argList.item[j].name, LK_MAX_TOKENARG_LEN, argv[i]);
                g_argList.item[j].hasValue = false;                 
            }
        }
        else   // non-parameter argument
        {
            _LKTRACEW(L"  %s: Non-Parameter argument \n", argv[i]);
            g_argList.item[j].isParam  = false;
            g_argList.item[j].hasValue = true;
            wcscpy_s(g_argList.item[j].value, LK_MAX_PARAMVALUE_LEN, argv[i]);  
            wcscpy_s(g_argList.item[j].name, LK_MAX_PARAMVALUE_LEN, LK_UNMAPPED_ARG_NAME);
        }
        
        g_argList.item[j].matchingGroup = LK_NOGROUP;
        g_argList.item[j].isMapped      = false;
        g_argList.item[j].isProcessed   = false;
        g_argList.count = j+1;
    }
    return LK_R_OK;
}

/*---------------------------------------------------------------------------*/
LK_RESULT LkInit()
{
    return _Lk_TokenListInit(LK_MAX_TOKENARG_COUNT);
}

/*---------------------------------------------------------------------------*/
LK_RESULT LkFree(void)
{
    _Lk_TokenListFree();                 
    return LK_R_OK;
}

/******************************************************************************

   Public API begins here
   
   ***************************************************************************/

   /* ---------------------------------------------------------------------------
    Sets the syntax for the command-line to be validated against.
   
   syntax:       The syntax specification string, with tokens separated by 
                 spaces.
   invalidToken: A pointer to a character array to store the token that 
                 caused an error, if it can be determined. 
   cchBuf:      The size of the invalidArg buffer, in characters. Must
                be >= LK_MAX_TOKENARG_LEN.                 
   
   Returns: LK_R_OK          if successful. 
            LK_E_MALLOC      if memory allocation error occurred.
            LK_E_TOKENCOUNT  if number of specified token exceeds the constant
                             LK_MAX_TOKENARG_COUNT.
            LK_E_UNINITIALIZED if library is not initialized yet.
            LK_E_NULLPOINTER   if NULL pointer is passed in call.
            LK_E_INVALIDSYNTAX if syntax string is not allowed.
            
            Other error codes can be returned. (See LK_RESULT)
  
   --------------------------------------------------------------------------*/
LK_RESULT LkSetSyntax(const wchar_t* syntax, wchar_t* invalidToken, size_t cchBuf)
{    
    _LKTRACE("_Lk_SetSyntax\n");
    
    if (cchBuf < LK_MAX_TOKENARG_LEN)
        return LK_E_INSUFFICIENTBUFFER;
    
    if (!g_tokenList)
        return LK_E_UNINITIALIZED;
    if (!syntax || !invalidToken)
        return LK_E_NULLPOINTER;
    
    _Lk_TokenListClear();
        
    g_syntaxValid = true;
    g_tokenCount    = 0;
    g_groupCount    = 0;
    
    LK_RESULT res;
    int    i = 0;
    int    iTokStart = 0;
    while(1)
    {        
        if (syntax[i] == L' '  || syntax[i] == L'\0')
        {           
            size_t cbToken = sizeof(wchar_t) * (i - iTokStart);

            if (cbToken  > LK_MAX_TOKENARG_LEN * sizeof (wchar_t))
                return LK_E_TOKENLENGTH;

            wchar_t* token = (wchar_t * )malloc(cbToken + sizeof(wchar_t));
            if (!token)
                return LK_E_MALLOC;
            
            memcpy(token, syntax + iTokStart, cbToken);
            token[cbToken/sizeof(wchar_t)] = L'\0'; 
            res = _Lk_ClassifyToken(token);
            if (LK_SUCCESS(res)){              
                free(token);
            }
            else 
            {
                _LKTRACEW(L"  Token classification fail (%s) (0x%08x)\n", token, res);
                g_syntaxValid = false;
                wcscpy_s(invalidToken, LK_MAX_TOKENARG_LEN, token);
                return res;
            }           
            if (syntax[i] == L'\0')
                break;
            
            while (syntax[++i] == L' '); /* Ignore multiple spaces */
            iTokStart = i;
            
            if (g_tokenCount == LK_MAX_TOKENARG_COUNT)
                return LK_E_TOKENCOUNT;        
        }
        else
            i++;
    };
    
    _LKTRACEW(L"  Function successful\n");
    return LK_R_OK;
}

/* ---------------------------------------------------------------------------
   Returns if the specified parameter or named-argument is present.
   
   paramName:   The command line parameter or named-argument to query.
   
   Returns: LK_R_OK       if successful.            
            LK_E_NOTFOUND if parameter does not exist.
   --------------------------------------------------------------------------*/
LK_RESULT LkIsParamPresent(const wchar_t* paramName)
{
    if (!paramName || !wcslen(paramName))
        return LK_E_INVALIDARG;
    
    size_t i;
    for (i = 0; i < g_argList.count; ++i)
        if (!_wcsicmp(paramName, g_argList.item[i].name))
            return LK_R_OK;
    
    return LK_E_NOTFOUND;
}

/* ---------------------------------------------------------------------------
   Returns the specified parameter or named-argument value. 
   
   paramName:   The command line parameter or named-argument to query.
   paramValue:  A pointer to a character array to store the returned value.
   
   Returns: LK_R_OK        if successful. 
            LK_E_UNDEFINED if no value was specified on the command-line.
            LK_E_NOTFOUND  if parameter or named-argument does not exist.
            
   Remarks:
   
   - If return value is not LK_R_OK, *paramValue will be set to NULL.
   - The caller *must not free* the returned pointer.
   --------------------------------------------------------------------------*/
LK_RESULT LkGetParamValue(const wchar_t* paramName, wchar_t** paramValue) {
    return _Lk_GetParamValue(paramName, paramValue);
}

/* ---------------------------------------------------------------------------
   Parses the specified command-line.
   
   argc:        The argument count.
   argv:        The array of arguments in the command line.
   invalidArg:  A  character array to store the argument that 
                is causing a parsing error, if it can be determined.
   cchBuf:      The size of the invalidArg buffer, in characters. Must
                be >= LK_MAX_TOKENARG_LEN.                
   
   Returns: LK_R_OK             if successful. 
            LK_E_INVALIDSYNTAX  if syntax to be parsed in invalid.
   --------------------------------------------------------------------------*/
LK_RESULT LkParse(int argc, wchar_t** argv, wchar_t* invalidArg, size_t cchBuf)
{    
    if (cchBuf < LK_MAX_TOKENARG_LEN)
        return LK_E_INSUFFICIENTBUFFER;
    
    g_argList.count = 0;
    
    if (!g_syntaxValid)
        return LK_E_INVALIDSYNTAX;
   
    LK_RESULT r = _Lk_ValidateArgV(argc, argv, invalidArg);

    if (LK_SUCCESS(r))
        r = _Lk_VerifyParamNames(invalidArg);

    if (LK_SUCCESS(r))
        r = _Lk_EvalArgs(invalidArg);
   
    return r;   
}

/* ---------------------------------------------------------------------------
   Parses the specified command-line.
   
   commandLine: The command-line to parse.
   invalidArg:  A character array to store the argument that 
                is causing a parsing error, if it can be determined.
   cchBuf:      The size of the invalidArg buffer, in characters. Must
                be >= LK_MAX_TOKENARG_LEN.
   
   Returns: LK_R_OK             if successful. 
            LK_E_INVALIDSYNTAX  if syntax to be parsed is invalid.  
   --------------------------------------------------------------------------*/
LK_RESULT LkParse2(wchar_t* commandLine, wchar_t* invalidArg, size_t cchBuf)
{
    if (cchBuf < LK_MAX_TOKENARG_LEN)
        return LK_E_INSUFFICIENTBUFFER;
    
    if (!g_syntaxValid)
        return LK_E_INVALIDSYNTAX;
    if (!commandLine)
        return LK_E_INVALIDARG;

    wchar_t* wCmdLineBuf = _wcsdup(commandLine);
    if (!wCmdLineBuf)
        return LK_E_MALLOC;

    wchar_t* buf = NULL;
    wchar_t* thisarg = wcstok_s(wCmdLineBuf, L" ", &buf);    
    wchar_t* argv[LK_MAX_TOKENARG_COUNT];
    argv[0] =  L"DUMMY.EXE"; //this wont be used
    int argc = 1;
    while (thisarg != NULL && argc < LK_MAX_TOKENARG_COUNT)
    {
        argv[argc++] = thisarg; 
        thisarg = wcstok_s(NULL, L" ", &buf);
    }
    
    LK_RESULT r = LkParse(argc, argv, invalidArg, cchBuf);
    free(wCmdLineBuf);

    return r;
}

/* ---------------------------------------------------------------------------
   Returns the extended error message for the last operation.
  --------------------------------------------------------------------------*/
const wchar_t* const LkGetExtErrorString() {
    return g_exErrBuffer;
}

/*-----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif 

#endif // __LINKOM_H__f82e85a35f914826a6af35aa02839625