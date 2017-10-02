/*********************************************
Example 13.3.4
Copyright(c) 2005-08, S. D. Rajan
Object-Oriented Numerical Analysis

Example to illustrate nested classes

*********************************************/

#include <iostream>
#include "material.h"

int main ()
{
    // define steel (SI units)
    std::string szName ("Steel-0.2%C-HR");
    CMaterialSI SteelSI (szName, 7850.0, 250.0e6, 200.0e9);
    SteelSI.Display ();

    // convert values to US Customary units
    CMaterialSI::CConvert convertUSC;
    CMaterialUSC SteelUSC (szName, convertUSC.GetValue (SteelSI, CMaterial::MASSDENSITY),
                                   convertUSC.GetValue (SteelSI, CMaterial::YIELDSTRENGTH),
                                   convertUSC.GetValue (SteelSI, CMaterial::MODULUSOFELASTICITY));
    CMaterialUSC::CConvert convertSI;

    // convert values back to SI and clone a new SI object
    CMaterialSI SteelSIClone (szName, convertSI.GetValue (SteelUSC, CMaterial::MASSDENSITY),
                              convertSI.GetValue (SteelUSC, CMaterial::YIELDSTRENGTH),
                              convertSI.GetValue (SteelUSC, CMaterial::MODULUSOFELASTICITY));
    // make sure we get back original values
    std::cout << std::endl;
    SteelSIClone.Display ();

    return 0;
}
