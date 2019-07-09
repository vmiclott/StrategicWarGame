#include "DWGraph.h"

using namespace std;

Node::Node(Entity* entity)
{
	this->entity = entity;
	this->edges = {};
}

Entity::UnitType* Node::GetType() {
	return type;
}

void Node::SetType(Entity::UnitType* unit) {
	type = unit;
}

Node::Node()
{
	this->edges = {};
}

void Node::SetEntity(Entity* entity)
{
	this->entity = entity;
}

//TODO: check for memoryleaks
Node::~Node()
{
	std::vector<Edge*>::iterator it = edges.begin();
	while (it != edges.end())
	{
		Edge* edge = *it;
		it = edges.erase(it);
		delete(edge);
	}
}

std::vector<Edge*> Node::GetEdges()
{
	return edges;
}

void Node::AddEdge(Node * end, int weight)
{
	edges.push_back(new Edge(this, end, weight));
}

Entity* Node::GetEntity() {
	return entity;
}

Edge::Edge(Node * start, Node * end, int weight)
{
	this->start = start;
	this->end = end;
	this->weight = weight;
}

Edge::~Edge()
{
}

Node * Edge::GetStart()
{
	return start;
}

Node * Edge::GetEnd()
{
	return end;
}

int Edge::GetWeight()
{
	return weight;
}

DWGraph::DWGraph(int size)
{
	nodes = new Node[size];
	this->size = size;
}

//TODO: check for memoryleaks
DWGraph::~DWGraph()
{
	delete[](nodes);
}

Node * DWGraph::GetNode(int id)
{
	return &nodes[id];
}

void DWGraph::addEntity(Entity * entity, int id)
{
	nodes[id] = *new Node(entity);
}

void DWGraph::ConnectNodes(Node & start, Node & end, int weight)
{
	start.AddEdge(&end, weight);
}

bool Edge::operator==(const Edge& edge2) {
	return this->end == edge2.end;
}

bool Edge::Equals(const Edge& edge2) {
	return this->start == edge2.start && this->weight == edge2.weight && this->end == edge2.end;
}
