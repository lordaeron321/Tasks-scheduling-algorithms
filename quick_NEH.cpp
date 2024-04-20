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

vector<vector<int>> ActualPermutation;
std::vector<std::vector<int>> tasks = {//testing data
   {74, 21, 58, 4, 21, 28, 58, 83, 31, 61, 94, 44, 97, 94, 66, 6, 37, 22, 99, 83},
   {28, 3, 27, 61, 34, 76, 64, 87, 54, 98, 76, 41, 70, 43, 42, 79, 88, 15, 49, 72},
   {89, 52, 56, 13, 7, 32, 32, 98, 46, 60, 23, 87, 7, 36, 26, 85, 7, 34, 36, 48},
   {60, 88, 26, 58, 76, 98, 29, 47, 79, 26, 19, 48, 95, 78, 77, 90, 24, 10, 39, 74},
   {54, 66, 12, 57, 70, 82, 99, 84, 16, 41, 23, 11, 68, 58, 30, 5, 5, 39, 74, 91},
   {92, 11, 54, 97, 57, 53, 65, 77, 51, 36, 53, 19, 54, 86, 40, 56, 79, 74, 91, 3},
   {9, 8, 88, 72, 27, 22, 50, 2, 49, 82, 93, 96, 43, 13, 60, 11, 37, 91, 84, 67},
   {4, 18, 25, 28, 95, 51, 84, 18, 6, 90, 69, 61, 57, 5, 75, 4, 38, 28, 4, 80},
   {25, 15, 91, 49, 56, 10, 62, 70, 76, 99, 58, 83, 84, 64, 74, 14, 18, 48, 96, 86},
   {15, 84, 8, 30, 95, 79, 9, 91, 76, 26, 42, 66, 70, 91, 67, 3, 98, 4, 71, 62}
};
int nom = tasks.size();
int nt = tasks[0].size();
vector<pair<int,int>> sums;
sums.resize(tasks[0].size());

for (int i = 0; i < nt; i++)
{
	for (int j = 0; j < nom; j++)
	{
		sums[i].first += tasks[j][i];
	}
	sums[i].second = i;
}
sort(sums.begin(), sums.end(), comp);
ActualPermutation.resize(nom);
for (int i = 0; i < nom; i++)ActualPermutation[i].resize(1);
for (int i = 0; i < nom; i++)ActualPermutation[i][0] = tasks[i][sums.back().second];
sums.pop_back();
//mamy juz peirwsze zadanie ulozeniu

for (int i = 0; i < nt-1; i++)
{
	vector<int> ActuallyConsideringTask;
	for (int i = 0; i < nom; i++)ActuallyConsideringTask.push_back(tasks[i][sums.back().second]);

	vector<vector<pair<int, int>>> ActualPermutationArray = NehArray(ActualPermutation);
	pair<int, int> IndexOfMinimalizingPosition = quickNehCmax(ActualPermutationArray,ActuallyConsideringTask);
	//podmianka zadan
	vector<vector<int>> CopyOfArrayAfterIndex;
	CopyOfArrayAfterIndex.clear();
	CopyOfArrayAfterIndex.resize(nom);
	for (int j = 0; j < nom; j++)CopyOfArrayAfterIndex[j].resize(ActualPermutation[0].size());
	//kopiowanie czesci po indeksie
	for (int rows = 0; rows < nom ; rows++)
	{
		for (int cols = IndexOfMinimalizingPosition.second; cols < ActualPermutation[0].size(); cols++)
		{
			if(IndexOfMinimalizingPosition.second < ActualPermutation[0].size()){
				CopyOfArrayAfterIndex[rows][cols - IndexOfMinimalizingPosition.second] = ActualPermutation[rows][cols];
				}
		}	
	}////koniec kopiowania czesci po indeksie
	for (int j = 0; j < nom; j++)ActualPermutation[j].resize(i + 2);

	for (int rows = 0; rows < nom; rows++)
	{
		for (int cols = 0; cols < ActualPermutation[0].size(); cols++)
		{
			if (cols == IndexOfMinimalizingPosition.second) {
				ActualPermutation[rows][cols] = ActuallyConsideringTask[rows];
			}
			else if (cols > IndexOfMinimalizingPosition.second) {
				ActualPermutation[rows][cols] = CopyOfArrayAfterIndex[rows][cols- IndexOfMinimalizingPosition.second-1];
			}
		}
	}
	sums.pop_back();
}
//reverse engineering
bool isInColumn = true;
cout << "order: ";
for (int i = 0; i < tasks[0].size(); i++)
{
		for (int k = 0; k < ActualPermutation[0].size(); k++)
		{
			for (int j = 0; j < tasks.size(); j++)
			{
				if (tasks[j][i] != ActualPermutation[j][k])isInColumn = false;
			}
			if (isInColumn == true)cout << k+1 << " ";
			isInColumn = true;
		}
}

cout << endl;

int m = ActualPermutation.size();
int n = ActualPermutation[0].size();

vector<vector<int>> InOutGraphLength;
InOutGraphLength.resize(m);
for (int i = 0; i < InOutGraphLength.size(); i++) {
	InOutGraphLength[i].resize(ActualPermutation[i].size());
}

InOutGraphLength[0][0] = ActualPermutation[0][0];


for (int i = 1; i < n; i++) {
	InOutGraphLength[0][i] = ActualPermutation[0][i] + InOutGraphLength[0][i - 1];
}
for (int i = 1; i < m; i++) {
	InOutGraphLength[i][0]= ActualPermutation[i][0] + InOutGraphLength[i - 1][0];
}
for (int i = 1; i < m; i++) {
	for (int j = 1; j < n; j++) {
		InOutGraphLength[i][j] = ActualPermutation[i][j] + max(InOutGraphLength[i - 1][j], InOutGraphLength[i][j - 1]);
	}
}

cout<<"length: " << InOutGraphLength[m - 1][n - 1];

	return 0;
}

