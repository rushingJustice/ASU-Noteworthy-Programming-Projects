/* EXAMPLE 4.3.1
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate scope rules.

*/
#include <iostream>
const float PI = 3.1415926f;                // global scope 
const float ANGLES_TO_RADIANS = 0.0174533f; // global scope 
float fRadius = -123.4f;                    // global scope

float AreaSegmentofCircle (float fAngle, float fRadius);
float ArcLengthSegmentofCircle (float fAngle, float fRadius);

int main ()
{
    float fRadius = 10.0f;    // local scope to the main program
    float fArcAngle = 120.0;  // local scope to the main program

    std::cout << "Phase 1 of main program\n";
    std::cout << "  Value of fRadius is : " << fRadius << "\n";
    std::cout << "Value of fArcAngle is : " << fArcAngle << "\n\n";

    if (fRadius > 0.0 && fArcAngle > 0.0)
    {
        float fArcLength;      // scope local to the block
        float fArea;           // scope local to the block
        fArcLength = ArcLengthSegmentofCircle (fArcAngle, fRadius);
        fArea = AreaSegmentofCircle (fArcAngle, fRadius);
        std::cout << "Executing the if block\n";
        std::cout << "Arc Length : " << fArcLength << "\n";
        std::cout << "      Area : " << fArea << "\n\n";
    }
    
    std::cout << "Phase 2 of main program\n";
    float fArcLength;      // scope local to the main program
    float fArea;           // scope local to the main program
    fArcLength = ArcLengthSegmentofCircle (fArcAngle, fRadius);
    fArea = AreaSegmentofCircle (fArcAngle, fRadius);
    std::cout << "Arc Length : " << fArcLength << "\n";
    std::cout << "      Area : " << fArea << "\n";

	return 0;
}

float AreaSegmentofCircle (float fAngle, float fRadius)
{
    float fArea;    // local scope

    fArea = PI*fRadius*fRadius*(fAngle/360.0f);
    return (fArea);
}

float ArcLengthSegmentofCircle (float fAngle, float fRadius)
{
    float fArcLength;    // local scope

    fArcLength = (ANGLES_TO_RADIANS*fAngle)*fRadius;
    return (fArcLength);
}

