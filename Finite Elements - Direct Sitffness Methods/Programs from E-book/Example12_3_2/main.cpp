/* EXAMPLE 12.3.2
   Copyright(c) 2005, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate how to write to and read from a binary file
       especially when the data set contains a character string.
       File is treated as a fixed record length file.

*/
#include <cstdlib>  // for exit()
#include <iostream> // for cin/cout
#include <fstream>  // for db file
#include "point.h"

int main ()
{
    std::fstream DBFile;
    DBFile.open ("points.db", std::ios::binary | std::ios::out |
                              std::ios::in     | std::ios::trunc);
    if (!DBFile)
    {
        std::cout << "Unable to open database file.\n";
        exit (0);
    }
    CPoint P;

    // obtain data from user and create database
    std::string szTag;
    int nRecords = 0;
    for (;;)
    {
        std::cout << "Input next data set (done tag terminates)\n";
        std::cin >> P;
        P.GetTag(szTag);
        if (szTag == "done")
            break;
        ++nRecords;
        DBFile.seekp ((nRecords-1)*sizeof(CPoint));
        DBFile.write (reinterpret_cast<const char*>(&P),
                      sizeof(CPoint));
        if (DBFile.bad())
        {
            std::cout << "Fatal error during write.\n";
            exit (0);
        }
    }

    // read the database and display data
    for (int i=1; i <= nRecords; i++)
    {
        DBFile.seekg ((i-1)*sizeof(CPoint));
        DBFile.read (reinterpret_cast<char*>(&P),
                     sizeof(CPoint));
        if (DBFile.bad())
        {
            std::cout << "Fatal error during read.\n";
            exit (0);
        }
        P.Display ("Current point: ");
    }

    DBFile.close ();

    return 0;
}
