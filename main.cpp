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

vector<vector<int>> getPairs(ifstream& file, int m, int n) 
{
	// Iterate through integer pairs - Store part number at that index with sprocket cost as value at index
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<vector<int>> result(n - 1);
	cmatch match;
	regex pair("\\d\\s\\d");

	// Iterate to line where n and m are declared and start on next line
	while(getline(file, line) && line_number < m) {
		// If buffer matches a single, break
		if(regex_match(line.c_str(), match, regex("\\d")))
			break;

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
			
			result[basic_part].push_back(intermediate_part);
			++line_number;
		}
	}

	return result;
}

vector<int> getSingles(ifstream& file, int n)
{
	// Iterate through single integers - Store sprocket cost for each part in sprocketCounts array
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<int> result(n);
	cmatch match;
	regex single("\\d*");

	while(getline(file, line) && line_number < n) {
		// Iterate to single ints
		if(start != true && regex_match(line.c_str(), match, single))
			start = true;

		// Iterate for as long as there is a single digit on a given line
		if(start == true) {
			result[line_number] = stoi(line);
			++line_number;
		}
	}

	return result;
}

int main() 
{
	// Open input file
	ifstream input_file;

	input_file.open("input.txt");

	if(!input_file) 
	{
		cout << "Unable to open input file" << endl;	
		return 1;
	}

	string line = "";
	int n, m;
	regex pair("\\d*\\s\\d*");
	cmatch match;

	// Get n and m
	while(getline(input_file, line)) 
	{
		if(regex_match(line.c_str(), match, pair)) 
		{
			int whitespace = line.find(" ");
			int length = line.length();

			n = getSubstring(line, 0, whitespace);
			m = getSubstring(line, whitespace + 1, length - whitespace - 1);

			break;
		}
	}

	vector<vector<int>> assembly_list = getPairs(input_file, m, n);

	// Print 2d vector for debugging
	cout << "Constructed from int pairs" << endl;
	for(int i = 0; i < assembly_list.size(); ++i) 
	{
		cout << "Index " << i << ": ";
		for(int j = 0; j < assembly_list[i].size(); j++) 
		{
			cout << assembly_list[i][j] << " ";
		}
		cout << endl;
	}

	vector<int> sprocketCounts = getSingles(input_file, n);

	// Print singles
	cout << endl;
	cout << "Contructed from single ints: " << endl;
	for(int i = 0; i < sprocketCounts.size(); ++i) {
		cout << "Index "<< i << ": " << sprocketCounts[i] << endl;
	}

	input_file.close();

	return 0;
}