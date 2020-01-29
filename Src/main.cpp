#include <ctime>
#include<iostream> 
#include <list> 
#include <vector>

#include "ParGreedy.h"
#include "main.h"
#include "util.h"

using namespace std;
// This struct represents a undirected graph using adjacency list 
size_t maxThreads = 11;
ctpl::thread_pool pool(maxThreads);
size_t leaves;
size_t numThreads;	
std::mutex mtx;
size_t maxDepth = 20;
size_t measured_Depth = 0;
size_t MVCSize = 30;

int main()
{
	vector<int> Visited;		//vertices visited by the algorithm (Minimum vertex cover)
	vector<int> VCMin;
	ParGreedy Gr(30);
	Gr.readGraph("List.txt");

	leaves = 0;
	numThreads = 0;

	clock_t begin = clock();
	VCMin = MVC(0, Gr, Visited, 0);	/*minimum vertex cover*/
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	printResults(VCMin, elapsed_secs);	//Printing the minimum vertex cover

	return 0;
}

void printResults(std::vector<int>& VCMin, double elapsed_secs)
{
	int sizemvc = VCMin.size();
	printf("---------------------------------------------------------- \n");
	printf("Minimum vertex cover is: ");
	for (int i = 0; i < VCMin.size(); i++) cout << VCMin[i] << "\t";
	printf("\n");
	printf("Size: %i \n", sizemvc);
	printf("Elapsed time : %f \n", elapsed_secs);
	printf("Number of leaves : %i \n", leaves);
	printf("Maximum depth reached : %i \n", measured_Depth);
	printf("---------------------------------------------------------- \n");
	system("pause");
}