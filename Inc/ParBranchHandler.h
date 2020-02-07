#ifndef PARBRANCHHANDLER_H
#define PARBRANCHHANDLER_H

#include <future>
#include <mutex>

#include "ctpl_stl.h"

struct FutureInfo
{
	std::future<std::vector<size_t>> ftr;
	std::vector<size_t> VCHolding;
	bool gotIn;
};

class ParBranchHandler
{
private:

	std::vector<FutureInfo> ftrInfos;
	/*std::vector<std::future<std::vector<size_t>>> currentFutures;
	//std::future<std::vector<size_t>> ftr;
	std::vector<size_t> VCHolding; //How to add template?
	bool gotIn;*/
	static ParBranchHandler* instance;

	size_t numThreads;	//Number of busy threads
	std::mutex mtx;
	ctpl::thread_pool pool;


	ParBranchHandler()
	{
		numThreads = 0;
		//pool.resize(1);
	}

protected:

public:


	static ParBranchHandler* Instance() //how to access the global ParBranchHandler
	{
		if (!instance)	//thread safe ?
			instance = new ParBranchHandler();

		return instance;
	}



	size_t GetMaxThreads()		//ParBranchHandler::getInstance()->MaxThreads() = 10;
	{
		return pool.size();
	}

	void SetMaxThreads(size_t n)
	{
		pool.resize(n);
	}


	void Lock()
	{
		this->mtx.lock();
	}


	void Unlock()
	{
		this->mtx.unlock();	//There is a warning, caller failing to hold lock "this->mtx"
	}

	//Default constructor
	/*ParBranchHandler() {
		gotIn = false;
	}*/

	template <typename F, typename... Args>
	int makeBranchingCall(F&& functionToCall, size_t id, Args&&... arguments) {

		FutureInfo ftrInfoLocal;
		ftrInfoLocal.gotIn = false;
		//ftrInfos[ftrInfos.size() - 1].gotIn = false;

		std::unique_lock<std::mutex> lock(this->mtx);
		//mtx.lock();
		int rId = ftrInfos.size();
		ftrInfos.resize(ftrInfos.size() + 1);

		if (numThreads < pool.size()) {
			numThreads++;
			ftrInfoLocal.gotIn = true;
			printf("GotIn at id %d \n", rId);
			//gotIn = true;
			//mtx.unlock();
			//ftrInfos[ftrInfos.size() - 1].ftr = pool.push(functionToCall, arguments...);
			ftrInfoLocal.ftr = pool.push(functionToCall, arguments...);
						//this->ftr = std::async(std::launch::async,
						//	functionToCall, arguments...);	//Not a thread pool
						//std::vector<size_t> valor = ftr.get();
		}
		else {
			//mtx.unlock();
			lock.unlock();
			ftrInfoLocal.VCHolding = functionToCall(id, arguments...);

			/*previous function works before pushing into ftrInfos
				sequential code
			*/
			//this->VCHolding = functionToCall(id, arguments...);
			//return functionToCall(arguments...);
		}

		ftrInfos[rId] = std::move(ftrInfoLocal);
		//ftrInfos.push_back(std::move(ftrInfoLocal)); //This line, should fix thread access

		/*Control access over id in ftrInfos, make it thread safe
			it also applies for two conditions: first time access and after*/


		return rId;
	}

	template <typename T>
	void getBranchingResult(int id, T& VC1) {

		if (id < 0)
			return;

		FutureInfo& ftrInfo = ftrInfos[id];
		if (ftrInfo.gotIn) {
			VC1 = ftrInfo.ftr.get();
		}
		else
		{
			VC1 = ftrInfo.VCHolding;
		}

		if (ftrInfo.gotIn)
		{
			std::unique_lock<std::mutex> lock(this->mtx);
			numThreads--;
		}
	}

	//Destructor
	~ParBranchHandler() {

	}

};


#endif
