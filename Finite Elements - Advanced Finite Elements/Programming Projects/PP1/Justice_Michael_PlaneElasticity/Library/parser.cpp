/*********************************************
Planar Frame Analysis Program
Copyright(c) 2000-15, S. D. Rajan
All rights reserved

Introduction to Structural Analysis and Design
Object-Oriented Numerical Analysis

Contains CParser class implementation.

*********************************************/
#include <cctype>
#include "parser.h"

CParser::CParser ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

CParser::~CParser ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

bool CParser::ReadNextLine (std::ifstream& FileInput, int& nLineNum, 
                            std::string& szInputString, const int MAXCHARS,
                            const std::string& szComment, bool bLowerCase)
// ---------------------------------------------------------------------------
// Function: reads the next line skipping over the comment lines
//           and converts all alphabets to lower case if requested
// Input:    file istream, line #, string to hold the input line,
//           max. # of characters expected in each input line,
//           comment character(s) at the beginning of a comment line,
//           lowercase conversion option
// Output:   updated values of line # and the string
//           return value is true if successful
//                           false if an error state is encountered
// Restriction: Cannot read a line over 256 characters
// ---------------------------------------------------------------------------
{
    int i;
    const int MAXCH = 256;
    static char szInp[MAXCH+1];

    // enough capacity to read and store?
    if (MAXCHARS > MAXCH)
        return false;

    // comment character(s)
    int nCLen = static_cast<int>(szComment.length());

    // read the line (skip over comment lines)
    for(;;)
    {
        ++nLineNum;
        FileInput.getline (szInp, MAXCHARS);

        // end-of-file?
        if (FileInput.eof())
            return false;
        if (FileInput.fail())
            FileInput.clear (FileInput.rdstate() & ~std::ios::failbit);
        // unrecoverable error?
        if (FileInput.bad())
            return false;

        // successful read
        szInputString = szInp;
        if (szInputString.substr(0,nCLen) != szComment)
        {
            // convert to lower case?
            if (bLowerCase)
            {
                for (i=0; i < static_cast<int>(szInputString.length()); i++)
                    szInputString[i] = tolower(szInputString[i]);
            }
            break;
        }
    }

    return true;
}

bool CParser::GetTokens (std::ifstream& IFile, int& nLineNumber,
                         std::vector<std::string>& szVTokens,
                         int& nTokens, const std::string& szDelimiters,
                         const std::string& szComment,
                         bool bLowerCase)
// ---------------------------------------------------------------------------
// Function: reads a line of input from a file and parses the input line
//           into distinct tokens based on the specified delimiter(s)
// Input:    ifstream object, delimiters
// Output:   vector containing the tokens, number of tokens
// ---------------------------------------------------------------------------
{
    static char szEntireLine[MAXCHARSPARSER+1];

    // comment character(s)
    int nCLen = static_cast<int>(szComment.length());

    for (;;)
    {
        ++nLineNumber;
        IFile.getline (szEntireLine, MAXCHARSPARSER);
        if (IFile.eof() && nTokens == 0)
            return false;
        if (IFile.fail())
            return false;

        // convert input line into std::string object
        std::string szLine (szEntireLine);
        if (szLine.substr(0,nCLen) != szComment)
        {
            // prepare to store the tokens
            szVTokens.clear();
            std::string::size_type start_loc, end_loc;
            std::string strCurDelimiter("");

            // get location of the first character that is not a delimiter
            start_loc = szLine.find_first_not_of (szDelimiters);

            // loop while the beginning index is not the end of string
            while (start_loc != std::string::npos)
            {
                if (start_loc > 0)
                    strCurDelimiter = szLine.at(start_loc-1);
                if (strCurDelimiter == "\"")
                {
                    // get location of the next delimiter
                    end_loc = szLine.find_first_of ("\"", start_loc);
                    if (end_loc == std::string::npos) 
                        return false; // missing end quote
                }
                else
                {
                    // get location of the next delimiter
                    end_loc = szLine.find_first_of (szDelimiters, start_loc);

                    // if this location is the end of string then adjust the value
                    // as string length
                    if (end_loc == std::string::npos) end_loc = szLine.length();
                }

                // save the string between start_loc and end_loc
                szVTokens.push_back (szLine.substr(start_loc,end_loc-start_loc));

                // get location of the next character that is not a delimiter
                start_loc = szLine.find_first_not_of (szDelimiters, end_loc);
            }

            // trim leading and trailing blank spaces
            nTokens = static_cast<int>(szVTokens.size ());
            if (nTokens == 0)
                continue; // skip blank line
            for (int i=0; i < nTokens; i++)
            {
                Trim(szVTokens[i]);
                // convert to lower case?
                if (bLowerCase)
                    ToLower(szVTokens[i]);
            }
            break;
        }
    }

    return true;
}

void CParser::TrimLeadingZeros (std::string& szTemp) const
{
    int nPos = 0;		// current parsing position

    // find first nonzero character
    while (szTemp[nPos] == '0' && nPos < MAXCHARSPARSER)
        nPos++;

    // length of string
    int nLength = static_cast<int>(szTemp.length());

    // zero-filled string?
    if (nPos == (MAXCHARSPARSER-1) || (nLength-nPos) == 0)
    {
        szTemp = "";
        return;
    }

    // extract string without leading zeros
    szTemp = szTemp.substr(nPos, nLength-nPos);
}

void CParser::TrimLeft (std::string& szTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all leading blank characters
// Input:    string
// Output:   string without leading blanks
// ----------------------------------------------------------------------------
{
    int nPos = 0;       // current parsing position

    // find first nonblank character
    while (szTemp[nPos] == ' ' && nPos < MAXCHARSPARSER)
        nPos++;

    // length of string
    int nLength = szTemp.length();
    if (nLength == 0)
        return;

    // empty string?
    if (nPos == (MAXCHARSPARSER-1) || (nLength-nPos) == 0)
    {
        szTemp = "";
        return;
    }

    // extract string without leading blanks
    szTemp = szTemp.substr(nPos, nLength-nPos);

}

void CParser::TrimRight (std::string& szTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all trailing blank characters
// Input:    string
// Output:   string without trailing blanks
// ----------------------------------------------------------------------------
{
    int nLast;      // current parsing position

    // find last nonblank character
    int nLength = szTemp.length();
    if (nLength == 0)
        return;
    for (int i=nLength-1; i >= 0; i--)
    {
        nLast = i;
        if (szTemp[nLast] != ' ') break;
    }

    // full string?
    if (nLast == (MAXCHARSPARSER-1)) {
        return;
    }

    // extract string without trailing blanks
    szTemp = szTemp.substr(0, nLast+1);

}

void CParser::Trim (std::string& szTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all leading and trailing blank characters
// Input:    string
// Output:   string without leading and trailing blanks
// ----------------------------------------------------------------------------
{
    TrimLeft (szTemp);
    TrimRight (szTemp);
}

void CParser::ToUpper (std::string& szInputString) const
// ----------------------------------------------------------------------------
// Function: Converts the string to upper case alphabets
// Input:    string
// Output:   upper case string 
// ----------------------------------------------------------------------------
{
    for (int i=0; i < static_cast<int>(szInputString.length()); i++)
        szInputString[i] = toupper(szInputString[i]);
}

void CParser::ToLower (std::string& szInputString) const
// ----------------------------------------------------------------------------
// Function: Converts the string to upper case alphabets
// Input:    string
// Output:   upper case string 
// ----------------------------------------------------------------------------
{
    for (int i=0; i < static_cast<int>(szInputString.length()); i++)
        szInputString[i] = tolower(szInputString[i]);
}

bool CParser::GetIntValue (const std::string& szUserInput,
                           int& nV) const
{
    long lV;

    if (!GetLongValue (szUserInput, lV))
        return false;
    nV = static_cast<int> (lV);
    return true;
}

bool CParser::GetLongValue (const std::string& szUserInput,
                            long& lV) const
{
    // initialize
    int nSign = 1;	    // assume positive input
    int nPos = 0;	    // current parsing position
    std::string szTemp;	// temporary string

    // trim leading and trailing blanks
    std::string szInput = szUserInput;
    Trim (szInput);

    // first character + or -
    if (szInput[nPos] == '-') {
        nSign = -1;		// negative integer
        nPos++;
    }
    else if (szInput[nPos] == '+') {
        nPos++;			// ignore + sign
    }

    // extract string without + or -
    int nLast = static_cast<int>(szInput.length());
    szTemp = szInput.substr(nPos, nLast-nPos);
    // trim leading zeros
    if (nLast > 1) TrimLeadingZeros (szTemp);

    // rest of the string is legal?
    int nLoc = static_cast<int>(szTemp.find_first_not_of ("0123456789"));
    if (nLoc >= 0)		// invalid input
        return false;		// set status for invalid input
    // invalid input
    else if (szTemp.length() == 0)
        return false;
    else
    {	
        // legal input so far.
        AtoL (szTemp, lV, nSign);
        // account for proper sign
        lV *= nSign;
    }

    return true;
}

bool CParser::GetFloatValue (const std::string& szUserInput,
                             float& fV) const
{
    double dV;

    if (!GetDoubleValue (szUserInput, dV))
        return false;
    fV = static_cast<float> (dV);

    return true;
}

bool CParser::GetDoubleValue (const std::string& szUserInput, 
                              double& dV) const
{
    // initialize
    double dSignM = 1.0;	// assume positive mantissa
    double dSignE = 1.0;	// assume positive exponent
    int nPos = 0;		    // current parsing position
    std::string szTemp;     // temporary string

    // store exponent, mantissa and fractional components
    std::string szExponent= "", szMantissa = "",
                szFraction = "";
    long lVE, lVM; 
    double dVF;

    // trim leading and trailing blanks
    std::string szInput = szUserInput;
    Trim (szInput);

    // first character + or -
    if (szInput[nPos] == '-') {
        dSignM = -1.0;		// negative 
        nPos++;
    }
    else if (szInput[nPos] == '+') {
        nPos++;			// ignore + sign
    }

    // extract string without + or - 
    szTemp = szInput.substr(nPos, szInput.length()-nPos);
    int nLast = static_cast<int>(szTemp.length());
    // trim leading zeros (unless the input is 0)
    if (nLast > 1) TrimLeadingZeros (szTemp);

    // is there an exponent?
    int nLocExp = static_cast<int>(szTemp.find_first_of ("eE"));
    if (nLocExp >= 0)
    { // yes
        szExponent = szTemp.substr(nLocExp+1, nLast-nLocExp);
        szMantissa = szTemp.substr(0, nLocExp);
        nLast = static_cast<int>(szExponent.length());
        // first character + or - in the exponent
        if (szExponent[0] == '-') {
            dSignE = -1.0;		// negative 
            szExponent = szExponent.substr(1, nLast-1);
        }
        else if (szExponent[0] == '+') {
            szExponent = szExponent.substr(1, nLast-1);
        }
    }
    else // no
        szMantissa = szTemp;

    // is there a decimal?
    int nLocDec = static_cast<int>(szMantissa.find ("."));
    if (nLocDec >= 0) {
        nLast = static_cast<int>(szMantissa.length());
        szFraction = szMantissa.substr(nLocDec+1, nLast-1);
        szMantissa = szMantissa.substr(0, nLocDec);
    }

    // valid string?
    int nLocE = static_cast<int>(szExponent.find_first_not_of ("0123456789"));
    int nLocM = static_cast<int>(szMantissa.find_first_not_of ("0123456789"));
    int nLocF = static_cast<int>(szFraction.find_first_not_of ("0123456789"));
    // invalid input
    if (nLocE >= 0 || nLocM >= 0 || nLocF >= 0)
        return false;		
    // invalid input
    else if (szMantissa.length() == 0 && szFraction.length() == 0)
    {
        dV = 0.0;
        return false;
    }
    // valid input
    else
    {	
        int nCharsE = static_cast<int>(szExponent.length()); 
        int nCharsM = static_cast<int>(szMantissa.length()); 
        int nCharsF = static_cast<int>(szFraction.length()); 
        // trim leading zeros
        if (nCharsE > 0) {
            TrimLeadingZeros (szExponent);
            nCharsE = static_cast<int>(szExponent.length()); 
        }
        if (nCharsM > 0) {
            TrimLeadingZeros (szMantissa);
            nCharsM = static_cast<int>(szMantissa.length()); 
        }
        // value of exponent
        if (!AtoL (szExponent, lVE, static_cast<int>(dSignE)))
            return false;
        // capacity of a long number
        std::string szT;
        int nChars = NumChars(DBL_MAX_10_EXP, szT);
        if (nCharsE > nChars)
            return false;
        else if (nCharsE == nChars)
        {
            if (szT < szExponent) return false;
        }
            
         // value of mantissa
         if (!AtoL (szMantissa, lVM, static_cast<int>(dSignM)))
             return false;
         // value of fractional component
         dVF = AtoDFraction (szFraction);

         // account for proper sign and final value
         dV = dSignM*(double(lVM) + dVF)*pow(10.0,dSignE*lVE);
    }

    return true;
}

// convert alpha string to long integer
bool CParser::AtoL (const std::string& szTemp, long& lV,
                    int nSign) const
{
    int nC = 0;
    lV = 0;
    int nLen = static_cast<int>(szTemp.length());

    // capacity of a long number
    //long lVMAX = (nSign >= 1 ? LONG_MAX : labs(LONG_MIN));
    long lVMAX = (nSign >= 1 ? LONG_MAX : LONG_MAX);
    std::string szT;
    int nChars = NumChars(lVMAX, szT);
    if (nLen > nChars)
        return false;
    else if (nLen == nChars)
    {
        if (szT < szTemp) return false;
    }
        
    // Example: value of xyz is
    //          z*10**0 + y*10**1 + z*10**2
    for (int i=nLen-1; i >= 0; i--)
    {
        lV += static_cast<long>((szTemp[i] - '0')*pow(10.0, nC));
        nC++;
    }

    return true;
}

// convert alpha string to double value < 1
double CParser::AtoDFraction (const std::string& szTemp) const
{
    double dVF = 0.0;
    int nC = 1;
    // Example: value of .xyz is
    //          x/10**1 + y/10**2 + z/10**3
    for (int i=0; i < static_cast<int>(szTemp.length()); i++)
    {
        dVF += static_cast<double>(szTemp[i] - '0')*pow(10.0, -nC);
        nC++;
    }

    return dVF;
}

int CParser::NumChars (long lV, std::string& szT) const
{
    int nC = 0;
    long lVT = lV;

    while (lVT > 0)
    {
        szT = szT + ' ';
        lVT /= 10;
        nC++;
    }

    int nX = nC-1;
    while (lV > 0)
    {
        int n = lV - (lV/10)*10;
        lV /= 10;
        szT[nX] = n + '0';
        nX--;
    }

    return nC;
}
