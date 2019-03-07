#pragma once

#include "Common.h"
#include "GameState.h"
#include "Assets.h"

#include <memory>

class GameEngine
{

protected:

    std::vector<std::shared_ptr<GameState>> m_states;
    std::vector<std::shared_ptr<GameState>> m_statesToPush;
	std::vector<std::string>				m_playerTreasures;
    sf::RenderWindow                        m_window;
    Assets                                  m_assets;
    size_t                                  m_popStates = 0;
    bool                                    m_running = true;

    void init(const std::string & path);
    void update();

    

public:
    
    GameEngine(const std::string & path);

    void pushState(std::shared_ptr<GameState> state);
    void popState();

	void addTreasures(const std::string newTreasure);

    void quit();
    void run();

    sf::RenderWindow & window();
    bool isRunning();

    const Assets & getAssets() const;
	const std::vector<std::string> & getTreasures() const;
};