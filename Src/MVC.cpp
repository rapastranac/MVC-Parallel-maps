#include"main.h"

std::vector<size_t> MVC(size_t id, GraphHandler& graph, std::vector<size_t>& Visited, size_t depth) {
	size_t sze = graph.getGraphSize(); //Size of adjacency list, if sze = 0; then there is no more edges in graph

	if (sze == 0) {
		ParBranchHandler::Instance()->Lock();
		if (graph.leaves == 0) {
			graph.currentMVCSize = Visited.size();
			graph.leaves++;
			ParBranchHandler::Instance()->Unlock();
			return Visited;	//Terminal case
		}
		else {
			if (Visited.size() < graph.currentMVCSize)
			{
				printf("MVC found so far has %u elements\n", graph.currentMVCSize);
				graph.currentMVCSize = Visited.size();
			}
			graph.leaves++;
			if (depth > graph.measured_Depth)
			{
				graph.measured_Depth = depth;
			}
			ParBranchHandler::Instance()->Unlock();
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

	size_t v = gLeft.getRandomVertex();

	int branchingCallId = -1;
	if (C1.size() >= graph.currentMVCSize) {
		VC1 = {};
	}
	else {
		C1.push_back(v);
		gLeft.removeVertex(v);
		gLeft.removeZeroVertexDegree();
		branchingCallId = ParBranchHandler::Instance()->makeBranchingCall(MVC, 0, ref(gLeft), ref(C1), ref(++depth));
	}

	gRight.removeNeiboursVertex(v, C2);
	gRight.removeZeroVertexDegree();

	if (C2.size() >= graph.currentMVCSize)
	{
		VC2 = {};
	}
	else {
		VC2 = MVC(0, gRight, C2, ++depth);
	}

	ParBranchHandler::Instance()->getBranchingResult(branchingCallId, VC1);

	return returnFunction(VC1, VC2);
}

const std::vector<size_t>& returnFunction(std::vector<size_t>& VC1, std::vector<size_t>& VC2)
{
	if (VC1.empty())	return VC2;
	else if (VC2.empty())	return VC1;
	else if (VC1.empty() && VC2.empty())	return {};
	else if (VC1.size() >= VC2.size())	return VC2;
	else return VC1;
}
