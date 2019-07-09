#include "World.h"
#include "Engine.h"
#include "UnitComponent.h"
#include "PositionComponent.h"
#include "AnimationComponent.h"
#include "TextComponent.h"

#include <math.h>

using namespace std;

World::World()
{
	width = STANDARD_WIDTH;
	height = STANDARD_HEIGHT;

	//Create the builders.
	ArcherBuilder* archerHuman = new ArcherBuilder(Entity::HUMAN);
	ArcherBuilder* archerAi = new ArcherBuilder(Entity::AI);
	InfantryBuilder* infantryHuman = new InfantryBuilder(Entity::HUMAN);
	InfantryBuilder* infantryAi = new InfantryBuilder(Entity::AI);
	FireBuilder* fireHuman = new FireBuilder(Entity::HUMAN);
	FireBuilder* fireAi = new FireBuilder(Entity::AI);
	HqBuilder* hqHuman = new HqBuilder(Entity::HUMAN);
	HqBuilder* hqAi = new HqBuilder(Entity::AI);
	BridgeBuilder* bridgebuilder = new BridgeBuilder;
	GroundBuilder* groundbuilder = new GroundBuilder;
	RockBuilder* rockbuilder = new RockBuilder;
	WaterBuilder* waterbuilder = new WaterBuilder;

	//Put them in the builders map.
	builders['1'] = infantryHuman;
	builders['2'] = archerHuman;
	builders['3'] = fireHuman;
	builders['7'] = infantryAi;
	builders['8'] = archerAi;
	builders['9'] = fireAi;
	builders['h'] = hqHuman;
	builders['H'] = hqAi;
	builders['B'] = bridgebuilder;
	builders['*'] = groundbuilder;
	builders['R'] = rockbuilder;
	builders['W'] = waterbuilder;


	//fill health vector
	health.resize(11);
	
	health[0] = Graphics::SPRITE_HEALTH_MIN;
	health[1] = Graphics::SPRITE_HEALTH1;
	health[2] = Graphics::SPRITE_HEALTH2;
	health[3] = Graphics::SPRITE_HEALTH3;
	health[4] = Graphics::SPRITE_HEALTH4;
	health[5] = Graphics::SPRITE_HEALTH5;
	health[6] = Graphics::SPRITE_HEALTH6;
	health[7] = Graphics::SPRITE_HEALTH7;
	health[8] = Graphics::SPRITE_HEALTH8;
	health[9] = Graphics::SPRITE_HEALTH9;
	health[10] = Graphics::SPRITE_HEALTH10;
}

World::~World() {
	ClearWorld();
}

int World::GetWidth() {
	return width;
}

void World::SetWidth(int w) {
	width = w;
}

int World::GetHeight() {
	return height;
}

void World::SetHeight(int h) {
	height = h;
}

void World::EntityFromCharacter(char c, int row, int col, Engine* engine)
{
	//Make entity based on character.
	EntityBuilder *eb = builders.find(c)->second;
	Entity* entity = eb->BuildEntity(row, col);
	groundTiles.push_back(entity);
	engine->AddEntity(entity);

	//Bij unit componenten komt er nog een extra grond tegel bij om het renderen te vergemakkelijken.
	if (entity->GetComponent(Component::UNIT) != NULL) {
		Entity* ent = builders['*']->BuildEntity(row, col);
			groundTiles.push_back(ent);
			matrix[row][col] = ent;
			engine->AddEntity(ent);
	}
	else
	{
		matrix[row][col] = entity;
	}
}

void World::ClearWorld()
{
	//Delete the builderMap.
	for (builderMap::iterator it = builders.begin(); it != builders.end(); ++it) {
		delete it->second;
	}
}

void World::SetDimensions(std::string width, std::string height)
{
	int w = std::stoi(width, nullptr, 10);
	int h = std::stoi(height, nullptr, 10);
	if (h<MIN_DIMENSION || h>MAX_HEIGHT || w<MIN_DIMENSION || w>MAX_WIDTH) {
		cerr << "Something went wrong trying to set the dimensions.";
	}
	else {
		SetWidth(w);
		SetHeight(h);
		//Resize the matrix of tiles.
		matrix.resize(h);
		for (int i = 0; i < h; ++i){
			matrix[i].resize(w);
		}
	}
}

tileMatrix World::GetMatrix() 
{
	return matrix;
}

Entity * World::getEntity(int row, int col)
{
	if (row < height && col < width && row >=0 && col >=0) {
		return matrix[row][col];
	}
	return nullptr;
}

std::vector<Entity*> World::GetGroundTiles()
{
	return groundTiles;
}

unsigned int World::CalcRange(PositionComponent* attacker, PositionComponent* defender)
{
	//Get the position in the level.
	Grid attackerCoord = Graphics::Instance().FromPx(attacker->pos);
	Grid defenderCoord = Graphics::Instance().FromPx(defender->pos);
	
	//Extract the coordinates.
	int row1 = attackerCoord.row;
	int col1 = attackerCoord.col;
	int row2 = defenderCoord.row;
	int col2 = defenderCoord.col;
	//cout << "row:" << row1 << " col:" << col1 << "\n";
	//cout << "row2:" << row2 << " col2:" << col2 << "\n";

	//Calculate the distance based on the coordinates.
	int temp1 = row1 - row2;
	int temp2 = col1 - col2;
	//cout << "temp1:" << temp1 << " temp2:" << temp2;
	float result = sqrt((temp1*temp1) + (temp2*temp2));
	//cout << "calcRange: " << int(result);

	return unsigned int(result);
}

bool World::IsEnemyInRange(Entity* attacker, Entity* defender)
{
    PositionComponent* apc = dynamic_cast<PositionComponent*>(attacker->GetComponent(Component::POSITION));
    PositionComponent* dpc = dynamic_cast<PositionComponent*>(defender->GetComponent(Component::POSITION));
    UnitComponent* auc = dynamic_cast<UnitComponent*>(attacker->GetComponent(Component::UNIT));

    unsigned int range = CalcRange(apc, dpc);

    return range >= auc->range_min && range <= auc->range_max;
}

void World::Fight(Entity* attackerUnit, Entity* defenderUnit, Entity* attackAnimation)
{
	PositionComponent* attackerPos = dynamic_cast<PositionComponent*>(attackerUnit->GetComponent(Component::POSITION));
	PositionComponent* defenderPos = dynamic_cast<PositionComponent*>(defenderUnit->GetComponent(Component::POSITION));
	attackAnimation->Add(new AnimationComponent(Vector2(defenderPos->pos.x, defenderPos->pos.y + 15), Vector2(defenderPos->pos.x, defenderPos->pos.y - 15)));
	attackAnimation->Add(new TextureComponent({ Graphics::SPRITE_TARGET_BLAST }));
	attackAnimation->Add(new PositionComponent(Vector2(defenderPos->pos.x, defenderPos->pos.y + 15),2));
	UnitComponent* attacker = dynamic_cast<UnitComponent*>(attackerUnit->GetComponent(Component::UNIT));
	UnitComponent* defender = dynamic_cast<UnitComponent*>(defenderUnit->GetComponent(Component::UNIT));

	//Get the type of units and set the damage the attacker can do.
	Entity::UnitType attackerType = attacker->type;
	Entity::UnitType defenderType = defender->type;
	int damage = attacker->dp;

	//Alter the damage depending on the unittypes.
	switch (attackerType)
	{
	case Entity::ARCHERY:
		if (defenderType == Entity::FIRE) {
			damage = damage + (damage / 2);
		}
		if (defenderType == Entity::INFANTRY) {
			damage = damage - (damage / 2);
		}
		break;
	case Entity::INFANTRY:
		if (defenderType == Entity::ARCHERY) {
			damage = damage + (damage / 2);
		}
		if (defenderType == Entity::FIRE) {
			damage = damage - (damage / 2);
		}
		break;
	case Entity::FIRE:
		if (defenderType == Entity::INFANTRY) {
			damage = damage + (damage / 2);
		}
		if (defenderType == Entity::ARCHERY) {
			damage = damage - (damage / 2);
		}
		break;
	default:
		break;
	}
	ostringstream convert;
	convert << damage;
	attackAnimation->Add(new TextComponent(convert.str()));
	//Inflict damage if attacker has actionpoints.
	if (attacker->ap > 0) {
		int health = defender->hp;
		//cout << "health: " << health << "\n";
		DamageUnit(defenderUnit, damage);
		attacker->ap--;
		//cout << "ap: " << attacker->ap << "\n";
	}

	//Reset the actionpoints of the attacker.
	//attacker->ap = attacker->maxAp;
}

void World::DamageUnit(Entity* defender, int damage) 
{
	UnitComponent* comp = dynamic_cast<UnitComponent*>(defender->GetComponent(Component::UNIT));
	int oldhp = comp->hp;
	int newhp = oldhp - damage;
	
	comp->hp = newhp;
	TextureComponent* text = dynamic_cast<TextureComponent*>(defender->GetComponent(Component::TEXTURE));
	text->removeTexture(health[oldhp]);
	if (newhp > 0) {
		
		text->addTexture(health[newhp]);
	}
	else {
		text->addTexture(health[0]);
	}
}

void World::Init(std::string world_file, Engine* engine) {
	//Make the filepath
	std::stringstream ss;
	ss << FILES_ASSETS_PATH << FILES_LEVELS_SUBDIRNAME << "/" << world_file;
	std::string s = ss.str();

	//Open the file
	std::ifstream input(s);
	if (!input.is_open()) {
		cerr << "Could not open: " << s;
	}

	//Create the world dimensions.
	std::string firstNumber;
	std::getline(input, firstNumber, '|');
	std::string secondNumber;
	std::getline(input, secondNumber);
	SetDimensions(firstNumber, secondNumber);

	//Read the characters and fill the matrix of Entities.
	std::string line;
	int lineCounter = 0;
	while (std::getline(input, line)) {
		for (size_t i = 0; i < line.size(); i++) {
			EntityFromCharacter(line[i], lineCounter, i, engine);
		}
		lineCounter++;
	}
}



