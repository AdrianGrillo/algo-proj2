#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

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


int main(){


    vector<tuple<int,int>> test;

    test.emplace_back(3,0);

    test.emplace_back(11,0);

    test.emplace_back(31,1);

    test.emplace_back(29,2);

    test.emplace_back(5,4);


    int answer = 0;
    answer = robotomatonWrapper(5, test);
    cout << answer;

    return answer;
}