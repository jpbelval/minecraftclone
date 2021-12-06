#include <SFML/Window.hpp>
#include <iostream>
#include "engine.h"
#include <string>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string ipAddress(argv[1]);
        Engine engine;
        engine.SetIpAdress(ipAddress);
        engine.SetMaxFps(60);
        engine.Start("clusterblock", 800, 600, false);
    }
    else
    {
        std::cout << "Veuillez entrer une adresse ip!" << std::endl;
    }
}
