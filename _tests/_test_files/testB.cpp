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
        "select * from student where fname = Flo or fname = Bo or fname = \"Sammuel L.\" or fname = Billy or fname = \"Mary Ann\"",
        "select * from student where (fname = Flo and lname = Yao) or major = CS",
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


bool test_sql_parenthesis(bool debug = false) {

    vector<string> cmds = {
        "select * from student where (fname = Flo)",
        "select * from student where (fname = Flo) or (lname < Z)",
        "select * from student where (fname = Flo and lname = Yao or major = CS)",
        "select * from student where (fname = Flo and (lname = Yao or major = CS))",
        "select * from student where (fname = Flo and (lname <= Yao and (major = Art or age > 20)))"
    };

    SQL sql;
    Table t;

    cout << endl
        << endl;
    for (int i = 0; i < cmds.size(); i++)
    {
        cout << "\n>" << cmds[i] << endl;
        if (debug)
            cout << sql.command(cmds[i]) << endl;
        else
            t = sql.command(cmds[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << "----- END TEST --------" << endl;

    return true;
}

TEST(TEST_SQL, TestSql) {

    //EXPECT_EQ(1, test_sql_relational(false));
    //EXPECT_EQ(1, test_sql_logical(false));
    //EXPECT_EQ(1, test_sql_logical2(false));
    EXPECT_EQ(1, test_sql_parenthesis(false));
}

bool test_batch_file(bool debug = false) {

    cout << endl << " ====== test_batch_file() ===========================" << endl;

    cout << endl << "||||||||||||||||| CREATE AND INSERT |||||||||||||||||" << endl;

    vector<string> create_insert = {
    "make table employee fields last, first, dep",
    "insert into employee values Blow, Joe, CS",
    "insert into employee values Johnson, \"Jimmy\", Chemistry",
    "insert into employee values Yang, Bo, CS",
    "insert into employee values \"Jackson\", Billy, Math",
    "insert into employee values Johnson, Billy, \"Phys Ed\"",
    "insert into employee values \"Van Gogh\", \"Jim Bob\", \"Phys Ed\"",
    "make table student fields fname, lname, major, age",
    "insert into student values Flo, Yao, CS, 20",
    "insert into student values \"Flo\", \"Jackson\", Math, 21",
    "insert into student values Calvin, Woo, Physics, 22",
    "insert into student values \"Anna Grace\", \"Del Rio\", CS, 22",
    "select * from student"
    };

    SQL sql;
    Table t;

    cout << endl
        << endl;
    for (int i = 0; i < create_insert.size(); i++)
    {
        cout << "\n>" << create_insert[i] << endl;
        if (debug)
            cout << sql.command(create_insert[i]) << endl;
        else
            t = sql.command(create_insert[i]);
    }

    cout << endl << "||||||||||||||||| SIMPLE SELECT |||||||||||||||||" << endl;


    vector<string> simple_select = {
        "select* from student",
        //------- simple strings -------------------
        "select* from student where lname = Jackson",
        //----- quoted strings ---------------------
        "select * from student where lname = \"Del Rio\"",
        //-------- non-existing string ------------------
        "select * from student where lname = \"Does Not Exist\""
    };

    cout << endl
        << endl;
    for (int i = 0; i < simple_select.size(); i++)
    {
        cout << "\n>" << simple_select[i] << endl;
        if (debug)
            cout << sql.command(simple_select[i]) << endl;
        else
            t = sql.command(simple_select[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << endl << "||||||||||||||||| RELATIONAL OPERATORS |||||||||||||||||" << endl;


    vector<string> relational_ops = {
        //.................
        //:Greater Than :
        //.................
        "select* from student where lname > Yang",
        //. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
        "select* from student where age > 50",
        //. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
        "select * from student where age > 53",
        //. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .
        "select* from student where age > 54",
        //.................
        //:Greater Equal :
        //.................
        "select * from student where lname >= Yang",
        //. . . . . . (Greater Equal non-existing: ) . . . . . . . . . . .
        "select * from employee where last >= Jack",
        //.................
        //:Less Than :
        //.................
        //. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .
        "select * from student where lname < Jackson",
        //. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
    "select * from student where age < 20",
    //. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .
    "select* from student where age < 19",
    //.................
    //:Less Equal :
    //.................
    "select * from student where lname <= Smith",
    //. . . . . . (Less Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last <= Peach"
    };

    cout << endl
        << endl;
    for (int i = 0; i < relational_ops.size(); i++)
    {
        cout << "\n>" << relational_ops[i] << endl;
        if (debug)
            cout << sql.command(relational_ops[i]) << endl;
        else
            t = sql.command(relational_ops[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << endl << "||||||||||||||||| LOGICAL OPERATORS |||||||||||||||||" << endl;


    vector<string> logical_ops = {
        //.................
        //:AND :
        //.................
        "select* from student where fname = \"Flo\" and lname = \"Yao\"",
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

    cout << endl
        << endl;
    for (int i = 0; i < logical_ops.size(); i++)
    {
        cout << "\n>" << logical_ops[i] << endl;
        if (debug)
            cout << sql.command(logical_ops[i]) << endl;
        else
            t = sql.command(logical_ops[i]);
        cout << "basic_test: records selected: " << sql.select_recnos() << endl;
    }

    cout << "----- END TEST --------" << endl;

    return true;
}

TEST(TEST_BATCH_FILE, TestBatchFile) {

    //EXPECT_EQ(1, test_sql_logical(false));
    //EXPECT_EQ(1, test_batch_file(false));
}

bool test_shunting_yard(bool debug = false) {

    cout << endl << " ====== test_shunting_yard() ===========================" << endl;

    SQL sql;
    Table t;

    //string cmd = "select * from student where lname = Malone and fname = Deza or fname = \"Mary Ann\"";
    string cmd = "select * from student where (age < 17 or age > 20) and lname = Jackson";

    cout << endl
        << endl;

    cout << "\n>" << cmd << endl;
    if (debug)
        cout << sql.command(cmd) << endl;
    else
        t = sql.command(cmd);
    cout << "test_shunting_yard: records selected: " << sql.select_recnos() << endl;


    return true;
}

TEST(TEST_GENERIC, TestGeneric) {

    //EXPECT_EQ(1, test_shunting_yard(false));
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

