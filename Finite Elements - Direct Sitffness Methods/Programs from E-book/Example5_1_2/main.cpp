/* EXAMPLE 5.1.2
   Copyright(c) 2005-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate truncation error in single and double precision.

*/
#include <iostream>
#include <iomanip>
#include <cmath>

int main ()
{
    int N;
    std::cout << "Input value for N: ";
    std::cin >> N;
    
    std::cout << std::setprecision(15) << std::scientific;

    // ----------------
    // single precision
    // ----------------
    std::cout << "Single precision results ....\n";
    const float PISP = 3.1415926f;
    float fSum = 0.0f;
    // process 1
    for (int i=1; i <= N; i++)
        fSum += 1.0f/(static_cast<float>(i)*static_cast<float>(i));
    std::cout << "Process 1: Sum is " << std::setw(20) << fSum << '\n';

    // process 2
    fSum = 0.0f;
    for (int i=N; i >= 1; i--)
        fSum += 1.0f/(static_cast<float>(i)*static_cast<float>(i));
    std::cout << "Process 2: Sum is " << std::setw(20) << fSum << '\n';
    std::cout << "Exact answer is " << PISP*PISP/6.0f << "\n\n";

    // ----------------
    // double precision
    // ----------------
    std::cout << "Double precision results ....\n";
    const double PIDP = 3.14159265358979;
    double dSum = 0.0;
    // process 1
    for (int i=1; i <= N; i++)
        dSum += 1.0/(static_cast<double>(i)*static_cast<double>(i));
    std::cout << "Process 1: Sum is " << std::setw(20) << dSum << '\n';

    // process 2
    dSum = 0.0;
    for (int i=N; i >= 1; i--)
        dSum += 1.0/(static_cast<double>(i)*static_cast<double>(i));
    std::cout << "Process 2: Sum is " << std::setw(20) << dSum << '\n';
    std::cout << "Exact answer is " << PIDP*PIDP/6.0 << '\n';
    
    return 0;
}
