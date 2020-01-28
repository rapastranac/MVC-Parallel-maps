#include"main.h"

std::vector<int> MVC(int id, ParGreedy & graph, std::vector<int> &visited, int depth) {
	size_t sze = graph.getGraphSize(); //Size of adjacency list, if sze = 0; then there is no more edges in graph
	static int dmdm;

	if (sze == 0) {
		mtx.lock();
		if (leaves == 0) {
			MVCSize = visited.size();
			leaves++;
			mtx.unlock();
			return visited;	/*Terminal case*/
		}
		else {
			if (visited.size() < MVCSize)
			{
				MVCSize = visited.size();
			}
			leaves++;
			mtx.unlock();
			return visited;	/*Terminal case*/
		}
	}
	//std::cout<<"Leaf # "<<leaves<<endl;
	//std::cout<<"Size of graph is "<<sze<<" MVC size so far is "<<MVCSize<<endl;
	//-------------
	vector<int> VC1;
	vector<int> VC2;
	vector<int> C1 = visited;
	vector<int> C2 = visited;
	ParGreedy gLeft = graph;	/*Let gLeft be a copy of graph*/
	ParGreedy gRight = graph; // graph;	/*Let gRight be a copy of graph*/
	bool gotIn = false;

	std::future<std::vector<int>> C1_tmp;

	int v = gLeft.getRandomVertex();

	C1.push_back(v);
	gLeft.removeVertex(v);
	if (C1.size() >= MVCSize) {
		VC1 = {};
	}
	else {
		gLeft.removeVertex(v);
		mtx.lock();
		if (numThreads < maxThreads) {
			numThreads++;
			gotIn = true;
			mtx.unlock();
			C1_tmp = pool.push(MVC, ref(gLeft), ref(C1), ref(depth));
		}
		else {
			mtx.unlock();
			VC1 = MVC(0, gLeft, C1, depth++);
		}
	}

	gRight.removeNeiboursVertex(v, C2);

	if (C2.size() >= MVCSize)
	{
		VC2 = {};
	}
	else {
		VC2 = MVC(0, gRight, C2, depth++);
	}


	if (gotIn) {
		VC1 = C1_tmp.get();
	}
	std::cin>>dmdm;
	mtx.lock();
	if (gotIn) {
		numThreads--;
		mtx.unlock();
	}
	else{
		mtx.unlock();
	}
	printf("Size of graph is %d MVC size so far is %d \n",sze,MVCSize);
	if (VC1.empty())	return VC2;
	else if (VC2.empty())	return VC1;
	else if (VC1.empty() && VC2.empty())	return {};
	else if (VC1.size() >= VC2.size())	return VC2;
	else return VC1;
}
