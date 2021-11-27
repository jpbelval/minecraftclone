
#include "server.h"
#include <enet/enet.h>
#include <stdio.h>
#include <string.h>

Server::Server()
{
    try
    {
        //Initialisation de ENet
        if(enet_initialize() != 0){
            throw("Can't initialize ENet!");
        }

        atexit(enet_deinitialize);

        //Creation du serveur en host
        m_address.host = ENET_HOST_ANY;
        m_address.port = 7777;

        m_server = enet_host_create(&m_address, 32, 1, 0, 0);

        if (m_server == NULL)
        {
            throw("Can't create ENet Server!");
        }
    }
    catch(std::string errorMessage)
    {
        std::cerr << errorMessage << '\n';
        m_initialized = false;
    }

}

Server::~Server() {}

void Server::BroadcastPacket(const char *data)
{
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(m_server, 0, packet);
}

void Server::SendPacket(ENetPeer* peer, const char* data){
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Server::ParseData(int id, char *data)
{
    std::cout << "Phrase: " << data << std::endl;

    int dataType;

    sscanf(data, "%d|", &dataType);

    switch(dataType)
    {
        case 1:
        {
            char msg[80];
            sscanf(data, "%*d|%[^\n]", &msg);

            BroadcastPacket(msg);
        }
        break;
        case 2:
        {
            // char username[80];
            // sscanf(data, "2|%[^\n]", &username);
            // std::cout << "USERNAME: " << username << "\n";

            // char send_data[1024] = {'\0'};
            // sprintf(send_data, "2|%d|%s", id, username);
            // std::cout << "SEND: " << send_data << "\n";
            // BroadcastPacket(send_data);

            break;
        }
    }
}

void Server::StartLoop()
{
    ENetEvent event;
    int new_player_id = 0;
    while(true)
    {
        while(enet_host_service(m_server, &event, 1000) > 0)
        {
            switch(event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    printf("A new client connected from %x:%u.\n",
                        event.peer -> address.host,
                        event.peer -> address.port);

                        for(auto const& x : m_playersDict)
                        {
                            char send_data[1024] = {'\0'};
                            sprintf(send_data, "3|%d", x.first);
                            BroadcastPacket(send_data);
                        }

                        new_player_id++;
                        m_playersDict[new_player_id] = new PlayerData(new_player_id);
                        event.peer->data = m_playersDict[new_player_id];

                        char data_to_send[126] = {'\0'};
                        sprintf(data_to_send, "4|%d", new_player_id);
                        SendPacket(event.peer, data_to_send);
                    break;

                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    printf ("A packet of length %u containing %s was received from %x:%u on channel %u.\n",
                    event.packet -> dataLength,
                    event.packet -> data,
                    event.peer -> address.host,
                    event.peer -> address.port,
                    event.channelID);
                    
                    ParseData(static_cast<PlayerData*>(event.peer->data)->GetId(), (char*)event.packet->data);
                    enet_packet_destroy(event.packet);

                }
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    printf("%x:%u disconnected.\n",
                            event.peer -> address.host,
                            event.peer -> address.port);

                    char send_data[126] = {'\0'};
                    sprintf(send_data, "4|%d", static_cast<PlayerData*>(event.peer->data)->GetId());
                    BroadcastPacket(send_data);

                    event.peer->data = NULL;
                    break;

                }
            }
        }

    }

    enet_host_destroy(m_server);
}