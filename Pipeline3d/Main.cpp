#include "Application.h"
#include <exception>
#include <iostream>

int main()
{
    Application app;
    try
    {
        app.Init();
        app.Loop();
        app.Destroy();
    }
    catch (const std::exception& error)
    {
        std::cerr << "Erreur : " << error.what() << std::endl;
        app.Destroy();
        return 1;
    }
    return 0;
}
