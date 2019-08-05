#include<iostream>
#include<set>
#include<vector>
#include<list>
#define INT_MAX 2147483646
struct Pair {
	int to;
	int weight;
	Pair(int to =0, int weight = 0) {
		this->to = to;
		this->weight =weight;
	};
};
struct IGraph {
	virtual ~IGraph() {}
	// Добавление ребра от from к to.
	virtual void AddEdge(int from, int to, int weight = 1) = 0;

	virtual int VerticesCount() const = 0;

	virtual std::vector<Pair> GetNextVertices(int vertex) const = 0;
	virtual std::vector<Pair> GetPrevVertices(int vertex) const = 0;
};


//Хранящий граф в виде массива списков смежности,
class CListGraph : public IGraph {
public:
	CListGraph(int vertices, bool oriented = true);
	CListGraph(const IGraph* new_graph, bool oriented = false);
	void Print();
	~CListGraph();
	void AddEdge(int from, int to, int weigth = 1) override;
	int VerticesCount() const override { return vertices_num; }
	std::vector<Pair> GetNextVertices(int vertex) const override;
	std::vector<Pair> GetPrevVertices(int vertex) const override;
private:
	bool oriented;
	std::vector<std::list<Pair>> graph;
	int vertices_num = 0;
};
CListGraph::CListGraph(int vertices, bool oriented) {
	vertices_num = vertices;
	this->oriented = oriented;
	for (int i = 0; i < vertices; i++) {
		graph.push_back(std::list<Pair>());
	}

}

CListGraph::~CListGraph() {

	for (int i = 0; i < vertices_num; i++) {
		graph[i].clear();
	}
}
void CListGraph::AddEdge(int from, int to, int weight) {
	
	(graph[from]).push_back(Pair(to, weight));
	if (!oriented) {
		(graph[to]).push_back(Pair(from, weight));
	}
}
std::vector<Pair> CListGraph::GetNextVertices(int vertex)const {
	std::vector<Pair> vec;
	std::list<Pair>::const_iterator it;
	for (it = graph[vertex].begin(); it != graph[vertex].end(); ++it) {
		vec.push_back((*it));
	}
	return vec;
}
std::vector<Pair> CListGraph::GetPrevVertices(int vertex)const {
	std::vector<Pair> vec;
	std::list<Pair>::const_iterator it;
	for (int i = 0; i < graph.size(); i++) {
		for (it = graph[i].begin(); it != graph[i].end(); ++it) {
			if ((it)->to == vertex)
				vec.push_back(i);
		}
	}
	return vec;
}
void CListGraph::Print() {
	std::cout << "Out_edges" << std::endl;
	for (int i = 0; i < graph.size(); ++i) {
		std::cout << i << ": ";
		for (Pair out_target : graph[i]) {
			std::cout <<"("<< out_target.to<<" "<<out_target.weight <<")"<< " ";
		}
		std::cout << std::endl;
	}
}

int Dijkstra(IGraph * graph, int start_vertex, int destin_vertex) {
	std::vector<bool>visited(graph->VerticesCount(), false);	
	
	std::vector<int>distance(graph->VerticesCount(), INT_MAX);
	std::vector<int>pred(graph->VerticesCount(), -1);

	std::set<std::pair<int, int>> qu;
	distance[start_vertex] = 0;
	visited[start_vertex] = true;

	//Use qu to store vertices and their weight to start_vertex
	// first->distance, second->vertex
	//push "start vertex"
	std::pair<int, int> v = std::make_pair(distance[start_vertex], start_vertex);
	qu.insert(v);
	pred[start_vertex] = -1;
	// first -> distance, second-> vertex number
	std::vector<Pair> vec;
	while (qu.size() != 0) {
		//get vertex with least direction to start
		auto iter = (qu.begin());
		auto current = *iter;
		qu.erase(iter);

		//get next verices form current vertice
		vec = graph->GetNextVertices(current.second);

		//for each if next vertices check weight to next vertices
		for (int i = 0; i < vec.size(); i++) {
			//curr_next - next vertex
			Pair curr_path = vec[i];
			//If distance from next vertice to start is greater than current+ weight of edge
			if (distance[curr_path.to] > distance[current.second] + curr_path.weight)
			{
				//decrease distance to 'from' of curr_path
				distance[curr_path.to] = distance[current.second] + curr_path.weight;
				pred[curr_path.to] = current.second;
			}
			if (visited[curr_path.to] == false) {
				auto v = std::make_pair(distance[curr_path.to], curr_path.to);
				qu.insert(v);
			}
		}
		visited[current.second] = true;
	}
	return distance[destin_vertex];
}

int main() {
	
 	int N_City = 0;
	std::cin >> N_City;
	int M_Road = 0;
	std::cin >> M_Road;

	CListGraph a(N_City, false);
	int from;
	int to;
	int weight;
	for (int i = 0; i < M_Road; i++) {
		std::cin >> from >> to >> weight;
		a.AddEdge(from, to, weight);
	}
	std::cin >> from >> to;
	
	std::cout << Dijkstra(&a, from, to) << '\n';

	return 0;
}
