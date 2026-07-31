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

TEST_CASE("Test removeClass", "[removeClass]") {
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

TEST_CASE("Test toggleEdgesClosure", "[edges][toggle]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("toggleEdgesClosure 1 1 2");
    REQUIRE(buffer.str() == "successful\n");
    buffer.str("");

    compass.ParseCommand("checkEdgeStatus 1 2");
    REQUIRE(buffer.str() == "closed\n");
    buffer.str("");

    compass.ParseCommand("toggleEdgesClosure 1 1 2");
    REQUIRE(buffer.str() == "successful\n");
    buffer.str("");

    compass.ParseCommand("checkEdgeStatus 1 2");
    REQUIRE(buffer.str() == "open\n");

    cout.rdbuf(old_cout);
}

TEST_CASE("Test checkEdgeStatus", "[edges][status]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("checkEdgeStatus 1 2");
    REQUIRE(buffer.str() == "open\n");
    buffer.str("");

    compass.ParseCommand("checkEdgeStatus 999 998");
    REQUIRE(buffer.str() == "DNE\n");

    cout.rdbuf(old_cout);
}

TEST_CASE("Test isConnected", "[edges][connected]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("isConnected 1 2");
    REQUIRE(buffer.str() == "successful\n");
    buffer.str("");

    compass.ParseCommand("isConnected 1 999");
    REQUIRE(buffer.str() == "unsuccessful\n");

    cout.rdbuf(old_cout);
}

TEST_CASE("Test printShortestEdges", "[shortest_path]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("insert \"Gator\" 11111111 1 1 COP3530");
    buffer.str(""); 

    //can reach class initially
    compass.ParseCommand("printShortestEdges 11111111");
    string actual_reachable = buffer.str();
    buffer.str(""); 

    //toggle edges to make class unreachable
    compass.ParseCommand("toggleEdgesClosure 2 1 4 2 4");
    buffer.str(""); 

    //class is now unreachable
    compass.ParseCommand("printShortestEdges 11111111");
    string actual_unreachable = buffer.str();

    cout.rdbuf(old_cout);

    string expected_reachable = "Time For Shortest Edges: Gator\nCOP3530: 23\n";
    REQUIRE(actual_reachable == expected_reachable);

    string expected_unreachable = "Time For Shortest Edges: Gator\nCOP3530: -1\n";
    REQUIRE(actual_unreachable == expected_unreachable);
}

TEST_CASE("Test printStudentZone", "[mst][student_zone]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    compass.ParseCommand("insert \"Gator\" 11111111 1 1 COP3530");
    buffer.str(""); 

    compass.ParseCommand("printStudentZone 11111111");
    string actual_output = buffer.str();

    cout.rdbuf(old_cout);

    string expected_output = "Student Zone Cost For Gator: 23\n"; 
    
    REQUIRE(actual_output == expected_output);
}

TEST_CASE("Test three edge cases", "[edge_cases]") {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    stringstream buffer;
    streambuf* old_cout = cout.rdbuf(buffer.rdbuf());

    //removing a nonexistent student
    compass.ParseCommand("remove 99999999");
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str("");

    //dropping a class that a student doesnt have
    compass.ParseCommand("insert \"Martina\" 10000001 1 1 COP3530");
    buffer.str(""); 
    
    compass.ParseCommand("dropClass 10000001 MAC2311"); 
    REQUIRE(buffer.str() == "unsuccessful\n");
    buffer.str("");

    //checking isConnected for a location ID that doesn't exist
    compass.ParseCommand("isConnected 9999 8888");
    REQUIRE(buffer.str() == "unsuccessful\n");

    cout.rdbuf(old_cout);
}