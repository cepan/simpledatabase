#include <iostream>
#include<parser.h>
#include<record.h>
#include<table.h>
#include<iomanip>
//#include <sql.h>
using namespace std;
void test_stk();

int main()
{
    cout << "====================================================================================================" << endl;
    cout << "STARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTARTSTART" << endl;
    cout << "====================================================================================================" << endl;



//    char s[] = "select firstname, age from student where lastname = wong and firstname > Joe or age > 50 ";
//    char o[] = "make table employee fields  last,  first,  dep";
//    char e[] = "insert into employee values Blow, 		Joe, 			CS";

//        Parser p (s);

//        MMap<string, string> parseTree;

//        parseTree = p.parse_tree();

//        cout<<"==main::parse_tree======"<<endl<<endl<<parseTree<<endl;


////        test_stk();
//        vector<string>ha= {"shawn ", "pan ", "is ", "so ", "handsome"};
//        Record r1  (ha) ;
//        cout<<r1[0]<<r1[1]<<r1[2]<<r1[3]<<r1[4]<<endl;




        //    Parser p("Select * from student, professor");
            //    Parser p("Select last name, first name from PCC student, professor");

            //    string s = "insert into employee values Davidson, Mary Alice, complaints, 2012";
            //    Parser p(s);
            //    Mmap<string, string> result = p.parseTree();

            //    cout<<"Map: \n"<<result<<endl;

            vector<string> v;
            v.push_back("id");
            v.push_back("first Name");
            v.push_back("last Name");
            v.push_back("age");
            v.push_back("major");

            table t("studentName", v);

            vector<string> input;
            input.push_back("10229139");
            input.push_back("Long");
            input.push_back("Truong");
            input.push_back("19");
            input.push_back("CS");

//            vector<string> input2;
//            input2.push_back("10409139");
//            input2.push_back("Nam");
//            input2.push_back("Truong");
//            input2.push_back("21");
//            input2.push_back("CS");

//            vector<string> input3;
//            input3.push_back("10609139");
//            input3.push_back("Zed");
//            input3.push_back("Dian");
//            input3.push_back("20");
//            input3.push_back("Math");

//            //            vector<string> input4;
//            //            input4.push_back("10809139");
//            //            input4.push_back("Roger");
//            //            input4.push_back("Lian");
//            //            input4.push_back("23");
//            //            input4.push_back("Gender Study");

//            //            vector<string> input5;
//            //            input5.push_back("10909139");
//            //            input5.push_back("Tingli");
//            //            input5.push_back("Wang");
//            //            input5.push_back("20");
//            //            input5.push_back("Geology");

//            t.insert_into(input);
//            t.insert_into(input2);
//            t.insert_into(input3);
////            t.insert_into(input4);
////            t.insert_into(input5);
//            t.select_all();
//            t.print_indices();




        cout << endl <<endl<<"====================================================================================================" << endl;
        cout << "DONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONEDONE"<<endl;
        cout << "====================================================================================================" << endl;

    return 0;
}
void test_stk(){
      char s[] = "select";
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

