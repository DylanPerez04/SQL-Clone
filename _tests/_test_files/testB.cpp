#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

//------------------------------------------------------------------------------------------
//Files we are testing:
#include "../../includes/sql/sql.h"


//------------------------------------------------------------------------------------------


using namespace std;

const vector<string> command_list = {

    //.................
    //:Greater Than :
    //.................
    "select * from student where lname > Yang",
    //. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where age > 50",
    //. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . .
    "select * from student where age > 53",
    //. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where age > 54",
    //.................
    //:Greater Equal :
    //.................
    "select * from student where lname >= Yang",
    //. . . . . . (Greater Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last >= Jack",
    //.................
    //:Less Than :
    //.................
    //. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . .
    "select * from student where lname < Jackson",
    //. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . .
    "select * from student where age < 20",
    //. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .
    "select * from student where age < 19",
    //.................
    //:Less Equal :
    //.................
    "select * from student where lname <= Smith",
    //. . . . . . (Less Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last <= Peach"

};

const int COMMANDS = 1;

bool test_sql_relational(bool debug = false) {

    SQL sql;
    Table t;

    cout << endl
        << endl;
    for (int i = 0; i < COMMANDS; i++)
    {
        cout << "\n>" << command_list[i] << endl;
        if (debug)
            cout << sql.command(command_list[i]) << endl;
        else
            t = sql.command(command_list[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << "----- END TEST --------" << endl;
    return true;

    return true;
}

TEST(TEST_SQL_RELATIONAL, TestRelational) {

    EXPECT_EQ(1, test_sql_relational(false));
}

//bool test_stub(bool debug = false)
//{
//  if (debug){
//    cout << "testB:: test-sub() entering test_sub" << endl;
//  }
//  return true;
//}
//
//TEST(TEST_STUB, TestStub) {
//  
//  //EXPECT_EQ(0, <your individual test functions are called here>);
//
//  EXPECT_EQ(1, test_stub(false));
//}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

