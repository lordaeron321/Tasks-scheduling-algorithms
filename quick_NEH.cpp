#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;
bool comp(pair<int, int>& first, pair<int, int>& second) {
	return first.first < second.first;
}
vector<vector<pair<int, int>>> NehArray(vector<vector<int>> tasks) {
	int m = tasks.size();
	int n = tasks[0].size();

	vector<vector<pair<int, int>>> InOutGraphLength;
	InOutGraphLength.resize(m);
	for (int i = 0; i < InOutGraphLength.size(); i++){
		InOutGraphLength[i].resize(tasks[i].size());
	}

	InOutGraphLength[0][0].first = tasks[0][0];
	InOutGraphLength[m - 1][n - 1].second = tasks[m - 1][n - 1];


	for (int i = 1; i < n; i++){
		InOutGraphLength[0][i].first = tasks[0][i] + InOutGraphLength[0][i - 1].first;
		InOutGraphLength[m-1][n-i-1].second = tasks[m - 1][n - i - 1] + InOutGraphLength[m - 1][n - i].second;
	}
	for (int i = 1; i < m; i++) {
		InOutGraphLength[i][0].first = tasks[i][0] + InOutGraphLength[i - 1][0].first;
		InOutGraphLength[m - i - 1][n - 1].second = tasks[m - i - 1][n - 1] + InOutGraphLength[m - i][n - 1].second;
	}
	for (int i = 1; i < m; i++){
		for (int j = 1; j < n; j++){
			InOutGraphLength[i][j].first = tasks[i][j] + max(InOutGraphLength[i - 1][j].first, InOutGraphLength[i][j - 1].first);
			InOutGraphLength[m - i - 1][n - j - 1].second = tasks[m - i - 1][n - j - 1] + max(InOutGraphLength[m - i][n - j - 1].second,
				InOutGraphLength[m - i - 1][n - j].second);
		}
	}
	return InOutGraphLength;
}

pair<int, int> quickNehCmax(vector<vector<pair<int,int>>> array, vector<int> task) {
	vector<int> InjectedTask;
	
	int BestTaskInjectionIndex = 0;
	int Cmin = INT_MAX, Cmax = 0;
	
	int m = array.size();
	int n = array[0].size();
	InjectedTask.resize(m);
	
	for (int i = 0; i < m; i++)Cmax = max(Cmax, task[i] + array[i][0].second);
	Cmin = Cmax;
	Cmax = 0;
	
	for (int i = 0; i < m; i++)Cmax = max(Cmax, task[i] + array[i][n-1].first);
	Cmin = min(Cmin,Cmax);
	if (Cmin == Cmax)BestTaskInjectionIndex = n;
	int i = 2;
	for (int i = 1; i < n; i++){
		Cmax = 0;
		InjectedTask[0] = array[0][i - 1].first + task[0];
		for (int j = 1; j < m; j++)InjectedTask[j] = max(InjectedTask[j - 1], array[j][i - 1].first) + task[j];
		for (int j = 0; j < m; j++)Cmax = max(Cmax,InjectedTask[j]+array[j][i].second);

		Cmin = min(Cmin, Cmax);
		if (Cmin == Cmax)BestTaskInjectionIndex = i;
	}

	pair<int, int> solution;
	solution.first = Cmin;
	solution.second = BestTaskInjectionIndex;

	return solution;
}
int main()
{
	vector<vector<int>> FinalPermutation;
	vector<vector<int>> tasks = {
		{1,4,3,2},
		{1,1,4,4},
		{3,2,3,1},
	};

	vector<pair<int,int>> sums;
	sums.resize(tasks[0].size());
	////

	return 0;
}

