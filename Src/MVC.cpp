#include"main.h"

std::vector<size_t> MVC(size_t id, GraphHandler & graph, std::vector<size_t> &Visited, size_t depth) {
	size_t sze = graph.getGraphSize(); //Size of adjacency list, if sze = 0; then there is no more edges in graph
	
	if (sze == 0) {
		graph.mtx.lock();
		if (leaves == 0) {
			MVCSize = Visited.size();
			leaves++;
			graph.mtx.unlock();
			return Visited;	/*Terminal case*/
		}
		else {
			if (Visited.size() < MVCSize)
			{
				printf("%i \n", MVCSize);
				MVCSize = Visited.size();
			}
			leaves++;
			graph.mtx.unlock();
			return Visited;	/*Terminal case*/
		}
	}
	//-------------
	vector<size_t> VC1;
	vector<size_t> VC2;
	vector<size_t> C1 = Visited;
	vector<size_t> C2 = Visited;
	GraphHandler gLeft = graph;	/*Let gLeft be a copy of graph*/
	GraphHandler gRight = graph; // graph;	/*Let gRight be a copy of graph*/
	bool gotIn = false;

	std::future<std::vector<int>> C1_tmp;
	
	size_t v = gLeft.getRandomVertex();

	if (C1.size() >= MVCSize) {
		VC1 = {};
	}
	else {
		C1.push_back(v);
		gLeft.removeVertex(v);
		gLeft.removeZeroVertexDegree();
		gLeft.makeBranchingCall(MVC, 0, ref(gLeft), ref(C1), ref(++depth));
		//graph.mtx.lock();
		//if (graph.numThreads < graph.maxThreads) {
		//	gLeft.numThreads++;
		//	gotIn = true;
		//	graph.mtx.unlock();
		//	//C1_tmp = pool.push(MVC, ref(gLeft), ref(C1), ref(depth));
		//	C1_tmp = graph.pool.push(MVC, ref(gLeft), ref(C1), ref(depth));
		//}
		//else {
		//	graph.mtx.unlock();
		//	//VC1 = gLeft.makeBranchingCall(MVC, 0, gLeft, C1, ++depth);
		//	gLeft.makeBranchingCall(MVC, 0, gLeft, C1, ++depth);
		//	//VC1 = MVC(0, gLeft, C1, ++depth);
		//}
	}

	gRight.removeNeiboursVertex(v, C2);
	gRight.removeZeroVertexDegree();

	if (C2.size() >= MVCSize)
	{
		VC2 = {};
	}
	else {
		VC2 = MVC(0, gRight, C2, ++depth);
	}

	gLeft.getBranchingResult(VC1);

	//if (gotIn) {
	//	VC1 = graph.getBranchingResult();
	//	//VC1 = C1_tmp.get();
	//}
	//graph.mtx.lock();
	//if (gotIn) {
	//	graph.numThreads--;
	//	graph.mtx.unlock();
	//}
	//else{
	//	graph.mtx.unlock();
	//}

	if (VC1.empty())	return VC2;
	else if (VC2.empty())	return VC1;
	else if (VC1.empty() && VC2.empty())	return {};
	else if (VC1.size() >= VC2.size())	return VC2;
	else return VC1;
}
