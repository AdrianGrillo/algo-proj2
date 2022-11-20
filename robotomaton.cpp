#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

int getSubstring(string line, int start, int end) 
{
	return stoi(line.substr(start, end));
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

//Function initializes 1d vector for memoizaztion and calls the contructRobotomaton function
int robotomatonWrapper(int numStages, vector<tuple <int, int> > assembly){
    vector<int> costs(numStages);

	for(int i = 0; i < numStages; i++){
        cout << get<0>(assembly[i]) << ' '<< get<1>(assembly[i]) << endl;
		costs[i] = 0;
	}
	return constructRobotomaton(numStages, assembly, costs);

}


vector<tuple<int, int>> getRobotomatonParts(ifstream& file, int* numStages)
{
    string line = "";
    vector<tuple<int,int>> answer;

    while(getline(file, line)) 
    {
        if (line.length() == 1){
            *numStages = stoi(line);
        }

        if(line.length() >= 3 && line != "omnidroid" && line != "robotomaton") 
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

int main(){

    int numStages = 0;

    ifstream input_file;
    input_file.open("input.txt");

    if(!input_file) 
    {
        cout << "Unable to open input file" << endl;
        return 1;
    }

    vector<tuple<int,int>> test = getRobotomatonParts(input_file, &numStages);

    int answer = 0;
    answer = robotomatonWrapper(numStages, test);
    cout << answer;

    return answer;
}