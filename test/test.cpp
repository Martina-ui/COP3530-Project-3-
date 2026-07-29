#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>

// change if you choose to use a different header name
#include "CampusCompass.h"

using namespace std;

TEST_CASE("Test five incorrect insert commands", "[insert]") {
    CampusCompass compass;

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf()); //source: https://cplusplus.com/reference/streambuf/streambuf/#google_vignette & https://stackoverflow.com/questions/8116541/what-exactly-is-streambuf-how-do-i-use-it

    //Invalid name
    compass.ParseCommand("insert \"A11y\" 45679999 1 1 COP3530");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //invalid UFID 
    compass.ParseCommand("insert \"Josh Smith\" 123 1 1 COP3530");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //Invalid class format
    compass.ParseCommand("insert \"Josh Smith\" 12345678 1 1 cop3530");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //Too many classes
    compass.ParseCommand("insert \"Josh Smith\" 12345678 1 7 COP3530 MAC2311 CDA3101 EEL3701 COP3502 COP3503 PHY2048");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //Duplicate UFID
    compass.ParseCommand("insert \"Josh Smith\" 12345678 1 1 COP3530");
    buffer.str(""); 
    compass.ParseCommand("insert \"Another Name\" 12345678 1 1 MAC2311");
    REQUIRE(buffer.str() == "unsuccessful\n");
    
    cout.rdbuf(old_cout);
}