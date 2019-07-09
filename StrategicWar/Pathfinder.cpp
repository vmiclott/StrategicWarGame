
#include "Pathfinder.h"
#include "PQueueEdge.h"
#include "constants.h"
#include "UnitComponent.h"


Pathfinder::Pathfinder(World* level, EntityStream* es) {
	this->es = es;

	this->width = level->GetWidth();
	this->height = level->GetHeight();

	CreateGraph(level);
}

Pathfinder::~Pathfinder() {
	delete(graph);
}

int Pathfinder::EstimateDistance(Entity& start, Entity& target) {
	Grid startPos = Graphics::Instance().FromPx(((PositionComponent*)(start.GetComponent(Component::POSITION)))->pos);
	Grid targetPos = Graphics::Instance().FromPx(((PositionComponent*)(target.GetComponent(Component::POSITION)))->pos);
	int col_difference = abs(startPos.col - targetPos.col);
	int row_difference = abs(startPos.row - targetPos.row);

	//if roads are present:
	return ROAD_DISTANCE * (col_difference + row_difference);
}

int Pathfinder::GetId(int row, int col, int width)
{
	return row*width + col;
}

int Pathfinder::GetIdFromEntity(Entity* entity, int width) {
	Grid pos = Graphics::Instance().FromPx(((PositionComponent*)entity->GetComponent(Component::POSITION))->pos);
	return GetId(pos.row, pos.col, width);
}

bool Pathfinder::CanWalkOver(Entity& start, Node& end)
{
	if (end.GetType() == nullptr) {
		GroundtypeComponent* comp = (GroundtypeComponent*)end.GetEntity()->GetComponent(Component::GROUNDTYPE);
		if (comp->type == GroundtypeComponent::ROCK || comp->type == GroundtypeComponent::WATER) {
			return false;
		}
		return true;
	}
	else {
		return false; //unit on given tile
	}
}

/*
returns distance between 2 NEIGHBORING entities. If entities aren't neighbors, then the value won't be correct.
*/
int Pathfinder::GetDistance(Entity& start, Entity& end) {
	Grid StartPos = Graphics::Instance().FromPx(((PositionComponent*)start.GetComponent(Component::POSITION))->pos);
	Grid EndPos = Graphics::Instance().FromPx(((PositionComponent*)end.GetComponent(Component::POSITION))->pos);

	if (StartPos.col == EndPos.col || StartPos.row == EndPos.row) {
		if (((GroundtypeComponent*)end.GetComponent(Component::GROUNDTYPE))->type == GroundtypeComponent::ROAD
			&& ((GroundtypeComponent*)start.GetComponent(Component::GROUNDTYPE))->type == GroundtypeComponent::ROAD) {

			return ROAD_DISTANCE;

		}
		else {
			return HORIZONTAL_VERTICAL_DISTANCE;
		}
	}
	else {
		return DIAGONAL_DISTANCE;
	}


}

void Pathfinder::ConnectNodeToNeighbors(Node* node) {
	Grid current = Graphics::Instance().FromPx(((PositionComponent*)node->GetEntity()->GetComponent(Component::POSITION))->pos);

	Node* Neighbor;

	for (int i = current.row - 1; i <= current.row + 1; i++) {
		for (int j = current.col - 1; j <= current.col + 1; j++) {
			if (i >= 0 && j >= 0 && i < height && j < width && (i!=current.row || j!=current.col)) {
				Neighbor = graph->GetNode(GetId(i,j, width));
				int distance = GetDistance(*node->GetEntity(), *Neighbor->GetEntity());
				graph->ConnectNodes(*node, *Neighbor,distance);
			}
		}
	}

}

void Pathfinder::CreateGraph(World* level)
{
	graph = new DWGraph(level->GetWidth()*level->GetHeight());
	vector<vector<Entity*>> tileMatrix = level->GetMatrix();

	for (int col = 0; col < level->GetWidth(); col++) {
		for (int row = 0; row < level->GetHeight(); row++) {
			Node* node = graph->GetNode(GetId(row, col, level->GetWidth()));
			node->SetEntity(tileMatrix[row][col]);
			node->SetType(nullptr);
		}
	}
	
	for (int col = 0; col < level->GetWidth(); col++) {
		for (int row = 0; row < level->GetHeight(); row++) {
			ConnectNodeToNeighbors(graph->GetNode(GetId(row, col, level->GetWidth())));
		}
	}
	
}


std::vector<Grid> Pathfinder::build_path(vector<Node*> came_from, Node* target, int distance)
{

	Node* original_target = target;

	Node* from = came_from[GetIdFromEntity(target->GetEntity(), width)];

	int count = 1;

	while (from != NULL) {
		count++;
		int id = GetIdFromEntity(from->GetEntity(), width);
		target = from;
		from = came_from[id];
	}

	std::vector<Grid> path;
	path.resize(count - 1);
	from = original_target;

	//path built backwards
	for (int i = count - 2; i >= 0; i--) {
		path[i] = Graphics::Instance().FromPx(((PositionComponent*)from->GetEntity()->GetComponent(Component::POSITION))->pos);
		from = came_from[GetIdFromEntity(from->GetEntity(), width)];
	}


	return path;


}

//TODO: remove. used for testing purposes
void printGraph(DWGraph* graph, int width, int height) {
	for (int i = 0; i < width*height; i++) {
		Node* cur = graph->GetNode(i);
		for (Edge* j : cur->GetEdges()) {
			Grid stuff = Graphics::Instance().FromPx(((PositionComponent*)j->GetEnd()->GetEntity()->GetComponent(Component::POSITION))->pos);
			cout << "Node" << i << ": " << stuff.col << "+" << stuff.row << "\n";
		}
		cout << "\n\n";
	}
}

std::vector<Grid> Pathfinder::FindPath(Entity* start, Entity* target)
{
	RemoveUnits();
	AddUnits();

	if (!CanWalkOver(*start, *graph->GetNode(GetIdFromEntity(target, width))))
	{
		return {};
	}

	return FindPurePath(start, target);

	
}

void Pathfinder::AddUnits() {
	Grid grid;
	for (Entity* unit : es->WithTag(Component::UNIT)) {
		grid = Graphics::Instance().FromPx(((PositionComponent*)unit->GetComponent(Component::POSITION))->pos);
		graph->GetNode(GetIdFromEntity(unit, width))->SetType(&((UnitComponent*)unit->GetComponent(Component::UNIT))->type);
	}
}

void Pathfinder::RemoveUnits() {
	for (int i = 0; i < width*height; i++) {
		graph->GetNode(i)->SetType(nullptr);
	}
}

std::vector<Grid> Pathfinder::FindPurePath(Entity* start, Entity* target) {
	RemoveUnits();
	AddUnits();


	//uses the tile instead of the unit entity
	Entity* end = graph->GetNode(GetIdFromEntity(target, width))->GetEntity();

	Node* startNode = graph->GetNode(GetIdFromEntity(start, width));

	std::vector<Grid> path;

	if (start == target)
	{
		return path;
	}

	const int size = width*height;

	PQueueEdge queue = PQueueEdge();



	std::vector<Node*> came_from;
	came_from.resize(width*height);
	//array to keep track of the distance from start to current_node
	std::vector<int> real_distance;
	real_distance.resize(width*height);

	Grid pos = Graphics::Instance().FromPx(((PositionComponent*)start->GetComponent(Component::POSITION))->pos);
	int id = GetId(pos.row, pos.col, width);

	int neighbor_id;

	for (Edge* i : graph->GetNode(id)->GetEdges()) {
		if (CanWalkOver(*start, *i->GetEnd()) || i->GetEnd() == graph->GetNode(GetIdFromEntity(target, width))) {
			neighbor_id = GetIdFromEntity(i->GetEnd()->GetEntity(), width);
			came_from[neighbor_id] = graph->GetNode(id);
			real_distance[neighbor_id] = i->GetWeight();
			queue.Update(i, i->GetWeight() + EstimateDistance(*(i->GetEnd()->GetEntity()), *end));
		}
	}

	came_from[id] = nullptr;

	Edge* current_edge;

	Node* current_node;


	vector<bool> already_visited_array;
	already_visited_array.resize(width*height, false);

	already_visited_array[id] = true;

	while (queue.Size() > 0)
	{
		//takes the node with shortest distance to start & estimated distance to target
		current_edge = queue.Pop();
		current_node = current_edge->GetEnd();

		pos = Graphics::Instance().FromPx(((PositionComponent*)current_node->GetEntity()->GetComponent(Component::POSITION))->pos);
		id = GetId(pos.row, pos.col, width);

		already_visited_array[id] = true;

		if (current_edge->GetEnd()->GetEntity() == end)
		{
			//shortest route to target found: initialize path creation in seperate method
			int total_distance = real_distance[id];
			//delete queue;
			return build_path(came_from, current_node, total_distance);
		}
		//go over neighbors of current node
		for (Edge* neighbor : current_node->GetEdges())
		{
			if (CanWalkOver(*start, *neighbor->GetEnd()) || neighbor->GetEnd() == graph->GetNode(GetIdFromEntity(target,width))) {
				neighbor_id = GetIdFromEntity(neighbor->GetEnd()->GetEntity(), width);
				//if neighbor hasn't been 'visited' yet: hasn't been the 'current_node' in a loop
				if (!already_visited_array[neighbor_id])
				{
					int distance_neighbor = real_distance[id] + neighbor->GetWeight();
					//checks if the current distance to the neighbor is better than a previous distance
					if (real_distance[neighbor_id] == 0 || distance_neighbor < real_distance[neighbor_id])
					{
						queue.Update(neighbor, distance_neighbor + EstimateDistance(*neighbor->GetEnd()->GetEntity(), *end));
						real_distance[neighbor_id] = distance_neighbor;
						came_from[neighbor_id] = current_node;
					}
				}
			}
		}
	}
	//delete(queue);
	return path;
}


