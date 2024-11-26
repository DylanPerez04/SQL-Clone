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

const int COMMANDS = 11;

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

    //EXPECT_EQ(1, test_sql_relational(false));
}

const vector<string> command_list2 = {
    //****************************************************************************
    // LOGICAL OPERATORS
    //****************************************************************************
    //.................
    //:AND :
    //.................
    "select * from student where fname = \"Flo\" and lname = \"Yao\"",
    //.................
    //:OR :
    //.................
    "select * from student where fname = Flo or lname = Jackson",
    //.................
    //:OR AND :
    //.................
    "select * from student where fname = Flo or major = CS and age <= 23",
    //.................
    //:AND OR AND :
    //.................
    "select * from student where age < 30 and major = CS or major = Physics and lname = Jackson",
    //.................
    //:OR AND OR :
    //.................
    "select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson"
};

const int COMMANDS2 = 5;
bool test_sql_logical(bool debug = false) {

    SQL sql;
    Table t;

    cout << endl
        << endl;
    for (int i = 0; i < COMMANDS2; i++)
    {
        cout << "\n>" << command_list2[i] << endl;
        if (debug)
            cout << sql.command(command_list2[i]) << endl;
        else
            t = sql.command(command_list2[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << "----- END TEST --------" << endl;

    return true;
}


bool test_sql_logical2(bool debug = false) {

    cout << endl << " ================ test_sql_logical2() ===================================" << endl << endl;

    const vector<string> CMDS = {
        "select * from student where lname = \"Sammuel L.\""
    };

    SQL sql;
    Table t;

    cout << endl
        << endl;
    for (int i = 0; i < CMDS.size(); i++)
    {
        cout << "\n>" << CMDS[i] << endl;
        if (debug)
            cout << sql.command(CMDS[i]) << endl;
        else
            t = sql.command(CMDS[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << "----- END TEST --------" << endl;

    return true;
}

TEST(TEST_SQL_LOGICAL, TestLogical) {

    //EXPECT_EQ(1, test_sql_logical(false));
    EXPECT_EQ(1, test_sql_logical2(false));
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

