#include "networking.h"

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
            
            break;
        
        default:
            break;
        }
    }
}

void Networking::ParseData(char* data)
{
    int dataType;
    int id;
    sscanf(data, "%d|%d", &dataType, &id);

    switch (expression)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
}
