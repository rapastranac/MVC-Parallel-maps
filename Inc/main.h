#ifndef MAIN_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define MAIN_H
#include <mutex>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <thread>

#include "ctpl_stl.h"
#include "ParGreedy.h"
#include "util.h"

extern size_t maxThreads;
extern size_t maxDepth;
extern size_t measured_Depth;
extern size_t MVCSize;
extern ctpl::thread_pool pool;
extern std::mutex mtx;
extern size_t leaves;
extern size_t numThreads;
std::vector<int> MVC(int, ParGreedy&, std::vector<int>&, int);

void printResults(std::vector<int>&, double);
#endif