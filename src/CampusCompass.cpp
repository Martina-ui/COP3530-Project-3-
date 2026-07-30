#include "CampusCompass.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    //load edges.csv
    ifstream edges_file(edges_filepath);
    if (!edges_file.is_open()) {
        return false; 
    }
    string line;
    getline(edges_file, line); 

    while (getline(edges_file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string loc1_str, loc2_str, name1, name2, time_str;

        getline(ss, loc1_str, ',');
        getline(ss, loc2_str, ',');
        getline(ss, name1, ',');
        getline(ss, name2, ',');
        getline(ss, time_str, ',');

        int loc1 = stoi(loc1_str);
        int loc2 = stoi(loc2_str);
        int time = stoi(time_str);

        campus_graph[loc1].push_back(Edge(loc2, time));
        campus_graph[loc2].push_back(Edge(loc1, time));
    }
    edges_file.close();

    //load classes.csv
    ifstream classes_file(classes_filepath);
    if (!classes_file.is_open()) {
        return false;
    }

    getline(classes_file, line); 

    while (getline(classes_file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string code, loc_str;

        getline(ss, code, ',');
        getline(ss, loc_str, ','); 

        ClassInfo info;
        info.class_code = code;
        info.location_id = stoi(loc_str);

        classes[code] = info;
    }
    classes_file.close();

    // return boolean based on whether parsing was successful or not
    return true;
}

void CampusCompass::ParseCommand(const string &command) {
    if (command.empty()) return;

    stringstream ss(command);
    string op;
    ss >> op; 

    if (op == "insert") {
        string dummy, name, student_ID;
        int residence_location_id, n;
        vector<string> class_codes;

        getline(ss, dummy, '"'); 
        getline(ss, name, '"');  

        ss >> student_ID >> residence_location_id >> n;

        bool valid_n_classes = true;
        for (int i = 0; i < n; i++) {
            string code;
            if (ss >> code) {
                class_codes.push_back(code);
            } else {
                valid_n_classes = false; 
                break;
            }
        }

        if (!valid_n_classes) {
            cout << "unsuccessful" << endl;
        } else {
            insert_student(name, student_ID, residence_location_id, class_codes);
        }
    }
    else if (op == "remove") {
        string student_ID;
        ss >> student_ID;
        remove_student(student_ID);
    }
    else if (op == "dropClass") {
        string student_ID, class_code;
        ss >> student_ID >> class_code;
        drop_class(student_ID, class_code);
    }
    else if (op == "replaceClass") {
        string student_ID, old_class_code, new_class_code;
        ss >> student_ID >> old_class_code >> new_class_code;
        replace_class(student_ID, old_class_code, new_class_code);
    }
    else if (op == "removeClass") {
        string class_code;
        ss >> class_code;
        remove_class(class_code);
    }
    else if (op == "toggleEdgesClosure") {
        int n;
        ss >> n;
        vector<int> location_ids;
        for (int i = 0; i < n * 2; i++) {
            int loc;
            ss >> loc;
            location_ids.push_back(loc);
        }
        toggle_edges_closure(location_ids);
    }
    else if (op == "checkEdgeStatus") {
        int loc_x, loc_y;
        ss >> loc_x >> loc_y;
        check_edge_status(loc_x, loc_y);
    }
    else if (op == "isConnected") {
        int loc_1, loc_2;
        ss >> loc_1 >> loc_2;
        is_connected(loc_1, loc_2);
    }
    else if (op == "printShortestEdges") {
        string student_ID;
        ss >> student_ID;
        print_shortest_edges(student_ID);
    }
    else if (op == "printStudentZone") {
        string student_ID;
        ss >> student_ID;
        print_student_zone(student_ID);
    }
    else {
        cout << "unsuccessful" << endl;
    }
}

void CampusCompass::insert_student(const string& name, const string& student_ID, int residence_location_id, const vector<string>& class_codes) {
    //validate the student
    if (student_ID.length() != 8) {
        cout << "unsuccessful" << endl;
        return;
    }
    for (char c : student_ID) {
        if (!isdigit(c)) {
            cout << "unsuccessful" << endl;
            return;
        }
    }

    if (students.find(student_ID) != students.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    for (char c : name) {
        if (!isalpha(c) && !isspace(c)) {
            cout << "unsuccessful" << endl;
            return;
        }
    }

    if (class_codes.size() < 1 || class_codes.size() > 6) {
        cout << "unsuccessful" << endl;
        return;
    }

    for (const string& code : class_codes) {
        if (code.length() != 7) {
            cout << "unsuccessful" << endl;
            return;
        }
        for (int i = 0; i < 3; i++) {
            if (!isupper(code[i])) { 
                cout << "unsuccessful" << endl;
                return;
            }
        }
        for (int i = 3; i < 7; i++) {
            if (!isdigit(code[i])) { 
                cout << "unsuccessful" << endl;
                return;
            }
        }
    }

    //if validation passes, student is created
    Student new_student;
    new_student.name = name;
    new_student.student_ID = student_ID;
    new_student.residence_location_id = residence_location_id;
    new_student.class_codes = class_codes;

    students[student_ID] = new_student;

    cout << "successful" << endl;
}

void CampusCompass::remove_student(const string& student_ID) {
    if (students.find(student_ID) == students.end()) {
        cout << "unsuccessful" << endl;
        return;
    }
    students.erase(student_ID);

    cout << "successful" << endl;
}

void CampusCompass::drop_class(const string& student_ID, const string& class_code){

}

void CampusCompass::replace_class(const string& student_ID, const string& old_class_code, const string& new_class_code){

}

void CampusCompass::remove_class(const string& class_code){

}

void CampusCompass::toggle_edges_closure(const vector<int>& location_ids){

}

void CampusCompass::check_edge_status(int location_x, int location_y){

}

void CampusCompass::is_connected(int location_1, int location_2){

}

void CampusCompass::print_shortest_edges(const string& student_ID){

} 

void CampusCompass::print_student_zone(const string& student_ID){

}

unordered_map<int, int> CampusCompass::get_shortest_path(int starting_location) {
    unordered_map<int, int> distances;

    return distances;
}

int CampusCompass::calculate_cost_of_mst(int residence_location_id, const vector<int>& class_location_ids){
    int total_cost = 0;

    return total_cost;
}