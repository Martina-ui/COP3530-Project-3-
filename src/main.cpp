#include <iostream>

#include "CampusCompass.h"

using namespace std;

int main() {
    CampusCompass compass;

    if (!compass.ParseCSV("data/edges.csv", "data/classes.csv")) {
        cout << "Error: Failed to load CSV files" << endl;
        return 0;
    }
    string num_lines_str;
    getline(cin, num_lines_str);
    int no_of_lines = stoi(num_lines_str);
    for (int i = 0; i < no_of_lines; i++) {
        string command;
        getline(cin, command);
        compass.ParseCommand(command);
    }
    return 0;
}
