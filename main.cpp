#include <iostream>
#include"sql.h"
#include<iomanip>
using namespace std;
void test_stk();

int main()
{
    cout << "====================================================================================================" << endl;
    cout << "STARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTART" << endl;
    cout << "====================================================================================================" << endl;



//    char s[] = "select firstname, age from student where lastname = wong and firstname > Joe or age > 50 ";
////    char o[] = "make table employee fields  last, 		first, 			dep";
//    char e[] = "insert into employee values \"Blow Dick.\", 		Joe, 			CS";

////        "dkfmdkfm sfgkndsfkg rt. erer."
//        Parser p (e);

//        p.parse_tree();

//        if(p.is_valid()){
//            cout<<" is_valid"<<endl;
//        }
//    test_stk();



Sql sql;
sql.run("_!select.txt");

        cout << endl <<endl<<"====================================================================================================" << endl;
        cout << "DONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONE"<<endl;
        cout << "====================================================================================================" << endl;

    return 0;
}

void test_stk(){
    char s[] = "insert into employee values \"Blow Dick\", 		Joe, 			CS";
    stk input(s);
         cout<<"input string: "<<s<<endl<<endl;
         token t;
         //The all too familiar golden while loop:
         input>>t;
         while(input.failed()){
             //process token here...

             cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
             t = token();
             input>>t;
         }


}
