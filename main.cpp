#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <istream>
#include <sstream>

using namespace std;

// Global for total number of robots in file
int totalRobots;

// Get substring and return as int
int getSubstring(string line, int start, int end) 
{
	return stoi(line.substr(start, end));
}

void getPartsAndDependencies(ifstream& file, int* n, int* m)
{
	string line = "";

	while(getline(file, line)) 
	{
        if(!totalRobots)
            totalRobots = stoi(line);

        cout << "Total robots: " << totalRobots << endl;
		if(line.length() >= 3 && line != "omnidroid" && line != "robotomaton") 
		{
			int whitespace = line.find(" ");
			int length = line.length();

			*n = getSubstring(line, 0, whitespace);
			*m = getSubstring(line, whitespace + 1, length - whitespace - 1);

			break;
		}
	}
}

void getAssemblyAndPartsList(ifstream& file, int n, int m,  vector<int>& part_cost, vector<vector<int>>& assembly_list) 
{
    // Initialize each vector at result[i] -1, this indicates that part #i isn't dependent on any other part
	for(int i = 0; i < assembly_list.size(); ++i){
		assembly_list[i].push_back(-1);
		// dependency_cost_total.push_back(0);
	}

    string line = "";
	int line_number = 0;

    while(getline(file, line)) 
    {
        if(line.length() >= 3)
        {
        // Get basic and intermediate parts
        int whitespace = line.find(" ");
        int length = line.length();

        int basic_part = getSubstring(line, 0, whitespace);
        int intermediate_part = getSubstring(line, whitespace + 1, length - whitespace - 1);

        // If -1 is at the location that a dependency shoud go, clear it before pushing the dependency
        if(assembly_list[intermediate_part][0] == -1)
            assembly_list[intermediate_part].clear();

        assembly_list[intermediate_part].push_back(basic_part);
        // dependency_cost_total[intermediate_part] += part_cost_individual[basic_part];
        } 
        else
        {
            ++line_number;
            part_cost[line_number] = stoi(line);
        }
	}
}

// vector<int> getPartCost(ifstream& file, int n)
// {
// 	string line = "";
// 	int line_number = 0;
// 	vector<int> result(n);

// 	while(getline(file, line) && line_number < n) 
// 	{
//         if(line.find(" ") == -1 && line != "omnidroid" && line != "robotomaton" && line != "")
//         {
//             result[line_number] = stoi(line);
//             ++line_number;
//         }
// 	}

// 	return result;
// }

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

    // Get n and m
	int n, m;
	getPartsAndDependencies(input_file, &n, &m);

    vector<int> part_cost(n);
    vector<vector<int>> assembly_list(n);
    getAssemblyAndPartsList(input_file, n, m, part_cost, assembly_list);

	for(int i = 0; i < assembly_list.size(); ++i) 
	{
		cout << "Index " << i << ": ";
		for(int j = 0; j < assembly_list[i].size(); j++) 
			cout << assembly_list[i][j] << " ";
		cout << endl;
	}


    for(int i = 0; i < part_cost.size(); ++i)
		cout << "Index "<< i << ": " << part_cost[i] << endl;
}