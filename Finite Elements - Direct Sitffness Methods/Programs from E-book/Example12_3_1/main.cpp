/* EXAMPLE 12.3.1
   Copyright(c) 2005, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate how to read and parse an input file
   (2) Use of "memory file"

*/
#include <iostream>
#include <fstream>
#include <sstream>

bool ReadNextLine (std::istream& FileInput, 
                   char *szInputString, const int MAXCHARS)
{

    // read the line
    FileInput.getline (szInputString, MAXCHARS);

    // end-of-file or fatal error returns false
    if (FileInput.eof())
        return false;
    if (FileInput.fail())
        FileInput.clear (FileInput.rdstate() & ~std::ios::failbit);
    if (FileInput.bad())
        return false;

    // successful read
    return true;
}

int main ()
{
   // open the db file
   std::ifstream FileInput;
   FileInput.open ("dbfile.txt", std::ios::in);
   if (!FileInput)
   {
       std::cout << "Unable to open dbfile.txt.\n";
       return 1;
   }

   // read the contents of the file
   const int MAXCHARS = 256;
   char szInputString[MAXCHARS];
   std::string szTempString;
   int nLine = 0;
   std::string szCity, szDate;
   int nMileage;

   // now read the file contents till eof or error occurs
   while (ReadNextLine (FileInput, szInputString, MAXCHARS))
   {
       std::cout << ++nLine << ":" << szInputString << '\n';
       szTempString = szInputString;
       // if line is not a remark or if string is not empty,
       // read the data on that line
       if (szTempString.substr(0,4) != "Rem:" &&
           szTempString.length() > 0)
       {
           std::istringstream szFormatString (szTempString);
           szFormatString >> szCity >> szDate >> nMileage;
           std::cout << "City: " << szCity << ". Date: " << szDate
                     << ". Mileage: " << nMileage << '\n';
       }
   }

   return 0;
}