/* EXAMPLE 7.5.2
   Copyright(c) 2010, S. D. Rajan
   Object-Oriented Numerical Analysis

  OBJECTIVES
   (1) Illustrate some advanced use of standard string class.

HERE ARE THE DETAILS OF THE std::string class

Members:
    (constructor)   Construct string object (constructor member) 
    operator=       String assignment (public member function) 
Iterators:
    begin       Return iterator to beginning (public member function) 
    end         Return iterator to end (public member function) 
    rbegin      Return reverse iterator to reverse beginning (public member function) 
    rend        Return reverse iterator to reverse end (public member function) 
Capacity:
    size        Return length of string (public member function) 
    length      Return length of string (public member function) 
    max_size    Return maximum size of string (public member function) 
    resize      Resize string (public member function) 
    capacity    Return size of allocated storage (public member function) 
    reserve     Request a change in capacity (public member function) 
    clear       Clear string (public member function) 
    empty   Test if string is empty (public member function) 
Element access:
    operator[]  Get character in string (public member function) 
    at          Get character in string (public member function) 
Modifiers:
    operator+=  Append to string (public member function) 
    append      Append to string (public member function) 
    push_back   Append character to string (public member function) 
    assign      Assign content to string (public member function) 
    insert      Insert into string (public member function) 
    erase       Erase characters from string (public member function) 
    replace     Replace part of string (public member function) 
    copy        Copy sequence of characters from string (public member function) 
    swap        Swap contents with another string (public member function) 
String operations:
    c_str               Get C string equivalent (public member function) 
    data                Get string data (public member function) 
    get_allocator       Get allocator (public member function) 
    find                Find content in string (public member function) 
    rfind               Find last occurrence of content in string (public member function) 
    find_first_of       Find character in string (public member function) 
    find_last_of        Find character in string from the end (public member function) 
    find_first_not_of   Find absence of character in string 
    find_last_not_of    Find absence of character in string from the end (public member function) 
    substr              Generate substring (public member function) 
    compare             Compare strings (public member function) 
*/
#include <iostream>
#include <string>

void Display (const std::string& szMessage,
              const std::string& szString)
{
    std::cout << szMessage << szString << "\n";
}

int main ()
{
    std::string szHeader (" This is a short string. ");
    // the length function (szHeader has leading and trailing blanks)
    std::cout << "'" << szHeader << "' has " << szHeader.length() 
		      << " characters.\n";

    // complete string comparison
    std::string szOne ("One of those");
    std::string szTwo (szOne);
    if (szOne.compare(szTwo) == 0)
        std::cout << "'" << szOne << "' is the same as '"
                         << szTwo << "'\n";
    else
        std::cout << "'" << szOne << "' is NOT the same as '"
                         << szTwo << "'\n";

    // partial string change
    szTwo.replace (0, 3, "Four");
    Display ("szTwo now is: ", szTwo);

    // partial string comparison
    if (szOne.compare (4,8, szTwo,5,8) == 0)
        std::cout << "Both string have 'of those' in them.\n";

    // clear a string
    szTwo.clear ();
    if (szTwo.length() == 0)
        std::cout << "szTwo: " << szTwo << " is an empty string.\n";

    // copy a substring
    szTwo = szOne.substr (0, 6) + " everything";
    Display ("szTwo now is: ", szTwo);

    // swap two strings
    Display ("szOne before swap: ", szOne);
    Display ("szTwo before swap: ", szTwo);
    szOne.swap (szTwo);
    Display ("szOne after swap: ", szOne);
    Display ("szTwo after swap: ", szTwo);

    // inserting, finding ....
    int nPosition = szHeader.find ("short");
    szHeader.insert (nPosition, "very ");
    Display ("szHeader after insert: ", szHeader);
    nPosition = szHeader.find_first_of ("ih");
    if (nPosition == std::string::npos)
        std::cout << "Could not find 'i' or 'h' in " 
		          << szHeader << "\n";
    else
        std::cout << "First 'i' or 'h' is at location: " 
		          << nPosition << "\n";
    nPosition = szHeader.find_last_of ("ih");
    if (nPosition == std::string::npos)
        std::cout << "Could not find 'i' or 'h' in " 
		          << szHeader << "\n";
    else
        std::cout << "Last 'i' or 'h' is at location: " 
		          << nPosition << "\n";

    return 0;
}

