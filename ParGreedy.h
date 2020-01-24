#ifndef PARGREEDY_H
#define PARGREEDY_H

using namespace std;

#include <iostream>
#include <map>
#include <set>
#include <stdlib.h>     /* srand, rand */
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "util.h"

class ParGreedy
{
private:
	size_t NVERTICES;			//number of vertices
	int max;					//Highest degree within graph
	vector<int> maxDegreePositionInList;	//Stores the positions of max degree vertices within the adjacency list
	map<int, set<int>> list;	//Adjacency list
	set<int> rows;				//Temporary variable to store 
	map<int, int> vertexDegree;	//list of vertices with their corresponding number of edges
public:
	//Default constructor
	ParGreedy()
	{
		NVERTICES = 0;
		max = 0;
	}
	//Parameterized constructor
	ParGreedy(size_t n)
	{
		NVERTICES = n;
		max = 0;
	}

	void addNeighbour(int val) {
		rows.insert(val);
	}

	void addRowToList(int vec0) {
		list.insert({ vec0,rows });
		rows.clear();
	}

	void removeVertex(int v) {
		int lenghtList = list.size();
		set<int>::iterator it = list[v].begin();

		while (it != list[v].end())
		{
			list[*it].erase(v);
			if (list[*it].size() == 0) {
				list.erase(*it);
			}
			vertexDegree[*it]--;
			if (vertexDegree[*it] == 0) {
				vertexDegree.erase(*it);
			}
			it++;
		}
		list.erase(v);
		vertexDegree.erase(v);
		//Recalculating the vertex with maximum number of edges
		int tmp = 0;
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

	void removeNeiboursVertex(int v, vector<int>& C2) {
		do
		{
			set<int>::iterator it = list[v].begin();
			int neighbour = *it;
			C2.push_back(neighbour);
			for (auto const& i : list[neighbour]) {
				list[i].erase(neighbour);
				if (list[i].size() == 0) {
					list.erase(i);
				}
				vertexDegree[i]--;
				if (vertexDegree[i] == 0) {
					vertexDegree.erase(i);
				}
			}

			list.erase(neighbour);
			vertexDegree.erase(neighbour);
			if (list[v].empty())
			{
				list.erase(v);
				break;
			}
		} while (true);

		//Recalculating the vertex with maximum number of edges
		int tmp = 0;
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

	void calculerVertexMaxDegree() {
		int tmp;
		/*Finding vertex degrees, in order to start exploring by these ones.*/
		for (rsize_t i = 0; i < NVERTICES; i++)
		{
			tmp = list[i].size();
			vertexDegree.insert({ i,tmp });
			if (tmp > max) max = tmp;
		}
		for (size_t i = 0; i < NVERTICES; i++)
		{
			if (vertexDegree[i] == max) {
				maxDegreePositionInList.push_back(i);
			}
		}
	}

	void readGraph(string NameOfFile) {
		using namespace std;
		string line;
		vector<string> split;
		int i = 0;
		while (1)
		{
			line = Util::GetFileLine(NameOfFile, i);
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
	}

	int getRandonVertex() {
		/*Here this will explore the list of higest degree vertices and
			it will choose any of them randomly*/
		srand(time(NULL));	// initialize random seed:
		int random = rand() % maxDegreePositionInList.size();
		random = 0;			//Remporary to choose only the first of the list
		return maxDegreePositionInList[random];
	}

	int getGraphSize() {
		return list.size();
	}

	//Copy constructor
	ParGreedy(const ParGreedy& src) {
		this ->NVERTICES = src.NVERTICES;
		this ->max = src.max;
		this->maxDegreePositionInList = src.maxDegreePositionInList;
		this->list = src.list;
		this ->vertexDegree = src.vertexDegree;
	}

	~ParGreedy()
	{
	}

};



#endif