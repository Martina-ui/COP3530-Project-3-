#pragma once
#include <string>
#include <vector>
#include <unordered_map>

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
    string student_ID;
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
    void insert_student(const string& name, const string& student_ID, int residence_location_id, const vector<string>& class_codes);
    void remove_student(const string& student_ID);
    void drop_class(const string& student_ID, const string& class_code);
    void replace_class(const string& student_ID, const string& old_class_code, const string& new_class_code);
    void remove_class(const string& class_code);
    void toggle_edges_closure(const vector<int>& location_ids);
    void check_edge_status(int location_x, int location_y); 
    void is_connected(int location_1, int location_2);
    void print_shortest_edges(const string& student_ID); 
    void print_student_zone(const string& student_ID); 
};
