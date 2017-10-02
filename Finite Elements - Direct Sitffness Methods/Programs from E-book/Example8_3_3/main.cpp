/* EXAMPLE 8.3.3
   Copyright(c) 2002-08, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Use of new and delete with vector data type
   (2) Pointer arithmetic

*********************************************/
#include <iostream>
#include <stdexcept>  // exception handling

int main ()
{
    float *fVX; // pointer to a float
    // allocate memory locations for 4 float values
    try
    {
        fVX = new float[4];
    }
    catch (std::bad_alloc)
    {
        std::cout << "Unable to allocate memory.\n";
        exit (1);
    }

    int i;
    // set the four values are 100, 101, 102, 103
    for (i=0; i < 4; i++)
    {
        fVX[i] = 100.0f + static_cast<float>(i);
    }

    // display the values
    for (i=0; i < 4; i++)
    {
        std::cout << "Location " << i << ": " << fVX[i]
                  << "\n";
    }

    // release the memory locations allocated before
    delete [] fVX;

    return 0;
}