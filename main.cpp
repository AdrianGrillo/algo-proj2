#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <utility>
#include <vector>

using namespace std;

// Get substring and return as int
int getSubstring(string line, int start, int end) 
{
	return stoi(line.substr(start, end));
}

int main() 
{
	// Open input file
	ifstream input_file;

	input_file.open("input.txt");

	if(!input_file) {
		cout << "Unable to open input file" << endl;	
		return 1;
	}

	string line = "";
	int n, m;
	regex pair("\\d*\\s\\d*");
	cmatch match;

	// Store the number of dependencies and parts
	while(getline(input_file, line)) {
		if(regex_match(line.c_str(), match, pair)) {
			int whitespace = line.find(" ");
			int length = line.length();

			n = getSubstring(line, 0, whitespace);
			m = getSubstring(line, whitespace + 1, length - whitespace - 1);

			break;
		}
	}

	// Store sprocket cost for each part in sprocketCounts array
	int line_number = 0;
	bool start = false;
	vector<int> sprocketCounts(n);
	regex single("\\d*");

	while(getline(input_file, line) && line_number < n) {
		// Iterate to two lines after the string
		if(start != true && regex_match(line.c_str(), match, single))
			start = true;

		// Iterate for as long as there is a single digit on a given line
		if(start == true) {
			sprocketCounts[line_number] = stoi(line);

			++line_number;
		}
	}

	// for(int i = 0; i < sprocketCounts.size(); ++i) {
	// 	cout << "Index "<< i << ": " << sprocketCounts[i] << endl;
	// }

	// --------------------------------------------------------------------------------------------------------------------------

	// Return to beginning of input file
	input_file.clear();
	input_file.seekg(0, ios::beg);

	// Store part number at that index with sprocket cost as value at index
	line_number = 0;
	start = false;
	vector<int> basic_parts(n - 1);
	regex lit("\\d*");

	cout << "hey" << endl;

	// Iterate to line where singles start and store them
	while(getline(input_file, line)) {
		if(start != true && regex_match(line.c_str(), match, lit)) {
			start = true;
		}

		cout << start <<endl;

		if(start == true) {
			cout << "hi" << endl;
			// Get basic and intermediate parts
			int whitespace = line.find(" ");
			int length = line.length();

			int basic_part = getSubstring(line, 0, whitespace);
			// int intermediate_part = getSubstring(line, whitespace + 1, length - whitespace - 1);

			// lookupAssembly[basic_part] = intermediate_part;

			basic_parts[basic_part] = 1;

			cout << basic_part;

			++line_number;
		}
	}

	for(int i = 0; i < basic_parts.size(); ++i) {
		cout << "Index "<< i << ": " << basic_parts[i] << endl;
	}

	input_file.close();

	return 0;
}