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

	string line = "";
	int n, m;
	regex regex("\\d*\\s\\d*");
	cmatch match;

	// Store the number of part and dependency pairs
	while(getline(input_file, line)) {
		if(regex_match(line.c_str(), match, regex)) {
			int whitespace = line.find(" ");
			int n = line.length();

			n = stoi(line.substr(0, whitespace));
			m = stoi(line.substr(whitespace + 1, n - whitespace - 1));

			break;
		}
	}

	// Store basic and intermediate parts in lookup array
	line = "";
	int line_number = 0;
	bool start = false;
	vector<int> lookup(m - 1);

	while(getline(input_file, line) && line_number < m) {
		// Iterate to two lines after the string
		if(start != true && regex_match(line.c_str(), match, regex))
			start = true;

		if(start == true) {
			++line_number;
			
			// Get basic and intermediate parts
			int whitespace = line.find(" ");
			int n = line.length();

			int basic_part = stoi(line.substr(0, whitespace));
			int intermediate_part = stoi(line.substr(whitespace + 1, n - whitespace - 1));
			
			// Part number = index, sprocket cost = value at that index
			lookup[basic_part] = intermediate_part;
		}

	}

	for(int i = 0; i < lookup.size(); ++i) {
		cout << "Value at index "<< i <<" : " << lookup[i] << endl;
	}

	return 0;
}