#pragma once
#include <string>
#include <vector>

using namespace std;

struct Edge {
    int destination_id;
    int time;
    bool is_closed;
    Edge(int destination, int time) : destination_id(destination), time(time), is_closed(false) {}
};

struct ClassInfo {
    string class_clode;
    int location_id;
};


struct Student {
    string name;
    string student_id;
    int residence_location_id;
    vector<string> class_codes;
};

class CampusCompass {
private:
    // Think about what member variables you need to initialize
    // perhaps some graph representation?
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
