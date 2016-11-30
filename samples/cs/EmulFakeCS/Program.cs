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

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmulFakeCS
{
    class Program
    {
        static void Main(string[] args)
        {
            Linkom.ILinkom linkom = new Linkom.Linkom();
            string invToken;
            string invArg;
            if (args.Length == 0)
            {
                ShowUsage();
                return;
            }
            try
            {
                linkom.SetSyntax("[/?] /DISK0= /RAM= /CACHE? [/DD?] [/DISK1=] [/FAST] [{/ALTCPU1|/ALTCPU2}] {/CGA|/EGA|/VGA|/MCGA} $biosrom $firmware0rom", out invToken);
                linkom.Parse(string.Join(" ", args), out invArg);
            }
            catch
            {
                Console.WriteLine(linkom.GetExtErrorString());
                return;
            }
            
            if (linkom.IsParamPresent("/?") == 0)
            {
                ShowUsage();
                return;
            }

            string BIOSROM = null, FirmwareROM = null;
            try
            {
                linkom.GetParamValue("$biosrom", out BIOSROM);
                linkom.GetParamValue("$firmware0rom", out FirmwareROM);
            }
            catch
            {
                if (BIOSROM == null)
                    Console.WriteLine("WARNING: Please provide a BIOS ROM name.");

                if (FirmwareROM == null)
                    Console.WriteLine("WARNING: Please provide a BIOS ROM name.");
            }




            System.Console.ReadKey();
        }

        private static void ShowUsage()
        {
            throw new NotImplementedException();
        }
    }
}
