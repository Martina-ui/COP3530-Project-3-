#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

struct Edge {
    int destination_id;
    int time;
    bool is_closed;
    Edge(int destination, int time) : destination_id(destination), time(time), is_closed(false) {}
};

struct ClassInfo {
    string class_code;
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
    unordered_map<int, vector<Edge>> campus_graph;
    unordered_map<string, Student> students;
    unordered_map<string, ClassInfo> classes;
    unordered_map<int, int> get_shortest_path(int starting_location); //helper for shortest path using dijkstra's algorithm
    int calculate_cost_of_mst(int residence_location_id, const vector<int>& class_location_ids); //helper for generating student zone subgraph 
public:
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    void ParseCommand(const string &command);
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
