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
    compass.ParseCommand("insert \"Martina Delger\" 123 1 1 COP3530");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //Invalid class format
    compass.ParseCommand("insert \"Martina Delger\" 12345678 1 1 cop3530");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //Too many classes
    compass.ParseCommand("insert \"Martina Delger\" 12345678 1 7 COP3530 MAC2311 CDA3101 EEL3701 COP3502 COP3503 PHY2048");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str(""); 

    //Duplicate UFID
    compass.ParseCommand("insert \"Martina Delger\" 12345678 1 1 COP3530");
    buffer.str(""); 
    compass.ParseCommand("insert \"Martina Delger\" 12345678 1 1 MAC2311");
    REQUIRE(buffer.str() == "unsuccessful\n");
    
    cout.rdbuf(old_cout);
}

TEST_CASE("Test remove", "[remove]") {
    CampusCompass compass;
    
    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("insert \"Martina Delger\" 12345678 1 1 COP3530");
    REQUIRE(buffer.str() == "successful\n");
    buffer.str(""); 

    compass.ParseCommand("remove 12345678");
    
    REQUIRE(buffer.str() == "successful\n");
    buffer.str(""); 

    compass.ParseCommand("remove 12345678");
    
    REQUIRE(buffer.str() == "unsuccessful\n");

    cout.rdbuf(old_cout);
}

TEST_CASE("Test dropClass", "[dropClass]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("insert \"Martina\" 10000001 1 2 COP3530 MAC2311");
    buffer.str(""); 

    compass.ParseCommand("dropClass 10000001 MAC2311");
    REQUIRE(buffer.str() == "successful\n");
    buffer.str("");

    compass.ParseCommand("dropClass 10000001 COP3530");
    REQUIRE(buffer.str() == "successful\n");
    buffer.str("");

    compass.ParseCommand("remove 10000001");
    REQUIRE(buffer.str() == "unsuccessful\n"); 

    cout.rdbuf(old_cout);
}

TEST_CASE("Test replaceClass", "[replaceClass]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("insert \"Martina\" 10000001 1 2 COP3530 MAC2311");
    buffer.str(""); 

    compass.ParseCommand("replaceClass 10000001 CDA3101 EEL3701");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str("");

    compass.ParseCommand("replaceClass 10000001 COP3530 CDA3101");
    REQUIRE(buffer.str() == "successful\n");

    cout.rdbuf(old_cout);
}

TEST_CASE("Test removeClass functionality", "[removeClass]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("insert \"Martina\" 10000001 1 2 COP3530 MAC2311");
    buffer.str("");
    compass.ParseCommand("insert \"Mikael\" 10000002 1 1 COP3530");
    buffer.str("");

    compass.ParseCommand("removeClass COP3530");
    
    REQUIRE(buffer.str() == "2\n"); 
    buffer.str("");

    compass.ParseCommand("remove 10000002");
    REQUIRE(buffer.str() == "unsuccessful\n"); 

    cout.rdbuf(old_cout);
}