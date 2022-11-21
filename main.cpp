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

void getParts(ifstream& file, int* n)
{
	string line = "";

	while(getline(file, line)) 
	{
        if(!totalRobots)
            totalRobots = stoi(line);

		if(line.length() >= 3 && line != "omnidroid" && line != "robotomaton") 
		{
			int whitespace = line.find(" ");
			*n = stoi(line.substr(0, whitespace));

			break;
		}
	}
}

void getAssemblyAndPartsList(ifstream& file, int n,  vector<int>& part_cost, vector<vector<int>>& assembly_list) 
{
    // Initialize each vector at result[i] -1, this indicates that part #i isn't dependent on any other part
	for(int i = 0; i < assembly_list.size(); ++i)
		assembly_list[i].push_back(-1);

    string line = "";
	int line_number = 0;

    while(getline(file, line)) 
    {
        if(line.length() >= 3)
        {
        // Get basic and intermediate parts
        int whitespace = line.find(" ");
        int length = line.length();

        int basic_part = stoi(line.substr(0, whitespace));;
        int intermediate_part = stoi(line.substr(whitespace + 1, length - whitespace + 1));

        // If -1 is at the location that a dependency shoud go, clear it before pushing the dependency
        if(assembly_list[intermediate_part][0] == -1)
            assembly_list[intermediate_part].clear();

        assembly_list[intermediate_part].push_back(basic_part);
        } 
        else
        {
            part_cost[line_number] = stoi(line);
            ++line_number;
        }
	}
}

void getDependencyCost(const vector<vector<int>>& assembly_list, const vector<int>& part_cost, vector<int>& dependency_cost)
{
    // Initialize depency_cost vector to 0
    for(int i = 0; i < assembly_list.size(); ++i)
        dependency_cost.push_back(0);

    for(int i = 0; i < assembly_list.size(); ++i) 
    {
        if(assembly_list[i][0] != -1) 
        {
            for(int j = 0; j < assembly_list[i].size(); ++j)
                dependency_cost[i] += part_cost[assembly_list[i][j]];
        }
    }
}

int constructOmnidroid(const vector<vector<int>> assembly_list, const vector<int> part_cost, const vector<int> dependency_cost)
{
	int result = 0, n = assembly_list.size();

	// Iterate through last index of dependency_cost_total and compute omnidroid cost
	for(int i = 0; i < assembly_list[n - 1].size(); ++i)
		result += dependency_cost[assembly_list[n - 1][i]] + part_cost[assembly_list[n - 1][i]];

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

    // Get n
	int n;
	getParts(input_file, &n);

    // Index = part number, value @ index = part cost
    vector<int> part_cost(n);

    // Index = part number, ints @ index = dependencies
    vector<vector<int>> assembly_list(n);
    getAssemblyAndPartsList(input_file, n, part_cost, assembly_list);

    // Index = part number, value @ index = cost of all dependencies
    vector<int> dependency_cost;
    getDependencyCost(assembly_list, part_cost, dependency_cost);

    int omnidroidCost = constructOmnidroid(assembly_list, part_cost, dependency_cost);

    cout << "Omnidroid cost: " << omnidroidCost << endl;

    input_file.close();

    return 0;
}