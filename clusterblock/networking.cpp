#include "networking.h"
#include "playerdata.h"
#include "vector3.h"
#include <cstdio>
#include <enet/enet.h>
#include <stdio.h>
#include <string.h>

Networking::Networking(std::string ipAddress, int port)
{
    try
    {
        //Initialisation de ENet
        if(enet_initialize() != 0){
            throw("Can't initialize ENet!");
        }

        atexit(enet_deinitialize);

        //Creation du host pour le client
        m_client = enet_host_create(NULL, 1, 1, 0, 0);

        if (m_client == NULL)
        {
            throw("Can't create ENet Client!");
        }
            
        enet_address_set_host(&m_address, ipAddress.c_str());
        m_address.port = port;


        //Connection au peer du host
        m_hostPeer = enet_host_connect(m_client, &m_address, 1, 0);

        if(m_hostPeer == NULL)
        {
            throw("No peer available for connection!");
        }
        ENetEvent event;
        //Attendre retour de connection du host
        if (enet_host_service(m_client, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            std::cout << "Connected to " << ipAddress << ":" << std::to_string(port) << std::endl;
        }
        else
        {
            enet_peer_reset(m_hostPeer);
            throw("Connection failed");
        }
    }
    catch(std::string errorMessage)
    {
        std::cerr << errorMessage << '\n';
    }

}

Networking::~Networking()
{
    ENetEvent event;

    enet_peer_disconnect(m_hostPeer, 0);

    while(enet_host_service(m_client, &event, 3000) > 0)
    {
        switch(event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts("DISCONNECTION SUCCEEDED.");
                break;
        }
    }
}

int Networking::GetId()
{
    return m_id;
}

void Networking::ReceiveData()
{
    ENetEvent event;

    while(enet_host_service(m_client, &event, 0) > 0) //Tester sans le while
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            ParseData((char*)event.packet->data);
            enet_packet_destroy(event.packet);
            break;
        
        }
    }
}

void Networking::ParseData(char* data)
{
    int dataType;
    int id;
    sscanf(data, "%d|%d", &dataType, &id);

    switch (dataType)
    {
        case 1: //Reception de position
        {
            if (id != m_id) {
                int x;
                int y;
                int z;
                sscanf(data, "%*d|%*d|%dx%dy%dz", &x, &y, &z);
                m_playersDict[id]->SetPosition(Vector3f(x,y,z));
            }
            break;
        }
        case 2: //Reception bloc brisé
        {
            if (id != m_id) {
                int x;
                int y;
                int z;
                sscanf(data, "%*d|%*d|%dx%dy%dz", &x, &y, &z);
                m_brokenBlockPosition = Vector3f(x,y,z);
            }
            break;
        }
        case 3: //Reception connexion autre user
        {
            if(id != m_id)
            {
                m_playersDict[id] = new PlayerData(id);
            }
            break;
        }
        case 4: //Reception id
        {
            m_id = id;
            break;
        }
    }
}

void Networking::SendPacket(char *data)
{
    ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(m_hostPeer, 0, packet);
}

Vector3f Networking::GetBrokenBlockPosition()
{
    return m_brokenBlockPosition;
}

void Networking::SetBrokenBlockPosition(Vector3f position)
{
    m_brokenBlockPosition = position;
}
