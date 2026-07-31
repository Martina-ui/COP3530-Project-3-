#include "CampusCompass.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <utility> 

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
        if (classes.find(code) == classes.end()) {
            cout << "unsuccessful" << endl;
            return;
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

void CampusCompass::drop_class(const string& student_ID, const string& class_code) {
    if (students.find(student_ID) == students.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    if (classes.find(class_code) == classes.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    auto& student_classes = students[student_ID].class_codes; 
    auto it = find(student_classes.begin(), student_classes.end(), class_code);
    
    if (it == student_classes.end()) {
        cout << "unsuccessful" << endl;
        return;
    }
    student_classes.erase(it);

    if (student_classes.empty()) {
        students.erase(student_ID);
    }

    cout << "successful" << endl;
}

void CampusCompass::replace_class(const string& student_ID, const string& old_class_code, const string& new_class_code) {
    if (students.find(student_ID) == students.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    if (classes.find(new_class_code) == classes.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    auto& student_classes = students[student_ID].class_codes;
    auto it_old = find(student_classes.begin(), student_classes.end(), old_class_code);
    if (it_old == student_classes.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    auto it_new = find(student_classes.begin(), student_classes.end(), new_class_code);
    if (it_new != student_classes.end()) {
        cout << "unsuccessful" << endl; 
        return;
    }
    *it_old = new_class_code;

    cout << "successful" << endl;
}

void CampusCompass::remove_class(const string& class_code) {
    if (class_code.length() != 7) {
        cout << "unsuccessful" << endl;
        return;
    }
    for (int i = 0; i < 3; i++) {
        if (!isupper(class_code[i])) {
            cout << "unsuccessful" << endl;
            return;
        }
    }
    for (int i = 3; i < 7; i++) {
        if (!isdigit(class_code[i])) {
            cout << "unsuccessful" << endl;
            return;
        }
    }

    if (classes.find(class_code) == classes.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    int students_affected_count = 0;
    vector<string> students_to_delete; 

    for (auto& pair : students) {
        auto& student_classes = pair.second.class_codes;
        auto it = find(student_classes.begin(), student_classes.end(), class_code);
        if (it != student_classes.end()) {
            student_classes.erase(it);
            students_affected_count++;

            if (student_classes.empty()) {
                students_to_delete.push_back(pair.first); 
            }
        }
    }

    if (students_affected_count == 0) {
        cout << "unsuccessful" << endl;
        return;
    }

    for (const string& id : students_to_delete) {
        students.erase(id);
    }

    cout << students_affected_count << endl;
}

void CampusCompass::toggle_edges_closure(const vector<int>& location_ids) {
    for (size_t i = 0; i < location_ids.size(); i += 2) {
        int loc_u = location_ids[i];
        int loc_v = location_ids[i+1];

        for (Edge& edge : campus_graph[loc_u]) {
            if (edge.destination_id == loc_v) {
                edge.is_closed = !edge.is_closed; 
                break; 
            }
        }

        for (Edge& edge : campus_graph[loc_v]) {
            if (edge.destination_id == loc_u) {
                edge.is_closed = !edge.is_closed;
                break;
            }
        }
    }
    cout << "successful" << endl;
}

void CampusCompass::check_edge_status(int location_x, int location_y) {
    if (campus_graph.find(location_x) != campus_graph.end()) {
        for (const Edge& edge : campus_graph[location_x]) {
            if (edge.destination_id == location_y) {
                if (edge.is_closed) {
                    cout << "closed" << endl;
                } else {
                    cout << "open" << endl;
                }
                return; 
            }
        }
    }
    cout << "DNE" << endl;
}

void CampusCompass::is_connected(int location_1, int location_2) {
    if (location_1 == location_2) {
        cout << "successful" << endl;
        return;
    }
    if (campus_graph.find(location_1) == campus_graph.end()) {
        cout << "unsuccessful" << endl;
        return;
    }

    unordered_set<int> visited;
    queue<int> q;
    q.push(location_1);
    visited.insert(location_1);
    while (!q.empty()) {
        int current_location = q.front();
        q.pop();
        if (current_location == location_2) {
            cout << "successful" << endl;
            return;
        }
        for (const Edge& edge : campus_graph[current_location]) {
            if (!edge.is_closed && visited.find(edge.destination_id) == visited.end()) {
                visited.insert(edge.destination_id);
                q.push(edge.destination_id);
            }
        }
    }
    cout << "unsuccessful" << endl;
}

void CampusCompass::print_shortest_edges(const string& student_ID) {
    if (students.find(student_ID) == students.end()) {
        return; 
    }
    const Student& student = students[student_ID];
    unordered_map<int, int> shortest_paths = get_shortest_path(student.residence_location_id);
    cout << "Time For Shortest Edges: " << student.name << endl;
    vector<string> sorted_classes = student.class_codes;
    sort(sorted_classes.begin(), sorted_classes.end());
    for (const string& class_code : sorted_classes) {
        int class_location = classes[class_code].location_id;

        if (shortest_paths.find(class_location) != shortest_paths.end()) {
            cout << class_code << ": " << shortest_paths[class_location] << endl;
        } else {
            cout << class_code << ": -1" << endl;
        }
    }
}

void CampusCompass::print_student_zone(const string& student_ID) {
    if (students.find(student_ID) == students.end()) {
        return; 
    }
    const Student& student = students[student_ID];
    vector<int> class_location_ids;
    for (const string& class_code : student.class_codes) {
        class_location_ids.push_back(classes[class_code].location_id);
    }

    int mst_cost = calculate_cost_of_mst(student.residence_location_id, class_location_ids);

    cout << "Student Zone Cost For " << student.name << ": " << mst_cost << endl;
}

unordered_map<int, int> CampusCompass::get_shortest_path(int starting_location) {
    unordered_map<int, int> distances;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //https://www.geeksforgeeks.org/cpp/pair-in-cpp-stl/

    pq.push({0, starting_location});
    distances[starting_location] = 0;

    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_loc = pq.top().second;
        pq.pop();
        if (current_dist > distances[current_loc]) {
            continue;
        }

        for (const Edge& edge : campus_graph[current_loc]) {
            if (edge.is_closed) {
                continue;
            }
            int new_dist = current_dist + edge.time;
            if (distances.find(edge.destination_id) == distances.end() || new_dist < distances[edge.destination_id]) {
                distances[edge.destination_id] = new_dist;
                pq.push({new_dist, edge.destination_id});
            }
        }
    }
    return distances;
}

int CampusCompass::calculate_cost_of_mst(int residence_location_id, const vector<int>& class_location_ids) {
    unordered_map<int, int> distances;
    unordered_map<int, int> parents; 
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    pq.push({0, residence_location_id});
    distances[residence_location_id] = 0;
    parents[residence_location_id] = residence_location_id; 

    while (!pq.empty()) {
        int current_dist = pq.top().first;
        int current_loc = pq.top().second;
        pq.pop();

        if (current_dist > distances[current_loc]) continue;

        for (const Edge& edge : campus_graph[current_loc]) {
            if (edge.is_closed) continue;
            
            int new_dist = current_dist + edge.time;
            if (distances.find(edge.destination_id) == distances.end() || new_dist < distances[edge.destination_id]) {
                distances[edge.destination_id] = new_dist;
                parents[edge.destination_id] = current_loc;
                pq.push({new_dist, edge.destination_id});
            }
        }
    }
    unordered_set<int> subgraph_nodes;
    subgraph_nodes.insert(residence_location_id);
    for (int class_loc : class_location_ids) {
        if (distances.find(class_loc) == distances.end()) continue; 
        int curr = class_loc;
        while (curr != residence_location_id) {
            subgraph_nodes.insert(curr);
            curr = parents[curr]; 
        }
    }
    int total_cost = 0;
    unordered_set<int> in_mst;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> mst_pq;
    in_mst.insert(residence_location_id);
    for (const Edge& edge : campus_graph[residence_location_id]) {
        if (!edge.is_closed && subgraph_nodes.find(edge.destination_id) != subgraph_nodes.end()) {
            mst_pq.push({edge.time, edge.destination_id});
        }
    }

    while (!mst_pq.empty() && in_mst.size() < subgraph_nodes.size()) {
        int weight = mst_pq.top().first;
        int node = mst_pq.top().second;
        mst_pq.pop();
        if (in_mst.find(node) != in_mst.end()) continue; 
        in_mst.insert(node);
        total_cost += weight; 
        for (const Edge& edge : campus_graph[node]) {
            if (!edge.is_closed && subgraph_nodes.find(edge.destination_id) != subgraph_nodes.end()) {
                if (in_mst.find(edge.destination_id) == in_mst.end()) {
                    mst_pq.push({edge.time, edge.destination_id});
                }
            }
        }
    }
    return total_cost;
}