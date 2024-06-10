#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include "../../includes/sql/sql.h"
#include <vector> 
using namespace std;

vector<string> gen_random(int count){
    vector<string> randomnums;
    srand(time(nullptr)); 
    for (int i = 0; i < count; i++) {
        int num = rand()% 90 + 10;  
        randomnums.push_back(to_string(num)); 
    }
    return randomnums;
};

int count(const vector<string>& strings, const string& input){
    // vector<string> result;
    int size = 0;
    for (int i = 0; i < strings.size(); i++){
      string str = strings[i];
        if (str <= input){
          size++;
          // result.push_back(str);
        }
    }
    return size;
};
  vector<string> splitString(const string& str){
    vector<string> nums;
    istringstream s(str);
    string token;
    while (s >> token) {
        nums.push_back(token);
    }
    return nums;
}

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
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	90",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	67",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	43",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	82",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	21",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	12",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	22",
//            "insert into students values \"Michael'Connor\",     Nor,	CS,	67",
//            "insert into students values \"Michael O'Connor\",     Nor,	History,	82",
//            "insert into students values \"Michael O'Connor\",     Nor,	CS,	80",
//            "insert into students values \" O'Connor\",     Nor,	CS,	67",
//            "insert into students values \"Michael O'Connor\",     Nor,	Math,	22"
          

// };

  vector<string> random_numbers = gen_random(50);
  // string s = "10 17 45 51 93 93 54 22 69 33 32 77 44 20 18 87 27 57 98 17 36 19 59 71 91 94 33 89 43 53";
  // vector<string> randomNumbers = splitString(s);

  // Map<string, 
  map<string, int> theresults;
  vector<string> commandlines;
  vector<string> insertlist;
  string table_name_temp = "losangelesan";
  insertlist.push_back("create table " + table_name_temp + " fields age");
  for (int i = 0; i < random_numbers.size(); i++){
    insertlist.push_back("insert into " + table_name_temp + " values " + random_numbers[i]);
    commandlines.push_back("select * from " + table_name_temp + " where age <= " + random_numbers[i]);

    int countt = count(random_numbers, random_numbers[i]);
    // myResults.insert(randomNumbers[i], result);
    theresults[random_numbers[i]] = countt;
  }  

 SQL sql;
 Table t;
  for (int i = 0; i < insertlist.size(); i++){
    if (i == insertlist.size()-1){
      cout << sql.command(insertlist[i]) << endl;
    }
    else
    sql.command(insertlist[i]);
  }


  // const vector<string> commandlines = {
  //     getTestCases();
  //     "select * from students where age <= 22" ///failing
  // };




//     const vector<string> commandlines = {
      
//       // "Select * From employee",
//       // "select * from employee",
//       // "SELECT fname FROM employee",
//       // "select * from students where age <= 30", //failllllllllll
//       // "select * from students where age <= 18",
//       // "select * from students where age <= 10",
//       // "select * from students where age <= 39",
//       // "select * from students where age <= 21",
//       // "select * from students where age <= 30",
//       // "select * from students where age <= 42",
//       // "select * from students where age <= 19",
//       // "select * from students where age <= 67",
//       // "select * from students where age <= 38",
//       "select * from students where age <= 22" ///failing
//       // "selECT lname, fname, major frOM students where (major = Mh and age < 25)",
//       // "select * from students",
//       // "select * from students where age > 19 and (lname = Jackson or major = Math and fname = \"Mary Ann\")",
//       // "select * from students where age < 50 and (fname = \"Teresa Mae\" or lname = Jackson and (age > 10 and (major = CS or major = Math)))",
//       // "select",
//       // "select * from students where major >= CS",
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
// // /*19*/     "select * from student where lname>J and (major=CS or major=Art)",
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
for (int i = 0; i < commandlines.size(); i++){
   cout << commandlines[i] << endl;
   t = sql.command(commandlines[i]);
   cout << t << endl;
   vector<long> actualresults = sql.select_recnos();
   cout << "basic_test: records selected: "<< actualresults.size() << endl;
   int expectedresults = theresults[random_numbers[i]];
   cout << "basic_test: records expected: "<< expectedresults << endl;
   cout << "vector order that we inserted in " << random_numbers << endl; 
   assert(expectedresults == actualresults.size());
}

  return true;
}

bool test_interactive(bool debug = false){
  // cout << "interactive test " << endl;
  // string input; 
  // cout << "write command: ";
  // getline(cin,input);
  // SQL sql;
  // Table t;
  // while (input != "quit"){
  //   try{
  //     t = sql.command(input);
  //     cout << t << endl;
  //   } 
  //   catch (bool valid){
  //     cout << "Invalid command. Try again: " << endl; 
  //   }
  //   cout << "write command: ";
  //   getline(cin,input);
  // }
  // cout << "ended test !" << endl;
  cout << "interactive test " << endl;
  string input; 
  cout << "write command: ";
  getline(cin,input);
  SQL sql;
  Table t;
  while (input != "quit"){
      t = sql.command(input);
      // if (!sql.is_error()){
         cout << t << endl;
      // }
      cout << "write command: ";
      getline(cin,input);
  }
  cout << "ended test !" << endl;

  return true; 
}

// TEST(TEST_STUB, TestStub) {
  
//   //EXPECT_EQ(0, <your individual test functions are called here>);

//   EXPECT_EQ(1, test_stub(false));
// }

TEST(TEST_INTERACTIVE, TestInteractive) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_interactive(false));
}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}
