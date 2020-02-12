#ifndef GRAPHHANDLER_H
#define GRAPHHANDLER_H

using namespace std;

#include <iostream>
#include <map>
#include <set>
#include <stdlib.h>     /* srand, rand */
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "ParBranchHandler.h"
#include "util.h"

class GraphHandler
{
private:
	size_t max;					//Highest degree within graph
	vector<size_t> maxDegreePositionInList;	//Stores the positions of max degree vertices within the adjacency list
	map<size_t, set<size_t>> list;	//Adjacency list
	set<size_t> rows;				//Temporary variable to store
	map<size_t, size_t> vertexDegree;	//list of vertices with their corresponding number of edges
	set<size_t> zeroVertexDegree;	//List of vertices with zero degree

private:
	void addRowToList(size_t vec0) {
		//list[vec0] = rows;
		list.insert(pair<size_t, set<size_t>>(vec0,rows));
		rows.clear();
	}

	void calculerVertexMaxDegree() {
		size_t tmp;
		/*Finding vertex degrees, in order to start exploring by these ones.*/
		for (size_t i = 0; i < list.size(); i++)
		{
			tmp = list[i].size();
			this->vertexDegree[i] = tmp;
			if (tmp > max)
			{
				this->max = tmp;
			}
		}
		for (size_t i = 0; i < list.size(); i++)
		{
			if (vertexDegree[i] == max) {
				this->maxDegreePositionInList.push_back(i);
			}
		}
	}

	void updateVertexDegree()
	{
		//Recalculating the vertex with maximum number of edges
		size_t tmp = 0;
		for (auto const& i : vertexDegree)
		{
			if (i.second > tmp) {
				tmp = i.second;
			}
		}
		max = tmp;
		maxDegreePositionInList.clear();
		/*storing position of highest degree vertices within adjacency list*/
		for (auto const& i : vertexDegree)
		{
			if (i.second == max) maxDegreePositionInList.push_back(i.first);
		}
	}
public:

	static size_t currentMVCSize;
	static size_t leaves;
	static size_t maxDepth;
	static size_t measured_Depth;

	//Parameterized constructor
	GraphHandler()
	{
		this->max = 0;
	}

	void addNeighbour(size_t val) {
		this->rows.insert(val);
	}

	void removeZeroVertexDegree()
	{
		/*After erasing vertices, some of them might end up with zero degree,
			this function is in charge of erasing those vertices*/
		for (auto i : zeroVertexDegree)
		{
			list.erase(i);
			vertexDegree.erase(i);
		}
		this->zeroVertexDegree.clear();
	}

	void removeVertex(size_t v) {

		auto it = list[v].begin();
		/*Here we explore all the neighbours of v, and then we find
			vertex v inside of those neighbours in order to erase v of them*/
		while (it != list[v].end())
		{
			list[*it].erase(v);
			if (list[*it].size() == 0) {
				//Store temporary position of vertices that ended up with no neighbours
				this->zeroVertexDegree.insert(*it);
			}
			this->vertexDegree[*it]--;
			it++;
		}

		/*After v is been erased from its neighbours, then v is erased
			from graph and the VertexDegree is updated*/
		this->list.erase(v);
		this->vertexDegree.erase(v);
		updateVertexDegree();

	}

	void removeNeiboursVertex(size_t v, vector<size_t>& C2) {
		std::set<size_t> neighboursOfv;
		neighboursOfv = list[v];

		for (auto i : neighboursOfv)
		{
			if (list.find(i) != list.end())
			{
				C2.push_back(i);
				removeVertex(i);
			}
		}
		//neighboursOfv.clear();
	}

	void readGraph(string NameOfFile, string directory) {
		using namespace std;
		string line;
		vector<string> split;
		size_t i = 0;
		while (1)
		{
			line = Util::GetFileLine(directory + NameOfFile, i);
			if (line == "") break;
			split = Util::Split(line, "\t");

			for (size_t i = 1; i != split.size(); i++)
			{
				addNeighbour(Util::ToInt(split[i]));
			}
			addRowToList(Util::ToInt(split[0]));

			i++;
		}
		calculerVertexMaxDegree();
		GraphHandler::currentMVCSize = list.size();
	}

	size_t getRandomVertex() {
		/*Here this will explore the list of higest degree vertices and
			it will choose any of them randomly*/
		srand(time(NULL));	// initialize random seed:
		size_t random = rand() % maxDegreePositionInList.size();
		random = 0;			//Temporary to choose only the first of the list
		return maxDegreePositionInList[random];
	}

	size_t getGraphSize() {
		return list.size();
	}

	//Copy constructor
	GraphHandler(const GraphHandler& src) {
		this->max = src.max;
		this->maxDegreePositionInList = src.maxDegreePositionInList;
		this->list = src.list;
		this->vertexDegree = src.vertexDegree;
	}

	~GraphHandler() {
		//printf("DESTROY %p \n", this);
	}



};



#endif
