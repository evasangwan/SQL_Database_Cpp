#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "../../includes/sql/sql.h"
#include <vector> 
using namespace std;

bool test_stub(bool debug = false)
{
  if (debug){
    cout << "testB:: test-sub() entering test_sub" << endl;
  }
//   const vector<string> insertlist = {
// /*05*/     "create table students fields  fname,          lname,    major,    age",
// /*06*/     "insert into students values Flo,               \"Yao Mi\", 	Art, 	20",
// /*07*/     "insert into students values Bo, 		           Yang, 	CS, 		28",
// /*08*/     "insert into students values \"Sam\",          Jackson, 	CS, 		40",
// /*09*/     "insert into students values \"Billy\",	       Jackson, 	Math,	27",
// /*10*/     "insert into students values \"Mary Ann\",     Davis,	Math,	30",
//            "insert into students values Nat,             Davis, 	Art, 	22",
// /*07*/     "insert into students values Mark, 		         Yao, 	CS, 		20",
// /*08*/     "insert into students values \"Sam L.\",      Davis, 	CS, 		30",
// /*09*/     "insert into students values \"Bill\",	      Davis, 	CS,	18",
// /*10*/     "insert into students values \"Mary Ann\",    Dalvis,	Art,	22",
//            "insert into students values \"Michael O'Connor\",     Markus,	CS,	30",
//            "insert into students values Flora,               \"Yao Mi\", 	CS, 	20",
// /*07*/     "insert into students values Borja, 		           M, 	CS, 		28",
// /*08*/     "insert into students values \"Sam Kay\",          Hell, 	CS, 		40",
// /*09*/     "insert into students values \"Billy\",	       Jackson, 	Math,	27",
// /*10*/     "insert into students values \"Mary Ann\",     Jell,	Math,	30",
//            "insert into students values Natalie,             Davis, 	Art, 	22",
// /*07*/     "insert into students values Markle, 		         Yao, 	CS, 		20",
// /*08*/     "insert into students values \"Sammy L.\",      Jackson, 	CS, 		30",
// /*09*/     "insert into students values \"Billy\",	      Davis, 	CS,	18",
// /*10*/     "insert into students values \"Mary Ann\",    Dalvis,	Math,	22",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	30",

// };
//  SQL sql;
//  Table t;
//   for (int i = 0; i < insertlist.size(); i++){
//     if (i == insertlist.size()-1){
//       cout << sql.command(insertlist[i]) << endl;
//     }
//     else
//     sql.command(insertlist[i]);
//   }
//     const vector<string> commandlines = {
      
//       // "Select * From employee",
//       "select * from employee",
//       "SELECT fname FROM employee",
//       // "selECT lname, fname, major frOM student where (major = CS ) and major = CS and ()"
//       "select * from students",
//       "select * from students where age > 19 and (lname = Jackson or major = Math and fname = \"Mary Ann\")",
//       "select * from student where age < 50 and (fname = \"Teresa Mae\" or lname = Jackson and (age > 10 and (major = CS or major = Math)))",
//       "select"
// //       "select * from student where major >= CS",
// //       "select lname, fname from student where fname = Nat",
// // "select fname,major,lname from student where lname = \"Yao Mi\"",
// // "create table job fields name, lname, job",
// // "insert into job values jackie, chan, \"movie star\"",
// // "insert into job values mary, chan, \"chef\"",
// // "insert into job values lowie, marsel, \"movie star\"",
// // "select * from job",
// // "select * from job where lname = \"chan\" and fname = jackie or job = chef",
// // "select * from job where fname > lowie"
// // "select fname, lname from student where fname = \"Mary Ann\"",
// // "select * from student where fname = \"MaryAnn\"",
// // "select * from student where lname = Jackson",
// // "select * from student where lname = \" \"",
// // "select * from student where (major=Art and major =CS)",
// // /*18*/     "select * from student where lname>Hell",
// // /*19*/     "select * from student where lname< Jackson and (major=CS or major=Art)"
// // "select * from student where (major=Math or major =CS)",
// // /*18*/     "select * from student where lname>J",
// // /*19*/     "select * from student where lname>J and (major=CS or major=Art)"
// // "select * from student where lname = \" \"",
// // "select age,lname,fname from student where major = CS",
// // "select * from student where lname = \"Davis\"",
// // "select major from jjj",
// // "select * from student where (major=Art or major =CS)",
// // /*18*/     "select * from student where lname>J",
// // /*19*/     "select * from student where lname>J and (major=CS or major=Art)"
// // "select fname, lname from student where fname = \"Mary Ann\"",
// // "select age,lname,fname from student where major = CS",
// // "select * from student where lname = \"Jell\"",
// // "select * from student where fname = \"MaryAnn\"",
// // "select * from student where lname = \" \"",
// // "select major from jjj",
// // /*17*/     "select * from student where (major=Art or major =CS)",
// // /*18*/     "select * from student where lname>J",
// // /*19*/     "select * from student where lname>J and (major=CS or major=Art)"
// };
// for (int i = 0; i < commandlines.size(); i++){
//   cout << commandlines[i] << endl;
//    t = sql.command(commandlines[i]);
//    cout << t << endl;
//       cout << "basic_test: records selected: "<< sql.select_recnos() << endl;
// }

  return true;
}

bool test_interactive(bool debug = false){
  cout << "interactive test " << endl;
  string input; 
  cout << "write command: ";
  getline(cin,input);
  SQL sql;
  Table t;
  while (input != "quit"){
    try{
      t = sql.command(input);
      cout << t << endl;
    } 
    catch (bool valid){
      cout << "Invalid command. Try again: " << endl; 
    }
    cout << "write command: ";
    getline(cin,input);
  }
  cout << "ended test !" << endl;

  return true; 
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(false));
}

TEST(TEST_INTERACTIVE, TestInteractive) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_interactive(false));
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}
