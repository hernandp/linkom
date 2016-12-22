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

#define LK_DEFINE_USER_LIMITS
#define LK_MAX_TOKENARG_LEN      16 /* Maximum argument and token length */
#define LK_MAX_TOKENARG_COUNT    8  /* Maximum argument and token count  */
#define LK_MAX_GROUP_COUNT       3  /* Maximum group count */
#define LK_MAX_GR_MEMBERS_COUNT  4  /* Maximum members per group count */
#define LK_MAX_PARAMVALUE_LEN    16  /* Max str length for param. values*/
//#define LK_ENABLE_TRACE
#include "linkom.h"

#define TESTINIT()        int _testc = 0; int _testok = 0; bool _r;
#define VERIFY(_expr)     _testc++;printf("%s " #_expr "\n",( _r = (_expr)) ? "[ OK ]":"[FAIL]"); if(_r) _testok++;
#define TESTEND           printf("\nTest results: %d executed, %d successful, %d failed\n", _testc, _testok, _testc - _testok); 
    
int main(int argc, char** argv)
{
    TESTINIT();
    wchar_t invToken[LK_MAX_TOKENARG_LEN];    
    wchar_t invArg[LK_MAX_TOKENARG_LEN];
    wchar_t* val = NULL;
    
    printf("-------- Initialization -------------------------------------- \n");
    VERIFY(LK_SUCCESS(LkInit()));
        
    printf("-------- Boundary checks ------------------------------------- \n");
    VERIFY(LkParse2(L"Parsing_without_syntax", invToken, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDSYNTAX);
    VERIFY(LkSetSyntax(L"[/A] [/XXXXXXXXXXXXXXXXXXXXXXX]", invToken, LK_MAX_TOKENARG_LEN) == LK_E_TOKENLENGTH);
    VERIFY(LkSetSyntax(L"[/0] [/1] [/2] [/3] [/4] [/5] [/6] [/7] [/8]", invToken, LK_MAX_TOKENARG_LEN) == LK_E_TOKENCOUNT);
    VERIFY(LkSetSyntax(L"{/a|/b} {/c|/d} {/e|/f} {/g|/h}", invToken, LK_MAX_TOKENARG_LEN) == LK_E_GROUPCOUNT);
    VERIFY(LkSetSyntax(L"[/A=]", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/A=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",invArg, LK_MAX_TOKENARG_LEN) == LK_E_VALUELENGTH);
    
    printf("-------- Basic checks   -------------------------------------- \n");
 
    VERIFY(LkSetSyntax(L"[/A] [/B] [/C]", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkSetSyntax(L"[/A] [C*", invToken, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDSYNTAX);
    VERIFY(invToken != NULL && !wcscmp(invToken,L"[C*"));
    VERIFY(LkSetSyntax(L"[/B] [/B]", invToken, LK_MAX_TOKENARG_LEN) == LK_E_DUPLICATETOKEN);
    VERIFY(LkSetSyntax(L"[/A] [/B]", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    
    VERIFY(LkParse2(L"", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);      
    VERIFY(LkParse2(L"/A", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/B", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);    
    VERIFY(LkParse2(L"/A /B", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/B /A", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/B /B", invArg, LK_MAX_TOKENARG_LEN) == LK_E_DUPLICATEARG);   
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/B"));
    VERIFY(LkParse2(L"/Z", invArg, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDARG);  
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/Z"));
    
    printf("-------- Optional arguments check  -------------------------- \n");
    VERIFY(LkSetSyntax(L"/A [/B]", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"", invArg, LK_MAX_TOKENARG_LEN) == LK_E_MISSINGARG);   
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/A"));
    VERIFY(LkParse2(L"/C", invArg, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDARG);    
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/C"));
    VERIFY(LkParse2(L"/A /C", invArg, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDARG);    
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/C"));
    VERIFY(LkParse2(L"/A", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);    
    VERIFY(LkParse2(L"/A /B", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);  
    VERIFY(LkParse2(L"/B /A", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);  
    
    printf("-------- Valued parameters check ---------------------------- \n");

    VERIFY(LkSetSyntax(L"/A= /B? [/C]", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/A=2 /B=", invArg, LK_MAX_TOKENARG_LEN) == LK_E_MISSINGVALUE);
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/B="));
    VERIFY(LkParse2(L"/A=1 /B=xyz /C", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);  
    VERIFY(LkGetParamValue(L"/C", &val) == LK_E_UNDEFINED);    
    VERIFY(LkParse2(L"/A=1 /B", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);      
    VERIFY(LkParse2(L"/A", invArg, LK_MAX_TOKENARG_LEN) == LK_E_MISSINGVALUE);   
    VERIFY(invArg != NULL && !wcscmp(invArg, L"/A"));
    VERIFY(LkGetParamValue(L"/C", &val) == LK_E_NOTFOUND);
    VERIFY(LkParse2(L"/A=1 /C", invArg, LK_MAX_TOKENARG_LEN) == LK_E_MISSINGARG);
    VERIFY(LkParse2(L"/A=1 /B=xyz", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);     
    VERIFY(LkIsParamPresent(L"/A") == LK_R_OK);    
    VERIFY(LkIsParamPresent(L"/B") == LK_R_OK);
    VERIFY(LkIsParamPresent(L"/X") == LK_E_NOTFOUND);      
    VERIFY(LkGetParamValue(L"/A", &val) == LK_R_OK);
    VERIFY(val != NULL && !wcscmp (val, L"1"));
    VERIFY(LkGetParamValue(L"/B", &val) == LK_R_OK);
    VERIFY(val != NULL && !wcscmp (val, L"xyz"));
    VERIFY(LkGetParamValue(L"/D", &val) == LK_E_NOTFOUND);
    
    printf("-------- Named arguments check ------------------------------- \n");
    VERIFY(LkSetSyntax(L"$file [$a] $b $c", invToken, LK_MAX_TOKENARG_LEN)== LK_E_INVALIDSYNTAX);
    VERIFY(LkSetSyntax(L"$file $a $b $c", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"test.txt 1 2 3", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkGetParamValue(L"$file", &val) == LK_R_OK);
    VERIFY(val != NULL && !wcscmp (val, L"test.txt"));
    VERIFY(LkParse2(L"test.txt 1 2", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"z z z", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
 
    printf("--- Named arguments check (getting values) ------------------- \n");
    VERIFY(LkParse2(L"test.txt 1", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkGetParamValue(L"$file", &val) == LK_R_OK);
    VERIFY(val != NULL && !wcscmp (val, L"test.txt"));
    VERIFY(LkGetParamValue(L"$a", &val) == LK_R_OK);
    VERIFY(val != NULL && !wcscmp (val, L"1"));
    VERIFY(LkGetParamValue(L"$b", &val) == LK_E_NOTFOUND);
    VERIFY(LkGetParamValue(L"$c", &val) == LK_E_NOTFOUND);
    
    printf("--- Named arguments check (count exceeded) ------------------- \n");
    VERIFY(LkParse2(L"test.txt 1 2 3 4 5 6 7 8 9", invArg, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDARG);
    VERIFY(invArg != NULL && !wcscmp (invArg, L"4"));
    
    printf("--- group argument test -------------------------------------- \n");
    VERIFY(LkSetSyntax(L"{x}", invToken, LK_MAX_TOKENARG_LEN)== LK_E_INVALIDSYNTAX);
    VERIFY(invToken != NULL && !wcscmp (invToken, L"{x}"));
    VERIFY(LkSetSyntax(L"{/x}", invToken, LK_MAX_TOKENARG_LEN)== LK_E_INVALIDSYNTAX);
    VERIFY(invToken != NULL && !wcscmp (invToken, L"{/x}"));
    VERIFY(LkSetSyntax(L"{/x|}", invToken, LK_MAX_TOKENARG_LEN)== LK_E_INVALIDSYNTAX);
    VERIFY(invToken != NULL && !wcscmp (invToken, L"{/x|}"));
    VERIFY(LkSetSyntax(L"{/on|/off} {/on|/x}", invToken, LK_MAX_TOKENARG_LEN) == LK_E_DUPLICATEGROUPITEM);   
    VERIFY(LkSetSyntax(L"{/on|/off|/auto}", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK); 
    VERIFY(LkParse2(L"/on",   invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);    
    VERIFY(LkParse2(L"/off",  invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/auto", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/auto /on",  invArg, LK_MAX_TOKENARG_LEN) == LK_E_CONTRADICTARG);    
    VERIFY(LkParse2(L"/auto /off", invArg, LK_MAX_TOKENARG_LEN) == LK_E_CONTRADICTARG);
    VERIFY(LkParse2(L"/on   /off", invArg, LK_MAX_TOKENARG_LEN) == LK_E_CONTRADICTARG);   
        
    VERIFY(LkSetSyntax(L"{/on|/off|/auto} {/x|/y|/z} {/a|/b}", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/on /a", invArg, LK_MAX_TOKENARG_LEN) == LK_E_NOGROUPMEMBERCHOSEN);
    VERIFY(LkParse2(L"/b /z /off", invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/b /z /off /y", invArg, LK_MAX_TOKENARG_LEN) == LK_E_CONTRADICTARG);    
  
    VERIFY(LkSetSyntax(L"{/on|/off|/auto} [{/x?|/y|/z}] {/a=|/b}", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"/on",   invArg, LK_MAX_TOKENARG_LEN) == LK_E_NOGROUPMEMBERCHOSEN); 
    VERIFY(LkParse2(L"/on /b",   invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK); 
    VERIFY(LkParse2(L"/on /a",   invArg, LK_MAX_TOKENARG_LEN) == LK_E_MISSINGVALUE);
    VERIFY(LkParse2(L"/on /a=1000",   invArg, LK_MAX_TOKENARG_LEN) == LK_R_OK); 
    VERIFY(LkGetParamValue(L"/a", &val) == LK_R_OK);
    VERIFY(val != NULL && !wcscmp(val, L"1000")); 
    
    VERIFY(LkSetSyntax(L"[{/help|/?}] [/fat=] /size= /boot= /media= [/lic] $file", invToken, LK_MAX_TOKENARG_LEN) == LK_R_OK);
    VERIFY(LkParse2(L"xxx", invArg, LK_MAX_TOKENARG_LEN) == LK_E_MISSINGARG);
    VERIFY(LkGetParamValue(L"$file", &val) == LK_R_OK);
    VERIFY(LkParse2(L"/zzzz", invArg, LK_MAX_TOKENARG_LEN) == LK_E_INVALIDARG);
    VERIFY(val != NULL && !wcscmp(val, L"xxx"));
   
    printf("--- Shutdown ------------------------------------------------- \n");
    
    VERIFY(LK_SUCCESS(LkFree()));    
    TESTEND;
}

static void Interactive(void)
{
    TESTINIT();
    VERIFY(LK_SUCCESS(LkInit()));
    wchar_t syntax[1024];
    wchar_t cmdline[1024];
    wchar_t invToken[LK_MAX_TOKENARG_LEN];
    printf("\nsyntax? ");
    fgetws (syntax, 1024, stdin);
    syntax[wcscspn(syntax, L"\n")] = L'\0';
    
    LK_RESULT r;
    if (!LK_SUCCESS( r = LkSetSyntax(syntax, invToken, LK_MAX_TOKENARG_LEN)))
    {
        printf("\nLkSetSyntax error 0x%08X / element: %ls\n", r, invToken);
        free(invToken);
        return;
    }    
    
    printf("\ncmdline? ");
    fgetws (cmdline, 1024, stdin);
    cmdline[wcscspn(cmdline, L"\n")] = L'\0';
    
    wchar_t invArg[LK_MAX_TOKENARG_LEN];
    if (!LK_SUCCESS(r = LkParse2(cmdline, invArg, LK_MAX_TOKENARG_LEN)))
    {
        if (r == LK_E_UNKNOWNARG)
        {
            printf("\nParse error, unknown argument: %ls\n", invArg);
        }
        free(invToken);
        return;
    }
       
    VERIFY(LK_SUCCESS(LkFree()));    
}
