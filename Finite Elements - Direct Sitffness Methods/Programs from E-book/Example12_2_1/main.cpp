/* EXAMPLE 12.2.1
   Copyright(c) 2001-05, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) To show file handling - input and output files.
   (2) Creating a tabular output.
*/

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

int main ()
{
    // filename to be stored here
    std::string szInputFileName, szOutputFileName;

    // input file
    std::ifstream FileInput;

    // output file
    std::ofstream FileOutput;

    // get the input file name from the user
    int nSuccess = 0;
    do
    {
        std::cout << "Input file name (including extension)? ";
        std::cin >> szInputFileName;
        std::cout << "Trying to read from file " << szInputFileName
                  << std::endl;

        // open the file
        // ios::in indicates the file is to be opened for
        // reading only
        FileInput.open (szInputFileName.c_str(), std::ios::in);

        if (FileInput.fail())
        {
            std::cout << "Unable to open input file" << std::endl;
            // must set the goodbit so that the open statement executes
            FileInput.clear ();
        }
        else
            nSuccess=1; // file opened successfully
    } while (nSuccess == 0);

    // get the output file name from the user
    nSuccess = 0;
    do
    {
        std::cout << "Output file name (including extension)? ";
        std::cin >> szOutputFileName;
        std::cout << "Creating file " << szOutputFileName << std::endl;

        // open the file
        // ios::out indicates the file is to be opened for
        // writing only
        FileOutput.open (szOutputFileName.c_str(), std::ios::out);
        if (FileOutput.fail())
        {
            std::cout << "Unable to open output file" << std::endl;
            FileOutput.clear (); 
        }
        else
            nSuccess=1; // file opened successfully
    } while (nSuccess == 0);

    // read the input and write the output
    int nPointNumber;
    float fX, fY, fTemp;

    // set column headers for output file
    FileOutput << setiosflags(std::ios::left) << std::setw (7)
        << "PID" << std::setw (15) << "X" << std::setw (15)
        << "Y" << std::setw (15) <<  "Temp" << std::endl;
    FileOutput << setiosflags(std::ios::left) << std::setw (7)
        << "---" << std::setw (15) << "-" << std::setw (15)
        << "-" << std::setw (15) << "----" << std::endl;

    int nLines = 0;
    for (;;)
    {
        FileInput >> nPointNumber;
        if (FileInput.eof()) break; // end of file reached?
        FileInput >> fX >> fY >> fTemp;
        if (FileInput.eof()) break; // end of file reached?
        nLines++;
        FileOutput << setiosflags(std::ios::left)
                   << std::setw(6) << nPointNumber
                   << std::setw(15) << fX
                   << std::setw(15) << fY
                   << std::setw(15) << fTemp << "\n";
    }

    // close the files and terminate the program
    std::cout << szInputFileName << " was read and had " << nLines
              << " lines of input.\n";
    FileInput.close ();
    FileOutput.close ();

    // all done
    return (0);
}