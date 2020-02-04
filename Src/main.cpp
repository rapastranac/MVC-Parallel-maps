#include <ctime>
#include<iostream>
#include <list>
#include <mutex>
#include <vector>

#include "ParBranchHandler.h"
#include "GraphHandler.h"
#include "main.h"
#include "util.h"

using namespace std;

//Declaring static class member
size_t ParBranchHandler::maxThreads = 11;
size_t ParBranchHandler::numThreads = 0;
std::mutex ParBranchHandler::mtx;
ctpl::thread_pool ParBranchHandler::pool(11);

//Creates here a global object Par Branch Handler type


//ctpl::thread_pool pool(5);
size_t leaves;

size_t maxDepth = 20;
size_t measured_Depth = 0;
size_t MVCSize = 80;


int main()
{
	vector<size_t> Visited;		//vertices visited by the algorithm (Minimum vertex cover)
	vector<size_t> VCMin;
	GraphHandler Gr;
	//size_t GraphHandler::leaves
	Gr.readGraph("List - 80.txt","Input/"); //it depends where the Makefile is located


	leaves = 0;
	
	
	clock_t begin = clock();
	//VCMin = Gr.makeBranchingCall(MVC, 0, Gr, Visited, 0);
	VCMin = MVC(0, Gr, Visited, 0);	/*minimum vertex cover*/
	clock_t end = clock();
	double elapsed_secs = ((double)end - (double)begin) / CLOCKS_PER_SEC;

	printResults(VCMin, elapsed_secs);	//Printing the minimum vertex cover

	return 0;
}

void printResults(std::vector<size_t>& VCMin, double elapsed_secs)
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
