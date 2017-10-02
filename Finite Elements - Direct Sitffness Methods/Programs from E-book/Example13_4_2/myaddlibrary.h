/* Example 13.4.2
   Copyright(c) 2005-08, S. D. Rajan
   Object-Oriented Numerical Analysis
*/
#ifndef __RAJAN__MYADDLIBRARY_H__
#define __RAJAN__MYADDLIBRARY_H__

class CMyAddLibrary
{
    public:
        CMyAddLibrary ();
        ~CMyAddLibrary ();

        float AddSimpleTwo (float, float);
        float AddSquareTwo (float, float);
        void Use_In_A_Function (float (CMyAddLibrary::*ptrFunc)(float fA, float fB));

    private:
};

#endif
