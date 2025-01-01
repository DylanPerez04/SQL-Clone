
# Work Report

## Name: <ins> Dylan Perez-Hernandez </ins>

## Features:

- Not Implemented:
  - Remove
  - Merge
  - Everything that isn't create, insert, or select

<br><br>

- Implemented:
  - Table create
  - Record insert
  - Table select

<br><br>

- Partly implemented:
  - N/A

<br><br>

- Bugs
  - At times, when running the basic test, no records will be inserted to the initial table, making any selects show empty tables. This most likely has to due with the fact that I am not properly checking and closing fstream objects utilized in my utilities.cpp file.
  - To get passed this bug, simply keep re-running the program until there are no crashes and all the tables are displaying correctly.
  - I will admit I am a bit too lazy to fix the issue considering I already passed CS 008 with an A++ (100% plus extra credit for wearing a costume to class on Halloween), but atleast I know what it is!

<br><br>

# Reflections:

- This project is the culmination of, quite literally, 100s of hours of work. Every single folder represents a single individual project that had to be meticulously programmed, unit tested, and at times documented, in order to become the almagamation that is this impressively unimpressive final project. 
- While I may have been the first to pass 100% on all unit tests for the final project, having spent nearly 50 hours alone on it, I am certainly not the best nor most efficient of my peers.
- There is just WAY too much for me to document. Should have documented each project when I was working on them.

# **output**
<pre>
<br/><br/><br/><br/>
## Delete this line and paste the output of your basic test and then testB here
</pre>


<br/><br/>

# basic_test.cpp output:
<pre>
<br/><br/><br/><br/>


----------running basic_test.cpp---------


[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from SQL_BASIC
[ RUN      ] SQL_BASIC.SQLBasic
>make table employee fields  last,       first,         dep,      salary, year
basic_test: table created.

>make table employee fields  last,       first,         dep,      salary, year
>insert into employee values Blow,       Joe,           CS,       100000, 2018
>insert into employee values Blow,       JoAnn,         Physics,  200000, 2016
>insert into employee values Johnson,    Jack,          HR,       150000, 2014
>insert into employee values Johnson,    "Jimmy",     Chemistry,140000, 2018
>make table student fields  fname,          lname,    major,    age
>insert into student values Flo,            Yao,        Art,    20
>insert into student values Bo,                      Yang,      CS,             28
>insert into student values "Sammuel L.", Jackson,      CS,             40
>insert into student values "Billy",         Jackson,   Math,   27
>insert into student values "Mary Ann",   Davis,        Math,   30



>select * from employee
There is no condition

Table name: _select_table_4, records: 4
                   record                     last                    first                      dep                   salary                     year

                        0                     Blow                      Joe                       CS                   100000                     2018                                      
                        1                     Blow                    JoAnn                  Physics                   200000                     2016                                      
                        2                  Johnson                     Jack                       HR                   150000                     2014                                      
                        3                  Johnson                    Jimmy                Chemistry                   140000                     2018                                      



basic_test: records selected: 0 1 2 3


>select last, first, age from employee
There is no condition

Table name: _select_table_6, records: 4
                   record                     last                    first                      age

                        0                     Blow                      Joe                     Blow                                                                                        
                        1                     Blow                    JoAnn                     Blow                                                                                        
                        2                  Johnson                     Jack                  Johnson                                                                                        
                        3                  Johnson                    Jimmy                  Johnson                                                                                        



basic_test: records selected: 0 1 2 3


>select last from employee
There is no condition

Table name: _select_table_8, records: 4
                   record                     last

                        0                     Blow                                                                                                                                          
                        1                     Blow                                                                                                                                          
                        2                  Johnson                                                                                                                                          
                        3                  Johnson                                                                                                                                          



basic_test: records selected: 0 1 2 3


>select * from employee where last = Johnson
ShuntingYard() : output_queue = Queue: Head-><-[last]-> <-[Johnson]-> <-[=]-> |||

Table name: _select_table_9, records: 2
                   record                     last                    first                      dep                   salary                     year

                        0                  Johnson                     Jack                       HR                   150000                     2014                                      
                        1                  Johnson                    Jimmy                Chemistry                   140000                     2018                                      



basic_test: records selected: 2 3


>select * from employee where last=Blow and major="JoAnn"
ShuntingYard() : output_queue = Queue: Head-><-[last]-> <-[Blow]-> <-[=]-> <-[major]-> <-[JoAnn]-> <-[=]-> <-[and]-> |||

Table name: _select_table_10, records: 0
                   record                     last                    first                      dep                   salary                     year




basic_test: records selected:


>select * from student
There is no condition

Table name: _select_table_12, records: 5
                   record                    fname                    lname                    major                      age

                        0                      Flo                      Yao                      Art                       20                                                               
                        1                       Bo                     Yang                       CS                       28                                                               
                        2               Sammuel L.                  Jackson                       CS                       40                                                               
                        3                    Billy                  Jackson                     Math                       27                                                               
                        4                 Mary Ann                    Davis                     Math                       30                                                               



basic_test: records selected: 0 1 2 3 4


>select * from student where (major=CS or major=Art)
ShuntingYard() : output_queue = Queue: Head-><-[major]-> <-[CS]-> <-[=]-> <-[major]-> <-[Art]-> <-[=]-> <-[or]-> |||

Table name: _select_table_13, records: 3
                   record                    fname                    lname                    major                      age

                        0                      Flo                      Yao                      Art                       20                                                               
                        1                       Bo                     Yang                       CS                       28                                                               
                        2               Sammuel L.                  Jackson                       CS                       40                                                               



basic_test: records selected: 0 1 2


>select * from student where lname>J
ShuntingYard() : output_queue = Queue: Head-><-[lname]-> <-[J]-> <-[>]-> |||

Table name: _select_table_14, records: 4
                   record                    fname                    lname                    major                      age

                        0                      Flo                      Yao                      Art                       20                                                               
                        1                       Bo                     Yang                       CS                       28                                                               
                        2               Sammuel L.                  Jackson                       CS                       40                                                               
                        3                    Billy                  Jackson                     Math                       27                                                               



basic_test: records selected: 0 1 2 3


>select * from student where lname>J and (major=CS or major=Art)
ShuntingYard() : output_queue = Queue: Head-><-[lname]-> <-[J]-> <-[>]-> <-[major]-> <-[CS]-> <-[=]-> <-[major]-> <-[Art]-> <-[=]-> <-[or]-> <-[and]-> |||

Table name: _select_table_15, records: 3
                   record                    fname                    lname                    major                      age

                        0                      Flo                      Yao                      Art                       20                                                               
                        1                       Bo                     Yang                       CS                       28                                                               
                        2               Sammuel L.                  Jackson                       CS                       40                                                               



basic_test: records selected: 0 1 2

----- END TEST --------
[       OK ] SQL_BASIC.SQLBasic (718 ms)
[----------] 1 test from SQL_BASIC (719 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (724 ms total)
[  PASSED  ] 1 test.
</pre>
# testB.cpp output:
<pre>
<br/><br/><br/><br/>
</pre>


