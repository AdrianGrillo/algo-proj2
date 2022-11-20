#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <regex>
#include <utility>
#include <vector>
#include <sstream>

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


void getPartsAndDependencies(ifstream& file, int* n, int* m)
{
	string line = "";
	regex pair("\\d*\\s\\d*");
	cmatch match;

	// Get n and m
	while(getline(file, line)) 
	{
		if(regex_match(line.c_str(), match, pair)) 
		{
			int whitespace = line.find(" ");
			int length = line.length();

			*n = getSubstring(line, 0, whitespace);
			*m = getSubstring(line, whitespace + 1, length - whitespace - 1);

			break;
		}
	}
}

// Create 2d vector out of int pairs
vector<vector<int>> constructAssemblyList(ifstream& file, vector<int> part_cost_individual, int m, int n) 
{
	cout << "assembly start" << endl;
	// Iterate through integer pairs - Store part number at that index with sprocket cost as value at index
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<vector<int>> result(n);
	cmatch match;
	regex pair("\\d\\s\\d");

	cout << "assembly initialize" << endl;
	// Initialize nested vectors to -1 and global lookup table to 0
	for(int i = 0; i < result.size(); ++i){
		// -1 indicates that part i isn't dependent on any other part
		result[i].push_back(-1);
		dependency_cost_total.push_back(0);
	}

	cout << "assembly iterate" << endl;
	// Iterate to line where n and m are declared and start on next line
	while(getline(file, line)) {
		if(start != true && regex_match(line.c_str(), match, pair))
			cout << "assembly iterate start = true" << endl;
			start = true;

		if(start == true) 
		{
			cout << "assembly iterate second if beginning" << endl;
			++line_number;
			if(line_number > m)
				break;

			// Get basic and intermediate parts
			cout << "assembly iterate second if 1st comment" << endl;
			int whitespace = line.find(" ");
			int length = line.length();
			cout << line << endl;
			int basic_part = getSubstring(line, 0, whitespace);
			int intermediate_part = getSubstring(line, whitespace + 1, length - whitespace - 1);

			cout << "assembly iterate second if 2nd comment" << endl;
			// If -1 is at the location that a dependency shoud go, clear it before pushing the dependency
			if(result[intermediate_part][0] == -1)
				result[intermediate_part].clear();

			result[intermediate_part].push_back(basic_part);
			dependency_cost_total[intermediate_part] += part_cost_individual[basic_part];
		}
	}

	cout << "assembly last for" << endl;
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
int constructOmnidroid(vector<vector<int>> assembly, vector<int> part_cost_individual)
{
	int result = 0, n = assembly.size();

	// Iterate through last index of dependency_cost_total and compute omnidroid cost
	for(int i = 0; i < assembly[n - 1].size(); ++i)
		result += dependency_cost_total[assembly[n - 1][i]] + part_cost_individual[assembly[n - 1][i]];

	result += part_cost_individual[n - 1];
	
	return result;
}

int Omnidroid(ifstream &input){
	int n, m;
	cout << "getParts" << endl;
	getPartsAndDependencies(input, &n, &m);

	cout << "getSingles" << endl;
	vector<int> part_cost_individual = getSingles(input, n);
	
	
	// Seek to beginning of input file
	input.clear();
	input.seekg(0);

	cout << "assembly list" << endl;
	vector<vector<int>> assembly_list = constructAssemblyList(input, part_cost_individual, m, n);

	cout << "print 1" << endl;
	// Print 2d vector for debugging
	cout << "Constructed from int pairs" << endl;
	for(int i = 0; i < assembly_list.size(); ++i) 
	{
		cout << "Index " << i << ": ";
		for(int j = 0; j < assembly_list[i].size(); j++) 
			cout << assembly_list[i][j] << " ";
		cout << endl;
	}

	cout << "print 2" << endl;

	// Print singles
	cout << endl;
	cout << "Contructed from single ints: " << endl;
	for(int i = 0; i < part_cost_individual.size(); ++i)
		cout << "Index "<< i << ": " << part_cost_individual[i] << endl;

	int omnidroidCost = constructOmnidroid(assembly_list, part_cost_individual);
	cout << endl;
	cout << "Total omnidroid cost: " << omnidroidCost << endl;

	return omnidroidCost;

}

//INCOMPLETE
vector<tuple<int, int>> robotomatonPairs(ifstream& file, int m) 
{
	// Iterate through integer pairs - Store part number at that index with sprocket cost as value at index
	string line = "";
	int line_number = 0;
	bool start = false;
	vector<tuple<int, int>> result(m);
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

int constructRobotomaton(int n, vector<tuple <int, int> > partList, vector<vector<int>> lookupTable){
	
	if (lookupTable[get<0>(partList[n])][get<1>(partList[n])] == -1){

		lookupTable[get<0>(partList[n])][get<1>(partList[n])] += get<0>(partList[n]);

		for(int i = 0; i < n; i ++){
			lookupTable[get<0>(partList[n-i])][get<1>(partList[n-i])] += constructRobotomaton(n+i, partList, lookupTable);
		}
	}
	
	return lookupTable[get<0>(partList[n])][get<1>(partList[n])];

}

int robotomatonWrapper(int numStages, vector<vector<int>> costs){
	vector<tuple <int, int> > parts;

	for(int i = 0; i < numStages; i++){
		costs[get<0>(parts[i])][get<1>(parts[i])] = -1;
	}

	return constructRobotomaton(numStages, parts, costs);

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
	string omni = "omnidroid";
	string robo = "robotomaton";
	stringstream robotnum;
	int robots;

	int totalSprockets;

	//get how many robots we gotta make
	getline(input_file, line);
	robotnum << line;
	robotnum >> robots;
	cout << "robots: " << robots << endl;

	//making the required number of robots
	ofstream output_file;
	output_file.open("output.txt");
	for(int i = 0; i < robots; ++i){
		getline(input_file, line);
		if (omni.compare(line) == 0) {
			cout << "omnidroid" << endl;
			//call omnidroid funct
			totalSprockets = Omnidroid(input_file);
			cout << totalSprockets << endl;
			output_file << totalSprockets << endl;
		} else if(robo.compare(line) == 0) {
			cout << "robotomaton" << endl;
			//call robotomaton funct
			//output_file << robotomatonWrapper() << endl;

		}

	}

	input_file.close();
	output_file.close();
	return 0;
}
