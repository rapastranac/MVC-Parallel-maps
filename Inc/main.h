#ifndef MAIN_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define MAIN_H
#include <mutex>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <thread>

#include "ctpl_stl.h"
#include "GraphHandler.h"
#include "ParBranchHandler.h"
#include "util.h"

extern size_t maxDepth;
extern size_t measured_Depth;
extern size_t MVCSize;
extern size_t leaves;

std::vector<size_t> MVC(size_t, GraphHandler&, std::vector<size_t>&, size_t);

void printResults(std::vector<size_t>&, double);
#endif