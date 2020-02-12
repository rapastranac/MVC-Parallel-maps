#include "main.h"

std::vector<size_t> MVC(int id, GraphHandler& graph, std::vector<size_t>& Visited, size_t depth) {
	size_t sze = graph.getGraphSize(); //Size of adjacency list, if sze = 0; then there is no more edges in graph
	if (sze == 0) {

		ParBranchHandler::Instance().Lock();
		if (GraphHandler::leaves == 0) {
			GraphHandler::currentMVCSize = Visited.size();
			GraphHandler::leaves++;
			ParBranchHandler::Instance().Unlock();
			return Visited;	//Terminal case
		}
		else {
			if (Visited.size() < GraphHandler::currentMVCSize)
			{
				printf("MVC found so far has %u elements\n", (int)GraphHandler::currentMVCSize);
				GraphHandler::currentMVCSize = Visited.size();
			}
			GraphHandler::leaves++;
			if (depth > GraphHandler::measured_Depth)
			{
				GraphHandler::measured_Depth = depth;
			}
			ParBranchHandler::Instance().Unlock();
			return Visited;	//Terminal case
		}
	}
	//-------------
	vector<size_t> VC1;
	vector<size_t> VC2;
	vector<size_t> C1 = Visited;
	vector<size_t> C2 = Visited;
	GraphHandler gLeft = graph;	/*Let gLeft be a copy of graph*/
	GraphHandler gRight = graph; // graph;	/*Let gRight be a copy of graph*/
	size_t newDepth = depth + 1;


	size_t v = gLeft.getRandomVertex();

	ParBranchHandler::FutureHolder resultHolder;

	if (C1.size() < GraphHandler::currentMVCSize)
	{
		C1.push_back(v);
		gLeft.removeVertex(v);
		gLeft.removeZeroVertexDegree();
		ParBranchHandler::Instance().makeBranchingCall(resultHolder, MVC, id, ref(gLeft), ref(C1), ref(newDepth));
	}

	gRight.removeNeiboursVertex(v, C2);
	gRight.removeZeroVertexDegree();

	if (C2.size() < GraphHandler::currentMVCSize)
	{
		VC2 = MVC(id, gRight, C2, newDepth);
	}

	VC1 = resultHolder.getResult();

	vector<size_t> returnVal = returnFunction(VC1, VC2);
	// printf("vx %d vc1 %d vc2 %d\n", vx.size(), VC1.size(), VC2.size());
	return returnVal;
}

const std::vector<size_t>& returnFunction(std::vector<size_t>& VC1, std::vector<size_t>& VC2)
{
	if (!VC1.empty() && !VC2.empty())
	{
		if (VC1.size() >= VC2.size())
		{
			return VC2;
		}
		else
		{
			return VC1;
		}
	}
	else if (!VC1.empty() && VC2.empty())
	{
		return VC1;
	}
	else if (VC1.empty() && !VC2.empty())
	{
		return VC2;
	}
	else {
		return VC1; //At this point VC1 or VC2 are empty, here empty vector is returned
	}
}
