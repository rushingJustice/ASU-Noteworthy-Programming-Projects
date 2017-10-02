/* EXAMPLE 5.1.4
   Copyright(c) 2005-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Understanding computational limits
       for integer and float types.
*/

#include <iostream>
#include <limits>       // if necessary use climits

int main ()
{
    std::cout << " Machine epsilon for a float number is " 
              << std::numeric_limits<float>::epsilon()  << '\n'
              << "Machine epsilon for a double number is " 
              << std::numeric_limits<double>::epsilon() << '\n'
              << "                      Max int value is "
              << std::numeric_limits<int>::max()        << '\n'
              << "                     Max long value is "
              << std::numeric_limits<long>::max()       << '\n'
              << "                    Max float value is "
              << std::numeric_limits<float>::max()      << '\n'
              << "                   Max double value is "
              << std::numeric_limits<double>::max()     << '\n'
              << "                      Min int value is "
              << std::numeric_limits<int>::min()        << '\n'
              << "                     Min long value is "
              << std::numeric_limits<long>::min()       << '\n'
              << "                    Min float value is "
              << std::numeric_limits<float>::max()      << '\n'
              << "                   Min double value is "
              << std::numeric_limits<double>::max()     << '\n'
              << "      Machine radix or base for int is " 
              << std::numeric_limits<int>::radix        << '\n'
              << "    Machine radix or base for float is " 
              << std::numeric_limits<float>::radix      << '\n';

    return 0;
}
