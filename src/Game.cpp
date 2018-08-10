#include"../include/Game.h"

using namespace bear;

Game::Game() :
	window(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE) // Create window
{
	// Init graphics
	if (!Graphics::init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
		std::cout << "Failed to init Graphics module\n";
	}

	switchGameState(GAME_STATE::PLAYING); // setup for default game state
	mainLoop();
}

Game::~Game()
{

}

void Game::eventInput(bear::Event &event)
{
	if (event.type == EventType::WindowReiszed) {
		Graphics::window_resize_callback(event.size.x, event.size.y);
	}
}

void Game::switchGameState(GAME_STATE new_state)
{
	game_state = new_state;
	switch (game_state) {
	case GAME_STATE::PLAYING:
		setupPlaying();
	}
}

void Game::mainLoop()
{
	while (window.isOpen()) {
		window.clear(clear_color);

		for (bear::Event event : window.getRegisteredEvents())
			eventInput(event);

		// State machine for what state the game is in and call the appropiate methods
		switch (game_state) {
		case GAME_STATE::PLAYING:
			gameLoop();
		}

		window.display();
	}
}

void Game::setupPlaying()
{
	std::cout << "setupPlaying() executed\n";
}

void Game::gameLoop()
{

}