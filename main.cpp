#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <regex>
#include <utility>
#include <vector>
#include <sstream>
#include <tuple>


using namespace std;


// Get substring and return as int
int getSubstring(string line, int start, int end) 
{
	return stoi(line.substr(start, end));
}

void getParts(ifstream& file, int* n)
{
	string line = "";

	while(getline(file, line)) 
	{

		if(line.length() >= 3) 
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
	for(int i = 0; i < (int) assembly_list.size(); ++i)
		assembly_list[i].push_back(-1);

	string line = "";
	int line_number = 0;

	while(getline(file, line)) 
	{
		if(line.empty()) {
			break;
		}
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
	for(int i = 0; i < (int) assembly_list.size(); ++i)
		dependency_cost.push_back(0);

	for(int i = 0; i < (int) assembly_list.size(); ++i) 
	{
		if(assembly_list[i][0] != -1) 
		{
			for(int j = 0; j < (int) assembly_list[i].size(); ++j)
				dependency_cost[i] += part_cost[assembly_list[i][j]];
		}
	}
}

int constructOmnidroid(const vector<vector<int>> assembly_list, const vector<int> part_cost, const vector<int> dependency_cost)
{
	int result = 0, n = assembly_list.size();

	// Iterate through last index of dependency_cost and compute omnidroid cost
	for(int i = 0; i < (int) assembly_list[n - 1].size(); ++i)
		result += dependency_cost[assembly_list[n - 1][i]] + part_cost[assembly_list[n - 1][i]];

	result += part_cost[n - 1];
	
	return result;
}

int Omnidroid(ifstream &input){
	// Get n
	int n;
	getParts(input, &n);

	// Index = part number, value @ index = part cost
	vector<int> part_cost(n);

	// Index = part number, ints @ index = dependencies
	vector<vector<int>> assembly_list(n);
	getAssemblyAndPartsList(input, n, part_cost, assembly_list);

	// Index = part number, value @ index = cost of all dependencies
	vector<int> dependency_cost;
	getDependencyCost(assembly_list, part_cost, dependency_cost);

	int omnidroidCost = constructOmnidroid(assembly_list, part_cost, dependency_cost);

	return omnidroidCost;
}

//Function handles robotomaton part computations
int constructRobotomaton(int n, vector<tuple <int, int> > partList, vector<int> lookupTable){
    int result = 0;
    int children = get<1>(partList[n-1]);
	
	if (lookupTable[n-1] == 0){

		lookupTable[n-1] += get<0>(partList[n-1]);
        result += get<0>(partList[n-1]);

		for(int i = 0; i < children+1; i ++){
			lookupTable[n-i] += constructRobotomaton(n-i, partList, lookupTable);
            result += constructRobotomaton(n-i, partList, lookupTable);
		}
	}

    return result;

}

vector<tuple<int, int>> getRobotomatonParts(ifstream& file, int* numStages)
{
    string line = "";
    vector<tuple<int,int>> answer;

    while(getline(file, line)) 
    {
		if (line.empty()) {
			break;
		}
        if (line.length() == 1){
            *numStages = stoi(line);
        }

        if(line.length() >= 3) 
        {
            int whitespace = line.find(" ");
            int length = line.length();

            int costs = getSubstring(line, 0, whitespace);
            int stage = getSubstring(line, whitespace + 1, length - whitespace - 1);
            
            answer.emplace_back(costs,stage);

        }
    }
    return answer;
}


//Function initializes 1d vector for memoizaztion and calls the contructRobotomaton function
int robotomatonWrapper(ifstream &input){
	int numStages = 0;
	vector<tuple<int, int>> assembly = getRobotomatonParts(input, &numStages);
    vector<int> costs(numStages);

	for(int i = 0; i < numStages; i++){
		costs[i] = 0;
	}
	return constructRobotomaton(numStages, assembly, costs);

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

	//making the required number of robots
	ofstream output_file;
	output_file.open("output.txt");
	ifstream test("output.txt");
	
	if (output_file.is_open()) {
		for(int i = 0; i < robots; ++i){
			getline(input_file, line);
			if (line.empty()) {
				getline(input_file, line);
			}
			if (omni.compare(line) == 0) {
				//call omnidroid funct
				totalSprockets = Omnidroid(input_file);
				cout << totalSprockets << endl;
				output_file << totalSprockets << endl;
			} else if(robo.compare(line) == 0) {
				//call robotomaton funct
				totalSprockets = robotomatonWrapper(input_file);
				cout << totalSprockets << endl;
				output_file << totalSprockets << endl;
				}
			}
		output_file.close();
  	} else {
		cout << "Unable to open file";
		return 1;
	}
  	
	

	input_file.close();

	return 0;
}
