/*********************************************
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CParser class definition.

*********************************************/
#ifndef __RAJAN_PARSER_H__
#define __RAJAN_PARSER_H__

#include <fstream>
#include <string>
#include <vector>

const int MAXCHARSPARSER = 256;

class CParser
{
    public:
        CParser ();
        ~CParser ();

        bool ReadNextLine (std::ifstream& FileInput, int& nL,
                           std::string& szInputString,
                           const int MAXCHARS, 
                           const std::string& szComment,
                           bool bLowerCase = true);
        bool GetTokens (std::ifstream& IFile, int& nLineNumber,
                        std::vector<std::string>& szVTokens,
                        int& nTokens, const std::string& szDelimiters,
                        const std::string& szComment,
                        bool bLowerCase = true);

        bool GetIntValue (const std::string& szInput, int& nV) const;
        bool GetLongValue (const std::string& szInput, long& lV) const;
        bool GetFloatValue (const std::string& szInput, float& fV) const;
        bool GetDoubleValue (const std::string& szInput, double& dV) const;

        void Trim (std::string& szTemp) const;
        void TrimLeft (std::string& szTemp) const;
        void TrimRight (std::string& szTemp) const;
        void TrimLeadingZeros (std::string& szTemp) const;
        void ToLower (std::string& szInputString) const;
        void ToUpper (std::string& szInputString) const;

    private:
        bool AtoL (const std::string& szTemp, long& lV,
                   int nSign) const;
        double AtoDFraction (const std::string& szTemp) const;
        int NumChars (long lV, std::string& szT) const;
};

#endif