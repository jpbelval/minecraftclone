#ifndef NETWORKING_H__
#define NETWORKING_H__

#include <enet/enet.h>
#include <iostream>

class Networking
{
    private:
    int m_id;
    ENetHost* m_client;
    ENetAddress m_address;
    ENetPeer* m_hostPeer;

    public:
    Networking(std::string ipAddress, int port); //Se connecte dans le constructeur

    ~Networking(); //Se déconnecte dans le destructeur

    int GetId();

    void SendPacket(char* data);

    void ParseData(char* data);

    void ReceiveData(); //Devra être call a chaque frame

};

#endif