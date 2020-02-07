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

ParBranchHandler* ParBranchHandler::instance = nullptr;

size_t GraphHandler::currentMVCSize = 0;

size_t GraphHandler::leaves=0;
size_t GraphHandler::maxDepth = 20;
size_t GraphHandler::measured_Depth = 0;


int main()
{
	ParBranchHandler::Instance()->SetMaxThreads(30);

	vector<size_t> Visited;		//vertices visited by the algorithm (Minimum vertex cover)
	vector<size_t> VCMin;
	GraphHandler Gr;

	Gr.readGraph("List - 30.txt", "Input/"); //it depends where the Makefile is located

	clock_t begin = clock();
	//VCMin = Gr.makeBranchingCall(MVC, 0, Gr, Visited, 0);
	VCMin = MVC(0, Gr, Visited, 0);	/*minimum vertex cover*/
	clock_t end = clock();
	double elapsed_secs = ((double)end - (double)begin) / CLOCKS_PER_SEC;

	printResults(VCMin, elapsed_secs, Gr);	//Printing the minimum vertex cover

	return 0;
}

void printResults(std::vector<size_t>& VCMin, double elapsed_secs, GraphHandler& Gr)
{
	size_t sizemvc = VCMin.size();
	printf("---------------------------------------------------------- \n");
	printf("Minimum vertex cover is: ");
	for (int i = 0; i < VCMin.size(); i++) cout << VCMin[i] << "\t";
	printf("\n");
	printf("Size: %u \n", sizemvc);
	printf("Elapsed time : %f \n", elapsed_secs);
	printf("Number of leaves : %u \n", Gr.leaves);
	printf("Maximum depth reached : %u \n", Gr.measured_Depth);
	printf("---------------------------------------------------------- \n");
	system("pause");
}
