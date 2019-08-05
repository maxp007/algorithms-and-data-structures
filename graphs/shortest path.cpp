#include<iostream>
#include<vector>
#include<list>
#include<queue>
#define INT_MAX 2147483647
struct IGraph {
	virtual ~IGraph() {}
	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<int> GetNextVertices(int vertex) const = 0;
	virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


//Хранящий граф в виде массива списков смежности,
class CListGraph : public IGraph {
public:
	CListGraph(int vertices, bool oriented = true);
	CListGraph(const IGraph* new_graph, bool oriented = false);
	void Print();
	~CListGraph();
	void AddEdge(int from, int to) override;
	int VerticesCount() const override { return vertices_num; }
	std::vector<int> GetNextVertices(int vertex) const override;
	std::vector<int> GetPrevVertices(int vertex) const override;
private:
	bool oriented;
	std::vector<std::list<int>> graph;
	int vertices_num = 0;
};
CListGraph::CListGraph(int vertices, bool oriented) {
	vertices_num = vertices;
	this->oriented = oriented;
	for (int i = 0; i < vertices; i++) {
		graph.push_back(std::list<int>());
	}

}

CListGraph::CListGraph(const IGraph * new_graph, bool oriented) {
	this->oriented = oriented;
	vertices_num = new_graph->VerticesCount();

	for (int i = 0; i < vertices_num; i++) {
		graph.push_back(std::list<int>());
	}
	std::vector<int> vec;
	for (int i = 0; i < vertices_num; i++) {
		vec = new_graph->GetNextVertices(i);
		for (int j = 0; j < vec.size(); j++) {
			(graph[i]).push_back(vec[j]);
		}
	}
}

CListGraph::~CListGraph() {

	for (int i = 0; i < vertices_num; i++) {
		graph[i].clear();
	}
}
void CListGraph::AddEdge(int from, int to) {
	
	(graph[from]).push_back(to);
	if (!oriented) {
		(graph[to]).push_back(from);
	}
}
std::vector<int> CListGraph::GetNextVertices(int vertex)const {
	std::vector<int> vec;
	std::list<int>::const_iterator it;
	for (it = graph[vertex].begin(); it != graph[vertex].end(); ++it) {
		vec.push_back((*it));
	}
	return vec;
}
std::vector<int> CListGraph::GetPrevVertices(int vertex)const {
	std::vector<int> vec;
	std::list<int>::const_iterator it;
	for (int i = 0; i < graph.size(); i++) {
		for (it = graph[i].begin(); it != graph[i].end(); ++it) {
			if (*it == vertex)
				vec.push_back(i);
		}
	}
	return vec;
}
void CListGraph::Print() {
	std::cout << "Out_edges" << std::endl;
	for (int i = 0; i < graph.size(); ++i) {
		std::cout << i << ": ";
		for (int out_target : graph[i]) {
			std::cout << out_target << " ";
		}
		std::cout << std::endl;
	}
}


int Dijkstra(IGraph * graph, int start_vertex, int destin_vertex) {
	std::vector<bool>visited(graph->VerticesCount(), false);
	std::vector<int>distance(graph->VerticesCount(), INT_MAX);
	std::vector<int>ways_num(graph->VerticesCount(), 0);
	std::queue<int> q;
	std::vector<int> vec;
	visited[start_vertex] = true;
	distance[start_vertex] = 0;
	ways_num[start_vertex] = 1;
	q.push(start_vertex);

	while (q.size() != 0) {
		int current = q.front();
		q.pop();
		vec = graph->GetNextVertices(current);
		for (int i = 0; i < vec.size(); i++) {
			if (visited[vec[i]] == false) {
				visited[vec[i]] = true;
				q.push(vec[i]);
				distance[vec[i]] = distance[current] + 1;
			} 
			if (distance[current] + 1 == distance[vec[i]])
				ways_num[vec[i]] += ways_num[current];
		}
	}
	return ways_num[destin_vertex];
}

int main() {

	//kolichestvo vershin
	int vertices = 0;
	std::cin >> vertices;
	//kolichestvo reber
	int n_edges = 0;
	std::cin >> n_edges;

	CListGraph a(vertices, false);
	int from;
	int to;
	// zadanie reber
	for (int i = 0; i < n_edges; i++) {
		std::cin >> from >> to;
		a.AddEdge(from, to);
	}
	
	//enter path
	std::cin >> from >> to;

	//algo
	std::cout<< Dijkstra(&a, from, to)<< '\n';

	return 0;
}
