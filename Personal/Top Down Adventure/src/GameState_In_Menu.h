#pragma once

#include "Common.h"
#include "GameState.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"

class GameState_In_Menu : public GameState
{
protected:

	EntityManager					m_entityManager;
	std::string						m_title;
	std::vector<std::string>		m_weaponStrings;
	std::shared_ptr<Entity>			m_player;
	sf::Text						m_menuText;
	size_t							m_selectedIndex = 0;

	void init(std::shared_ptr<Entity> & player);
	void update();
	void sUserInput();
	void sRender();


public:

	GameState_In_Menu(GameEngine & game, std::shared_ptr<Entity> & player);
};