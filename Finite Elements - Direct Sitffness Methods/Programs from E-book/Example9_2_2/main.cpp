#include <iostream>
#include "list.h"

int main (int argc, char* argv[])
{
    TListOfStudents ListOfItems;

    CreateList (ListOfItems);
	
    DisplayList (ListOfItems);

    return 0;
}
