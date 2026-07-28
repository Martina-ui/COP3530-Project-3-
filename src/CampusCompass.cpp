#include "CampusCompass.h"

#include <string>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
    return true;
}

void CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
}

void CampusCompass::insert_student(const string& name, const string& student_ID, int residence_location_id, const vector<string>& class_codes) {

}

void CampusCompass::remove_student(const string& student_ID){

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