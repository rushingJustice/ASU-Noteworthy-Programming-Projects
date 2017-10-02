/* EXAMPLE 3.3.1
   Copyright(c) 2001-09, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate the use of different control statements.
   (2) Show healthy programming styles.

*/
#include <iostream>

int main ()
{

    float fNewGPA = 0.0;          // new GPA
    int   nNewSemHrs = 0;         // new number of sem hours
    float fCumulativeGPA = 0.0;   // current GPA (excluding this semester)
    int   nCumulativeSemHrs = 0;  // total number of sem hours (excluding
                                  // this semester)

    // get current data on student
    std::cout << "Enter current GPA: ";
    std::cin >> fCumulativeGPA;
    std::cout << "Enter current # of sem. hrs.: ";
    std::cin >> nCumulativeSemHrs;
    // TODO: check whether these values are valid.
    
    // semester-related variables
    char  cCourseGrade;     // grade for the course
    int   nCourseSemHrs;    // sem hrs for the course
    float fSemGPA = 0.0;    // semester GPA
    int   nTotalSemHrs = 0; // total semester hours
    bool  bError;           // input error indicator

    // loop until done
    for (;;)
    {
        // get course grade
        std::cout << "Enter the course grade (Type S to end): ";
        std::cin >> cCourseGrade;
        if (cCourseGrade == 'S') break;

        // set no error in user input
        bError = false;

        // get course semester hours
        std::cout << "Enter course sem hrs: ";
        std::cin >> nCourseSemHrs;
        // TODO: check whether this value is valid.

        // branch on the course grade
        switch (cCourseGrade)
        {
            // note the type-cast conversions below
            case 'A': fSemGPA += 4.0f*float(nCourseSemHrs);
                break;
            case 'B': fSemGPA += 3.0f*float(nCourseSemHrs);
                break;
            case 'C': fSemGPA += 2.0f*(float)nCourseSemHrs;
                break;
            case 'D': fSemGPA += 1.0f*static_cast<float>(nCourseSemHrs);
                break;
            case 'E': 
                break;
            // invalid grade input
            default:
                std::cout << "Invalid grade " << cCourseGrade << ".\n";
                bError = true;
                break;
        }
        if (!bError)
            nTotalSemHrs += nCourseSemHrs;
    }

    // compute (a) new semester hours and GPA
    //         (b) this semester's GPA
    nNewSemHrs = nTotalSemHrs + nCumulativeSemHrs;
    if (nTotalSemHrs > 0)
        fSemGPA /= float (nTotalSemHrs);    // avoid divide by zero
    fNewGPA = float(nTotalSemHrs*fSemGPA+nCumulativeSemHrs*fCumulativeGPA)
                     /float(nNewSemHrs);

    // display the output
    std::cout << std::endl <<"    FINAL GRADE REPORT";
    std::cout << std::endl <<"    ------------------" << std::endl;
    std::cout << "            GPA before this semester: " << fCumulativeGPA
              << std::endl;
    std::cout << "Total sem. hrs. before this semester: " << nCumulativeSemHrs
              << std::endl;
    std::cout << "                   GPA this semester: " << fSemGPA
              << std::endl;
    std::cout << "       Total sem. hrs. this semester: " << nTotalSemHrs
              << std::endl;
    std::cout << "                     Cumulative GPA : " << fNewGPA
              << std::endl;
    std::cout << "                Cumulative sem. hrs.: " << nNewSemHrs
              << std::endl << std::endl;
    // TODO: display just enough significant digits

    return 0;   // parenthesis is optional
}
