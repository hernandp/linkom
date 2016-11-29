# Linkom
**A single-header, command-line parser library in C**

## License Terms

Linkom Library 
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

## Introduction

Linkom is a single-header, command-line parser library written in C. The language and distribution decision was based on the fact that A LOT of command line utilities are still being written in C (and not just C++).

For programmers using other languages than C/C++, a COM component wrapper is projected for the near future.

Note that the symbols and jargon that the library and it's documentation use, such as "/" character for parameters and { } for grouping them, is based on the classic DOS/CPM/Microsoft documentation style.  This is different from the UNIX/Linux and GNU tradition where '--' and "options"  are used instead.

In the future, the library *could* support varying "symbolisms" to handle different styles.

## A note about why wchar_t

As Linkom was developed at first for homebrew Windows platform projects, the character arrays  and functions used are `wchar_t` based. A conversion to `char`-based, UTF8 strings, maybe interesting to implement. 

## Terminology

The following conventions in this library and it's documentation. 

**Syntax** is the string the command-line will be validated against by the parser.

**Token** is a component in the *Syntax* string, delimited by spaces.

**Argument** is the component in which a command-line is separated into. 

**Parameter** is an argument starting with '/' and that may carry a value after ':' or '=' character. This is the analog of the UNIX "option".

## Building the library

*Linkom* is a single-header library containing C code, so it does not require building any LIB or DLL file to be used; any reasonably modern C compiler should handle it. The file TESTRUN.C is included for development and testing purposes, it will execute several tests and report which of them failed.

The library was tested on Windows and Microsoft Visual C++ 2012/2015 compilers only. 

## Using the library

You can include the library code by just inserting 

    #include "linkom.h"

at the top of your desired C/C++ source file.

All the public API  functions return an unsigned integer value from the `LK_RESULT` enumeration (see the header file for reference). You can use the `LK_SUCCESS` macro to test for the `LK_R_OK` value.

Let's continue looking at typical usage examples.

### Initialization and shutdown

Before anything, you should initialize the library with `LkInit`. To shutdown, call `LkFree` to free used memory resources.

    /* Ex 1: Initialization and shutdown */

    LK_RESULT r = LkInit();
    if (r != LK_R_OK) 
    {
       printf ("Initialization error");
       exit(EXIT_FAILURE);
    }

    // ...
    // use library here
    // ...
    
    
    LkFree();

### Basic command line parsing

The basic procedure to work with a command line string is to stablish a syntax for the parser to work against first, and call the parser itself with a command-line which will return a successful return value if the command-line is compliant with the syntax specification.

    /* Ex 2: Parsing a command line, basic example
    
    wchar_t invToken[256], invArg[256];
    LK_RESULT r = LkSetSyntax(L"$filename", invToken, _countof(invToken)));
    if (r == LK_R_OK)
    {
       r = LkParse(argc, argv, invArg, _countof(invArg));
       if (r == LK_R_OK)
       {
          // Work with args...
       }
    }
    
The syntax specification says that we'll accept any command line with one named argument (`$filename`) that may be present or not. Named arguments, preceded with the `$` prefix, are mapped 1-to-1 to the arguments found in
the command line specified at the `LkParse` call. 

Use spaces between syntax tokens: `$a $b` is allowed while `$a$b` is not.

The second parameter of `LkSetSyntax`, as well as the third of `LkParse`, must supply a pointer to a buffer with a minimum of `LK_MAX_TOKENARG_LEN` characters. If the function fails, this buffer will be filled with the token or parameter where the syntax evaluation or parsing stopped due to error.

You can get the argument mapped using the `LkGetParamValue` function, as follows:

    wchar_t* file;
    LkGetParamValue (L"$filename", &file);
    if (file)
    {
       printf("Provided filename: %S", file);
    }
    
Now suppose the following syntax spec:

    LkSetSyntax(L"$a $b $c $d", invToken, _countof(invToken)));

This will assign each argument from the command line to each syntax token ($a...$d) in order. 

### Using parameters

In addition of named-parameters, standard parameter tokens beginning with `/` can be used in the syntax-specification with varying forms. First, any parameter enclosed with brackets `[ ]` will make it optional. Second, a parameter may ask for a value (which can be also optional), using `=` or `?` character suffixes. Briefly,

* `/X` token specifies a required parameter.
* `/FILE=` token specifies a required parameter and value (e.g: a valid command line argument to match could be `/FILE=readme.txt` or `/FILE:copy.com`)
* `/SYS?` token specifies a required parameter and an optional value (valid command line arguments to match could be `/SYS=unix`, `/SYS:win` or `SYS`)).

Enclosing the above parameters with `[ ]` will make them optional.

### Exclusive parameter groups

Enclosing a set of parameters with braces `{ }` ,separated by `|` allows to enforce the use of one parameter at maximum from the group. If two or more parameters from the same group are on the command line string, the parser will fail. Let's see an example:

    LkSetSyntax(L"{/x|/y|/z}", invToken, _countof(invToken)));
	
With the above syntax specification, parsing "/x" "/y" or "/z" will succeed, but "/x /y" or "/z /x" will not. 
A group token can be optional and the parameter themselves can be valued parameters or optionally-valued parameters, as in 

    LkSetSyntax(L"{/x|/y} [{/a?|b=|/c}]", invToken, _countof(invToken))); 
	
From this syntax spec, `/x /a:1`  `/y /a`  or  `/x`  will be parsed successfully.

Note that *named-arguments are not supported within groups*.

### Error messages facility

The library can facilitate handling the printing of all or a subset of error conditions during parsing phase using the 
`LkGetExtErrorString` function, which will print a descriptive message along with (optional) information about the source of the conflict.

Suppose the following syntax specification:

`{/A|/B}`

Parsing the command line `/A /B` against that syntax will make the `LkParse` call to return with the `LK_E_CONTRADICTARG` value, with the last character array argument of `LkParse`  filled with the arguments in conflict. To inform the user you could simply call e.g:


    if (r != LK_R_OK) {
        _putws(LkGetExtErrorString());
        return -1;
    }

The above example will print:

`Contradictory arguments: /A /B`

See the header file for the message-table, which you can localize (I18N) to suit your needs.

## FAQ

* Q: Are arrays of strings supported "automagically"?  e.g: passing one or more filenames in the command-line.
  
* A: There is no provided support. You can handle it by using a named argument or a parameter with a value, for example the following syntax specifications are an example:
   
         LkSetSyntax(L"/files=", invToken, _countof(invToken))); 
         LkSetSyntax(L"$filenamelist", invToken, _countof(invToken))); 
   
   where the following would be acceptable command-lines to match: `/FILES=1.txt,2.txt,3.txt,4.txt` and  `1.txt;2.exe;3.pif;4.com` respectively.
   
   In each case you should handle the comma-delimited list yourself after getting value:
   
         LkGetParamValue(L"$filenamelist", wFileList);
		 if ( wFileList )
		      // parse command-delimited value...
			  
			  
			  