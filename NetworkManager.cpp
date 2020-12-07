#define _CRT_SECURE_NO_WARNINGS
#include "NetworkManager.h"
#include "Engine.h"
#include "Block.h"

/*
* NETCODE OVERVIEW
* 
* Basically need to reproduce projectiles on both sides
* 
* (All incoming packets will modify networked player)
* 
* Use of packet messaging doesn't mean we no longer 
* do client side validation
* 
* OPCODE 0 // The only message sent solely by server
*		INIT_GAME | L || R | NUM_BLOCKS | (x,y,w,h)...
* 
* OPCODE 1
*		SET_ANGLE | THETA
*		nwPlayer.setCannonAngle(theta);
* 
* OPCODE 2 
*		LOAD_CANNON | NULL
*		nwPlayer.loadCannon();
* 
* OPCODE 3
*		FIRE_CANNON | POWER
*		nwPlayer.shootCannon(power);
* 
*/

TCPsocket NetworkManager::s_Sock = nullptr;
bool NetworkManager::s_IsConnected = false;
bool NetworkManager::s_IsGameReady = false;

NetworkManager::NetworkManager() {
	m_nwPlayer = nullptr;
}
NetworkManager::~NetworkManager() {
	disconnectSocket();
	SDLNet_Quit();
}

void NetworkManager::disconnectSocket() {
	SDLNet_TCP_Close(s_Sock);
	s_IsConnected = false;
	s_IsGameReady = false;
}

void NetworkManager::initConnection(const char* ip, const char* port) {
	m_Port = (Uint16)strtol(port, NULL, 0);

	/* initialize SDL_net */
	if (SDLNet_Init() == -1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}
	// Resolve IP
	if (SDLNet_ResolveHost(&m_Ip, ip, m_Port) == -1)
	{
		printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(3);
	}

	// Open socket
	s_Sock = SDLNet_TCP_Open(&m_Ip);
	if (!s_Sock)
	{
		printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(4);
	}

	s_IsConnected = true;
}

void NetworkManager::waitReady() {
	while (!s_IsGameReady) {
    std::cout << "NETWORK: Waiting for game to start..." << std::endl;
		SDL_Delay(1000);
	}
}

void NetworkManager::sendPacket(const std::string packet) {
	int result = SDLNet_TCP_Send(s_Sock, packet.c_str(), packet.length());

	if (result < packet.length()) {
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		exit(4);
	}
	else {
    std::cout << "NETWORK: Sending Pakcet: " << packet << std::endl;
	}

}

void NetworkManager::readPacket() {
	std::cout << "NETWORK: Listening for packet..." << std::endl;
	int result = SDLNet_TCP_Recv(s_Sock, m_Message, 1024);

	if (result <= 0) {
		printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
		disconnectSocket();
	}

  std::cout << "NETWORK: Got packet: " << m_Message << std::endl;
}

void NetworkManager::parsePacket() {
	// Parse the message stored in m_Message
	char* iter = strtok(m_Message, ",");
	while (iter != NULL)
	{
		m_Parsed.push_back(std::string (iter));
		iter = strtok(NULL, ",");
	}
	// Clear message
	memset(m_Message, 0, 1024);
}

void NetworkManager::processPacket() {
	if (!NetworkManager::s_IsConnected) return;

	parsePacket();

	std::string OP = m_Parsed.at(0);
	if (OP == "0") {
		// Game Initialisation
    std::cout << "NETWORK: Game Initialisation Started" << std::endl;

		// Server will tell us either "L" or "R" to tell us which player we control
		if (m_Parsed.at(1) == "L") {
      Engine::addPlayer(Player::PlayerSide::LEFT, false);
      Engine::addPlayer(Player::PlayerSide::RIGHT, true, &m_nwPlayer);
		}
		else {
      Engine::addPlayer(Player::PlayerSide::LEFT, true, &m_nwPlayer);
      Engine::addPlayer(Player::PlayerSide::RIGHT, false);
		}
    std::cout << "NETWORK: Game Initialisation Done" << std::endl;

		// Add Blocks
		int numBlocks = stoi(m_Parsed.at(2));
		int iter = 3;
		for (int i = 0; i < numBlocks; i++) {
			int xoff = stoi(m_Parsed.at(iter));
			int y = stoi(m_Parsed.at(iter + 1));
      int w = stoi(m_Parsed.at(iter + 2));
      int h =  stoi(m_Parsed.at(iter + 3));

      Block* l = new Block("assets/block.png", xoff - (w / 2), y, w, h);
      Engine::s_GameObjects.push_back(l);
      Engine::s_EntityList.push_back(l);
      Block* r = new Block("assets/block.png", 1200 - xoff - (w / 2), y, w, h);
      Engine::s_GameObjects.push_back(r);
      Engine::s_EntityList.push_back(r);

			iter += 4;
		}

		s_IsGameReady = true;
	}
	else if (OP == "1") {
		// Set Angle
		m_nwPlayer->setCannonAngle(stoi(m_Parsed.at(1)));
	}
	else if (OP == "2") {
		// Load Cannon
		m_nwPlayer->loadCannon();
	}
	else if (OP == "3") {
		// Fire Cannon
		m_nwPlayer->shootCannon(std::stoi(m_Parsed.at(1)));
	}
	
	m_Parsed.clear();
}
