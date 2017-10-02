/* EXAMPLE 3.3.3
   Copyright(c) 2002-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate exhaustive search (trial and error) technique.
   (2) Loop termination techniques.

*/
#include <iostream>
#include <cmath>

int main ()
{
    float fInitialHeight;   // height from which ball is dropped
    float fCoefofR;         // coefficient of restitution
    float fTargetHeight;    // target height
    int   nCurIter = 0;     // current iteration
    bool  bError = false;   // error indicator
    float fHeightNew;       
    float fHeightOld;   
    float fDiffHeightOld;
    float fDiffHeightNew;
    const int MAXITERATIONS = 1000;

    // get the input from the user
    std::cout << "Initial height: ";
    std::cin >> fInitialHeight;
    std::cout << "Coefficient of Restitution: ";
    std::cin >> fCoefofR;
    std::cout << "Target height: ";
    std::cin >> fTargetHeight;

    // initialize
    fHeightNew = fHeightOld = fInitialHeight;
    fDiffHeightOld = fInitialHeight;

    // loop until convergence
    for (;;)
    {
        // increment iteration counter
        nCurIter++;

        // too many iterations?
        if (nCurIter > MAXITERATIONS)
        {
            bError = true;
            break;
        }

        // compute new height
        fHeightNew *= static_cast<float>(pow(fCoefofR,2.0f));

        // compute difference in heights
        float fD = fHeightNew - fTargetHeight;
        fDiffHeightNew = static_cast<float>(fabs(fD));

        // passed the lowest point?
        if (fDiffHeightNew > fDiffHeightOld)
        {
            // yes. this is the solution.
            fHeightNew = fHeightOld;
            nCurIter--;
            break;
        }
        
        // update solution
        fDiffHeightOld = fDiffHeightNew;
        fHeightOld = fHeightNew;
    }

    // analytical prediction
    float fDiffL, fDiffU;
    float fBounces = log(fTargetHeight/fInitialHeight)/
                     (2.0f*log(fCoefofR));
    int nUpper = static_cast<int>(ceil(fBounces));
    int nLower = (nUpper <= 2 ? 1 : (nUpper - 1));
    fDiffL = static_cast<float>(pow(fCoefofR, 2.0f*nLower)*fInitialHeight);
    fDiffL = static_cast<float>(fabs(fDiffL - fTargetHeight));
    fDiffU = static_cast<float>(pow(fCoefofR, 2.0f*nUpper)*fInitialHeight);
    fDiffU = static_cast<float>(fabs(fDiffU - fTargetHeight));
    int nBounces = (fDiffL < fDiffU? nLower : nUpper);
    float fHeight = static_cast<float>
                    (pow(fCoefofR, 2.0f*nBounces)*fInitialHeight);

    // print result
    std::cout << std::endl;
    if (bError)
        std::cout << "Unable to find the solution.\n";
    else
        std::cout << "Numerical Solution" << "\n"
                  << "     Initial height: " << fInitialHeight << "\n"
                  << "      Target height: " << fTargetHeight << "\n"
                  << "Coef of Restitution: " << fCoefofR << "\n"
                  << "  Number of bounces: " << nCurIter << "\n"
                  << "     Height reached: " << fHeightNew << "\n\n"
                  << "Analytical solution" << "\n"
                  << "  Number of bounces: " << nBounces << "\n"
                  << "     Height reached: " << fHeight << "\n";

    return 0;
}