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

	// Get n and m
	while(getline(input_file, line)) {
		if(regex_match(line.c_str(), match, pair)) {
			int whitespace = line.find(" ");
			int length = line.length();

			n = getSubstring(line, 0, whitespace);
			m = getSubstring(line, whitespace + 1, length - whitespace - 1);

			break;
		}
	}

	// Iterate through integer pairs - Store part number at that index with sprocket cost as value at index
	int line_number = 0;
	bool start = false;
	vector<vector<int>> assembly_list(n - 1);

	// Iterate to line where n and m are declared and start on next line
	while(getline(input_file, line) && line_number < m) {
		if(start != true && regex_match(line.c_str(), match, pair)) {
			start = true;
			continue;
		}

		if(start == true) {
			// Get basic and intermediate parts
			int whitespace = line.find(" ");
			int length = line.length();

			int basic_part = getSubstring(line, 0, whitespace);
			int intermediate_part = getSubstring(line, whitespace + 1, length - whitespace - 1);
			
			assembly_list[basic_part].push_back(intermediate_part);
			++line_number;
		}
	}

	// Print 2d vector for debugging
	cout << "Constructed from int pairs" << endl;
	for(int i = 0; i < assembly_list.size(); ++i) {
		cout << "Index " << i << ": ";
		for(int j = 0; j < assembly_list[i].size(); j++) {
			cout << assembly_list[i][j] << " ";
		}
		cout << endl;
	}

	// Iterate through single integers - Store sprocket cost for each part in sprocketCounts array
	line_number = 0;
	start = false;
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

	cout << endl;
	cout << "Contructed from single ints: " << endl;
	for(int i = 0; i < sprocketCounts.size(); ++i) {
		cout << "Index "<< i << ": " << sprocketCounts[i] << endl;
	}

	input_file.close();

	return 0;
}