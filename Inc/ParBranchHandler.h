#ifndef PARBRANCHHANDLER_H
#define PARBRANCHHANDLER_H

#include <future>
#include <iostream>
#include <mutex>

#include "ctpl_stl.h"
#include "util.h"

class ParBranchHandler
{
private:
	int numThreads;	//Number of busy threads
	std::mutex mtx;
	ctpl::thread_pool pool;

	static ParBranchHandler* INSTANCE;
	static std::once_flag initInstanceFlag;


	ParBranchHandler()
	{
		numThreads = 0;
		SetMaxThreads(numThreads);
	}
	~ParBranchHandler() = default;
	ParBranchHandler(const ParBranchHandler&) = delete;
	ParBranchHandler& operator=(const ParBranchHandler&) = delete;
	static void initSingleton()
	{
		INSTANCE = new ParBranchHandler();
	}

public:

	static ParBranchHandler& Instance() //how to access the global ParBranchHandler
	{
		std::call_once(initInstanceFlag, &ParBranchHandler::initSingleton);
		return *INSTANCE;
	}

	size_t GetMaxThreads()		//ParBranchHandler::getInstance()->MaxThreads() = 10;
	{
		return pool.size();
	}

	void SetMaxThreads(size_t n)
	{
		pool.resize(n);
	}

	void DecreaseNumThreads()
	{
		numThreads--;
	}

	void Lock()
	{
		mtx.lock();
	}

	void Unlock()
	{
		mtx.unlock();	//There is a warning, caller failing to hold lock "this->mtx"
	}

	class FutureHolder
	{
		friend class ParBranchHandler;
	private:
		std::future<std::vector<size_t>> ftr;
		std::vector<size_t> VCHolding;
		bool isAThreadPicked;
	public:
		FutureHolder() {
			isAThreadPicked = false;
		}

		vector<size_t> getResult()
		{
			if (isAThreadPicked)
			{
				VCHolding = ftr.get();
				ParBranchHandler::Instance().DecreaseNumThreads();
			}
			return VCHolding;
		}
	};

	template <typename F, typename... Args>
	void makeBranchingCall(FutureHolder& resultHolder, F&& functionToCall, int id, Args&&... arguments) {

		//std::cout<<"BRANCHING"<<std::endl;
		Lock();
		if (numThreads < pool.size()) {
			numThreads++;
			Unlock();
			resultHolder.isAThreadPicked = true;
			resultHolder.ftr = std::move(pool.push(functionToCall, arguments...));
		}
		else {
			Unlock();
			resultHolder.VCHolding = functionToCall(id, arguments...);
		}

	}

};

#endif
