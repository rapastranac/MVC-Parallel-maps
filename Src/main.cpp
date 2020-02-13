#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <vector>

#include "ParBranchHandler.h"
#include "GraphHandler.h"
#include "main.h"
#include "util.h"

using namespace std;

ParBranchHandler* ParBranchHandler::INSTANCE = nullptr;
std::once_flag ParBranchHandler::initInstanceFlag;

size_t GraphHandler::currentMVCSize = 0;

size_t GraphHandler::leaves = 0;
size_t GraphHandler::maxDepth = 30;
size_t GraphHandler::measured_Depth = 0;


int main()
{
	ParBranchHandler::Instance().SetMaxThreads(11);

	vector<size_t> Visited;		//vertices visited by the algorithm (Minimum vertex cover)
	vector<size_t> VCMin;
	GraphHandler Gr;

	Gr.readGraph("List - 80.txt", "Input/"); //it depends where the Makefile is located

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	try {
		VCMin = MVC(0, Gr, Visited, 0);    /*minimum vertex cover*/
	}
	catch (int e)
	{
		cout << "An exception occurred. Exception Nr. " << e << '\n';
	}
	cout << "DONE!" << endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	double elapsed_secs = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

	printResults(VCMin, elapsed_secs);	//Printing the minimum vertex cover

	return 0;
}

void printResults(std::vector<size_t>& VCMin, double elapsed_secs)
{
	size_t sizemvc = VCMin.size();
	printf("---------------------------------------------------------- \n");
	printf("Minimum vertex cover is: ");
	for (size_t i = 0; i < VCMin.size(); i++) cout << VCMin[i] << "\t";
	printf("\n");
	printf("Size: %zu \n", sizemvc);
	printf("Elapsed time : %f \n", elapsed_secs);
	printf("Number of leaves : %zu \n", GraphHandler::leaves);
	printf("Maximum depth reached : %zu \n", GraphHandler::measured_Depth);
	printf("---------------------------------------------------------- \n");
	//system("pause");
}
