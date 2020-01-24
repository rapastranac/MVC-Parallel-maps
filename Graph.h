#ifndef GRAPH_H
#define GRAPH_H
#include <vector>

struct Graph
{
	int V=0;					/*number of vertices */
	std::vector<std::vector<int>> adj;	/*adjacency matrix*/
	std::vector<int> Vertex_degree;	/*list of vertices with their corresponding number of edges*/
	std::vector<int> Max_degree_pos; /*Stores the positions of max degree vertices within the adjacency list */
	int max;					/*Highest degree within graph*/
	void addEdge(int v, int w)
	{
		adj[v].push_back(w);
	}
	void remove_edge(int r, int v) {
		/*v is the vertex to be erased*/
		/*r is the position of v within the adjacency list*/
		std::vector<int>::iterator toErase;		/*iterator to erase specific element*/
		std::vector<int> row;						/*stores the row index of higest degree vertices for erasing*/

		for (auto j = adj[r].begin() + 1; j < adj[r].end(); j++)
		{
			int i = 0;
			while (i < adj.size()) {
				if (adj[i][0] == *j) {
					toErase = std::find(adj[i].begin() + 1, adj[i].end(), v);
					if (toErase != adj[i].end()) {
						adj[i].erase(toErase);
						Vertex_degree[i] = Vertex_degree[i] - 1;
						if (adj[i].size() == 1) {
							if (i > r) row.push_back(i - 1);
							else row.push_back(i);
							/*every index bigger than the current vertex will be subtracted 1
							to keep row pointing the corresponding index in V_more_edges*/
						}
					}
					break;
				}
				i++;
			}
		}
		adj.erase(adj.begin() + r);						//current vertex is erased from graph
		Vertex_degree.erase(Vertex_degree.begin() + r);	//location containing the number of edges
														//of current vertex is erased
		int k = row.size() - 1;			/*all the vertices that ended up with 0 edges ...*/
		while (!row.empty()) {			/*... will be erased*/
			if (Vertex_degree[row[k]] == 0) {
				adj.erase(adj.begin() + row[k]);
				Vertex_degree.erase(Vertex_degree.begin() + row[k]);
				row.pop_back();
			}
			k--;
		}
		std::vector<int>::iterator i;	/*Recalculating the vertex with maximum number of edges*/
		int tmp = 0;
		for (int i = 0; i < Vertex_degree.size(); i++)
		{
			if (Vertex_degree[i] >= tmp) {
				tmp = Vertex_degree[i];
			}
		}
		max = tmp;
		Max_degree_pos.clear();
		/*storing position of highest degree vertices within adjacency list*/
		for (int i = 0; i < Vertex_degree.size(); i++)
		{
			if (Vertex_degree[i] == max) Max_degree_pos.push_back(i);
		}
	}
	
};

#endif