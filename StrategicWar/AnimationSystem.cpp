#include "AnimationSystem.h"

void AnimationSystem::Update()
{
	//Normally there should only be one such an entity at a time.
	Context::GameState gs = engine->GetContext()->GetGameState();
	if (gs == Context::AI_MOVE || gs == Context::PL_MOVE) {
		for (Entity* i : engine->GetEntityStream()->WithTags({ Component::ANIMATION, Component::POSITION, Component::UNIT }))
		{
			AnimationComponent* ac = dynamic_cast<AnimationComponent*>(i->GetComponent(Component::ANIMATION));
			PositionComponent* pc = dynamic_cast<PositionComponent*>(i->GetComponent(Component::POSITION));
			UnitComponent* uc = dynamic_cast<UnitComponent*>(i->GetComponent(Component::UNIT));

			if (ac != nullptr)
			{
				if (ac->path.size() > 0)
				{
					//convert the path of grids to a path of vector2's
					std::vector<Vector2> path;
					for (Grid g : ac->path)
					{
						path.push_back(Graphics::Instance().ToPx(g));
					}
					int progress = ac->progress;
					Vector2 startPos = ac->startPos;
					Vector2 endPos = path[path.size() - 1];
					Vector2 curPos = pc->pos;
					Vector2 nextPos;
					if (progress < static_cast<int>(path.size()))
						nextPos = path[progress];

					if (startPos != endPos && uc->ap > 0)
					{
						//increase the position by 1/60 of distance between current and next tile, it'll take 1second to move 1tile
						curPos.x = curPos.x + (nextPos.x - startPos.x) / 60;
						curPos.y = curPos.y + (nextPos.y - startPos.y) / 60;
						pc->pos = curPos;
						//unit arrived at next gridtile ap lowers by 1 and progress on path increases by 1
						if (curPos == nextPos)
						{
							ac->progress++;
							ac->startPos = nextPos;
							uc->ap--;
						}
					}
					//if the unit has no more AP or has reached the end of the path, the animationcomp gets removed
					else
					{
						if (uc->player == Entity::AI)
						{
							engine->GetContext()->SetGameState(Context::AI_PLAY);
						}
						else
						{
							engine->GetContext()->SetGameState(Context::PL_PLAY);
						}

						delete (i->Remove(ac));
					}
				}
			}
		}
	}
	else if (gs == Context::AI_ATTACK || gs == Context::PL_ATTACK) {
		for (Entity* i : engine->GetEntityStream()->WithTags({ Component::ANIMATION, Component::POSITION })) {
			AnimationComponent* ac = dynamic_cast<AnimationComponent*>(i->GetComponent(Component::ANIMATION));
			PositionComponent* pc = dynamic_cast<PositionComponent*>(i->GetComponent(Component::POSITION));
			
			if (ac != nullptr) {
				Vector2 curPos = pc->pos;
				Vector2 endPos = ac->endPos;
				Vector2 startPos = ac->startPos;
				curPos.x = curPos.x + (endPos.x - startPos.x) / 60;
				curPos.y = curPos.y + (endPos.y - startPos.y) / 60;
				pc->pos = curPos;
				if (curPos == endPos) {
					engine->RemoveEntity(i);
					if (gs == Context::AI_ATTACK) {
						engine->GetContext()->SetGameState(Context::AI_PLAY);
					}
					else {
						engine->GetContext()->SetGameState(Context::PL_PLAY);
					}
				}
			}
			
		}
	}
}
