#pragma once
#include "SDL_net.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <iostream>
#include "Player.h"

class NetworkManager
{
public:
  NetworkManager();
  ~NetworkManager();

  void initConnection(const char* ip, const char* port);
  static void waitReady();
  void readPacket();
  void processPacket();
  static void disconnectSocket();

  static void sendPacket(const std::string packet);

  static TCPsocket s_Sock;
  static bool s_IsConnected;
  static bool s_IsGameReady;

private:
  void parsePacket();

  IPaddress m_Ip;
  Uint16 m_Port;
  char m_Message[1024];
  std::vector<std::string> m_Parsed;
  Player* m_nwPlayer;
  Player::PlayerSide m_localPlayer;
};

