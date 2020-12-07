/*
 * Cannon Commando
 * jnasser
 * Fall 2020 - CPSC 4160
*/

#include "Engine.h"
#include "NetworkManager.h"
#include "GameManager.h"
#include "Constants.h"
#include <thread>

/*
* Function that runs on a separate thread,
* responsible for incoming network messages
*/
void listenNetwork(NetworkManager* net) {
	std::cout << "INFO: Beginning network thread." << std::endl;
	while (net && NetworkManager::s_IsConnected) {
		net->readPacket();
		net->processPacket();
	}
	std::cout << "INFO: Terminating network thread." << std::endl;
}

int main(int argc, char* argv[]) {

	bool isMp = argc == 3;

	std::thread netWorker;

	GameManager* gm = new GameManager();
  NetworkManager *net = new NetworkManager();
	Engine *ngn = new Engine();

  ngn->initEngine(isMp);

	std::cout << "INFO: Finished initializing engine." << std::endl;

	//-------------------
	// Game Loop
	//-------------------
	do {
		// Add all the entities possible depending on mp or not
		ngn->prepareMatch();

    // Multiplayer Game Creation
    if (isMp) {
      Engine::renderScreen("assets/waitserver.png");
			net->initConnection(argv[1], argv[2]);
      netWorker = std::thread(listenNetwork, net);
      net->waitReady();
    }

    gm->trackPlayers(Engine::s_P1, Engine::s_P2);

    std::cout << "INFO: Game is starting!" << std::endl;
    while (Engine::s_IsGameRunning) {
      Uint32 startTime = SDL_GetTicks();
      
      ngn->handleInput();
      ngn->updateEngine();
      ngn->renderEngine();
			gm->checkGameState();
      
      Uint32 duration = SDL_GetTicks() - startTime;
      if (duration < CONSTANTS::FRAME_DURATION) {
        SDL_Delay(CONSTANTS::FRAME_DURATION - duration);
      }
    }

		if(isMp) netWorker.join();
		gm->getInput();

	} while (gm->isPlayAgain());

	// Clean up
	delete ngn;
	delete gm;
	if (isMp) delete net;

	return 0;
}
