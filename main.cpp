#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main() {

	// Open input file
	ifstream input_file;

	input_file.open("input.txt");

	if(!input_file) {
		cout << "Unable to open input file" << endl;	
		return 1;
	}

	string line;
	int parts, dependencies;
	regex regex("\\d\\s\\d");
	cmatch match;

	// Store the number of parts and dependencies
	while(getline(input_file, line)) {
		if(regex_match(line.c_str(), match, regex)) {
			parts = stoi(line.substr(0, 1));
			dependencies = stoi(line.substr(1, 2));
			cout << parts << endl;
			cout << dependencies << endl;
			break;
		}
	}

	return 0;
}