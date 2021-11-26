#ifndef NETWORKING_H__
#define NETWORKING_H__

#include <enet/enet.h>

class Networking
{
    private:
    int m_id;

    public:
    Networking(); //Se connecte dans le constructeur

    ~Networking(); //Se déconnecte dans le destructeur

    int GetId() {return m_id;}

    void SendPacket(ENetPeer* peer, char* data);

    void ParseData(char* data);

    void ReceiveData(ENetHost* host); //Devra être call a chaque frame

};

#endif