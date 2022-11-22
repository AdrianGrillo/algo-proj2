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
	int line_length = 0;
	string line = "";
	char lastCH;

	while(getline(file, line)) 
	{
		lastCH = line[line.length() - 1];
		if(strcmp(&lastCH, "\r")) 
			line_length = line.length() - 1;
		else
			line_length = line.length();

		if(line_length >= 3) 
		{
			int whitespace = line.find(" ");
			*n = stoi(line.substr(0, whitespace));

			break;
		}
	}
}

void getAssemblyAndPartsList(ifstream& file, int n,  vector<int>& part_cost, vector<vector<int>>& assembly_list) 
{
	cout << "ass" << endl;
	// Initialize each vector at result[i] -1, this indicates that part #i isn't dependent on any other part
	for(int i = 0; i < (int) assembly_list.size(); ++i)
		assembly_list[i].push_back(-1);

	string line = "";
	int line_number = 0;
	int line_length = 0;
	char lastCH;
	while(getline(file, line)) 
	{
		cout << line << endl;
		cout << "line len b4 everything: " << line.length() << endl;
		lastCH = line[line.length() - 1];
		if(strcmp(&lastCH, "\r")) {
			line_length = line.length() - 1;
		} else {
			line_length = line.length();
			cout << "line len: " << line_length << endl;
			cout << "line len(): " << line.length() << endl;
		}
    		

		if(line == "\n" || line == "\r" || line.empty()) {
			break;
		}

		if(line_length >= 3)
		{
			// Get basic and intermediate parts
			int whitespace = line.find(" ");
			int length = line_length;

			int basic_part = stoi(line.substr(0, whitespace));
			int intermediate_part = stoi(line.substr(whitespace + 1, length - whitespace + 1));

			// If -1 is at the location that a dependency shoud go, clear it before pushing the dependency
			if(assembly_list[intermediate_part][0] == -1)
				assembly_list[intermediate_part].clear();

			assembly_list[intermediate_part].push_back(basic_part);
			cout << "assembly list: " << assembly_list[intermediate_part][0] << endl;
		} 
		else
		{
			cout << "else" << endl;
			part_cost[line_number] = stoi(line);
			++line_number;
		}
	}
}

void getDependencyCost(const vector<vector<int>>& assembly_list, const vector<int>& part_cost, vector<int>& dependency_cost)
{
	cout << "dep" << endl;
	cout << "ass size: " << assembly_list.size() << endl;
	// Initialize depency_cost vector to 0
	cout << "dep 1 for" << endl;
	for(int i = 0; i < (int) assembly_list.size(); ++i)
		dependency_cost.push_back(0);

	cout << "dep 2 for" << endl;
	for(int i = 0; i < (int) assembly_list.size(); ++i) 
	{
		cout << "dep 2 for i:" << i << endl;
		if(assembly_list[i][0] != -1) 
		{
			cout << "dep 3 for" << endl;
			for(int j = 0; j < (int) assembly_list[i].size(); ++j)
				dependency_cost[i] += part_cost[assembly_list[i][j]];
		}
	}
}

int constructOmnidroid(const vector<vector<int>> assembly_list, const vector<int> part_cost, const vector<int> dependency_cost)
{
	cout << "constr omni" << endl;
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
	//cout << "constr robo" << endl;
    int result = 0;
    int children = get<1>(partList[n-1]);
	//cout << "children: " << children << endl;

	if (lookupTable[n-1] == 0){

		lookupTable[n-1] += get<0>(partList[n-1]);
        result += get<0>(partList[n-1]);
		//cout << "constr robo for" << endl;
		for(int i = 0; i < children+1; i ++){
			// cout << "i: " << i << endl;
			// cout << "n-i: " << (n-i) << endl;
			lookupTable[n-i] += constructRobotomaton(n-i, partList, lookupTable);
            result += constructRobotomaton(n-i, partList, lookupTable);
		}
	}

    return result;

}

vector<tuple<int, int>> getRobotomatonParts(ifstream& file, int* numStages)
{
	cout << "getrobo" << endl;
    string line = "";
	char lastCH;
	int line_number = 0;
	int line_length = 0;
    vector<tuple<int,int>> answer;

    while(getline(file, line)) 
    {
		lastCH = line[line.length() - 1];
		if(strcmp(&lastCH, "\r")) 
			line_length = line.length() - 1;
		else
			line_length = line.length();
		if (line == "\n" || line == "\r" || line.empty()) {
			break;
		}
        if (line_length == 1){
            *numStages = stoi(line);
        }

        if(line_length >= 3) 
        {
			line_number++;
			cout << line << endl;
            int whitespace = line.find(" ");
            int length = line_length;

            int costs = getSubstring(line, 0, whitespace);
			cout << "costs: " << costs << endl;
            int stage = getSubstring(line, whitespace + 1, length - whitespace - 1);
			cout << "stage: " << stage << endl;
            
            answer.emplace_back(costs,stage);
			 cout << "vector<0>: " << get<0>(answer[line_number-1]) << endl;
			 cout << "vector<1>: " << get<1>(answer[line_number-1]) << endl;

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
	// ofstream output_file;
	// output_file.open("output.txt");
	// if (output_file.is_open()) {
		for(int i = 0; i < robots; ++i){
			while (getline(input_file, line)) {
				if (line == "\n" || line == "\r" || line.empty()) {
					continue;
				} else {
					break;
				}
			}
			// output_file << i << endl;
			cout << "start: " << line << endl;

			if (omni.compare(line)==0 || line == "omnidroid\r") {
				//call omnidroid funct
				cout << "omni" << endl;
				totalSprockets = Omnidroid(input_file);
				cout << totalSprockets << endl;
				// output_file << "omni: " << totalSprockets << endl;
			} else if(robo.compare(line)==0 || line == "robotomaton\r") {
				//call robotomaton funct
				cout << "robo" << endl;
				totalSprockets = robotomatonWrapper(input_file);
				cout << totalSprockets << endl;
				// output_file << "robo: " << totalSprockets << endl;
				}
			}
	// 	output_file.close();
  	// } else {
	// 	cout << "Unable to open file";
	// 	return 1;
	// }
  	
	

	input_file.close();

	return 0;
}
