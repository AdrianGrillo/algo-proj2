#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <utility>
#include <vector>
#include <tuple>

using namespace std;

// Omnidroid process
// 1. Extract n and m
// 2. Extract pairs to 2d vector
// 3. Extract singles to 1d vector
// 4. Loop through 2d vector and reference 1d vector to calculate sprocket cost

// Get substring and return as int
int getSubstring(string line, int start, int end) 
{
	return stoi(line.substr(start, end));
}

// Create 2d vector out of int pairs
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

		if(start != true && regex_match(line.c_str(), match, pair))
			start = true;

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
	while(getline(file, line)) {
		// Iterate to single ints
		if(start != true && regex_match(line.c_str(), match, pair))
			break;
	}

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

// Actual process of constructing robot from extracted pairs and singles
int constructOmnidroid(vector<vector<int>> assembly, vector<int> part_cost)
{
	int result = 0;
	vector<int> dependencies(assembly.size());

	// Sum dependencies
	for(int i = 0; i < assembly.size(); ++i)
		dependencies[i] = assembly[i].size();

	// Loop through singles
	for(int i = 0; i < part_cost.size(); ++i)
		result += part_cost[i];
	
	// Loop through pairs
	for(int i = 0; i < assembly.size(); ++i) 
		// If the vector at assembly[i] has more than one index, add part_cost[i] to result for each index at assembly[i]
		if(assembly[i].size() > 1)
			result += dependencies[i] * part_cost[i];

	return result;
}


//INCOMPLETE
vector<tuple<int, int>> robotomatonPairs(ifstream& file, int m) 
{
	// Iterate through integer pairs - Store part number at that index with sprocket cost as value at index
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<tuple<int, int>> result(m-1);
	cmatch match;
	regex pair("\\d\\s\\d");

	// Iterate to line where n and m are declared and start on next line
	while(getline(file, line) && line_number < m) {
		// If buffer matches a single, skip
		if(regex_match(line.c_str(), match, regex("\\d")))
			continue; 

		if(start != true && regex_match(line.c_str(), match, pair))
			start = true;

		if(start == true) {
			// Get basic and intermediate parts
			int whitespace = line.find(" ");
			int length = line.length();

			int cost = getSubstring(line, 0, whitespace);
			int stage = getSubstring(line, whitespace + 1, length - whitespace - 1);
			
			result.emplace_back(cost, stage);

			++line_number;
		}
	}

	return result;
}

int robotomatonWrapper(int numStages, vector<vector<int>> costs){
	vector<tuple <int, int> > parts;

	for(int i = 0; i < numStages; i++){
		costs[get<0>(parts[i])][get<1>(parts[i])] = -1;
	}

	return constructRobotomaton(numStages, parts, costs);

}


int constructRobotomaton(int n, vector<tuple <int, int> > partList, vector<vector<int>> lookupTable){
	
	if (lookupTable[get<0>(partList[n])][get<1>(partList[n])] = -1){

		lookupTable[get<0>(partList[n])][get<1>(partList[n])] += get<0>(partList[n]);

		for(int i = 0; i < n; i ++){
			lookupTable[get<0>(partList[n-i])][get<1>(partList[n-i])] += constructRobotomaton(n+i, partList, lookupTable);
		}
	}
	
	return lookupTable[get<0>(partList[n])][get<1>(partList[n])];

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
			cout << assembly_list[i][j] << " ";
		cout << endl;
	}

	// Seek to beginning of input file
	input_file.clear();
	input_file.seekg(0);

	vector<int> sprocketCounts = getSingles(input_file, n);

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