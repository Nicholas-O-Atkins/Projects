#include "GameState_Play.h"
#include "GameState_In_Menu.h"
#include "Common.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"

GameState_In_Menu::GameState_In_Menu(GameEngine & game, std::shared_ptr<Entity> & player)
	: GameState(game)
	, m_player(player)
{
	init(m_player);
}

void GameState_In_Menu::init(std::shared_ptr<Entity> & player) \
{
	m_title = "Weapon Selection";

	m_weaponStrings.push_back("Sword");
	m_weaponStrings.push_back("Bow");

	m_menuText.setFont(m_game.getAssets().getFont("Mario"));
	m_menuText.setCharacterSize(64);
}

void GameState_In_Menu::update()
{
	m_entityManager.update();

	sUserInput();
	sRender();
}

void GameState_In_Menu::sUserInput()
{
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
				if (m_selectedIndex > 0) { m_selectedIndex--; }
				else { m_selectedIndex = m_weaponStrings.size() - 1; }
				break;
			}
			case sf::Keyboard::S:
			{
				m_selectedIndex = (m_selectedIndex + 1) % m_weaponStrings.size();
				break;
			}
			case sf::Keyboard::D:
			{
				m_player->getComponent<CWeapon>()->weapon = m_weaponStrings[m_selectedIndex];
				m_game.popState();
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
}

void GameState_In_Menu::sRender()
{
	// clear the window to a blue
	m_game.window().setView(m_game.window().getDefaultView());
	m_game.window().clear(sf::Color(0, 0, 0));

	// draw the game title in the top-left of the screen
	m_menuText.setCharacterSize(48);
	m_menuText.setString(m_title);
	m_menuText.setFillColor(sf::Color::White);
	m_menuText.setPosition(sf::Vector2f(10, 10));
	m_game.window().draw(m_menuText);

	// draw all of the menu options
	for (size_t i = 0; i < m_weaponStrings.size(); i++)
	{
		m_menuText.setString(m_weaponStrings[i]);
		m_menuText.setFillColor(i == m_selectedIndex ? sf::Color::White : sf::Color(100, 100, 100));
		m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
		m_game.window().draw(m_menuText);
	}

	// draw the controls in the bottom-left
	m_menuText.setCharacterSize(20);
	m_menuText.setFillColor(sf::Color(100, 100, 100));
	m_menuText.setString("up: w     down: s    select: d      back: esc");
	m_menuText.setPosition(sf::Vector2f(10, 730));
	m_game.window().draw(m_menuText);

	m_game.window().display();
}