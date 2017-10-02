#ifndef __RAJAN_LIST_H__
#define __RAJAN_LIST_H__

//---------------------------------------------------------------------------
#include <iostream>
#include <string>
using std::string;
const int Maximum = 100;
//---------------------------------------------------------------------------

class TListOfStudents
{
    public:
        class TStudent
        {
            private:
                string FullName;
                int    Age;
                char   Gender;
            public:
                void   setFullName(const string& Name) { FullName = Name; }
                string getFullName() const { return FullName; }
                void   setAge(const int a) { Age = a; }
                int    getAge() const { return Age; }
                void   setGender(const char g) { Gender = g; }
                char   getGender() const { return Gender; }
                TStudent();
                TStudent(const string& FName, int a, char g);
                virtual ~TStudent() {}
        };
    private:
        TStudent Item[Maximum];

    public:
        TListOfStudents() : Count(0){}
        inline ~TListOfStudents() {}
        void Add(const TStudent& d);
        TStudent Retrieve(const int n);
        int Count;
};
//---------------------------------------------------------------------------
TListOfStudents::TStudent::TStudent()
{
    FullName = "";
    Age      = 0;
    Gender   = 'M';
}
//---------------------------------------------------------------------------
TListOfStudents::TStudent::TStudent(const string& FName, int a, char g)
{
    FullName = FName;
    Age      = a;
    Gender   = g;
}
//---------------------------------------------------------------------------
void TListOfStudents::Add (const TStudent& d)
{
    if( Count < Maximum )
    {
	    Item[Count] = d;
	    Count++;
    }
}
//---------------------------------------------------------------------------
TListOfStudents::TStudent TListOfStudents::Retrieve(const int n)
{
    return Item[n];
}
//---------------------------------------------------------------------------
void CreateList(TListOfStudents &ListOfItems)
{
	TListOfStudents::TStudent Pupil;

	Pupil.setFullName("Harry Seamen");
	Pupil.setAge(12);
	Pupil.setGender('M');
	ListOfItems.Add(Pupil);

	Pupil.setFullName("Hermine Grand");
	Pupil.setAge(14);
	Pupil.setGender('F');
	ListOfItems.Add(Pupil);

	Pupil.setFullName("Ronald Hannah");
	Pupil.setAge(18);
	Pupil.setGender('M');
	ListOfItems.Add(Pupil);

	Pupil.setFullName("Johnny Hancock");
	Pupil.setAge(16);
	Pupil.setGender('M');
	ListOfItems.Add(Pupil);

}
//---------------------------------------------------------------------------
void DisplayList(TListOfStudents &ListOfItems)
{
    std::cout << "Full Name\tAge\tGender";
	string Gender[] = { "Male", "Female", "Unknown" };

	for(int i = 0; i < ListOfItems.Count; i++)
	{
        std::cout << "\n" << ListOfItems.Retrieve(i).getFullName()
		          << "\t" << ListOfItems.Retrieve(i).getAge()
		          << "\t";
		switch(ListOfItems.Retrieve(i).getGender())
		{
		    case 'm':
		    case 'M':
			    std::cout << Gender[0];
			    break;
		    case 'f':
		    case 'F':
			    std::cout << Gender[1];
			    break;
		    default:
			    std::cout << Gender[2];
		}
	}
}
//---------------------------------------------------------------------------
#endif