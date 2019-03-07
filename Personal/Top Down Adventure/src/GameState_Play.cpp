#include "GameState_Play.h"
#include "GameState_In_Menu.h"
#include "Common.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"

GameState_Play::GameState_Play(GameEngine & game, const std::string & levelPath)
	: GameState(game)
	, m_levelPath(levelPath)
{
	init(m_levelPath);
}

void GameState_Play::init(const std::string & levelPath)
{
	loadLevel(levelPath);
}

void GameState_Play::loadLevel(const std::string & filename)
{
	m_entityManager = EntityManager();

	std::ifstream fin(filename);
	if (!fin.is_open())
	{
		std::cout << "Error failed to load " << filename << "\n";
		exit(-1);
	}
	std::string token, type, ai, path;
	int tx, ty, rx, ry, bm, bv;
	float speed;
	while (fin.good())
	{
		fin >> token;
		//loading the tile and npc
		if (token == "Tile" || token == "NPC")
		{
			fin >> type >> rx >> ry >> tx >> ty >> bm >> bv;
			auto block = m_entityManager.addEntity(token);
			block->addComponent<CTransform>(Vec2(tx * 64 + 1280 * rx, ty * 64 + 768 * ry));
			block->addComponent<CAnimation>(m_game.getAssets().getAnimation(type), true);
			block->addComponent<CBoundingBox>(block->getComponent<CAnimation>()->animation.getSize(), bm, bv);

			if (type == "Tektite") 
			{
				block->addComponent<CHealth>(2);
			}
			else if (type == "Knight")
			{
				block->addComponent<CHealth>(5);
			}

			if (token == "NPC")
			{
				fin >> ai >> speed;
				if (ai == "Follow")
				{
					block->addComponent<CFollowPlayer>(Vec2(tx * 64 + 1280 * rx, ty * 64 + 768 * ry), speed);
				}
				else
				{
					int n, xi, yi;
					fin >> n;
					std::vector<Vec2> pos;
					while (n > 0)
					{
						fin >> xi >> yi;
						pos.push_back(Vec2(xi * 64 + 1280 * rx, yi * 64 + 768 * ry));
						n--;
					}
					block->addComponent<CPatrol>(pos, speed);
				}
			}

		}
		else if (token == "Track")
		{
			fin >> path;
			if (!m_track.openFromFile(path))
			{
				std::cout << path << " Not found\n";
			}
			m_track.setVolume(50.0f);
			m_track.play();
		}
		//loading the player
		else
		{

			fin >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY >> m_playerConfig.SPEED;
			spawnPlayer();
		}
	}
}

void GameState_Play::spawnPlayer()
{
	m_player = m_entityManager.addEntity("player");
	m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
	m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("StandDown"), true);
	m_player->addComponent<CState>("StandDown");
	m_player->addComponent<CInput>();
	//m_player->addComponent<CHealth>(20);
	m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY), false, false);
	m_player->addComponent<CWeapon>();
	// New element to CTransform: 'facing', to keep track of where the player is facing
	m_player->getComponent<CTransform>()->facing = Vec2(0, 1);
}

void GameState_Play::spawnSword(std::shared_ptr<Entity> entity)
{
	auto eTransform = entity->getComponent<CTransform>();

	auto sword = m_entityManager.addEntity("sword");
	sword->addComponent<CTransform>(entity->getComponent<CTransform>()->pos + eTransform->facing * 64);

	if (m_player->getComponent<CState>()->state == "AtkDown")
	{
		sword->addComponent<CAnimation>(m_game.getAssets().getAnimation("SwordUp"), true);
		sword->getComponent<CTransform>()->scale.y = -1;
	}
	else if (m_player->getComponent<CState>()->state == "AtkUp")
	{
		sword->addComponent<CAnimation>(m_game.getAssets().getAnimation("SwordUp"), true);
	}
	else if (m_player->getComponent<CState>()->state == "AtkRight")
	{
		sword->addComponent<CAnimation>(m_game.getAssets().getAnimation("SwordRight"), true);
	}
	else if (m_player->getComponent<CState>()->state == "AtkLeft")
	{
		sword->addComponent<CAnimation>(m_game.getAssets().getAnimation("SwordRight"), true);
		sword->getComponent<CTransform>()->scale.x = -1;
	}
	sword->addComponent<CBoundingBox>(sword->getComponent<CAnimation>()->animation.getSize(), false, false);
	sword->addComponent<CLifeSpan>(150);
}

void GameState_Play::spawnBow(std::shared_ptr<Entity> entity)
{
	auto eTrans = entity->getComponent<CTransform>();

	auto bow = m_entityManager.addEntity("bow");
	bow->addComponent<CTransform>(eTrans->pos + eTrans->facing * 48);
	if (m_player->getComponent<CState>()->state == "AtkRight")
	{
		bow->addComponent<CAnimation>(m_game.getAssets().getAnimation("BowRight"), true);
	}
	else if (m_player->getComponent<CState>()->state == "AtkLeft")
	{
		bow->addComponent<CAnimation>(m_game.getAssets().getAnimation("BowRight"), true);
		bow->getComponent<CTransform>()->scale.x = -1;
	}
	else if (m_player->getComponent<CState>()->state == "AtkUp")
	{
		bow->addComponent<CAnimation>(m_game.getAssets().getAnimation("BowUp"), true);
	}
	else if (m_player->getComponent<CState>()->state == "AtkDown")
	{
		bow->addComponent<CAnimation>(m_game.getAssets().getAnimation("BowUp"), true);
		bow->getComponent<CTransform>()->scale.y = -1;
	}
	bow->addComponent<CLifeSpan>(150);

	auto arrow = m_entityManager.addEntity("arrow");
	Vec2 aHB = (eTrans->facing * 64);
	aHB.x = std::abs(aHB.x);
	aHB.y = std::abs(aHB.y);
	if (aHB.x == 0)
	{
		aHB.x += 32;
	}
	else if (aHB.y == 0)
	{
		aHB.y += 32;
	}
	arrow->addComponent<CTransform>(eTrans->pos + eTrans->facing * 64);
	if (m_player->getComponent<CState>()->state == "AtkRight")
	{
		arrow->addComponent<CAnimation>(m_game.getAssets().getAnimation("ArrowRight"), true);
	}
	else if (m_player->getComponent<CState>()->state == "AtkLeft")
	{
		arrow->addComponent<CAnimation>(m_game.getAssets().getAnimation("ArrowRight"), true);
		arrow->getComponent<CTransform>()->scale.x = -1;
	}
	else if (m_player->getComponent<CState>()->state == "AtkUp")
	{
		arrow->addComponent<CAnimation>(m_game.getAssets().getAnimation("ArrowUp"), true);
	}
	else if (m_player->getComponent<CState>()->state == "AtkDown")
	{
		arrow->addComponent<CAnimation>(m_game.getAssets().getAnimation("ArrowUp"), true);
		arrow->getComponent<CTransform>()->scale.y = -1;
	}
	arrow->getComponent<CTransform>()->speed = Vec2(eTrans->facing.x * m_playerConfig.SPEED * 2, eTrans->facing.y * m_playerConfig.SPEED * 2);
	arrow->addComponent<CBoundingBox>(aHB, false, false);
	arrow->addComponent<CLifeSpan>(1000);
}

void GameState_Play::update()
{
	m_entityManager.update();

	if (!m_paused)
	{
		sAI();
		sAnimation();
		sMovement();
		sLifespan();
		sCollision();
	}

	sUserInput();
	sRender();
}

void GameState_Play::sMovement()
{
	// Implement all entity movement here
	auto ptrans = m_player->getComponent<CTransform>();
	auto pinput = m_player->getComponent<CInput>();
	auto pState = m_player->getComponent<CState>();

	// Updating prevpos
	for (auto e : m_entityManager.getEntities())
	{
		e->getComponent<CTransform>()->prevPos = e->getComponent<CTransform>()->pos;
		if (e->getComponent<CAnimation>()->animation.getName() == "BowUp" || e->getComponent<CAnimation>()->animation.getName() == "BowRight" || e->getComponent<CAnimation>()->animation.getName() == "SwordRight" || e->getComponent<CAnimation>()->animation.getName() == "SwordUp")
		{
			e->getComponent<CTransform>()->pos += m_player->getComponent<CTransform>()->speed;
		}
		else
		{
			e->getComponent<CTransform>()->pos += e->getComponent<CTransform>()->speed;
		}
		
	}


	// Player Movement
	if (pinput->up)
	{
		// up down
		if (pinput->down)
		{
			// up down left
			if (pinput->left)
			{
				// up down left right
				if (pinput->right)
				{
					ptrans->speed = Vec2(0, 0);
					// assign the appropriate standing state based on the direction the player is facing
					if (ptrans->facing == Vec2(-1, 0))
					{
						pState->state = "StandLeft";
					}
					else if (ptrans->facing == Vec2(1, 0))
					{
						pState->state = "StandRight";
					}
					else if (ptrans->facing == Vec2(0, 1))
					{
						pState->state = "StandDown";
					}
					else if (ptrans->facing == Vec2(0, -1))
					{
						pState->state = "StandUp";
					}
				}
				// up down left
				else
				{
					ptrans->speed = Vec2(-m_playerConfig.SPEED, 0);
					ptrans->facing = Vec2(-1, 0);
					pState->state = "RunLeft";
				}
			}
			// up down right
			else if (pinput->right)
			{
				ptrans->speed = Vec2(m_playerConfig.SPEED, 0);
				ptrans->facing = Vec2(1, 0);
				pState->state = "RunRight";
			}
			// up down
			else
			{
				ptrans->speed = Vec2(0, 0);
				// assign the appropriate standing state based on the direction the player is facing
				if (ptrans->facing == Vec2(-1, 0))
				{
					pState->state = "StandLeft";
				}
				else if (ptrans->facing == Vec2(1, 0))
				{
					pState->state = "StandRight";
				}
				else if (ptrans->facing == Vec2(0, 1))
				{
					pState->state = "StandDown";
				}
				else if (ptrans->facing == Vec2(0, -1))
				{
					pState->state = "StandUp";
				}
			}
		}
		// up left
		else if (pinput->left)
		{
			// up left right
			if (pinput->right)
			{
				ptrans->speed = Vec2(0, -m_playerConfig.SPEED);
				ptrans->facing = Vec2(0, -1);
				pState->state = "RunUp";
			}
			// up left
			else
			{
				ptrans->speed = Vec2(-m_playerConfig.SPEED, 0);
				ptrans->facing = Vec2(-1, 0);
				pState->state = "RunLeft";
			}
		}
		// up right
		else if (pinput->right)
		{
			ptrans->speed = Vec2(m_playerConfig.SPEED, 0);
			ptrans->facing = Vec2(1, 0);
			pState->state = "RunRight";
		}
		// up
		else
		{
			ptrans->speed = Vec2(0, -m_playerConfig.SPEED);
			ptrans->facing = Vec2(0, -1);
			pState->state = "RunUp";
		}
	}
	else if (pinput->down)
	{
		// down right
		if (pinput->right)
		{
			// down right left
			if (pinput->left)
			{
				ptrans->speed = Vec2(0, m_playerConfig.SPEED);
				ptrans->facing = Vec2(0, 1);
				pState->state = "RunDown";
			}
			// down right
			else
			{
				ptrans->speed = Vec2(m_playerConfig.SPEED, 0);
				ptrans->facing = Vec2(1, 0);
				pState->state = "RunRight";
			}
		}
		// down left
		else if (pinput->left)
		{
			ptrans->speed = Vec2(-m_playerConfig.SPEED, 0);
			ptrans->facing = Vec2(-1, 0);
			pState->state = "RunLeft";
		}
		// down
		else
		{
			ptrans->speed = Vec2(0, m_playerConfig.SPEED);
			ptrans->facing = Vec2(0, 1);
			pState->state = "RunDown";
		}
	}
	else if (pinput->right)
	{
		// right left
		if (pinput->left)
		{
			ptrans->speed = Vec2(0, 0);
			// assign the appropriate standing state based on the direction the player is facing
			if (ptrans->facing == Vec2(-1, 0))
			{
				pState->state = "StandLeft";
			}
			else if (ptrans->facing == Vec2(1, 0))
			{
				pState->state = "StandRight";
			}
			else if (ptrans->facing == Vec2(0, 1))
			{
				pState->state = "StandDown";
			}
			else if (ptrans->facing == Vec2(0, -1))
			{
				pState->state = "StandUp";
			}
		}
		// right
		else
		{
			ptrans->speed = Vec2(m_playerConfig.SPEED, 0);
			ptrans->facing = Vec2(1, 0);
			pState->state = "RunRight";
		}
	}
	else if (pinput->left)
	{
		ptrans->speed = Vec2(-m_playerConfig.SPEED, 0);
		ptrans->facing = Vec2(-1, 0);
		pState->state = "RunLeft";
	}
	else
	{
		ptrans->speed = Vec2(0, 0);
		// assign the appropriate standing state based on the direction the player is facing
		if (ptrans->facing == Vec2(-1, 0))
		{
			pState->state = "StandLeft";
		}
		else if (ptrans->facing == Vec2(1, 0))
		{
			pState->state = "StandRight";
		}
		else if (ptrans->facing == Vec2(0, 1))
		{
			pState->state = "StandDown";
		}
		else if (ptrans->facing == Vec2(0, -1))
		{
			pState->state = "StandUp";
		}
	}

}

void GameState_Play::sAI()
{
	// Implement Follow behavior of NPCS
	// Implement Patrol behavior of NPCS

	for (auto ent : m_entityManager.getEntities("NPC"))
	{
		//patrol behavior
		if (ent->hasComponent<CPatrol>())
		{
			std::shared_ptr<CPatrol> patrol = ent->getComponent<CPatrol>();
			Vec2 dmx = ent->getComponent<CTransform>()->pos + Vec2(2, 2);
			Vec2 dmn = ent->getComponent<CTransform>()->pos - Vec2(2, 2);

			Vec2 pPos = patrol->positions[patrol->currentPosition];
			//checking if the entity has reached its goal
			if (dmn.x <= pPos.x && dmx.x >= pPos.x && dmn.y <= pPos.y&& dmx.y >= pPos.y)
			{
				patrol->currentPosition = (patrol->currentPosition + 1) % patrol->positions.size();
			}
			else
			{
				//updating the speed in the right direction
				Vec2 D = patrol->positions[patrol->currentPosition] - ent->getComponent<CTransform>()->pos;
				float angle = atan2f(D.y, D.x);

				Vec2 speed = Vec2(patrol->speed*cosf(angle), patrol->speed*sinf(angle));

				ent->getComponent<CTransform>()->speed = speed;

			}
		}
		//follow behavior, I'll see if this can be smaller
		else if (ent->hasComponent<CFollowPlayer>())
		{
			bool walk = true;
			for (auto block : m_entityManager.getEntities("Tile"))
			{
				if (block->getComponent<CBoundingBox>()->blockVision)
				{
					Vec2 bPos = block->getComponent<CTransform>()->pos;
					//making the different segments points
					bPos -= block->getComponent<CBoundingBox>()->halfSize;
					Vec2 a = bPos;
					bPos.x += block->getComponent<CAnimation>()->animation.getSize().x;
					Vec2 b = bPos;
					bPos.x -= block->getComponent<CAnimation>()->animation.getSize().x;
					bPos.y += block->getComponent<CAnimation>()->animation.getSize().y;
					Vec2 c = bPos;
					bPos.x += block->getComponent<CAnimation>()->animation.getSize().x;
					Vec2 d = bPos;

					//restoring the value for the position
					bPos.x -= block->getComponent<CAnimation>()->animation.getSize().x;
					bPos.y -= block->getComponent<CAnimation>()->animation.getSize().y;
					bPos += block->getComponent<CBoundingBox>()->halfSize;

					Vec2 entPos = ent->getComponent<CTransform>()->pos;
					Vec2 playerPos = m_player->getComponent<CTransform>()->pos;

					Intersect segAB = Physics::LineIntersect(a, b, entPos, playerPos);
					Intersect segAC = Physics::LineIntersect(a, c, entPos, playerPos);
					Intersect segBD = Physics::LineIntersect(b, d, entPos, playerPos);
					Intersect segCD = Physics::LineIntersect(c, d, entPos, playerPos);
					if (segAB.result || segAC.result || segBD.result || segCD.result)
					{
						walk = false;
						break;
					}
				}
			}
			for (auto block : m_entityManager.getEntities("NPC"))
			{
				if (block->getComponent<CBoundingBox>()->blockVision)
				{
					Vec2 bPos = block->getComponent<CTransform>()->pos;
					//making the different segments
					bPos -= block->getComponent<CBoundingBox>()->halfSize;
					Vec2 a = bPos;
					bPos.x += block->getComponent<CAnimation>()->animation.getSize().x;
					Vec2 b = bPos;
					bPos.x -= block->getComponent<CAnimation>()->animation.getSize().x;
					bPos.y += block->getComponent<CAnimation>()->animation.getSize().y;
					Vec2 c = bPos;
					bPos.x += block->getComponent<CAnimation>()->animation.getSize().x;
					Vec2 d = bPos;

					//restoring the value for the position
					bPos.x -= block->getComponent<CAnimation>()->animation.getSize().x;
					bPos.y -= block->getComponent<CAnimation>()->animation.getSize().y;
					bPos += block->getComponent<CBoundingBox>()->halfSize;

					Vec2 entPos = ent->getComponent<CTransform>()->pos;
					Vec2 playerPos = m_player->getComponent<CTransform>()->pos;

					Intersect segAB = Physics::LineIntersect(a, b, entPos, playerPos);
					Intersect segAC = Physics::LineIntersect(a, c, entPos, playerPos);
					Intersect segBD = Physics::LineIntersect(b, d, entPos, playerPos);
					Intersect segCD = Physics::LineIntersect(c, d, entPos, playerPos);
					if (segAB.result || segAC.result || segBD.result || segCD.result)
					{
						walk = false;
						break;
					}
				}
			}
			Vec2 D;
			if (walk)
			{
				//updating the speed in the right direction
				D = m_player->getComponent<CTransform>()->pos - ent->getComponent<CTransform>()->pos;

				float angle = atan2f(D.y, D.x);

				Vec2 speed = Vec2(ent->getComponent<CFollowPlayer>()->speed*cosf(angle), ent->getComponent<CFollowPlayer>()->speed*sinf(angle));

				ent->getComponent<CTransform>()->speed = speed;
			}
			else
			{
				//returning the entity home
				float dist = Physics::FollowDist(ent, ent->getComponent<CFollowPlayer>()->home);
				if (dist > 1) 
				{
					D = ent->getComponent<CFollowPlayer>()->home - ent->getComponent<CTransform>()->pos;
					float angle = atan2f(D.y, D.x);

					Vec2 speed = Vec2(ent->getComponent<CFollowPlayer>()->speed*cosf(angle), ent->getComponent<CFollowPlayer>()->speed*sinf(angle));

					ent->getComponent<CTransform>()->speed = speed;
				}
				else
				{
					Vec2 speed = Vec2(0, 0);

					ent->getComponent<CTransform>()->speed = speed;
				}
				
			}
			
		}
	}
}

void GameState_Play::sLifespan()
{
	// Implement Lifespan
	for (auto & ent : m_entityManager.getEntities())
	{
		auto eLife = ent->getComponent<CLifeSpan>();
		if (eLife && eLife->clock.getElapsedTime().asMilliseconds() >= eLife->lifespan)
		{
			ent->destroy();
		}
	}
}

void GameState_Play::sCollision()
{
	// Implement Collision detection / resolution
	for (auto e : m_entityManager.getEntities("Tile"))
	{
		if (e->hasComponent<CBoundingBox>())
		{
			//shortened player vars
			auto pTrans = m_player->getComponent<CTransform>();
			auto pState = m_player->getComponent<CState>();
			auto pBound = m_player->getComponent<CBoundingBox>();
			//shortened entity vars
			auto eTrans = e->getComponent<CTransform>();
			auto eBound = e->getComponent<CBoundingBox>();

			if (eBound->blockMove)
			{
				// Calculations for the prevPos overlap
				Vec2 checker = Physics::GetOverlap(m_player, e);

				Vec2 prechecker = Physics::GetPreviousOverlap(m_player, e);

				//if there is an overlap
				if (checker.x > 0 && checker.y > 0)
				{
					if (prechecker.x > 0) //y collision
					{
						if (pTrans->pos.y < eTrans->pos.y) //from the bottom
						{
							pTrans->pos.y -= checker.y;
						}
						else //from the top
						{
							pTrans->pos.y += checker.y;
						}

					}
					else if (prechecker.y > 0) //x collision
					{
						if (pTrans->pos.x < eTrans->pos.x) //left
						{
							pTrans->pos.x -= checker.x;
						}
						else //right
						{
							pTrans->pos.x += checker.x;
						}
					}
					else // For some reason this is called when walking to the left
					{
						if (checker.x > checker.y)
						{
							if (pTrans->pos.y < eTrans->pos.y)
							{
								pTrans->pos.y -= checker.y;
							}
							else
							{
								pTrans->pos.y += checker.y;
							}
						}
						else
						{
							if (pTrans->pos.x < eTrans->pos.x)
							{
								pTrans->pos.x -= checker.x;
							}
							else
							{
								pTrans->pos.x += checker.x;
							}
						}

					}
				}

				for (auto n : m_entityManager.getEntities("NPC"))
				{
					auto nTrans = n->getComponent<CTransform>();

					Vec2 nChecker = Physics::GetOverlap(n, e);
					Vec2 nPrechecker = Physics::GetPreviousOverlap(n, e);

					if (nChecker.x > 0 && nChecker.y > 0)
					{
						if (nPrechecker.x > 0) //y collision
						{
							if (nTrans->pos.y < eTrans->pos.y) //from the bottom
							{
								nTrans->pos.y -= nChecker.y;
							}
							else //from the top
							{
								nTrans->pos.y += nChecker.y;
							}

						}
						else if (nPrechecker.y > 0) //x collision
						{
							if (nTrans->pos.x < eTrans->pos.x) //left
							{
								nTrans->pos.x -= nChecker.x;
							}
							else //right
							{
								nTrans->pos.x += nChecker.x;
							}
						}
						else // For some reason this is called when walking to the left
						{
							if (nChecker.x > nChecker.y)
							{
								if (nTrans->pos.y < eTrans->pos.y)
								{
									nTrans->pos.y -= nChecker.y;
								}
								else
								{
									nTrans->pos.y += nChecker.y;
								}
							}
							else
							{
								if (nTrans->pos.x < eTrans->pos.x)
								{
									nTrans->pos.x -= nChecker.x;
								}
								else
								{
									nTrans->pos.x += nChecker.x;
								}
							}

						}
					}

				}

				for (auto a : m_entityManager.getEntities("arrow")) 
				{
					auto aTrans = a->getComponent<CTransform>();

					Vec2 aChecker = Physics::GetOverlap(a, e);

					if (aChecker.x > 0 && aChecker.y > 0)
					{
						if (e->getComponent<CAnimation>()->animation.getName().find("Water")) 
						{
							a->destroy();
						}
					}
				}

				for (auto d : m_entityManager.getEntities("doors")) 
				{
					auto dTrans = d->getComponent<CTransform>();

					Vec2 dChecker = Physics::GetOverlap(d, m_player);

					if (dChecker.x > 0 && dChecker.y > 0)
					{
						if (d->getComponent<CAnimation>()->animation.getName() == "BoneGate")
						{
							if (m_player->getComponent<CInventory>()->item == "Bone Key")
							{
								d->destroy();
							}
						}
						else if (d->getComponent<CAnimation>()->animation.getName() == "RustGate")
						{
							if (m_player->getComponent<CInventory>()->item == "Rusty Key")
							{
								d->destroy();
							}
						}
						if (d->getComponent<CAnimation>()->animation.getName() == "GoldGate")
						{
							if (m_player->getComponent<CInventory>()->item == "Golden Key")
							{
								d->destroy();
							}
						}
					}
				}
			}
			else if (e->getComponent<CAnimation>()->animation.getName() == "Black")
			{
				Vec2 checker = Physics::GetOverlap(m_player, e);

				if (checker.x > 0 && checker.y > 0)
				{
					std::cout << "Player Pos: " << pTrans->pos.x << " " << pTrans->pos.y << "\n";
					if (eTrans->pos == Vec2(4*64, 3*64))
					{
						pTrans->pos = Vec2(1 * 1280 + 14 * 64, 5 * 64);
					}
					else if (eTrans->pos == Vec2(1*1280 + 14*64, 3*64))
					{
						pTrans->pos = Vec2(4 * 64, 5 * 64);
					}
				}
			}
		}
	}
	for (auto n : m_entityManager.getEntities("NPC"))
	{
		if (n->hasComponent<CBoundingBox>())
		{
			// check for NPC/Player collision (if the NPC is exploding, do not kill the player)
			Vec2 npChecker = Physics::GetOverlap(n, m_player);
			if (npChecker.x > 0 && npChecker.y > 0 && n->getComponent<CAnimation>()->animation.getName() != "Explosion")
			{
				m_player->destroy();
				spawnPlayer();
			}

			// check for NPC/Sword collision
			for (auto sword : m_entityManager.getEntities("sword"))
			{
				Vec2 nsChecker = Physics::GetOverlap(n, sword);
				if (nsChecker.x > 0 && nsChecker.y > 0)
				{

					n->getComponent<CHealth>()->health -= 3;
					if (n->getComponent<CHealth>()->health < 1)
					{
						auto t = m_entityManager.addEntity("explosion");
						t->addComponent<CTransform>(n->getComponent<CTransform>()->pos);
						t->addComponent<CAnimation>(m_game.getAssets().getAnimation("Explosion"), true);
						t->addComponent<CLifeSpan>(1000);
						n->destroy();
					}
					
				}
			}

			// check for NPC/arrow collision
			for (auto arrow : m_entityManager.getEntities("arrow"))
			{
				Vec2 naChecker = Physics::GetOverlap(n, arrow);
				if (naChecker.x > 0 && naChecker.y > 0)
				{
					n->getComponent<CHealth>()->health -= 2;
					if (n->getComponent<CHealth>()->health < 1)
					{
						auto t = m_entityManager.addEntity("explosion");
						t->addComponent<CTransform>(n->getComponent<CTransform>()->pos);
						t->addComponent<CAnimation>(m_game.getAssets().getAnimation("Explosion"), true);
						t->addComponent<CLifeSpan>(1000);
						n->destroy();
					}
					arrow->destroy();
				}
			}
		}
	}

	
}

void GameState_Play::sAnimation()
{
	// Implement updating and setting of all animations here

	// player animations
	std::string pState = m_player->getComponent<CState>()->state;
	auto pAnimation = m_player->getComponent<CAnimation>();
	if (pState == "AtkLeft" && pAnimation->animation.getName() != "AtkRight")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("AtkRight"), true);
		m_player->getComponent<CTransform>()->scale.x = -1;
	}
	else if (pState == "RunLeft" && pAnimation->animation.getName() != "RunRight")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("RunRight"), true);
		m_player->getComponent<CTransform>()->scale.x = -1;
	}
	else if (pState == "StandLeft" && pAnimation->animation.getName() != "StandRight")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("StandRight"), true);
		m_player->getComponent<CTransform>()->scale.x = -1;
	}
	else if (pState == "AtkRight" && pAnimation->animation.getName() != "AtkRight")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("AtkRight"), true);
		m_player->getComponent<CTransform>()->scale.x = 1;
	}
	else if (pState == "RunRight" && pAnimation->animation.getName() != "RunRight")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("RunRight"), true);
		m_player->getComponent<CTransform>()->scale.x = 1;
	}
	else if (pState == "StandRight" && pAnimation->animation.getName() != "StandRight")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("StandRight"), true);
		m_player->getComponent<CTransform>()->scale.x = 1;
	}
	else if (pState == "AtkUp" && pAnimation->animation.getName() != "AtkUp")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("AtkUp"), true);
	}
	else if (pState == "RunUp" && pAnimation->animation.getName() != "RunUp")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("RunUp"), true);
	}
	else if (pState == "StandUp" && pAnimation->animation.getName() != "StandUp")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("StandUp"), true);
	}
	else if (pState == "AtkDown" && pAnimation->animation.getName() != "AtkDown")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("AtkDown"), true);
	}
	else if (pState == "RunDown" && pAnimation->animation.getName() != "RunDown")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("RunDown"), true);
	}
	else if (pState == "StandDown" && pAnimation->animation.getName() != "StandDown")
	{
		m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("StandDown"), true);
	}

	// update all animations
	for (auto ent : m_entityManager.getEntities())
	{
		if (ent->hasComponent<CAnimation>())
		{
			ent->getComponent<CAnimation>()->animation.update();
		}
	}
}

void GameState_Play::sUserInput()
{
	auto pInput = m_player->getComponent<CInput>();

	sf::Event event;
	while (m_game.window().pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_game.quit();
		}
		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
			{
				m_game.popState();
				break;
			}
			case sf::Keyboard::W:
			{
				pInput->up = true;
				break;
			}
			case sf::Keyboard::A:
			{
				pInput->left = true;
				break;
			}
			case sf::Keyboard::S:
			{
				pInput->down = true;
				break;
			}
			case sf::Keyboard::D:
			{
				pInput->right = true;
				break;
			}
			case sf::Keyboard::Z:
			{
				init(m_levelPath);
				break;
			}
			case sf::Keyboard::R:
			{
				m_drawTextures = !m_drawTextures;
				break;
			}
			case sf::Keyboard::F:
			{
				m_drawCollision = !m_drawCollision;
				break;
			}
			case sf::Keyboard::Y:
			{
				m_follow = !m_follow;
				break;
			}
			case sf::Keyboard::P:
			{
				setPaused(!m_paused);
				break;
			}
			case sf::Keyboard::Space:
			{
				// change the state of the player to the appropriate attack state
				auto pState = m_player->getComponent<CState>();
				if (pState->state == "StandDown" || pState->state == "RunDown")
				{
					pState->state = "AtkDown";
				}
				else if (pState->state == "StandUp" || pState->state == "RunUp")
				{
					pState->state = "AtkUp";
				}
				else if (pState->state == "StandLeft" || pState->state == "RunLeft")
				{
					pState->state = "AtkLeft";
				}
				else if (pState->state == "StandRight" || pState->state == "RunRight")
				{
					pState->state = "AtkRight";
				}

				// spawn the right weapon
				if (m_player->getComponent<CWeapon>()->weapon == "Sword")
				{
					spawnSword(m_player);
				}
				else if(m_player->getComponent<CWeapon>()->weapon == "Bow")
				{
					spawnBow(m_player);
				}
				break;
			}
			default:
			{
				break;
			}
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
			{
				pInput->up = false;
				break;
			}
			case sf::Keyboard::A:
			{
				pInput->left = false;
				break;
			}
			case sf::Keyboard::S:
			{
				pInput->down = false;
				break;
			}
			case sf::Keyboard::D:
			{
				pInput->right = false;
				break;
			}
			case sf::Keyboard::I:
			{
				//m_paused = true;
				m_game.pushState(std::make_shared<GameState_In_Menu>(m_game, m_player));
				//May need to unpause ourself
				break;
			}
			case sf::Keyboard::Space:
			{
				pInput->shoot = false;
				pInput->canShoot = true;
				break;
			}
			default:
			{
				break;
			}
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			//converting to the correct coords
			sf::Vector2i pixel_pos = sf::Mouse::getPosition(m_game.window());
			sf::Vector2f coord_pos = m_game.window().mapPixelToCoords(pixel_pos);
			std::cout << coord_pos.x << " " << coord_pos.y << "\n";
		}

	}
}

void GameState_Play::sRender()
{
	// TODO: set the window view correctly
	// create a view with the rectangular area of the 2D world to show
	sf::View view1;
	//it's only centering on the player now!! change this later on
	if (m_follow)
	{
		view1.setCenter(sf::Vector2f(m_player->getComponent<CTransform>()->pos.x, m_player->getComponent<CTransform>()->pos.y));
	}
	else
	{
		// set the initial view to the center of the map
		view1.setCenter(m_game.window().getSize().x/2.0f, m_game.window().getSize().y/2.0f);

		// find notlink's position with respect to the center room
		auto pTrans = m_player->getComponent<CTransform>();
		int rx = floor(pTrans->pos.x/m_game.window().getSize().x);
		int ry = floor(pTrans->pos.y/m_game.window().getSize().y);
		//std::cout << pTrans->pos.x << ';' << pTrans->pos.y << ';' << rx << ';' << ry << '\n';

		// reposition the camera in the correct room
		view1.move(((float)rx)*m_game.window().getSize().x, ((float)ry)*m_game.window().getSize().y);
	}
	view1.setSize(sf::Vector2f(m_game.window().getSize().x, m_game.window().getSize().y));
	m_game.window().clear(sf::Color(255, 192, 122));

	m_game.window().setView(view1);
	// draw all Entity textures / animations
	if (m_drawTextures)
	{
		for (auto e : m_entityManager.getEntities())
		{
			auto transform = e->getComponent<CTransform>();

			if (e->hasComponent<CAnimation>())
			{
				auto animation = e->getComponent<CAnimation>()->animation;
				animation.getSprite().setRotation(transform->angle);
				animation.getSprite().setPosition(transform->pos.x, transform->pos.y);
				animation.getSprite().setScale(transform->scale.x, transform->scale.y);
				m_game.window().draw(animation.getSprite());
			}
		}
	}

	// draw all Entity collision bounding boxes with a rectangleshape
	if (m_drawCollision)
	{
		sf::CircleShape dot(4);
		dot.setFillColor(sf::Color::Black);
		for (auto e : m_entityManager.getEntities())
		{
			if (e->hasComponent<CBoundingBox>())
			{
				auto box = e->getComponent<CBoundingBox>();
				auto transform = e->getComponent<CTransform>();
				sf::RectangleShape rect;
				rect.setSize(sf::Vector2f(box->size.x - 1, box->size.y - 1));
				rect.setOrigin(sf::Vector2f(box->halfSize.x, box->halfSize.y));
				rect.setPosition(transform->pos.x, transform->pos.y);
				rect.setFillColor(sf::Color(0, 0, 0, 0));

				if (box->blockMove && box->blockVision)
				{
					rect.setOutlineColor(sf::Color::Black);
				}
				if (box->blockMove && !box->blockVision)
				{
					rect.setOutlineColor(sf::Color::Blue);
				}
				if (!box->blockMove && box->blockVision)
				{
					rect.setOutlineColor(sf::Color::Red);
				}
				if (!box->blockMove && !box->blockVision)
				{
					rect.setOutlineColor(sf::Color::White);
				}
				rect.setOutlineThickness(1);
				m_game.window().draw(rect);
			}

			if (e->hasComponent<CPatrol>())
			{
				auto & patrol = e->getComponent<CPatrol>()->positions;
				for (size_t p = 0; p < patrol.size(); p++)
				{
					dot.setPosition(patrol[p].x, patrol[p].y);
					m_game.window().draw(dot);
				}
			}

			if (e->hasComponent<CFollowPlayer>())
			{
				sf::VertexArray lines(sf::LinesStrip, 2);
				lines[0].position.x = e->getComponent<CTransform>()->pos.x;
				lines[0].position.y = e->getComponent<CTransform>()->pos.y;
				lines[0].color = sf::Color::Black;
				lines[1].position.x = m_player->getComponent<CTransform>()->pos.x;
				lines[1].position.y = m_player->getComponent<CTransform>()->pos.y;
				lines[1].color = sf::Color::Black;
				m_game.window().draw(lines);
				dot.setPosition(e->getComponent<CFollowPlayer>()->home.x, e->getComponent<CFollowPlayer>()->home.y);
				m_game.window().draw(dot);
			}
		}
	}

	m_game.window().display();
}
