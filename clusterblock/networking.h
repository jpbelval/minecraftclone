#ifndef NETWORKING_H__
#define NETWORKING_H__

#include <enet/enet.h>
#include <iostream>
#include <map>
#include "playerdata.h"
#include "vector3.h"

class Networking
{
    private:
    int m_id;
    ENetHost* m_client;
    ENetAddress m_address;
    ENetPeer* m_hostPeer;
    std::map<int, PlayerData*> m_playersDict;
    Vector3f m_brokenBlockPosition;

    public:
    Networking(std::string ipAddress, int port); //Se connecte dans le constructeur

    ~Networking(); //Se déconnecte dans le destructeur

    int GetId();

    void SendPacket(char* data);

    void ParseData(char* data);

    Vector3f GetBrokenBlockPosition();

    void SetBrokenBlockPosition(Vector3f position);

    void ReceiveData(); //Devra être call a chaque frame

    int GetPlayerNumber();

    std::map<int, PlayerData*> GetPlayers();

};

#endif