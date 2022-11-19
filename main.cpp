#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <utility>
#include <vector>

using namespace std;

// Omnidroid process
// 1. Extract n and m
// 2. Extract pairs to 2d vector
// 3. Extract singles to 1d vector
// 4. Loop through 2d vector and reference 1d vector to calculate sprocket cost

// Global dependency cost table
vector<int> dependency_cost_total;

// Get substring and return as int
int getSubstring(string line, int start, int end) 
{
	return stoi(line.substr(start, end));
}

// Create 2d vector out of int pairs
vector<vector<int>> constructLookupTable(ifstream& file, vector<int> sprocketCounts, int m, int n) 
{
	// Iterate through integer pairs - Store part number at that index with sprocket cost as value at index
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<vector<int>> result(n);
	cmatch match;
	regex pair("\\d\\s\\d");

	// Initialize nested vectors to -1 and global lookup table to 0
	for(int i = 0; i < result.size(); ++i){
		// -1 indicates that part i isn't dependent on any other part
		result[i].push_back(-1);
		dependency_cost_total.push_back(0);
	}

	// Iterate to line where n and m are declared and start on next line
	while(getline(file, line)) {
		if(start != true && regex_match(line.c_str(), match, pair))
			start = true;

		if(start == true) 
		{
			++line_number;
			if(line_number > m)
				break;

			// Get basic and intermediate parts
			int whitespace = line.find(" ");
			int length = line.length();

			int basic_part = getSubstring(line, 0, whitespace);
			int intermediate_part = getSubstring(line, whitespace + 1, length - whitespace - 1);

			// If -1 is at the location that a dependency shoud go, clear it before pushing the dependency
			if(result[intermediate_part][0] == -1)
				result[intermediate_part].clear();

			result[intermediate_part].push_back(basic_part);
			dependency_cost_total[intermediate_part] += sprocketCounts[basic_part];
		}
	}

	for(int i = 0; i < dependency_cost_total.size(); ++i)
		cout << dependency_cost_total[i] << endl;

	return result;
}

// Index = part number, value @ index = part cost
vector<int> getSingles(ifstream& file, int n)
{
	// Iterate through single integers - Store sprocket cost for each part in sprocketCounts array
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<int> result(n);
	cmatch match;
	regex single("\\d*"), pair("\\d\\s\\d");

	// Iterate past first single digit since it has nothing to do with omnidroid assembly or else regex will match it
	while(getline(file, line)) 
	{
		// Iterate to single ints
		if(start != true && regex_match(line.c_str(), match, pair))
			break;
	}

	while(getline(file, line) && line_number < n) 
	{
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

// Actual process of constructing robot from extracted pairs and singles
int constructOmnidroid(vector<vector<int>> assembly, vector<int> part_cost)
{
	int result = 0, n = assembly.size();

	// Iterate through last index of dependency_cost_total and compute omnidroid cost
	for(int i = 0; i < assembly[n - 1].size(); ++i)
		result += dependency_cost_total[assembly[n - 1][i]] + part_cost[assembly[n - 1][i]];

	result += part_cost[n - 1];
	
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
	vector<int> sprocketCounts = getSingles(input_file, n);

	// Seek to beginning of input file
	input_file.clear();
	input_file.seekg(0);

	vector<vector<int>> assembly_list = constructLookupTable(input_file, sprocketCounts, m, n);

	// Print 2d vector for debugging
	cout << "Constructed from int pairs" << endl;
	for(int i = 0; i < assembly_list.size(); ++i) 
	{
		cout << "Index " << i << ": ";
		for(int j = 0; j < assembly_list[i].size(); j++) 
			cout << assembly_list[i][j] << " ";
		cout << endl;
	}



	// Print singles
	cout << endl;
	cout << "Contructed from single ints: " << endl;
	for(int i = 0; i < sprocketCounts.size(); ++i)
		cout << "Index "<< i << ": " << sprocketCounts[i] << endl;

	int omnidroidCost = constructOmnidroid(assembly_list, sprocketCounts);
	cout << endl;
	cout << "Total omnidroid cost: " << omnidroidCost << endl;

	input_file.close();

	return 0;
}