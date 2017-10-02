#include <iostream>
#include "math.h"

int CMath::nAddCounter = 0;
int CMath::nSubtractCounter = 0;
int CMath::nMultiplyCounter = 0;
int CMath::nDivideCounter = 0;

double CMath::Add (double d1, double d2)
{
    ++nAddCounter;
    return (d1 + d2);
}

double CMath::Subtract (double d1, double d2)
{
    ++nSubtractCounter;
    return (d1 - d2);
}

double CMath::Multiply (double d1, double d2)
{
    ++nMultiplyCounter;
    return (d1 * d2);
}

double CMath::Divide (double d1, double d2)
{
    ++nDivideCounter;
    return (d1 / d2);
}

void CMath::DisplayCounters ()
{
    std::cout << "\nOperations Count\n"
              << "Addition       " << nAddCounter      << " time(s)\n"
              << "Subtraction    " << nSubtractCounter << " time(s)\n"
              << "Multiplication " << nMultiplyCounter << " time(s)\n"
              << "Division       " << nDivideCounter   << " time(s)\n";
}
