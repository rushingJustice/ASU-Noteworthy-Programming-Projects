/*********************************************
Design3D Program
Copyright(c) 2000,  Hawthorne & York, Inc.
All rights reserved
********************************************

Created On:         March 1, 2001
Created By:         S. D. Rajan
Last modified by:   S. D. Rajan
Last modified on:   May 28, 2001
Functionality:      
Input:              
Output:             
List of Changes:    
    4-18-2001  SDR  Documentation added
    5-28-2001  SDR  Converted to class-based functions
	8-09-2001  ADM  Lint

********************************************/

#ifndef __HYI_UTILITYFUNCTIONS_H__
#define __HYI_UTILITYFUNCTIONS_H__

#pragma once

#include <string>
using std::string;

class CUtility
{
    public:
        CUtility ();
        static void ReadFromKBD ();
        static bool YesNo (const string&);
        static void ReadFromKBD (string& szResponse);
		static void ReadFromKBD (const char*, char*);
        static void DisplayOnConsole (const string& szDisplay);
        static void DisplayOnConsole (const char* szDisplay);
        static void DisplayOnConsole (const string& szDisplay, int);
        static void DisplayOnConsole (const char* szDisplay, int);
        static void DisplayOnConsole (const char* szDisplay, const string&);
        static void DisplayOnConsole (int n);
};

#endif
