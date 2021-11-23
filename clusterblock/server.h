#ifndef SERVER_H__
#define SERVER_H__
#include <enet/enet.h>

class Server
{
private:
    ENetAddress address;
    ENetHost* serverHost;

public:
    Server(/* args */);
    ~Server();
};

Server::Server(/* args */)
{
    if (enet_initialize() != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    address.host = ENET_HOST_ANY;
    address.port = 8123;

    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL)
    {
        fprintf (stderr, 
                "An error occurred while trying to create an ENet server host.\n");
        exit (EXIT_FAILURE);
    }

}

Server::~Server()
{
    enet_host_destroy(server);
    enet_deinitialize();
}

#endif