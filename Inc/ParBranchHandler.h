#ifndef PARBRANCHHANDLER_H
#define PARBRANCHHANDLER_H

#include <future>
#include <mutex>

#include "ctpl_stl.h"

class ParBranchHandler
{
private:
	std::future<std::vector<size_t>> ftr;
	std::vector<size_t> VCHolding; //How to add template?
	bool gotIn;

protected:

public:

	static size_t maxThreads;	//Number of threads set by the user
	static size_t numThreads;	//Number of busy threads
	static std::mutex mtx;
	static ctpl::thread_pool pool;

	//Default constructor
	ParBranchHandler() {
		gotIn = false;
	}
	
	template <typename F, typename... Args>
	void makeBranchingCall(F&& functionToCall, Args&&... arguments) {
		//ftr = functionToCall(arguments...);
		gotIn = false;

		mtx.lock();
		if (numThreads < maxThreads) {
			numThreads++;
			gotIn = true;
			mtx.unlock();
			/* ftr = pool.push(functionToCall, arguments...); /*Need to ignore
						the very first element of arguments*/
			this->ftr = std::async(std::launch::async,
				functionToCall, arguments...);	//Not a thread pool
			//std::vector<size_t> valor = ftr.get();
		}
		else {
			mtx.unlock();
			this->VCHolding = functionToCall(arguments...);
			//return functionToCall(arguments...);
		}

		//return functionToCall(arguments...);
	}

	template <typename T>
	void getBranchingResult(T &VC1) {
		if (gotIn) {
			VC1 = ftr.get();
		}
		else
		{
			VC1 = VCHolding;
		}
		mtx.lock();
		if (gotIn) {
			numThreads--;
			mtx.unlock();
		}
		else {
			mtx.unlock();
		}
	}

	//Destructor
	~ParBranchHandler() {

	}



};

#endif