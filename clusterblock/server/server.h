#ifndef SERVER_H__
#define SERVER_H__


#include <enet/enet.h>
#include "../playerdata.h"
#include <map>

class Server
{
    private:
    bool m_initialized;
    ENetHost* m_server;
    ENetAddress m_address;
    std::map<int, PlayerData*> m_playersDict;

    public:
    Server();
    ~Server();

    void BroadcastPacket(const char* data);

    void SendPacket(ENetPeer* peer, const char* data);

    void ParseData(int id, char* data);

    void StartLoop();
};

#endif