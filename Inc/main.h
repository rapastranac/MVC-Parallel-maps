#ifndef MAIN_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define MAIN_H
#include <mutex>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <thread>

#include "ctpl_stl.h"
#include "ParGreedy.h"
#include "util.h"

extern int maxThreads;
extern int maxDepth;
extern int measured_Depth;
extern size_t MVCSize;
extern ctpl::thread_pool pool;
extern std::mutex mtx;
extern long long int leaves;
extern int numThreads;
std::vector<int> MVC(int id, ParGreedy &g, std::vector<int> &Visited, int depth);

void printResults(std::vector<int>& VCMin, double elapsed_secs);
#endif