/* EXAMPLE 9.1.1
   Copyright(c) 2002, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate overloaded operator member functions
   (2) Illustrate overloaded operator friend functions
   (3) Use of the this operator

*********************************************/
#include "point.h"

int main ()
{
    // declare and initialize two points P1 and P2
    CPoint P1 (1.0f, 1.0f);
    CPoint P2 (1.1f, 1.1f);

    // use of copy constructor
    CPoint P3 = P1;
    P3.Display ("Point P3: ");

    // use of overloaded + operator and copy constructor
    CPoint P4 = P1 + P2;
    P4.Display ("Point P4: ");

    // overloaded != operator
    if (P1 != P2)
        std::cout << "Unequal coordinates.\n";
    else
        std::cout << "Equal coordinates.\n";

    // overloaded >> operator
    std::cin >> P1;
    std::cin >> P2;

    // overloaded - operator
    P3 = P1 - P2;

    // overloaded << operator
	std::cout << "Point P3: " << P3 << "\n";
    
    return 0;
}