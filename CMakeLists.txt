cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(barkeshli_s22)


#. . . . . . . . . . . . . . . . . . . .
include(GNUInstallDirs)
if(MSVC)
    add_compile_options(/W4)
    add_link_options(/W4)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
    # message("compiler: MSVC")
else()
    # message("compiler: NOT MSVC")
    # add_compile_options(-Wall -Wextra -pedantic -Werror -Wl,--fatal-warnings)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()


# place binaries and libraries according to GNU standards
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})



if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()
#. . . . . . . . . . . . . . . . . . . .

# ------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

 
enable_testing() 


# ------------------------------------




#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#   Grab all cpp files from includes folder
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
set(INCLUDES_FOLDER includes)
FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}   "${INCLUDES_FOLDER}/*.cpp" )
#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 




ADD_EXECUTABLE(main
    main.cpp
    ${SOURCE_FILES}
 "includes/sql/sql.h" "includes/sql/sql.cpp" "includes/binary_files/file_record.cpp" "includes/binary_files/file_record.h"  "includes/binary_files/utilities.h" "includes/bplustree/bplustree.h" "includes/bplustree/btree_array_funcs.h" "includes/bplustree/map.h" "includes/bplustree/multimap.h" "includes/linked_list_functions/linkedlistfunctionsTemplated.h" "includes/node/node.h" "includes/parser/parser.cpp" "includes/parser/parser.h" "includes/parser/sql_parser_functions.cpp" "includes/parser/sql_parser_functions.h" "includes/parser/typedefs.h" "includes/queue/MyQueue.h" "includes/shunting_yard/shunting_yard.h" "includes/stack/MyStack.h" "includes/stl_utils/vector_utilities.h" "includes/table/table.cpp" "includes/table/table.h" "includes/table/typedefs.h" "includes/token/child_tokens.cpp" "includes/token/child_tokens.h" "includes/token/token.cpp" "includes/token/token.h" "includes/tokenizer/constants.h" "includes/tokenizer/ftokenize.cpp" "includes/tokenizer/ftokenize.h" "includes/tokenizer/state_machine_functions.cpp" "includes/tokenizer/state_machine_functions.h" "includes/tokenizer/stokenize.cpp" "includes/tokenizer/stokenize.h" "includes/bplustree/set_class.h" "includes/queue/queue_composition.h" "includes/stack/stack_composition.h" "includes/stub/stub.cpp" "includes/stub/stub.h" "includes/binary_files/utilities.cpp")

ADD_EXECUTABLE(basic_test
    _tests/_test_files/basic_test.cpp
    ${SOURCE_FILES}
 "includes/sql/sql.h" "includes/sql/sql.cpp" "includes/binary_files/file_record.cpp" "includes/binary_files/file_record.h"  "includes/binary_files/utilities.h" "includes/bplustree/bplustree.h" "includes/bplustree/btree_array_funcs.h" "includes/bplustree/map.h" "includes/bplustree/multimap.h" "includes/linked_list_functions/linkedlistfunctionsTemplated.h" "includes/node/node.h" "includes/parser/parser.cpp" "includes/parser/parser.h" "includes/parser/sql_parser_functions.cpp" "includes/parser/sql_parser_functions.h" "includes/parser/typedefs.h" "includes/queue/MyQueue.h" "includes/shunting_yard/shunting_yard.h" "includes/stack/MyStack.h" "includes/stl_utils/vector_utilities.h" "includes/table/table.cpp" "includes/table/table.h" "includes/table/typedefs.h" "includes/token/child_tokens.cpp" "includes/token/child_tokens.h" "includes/token/token.cpp" "includes/token/token.h" "includes/tokenizer/constants.h" "includes/tokenizer/ftokenize.cpp" "includes/tokenizer/ftokenize.h" "includes/tokenizer/state_machine_functions.cpp" "includes/tokenizer/state_machine_functions.h" "includes/tokenizer/stokenize.cpp" "includes/tokenizer/stokenize.h" "includes/bplustree/set_class.h" "includes/queue/queue_composition.h" "includes/stack/stack_composition.h" "includes/stub/stub.cpp" "includes/stub/stub.h" "includes/binary_files/utilities.cpp")

ADD_EXECUTABLE(testB
    _tests/_test_files/testB.cpp
    ${SOURCE_FILES}
    #- - - - - - - - - - - - - - - - 
 "includes/sql/sql.h" "includes/sql/sql.cpp" "includes/binary_files/file_record.cpp" "includes/binary_files/file_record.h"  "includes/binary_files/utilities.h" "includes/bplustree/bplustree.h" "includes/bplustree/btree_array_funcs.h" "includes/bplustree/map.h" "includes/bplustree/multimap.h" "includes/linked_list_functions/linkedlistfunctionsTemplated.h" "includes/node/node.h" "includes/parser/parser.cpp" "includes/parser/parser.h" "includes/parser/sql_parser_functions.cpp" "includes/parser/sql_parser_functions.h" "includes/parser/typedefs.h" "includes/queue/MyQueue.h" "includes/shunting_yard/shunting_yard.h" "includes/stack/MyStack.h" "includes/stl_utils/vector_utilities.h" "includes/table/table.cpp" "includes/table/table.h" "includes/table/typedefs.h" "includes/token/child_tokens.cpp" "includes/token/child_tokens.h" "includes/token/token.cpp" "includes/token/token.h" "includes/tokenizer/constants.h" "includes/tokenizer/ftokenize.cpp" "includes/tokenizer/ftokenize.h" "includes/tokenizer/state_machine_functions.cpp" "includes/tokenizer/state_machine_functions.h" "includes/tokenizer/stokenize.cpp" "includes/tokenizer/stokenize.h" "includes/bplustree/set_class.h" "includes/queue/queue_composition.h" "includes/stack/stack_composition.h" "includes/stub/stub.cpp" "includes/stub/stub.h" "includes/binary_files/utilities.cpp")

TARGET_LINK_LIBRARIES(basic_test gtest)
TARGET_LINK_LIBRARIES(testB gtest)

 

