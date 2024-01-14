#include "Application.h"

void Application::Run()
{
    //start func call
    Startup();

    // Main loop
   
    while (!IsRunning)
    {
        Loop();
    }
};



void Application::Startup()
{

};

void Application::Loop()
{
    InputLoop();
    
};

void Application::InputLoop()
{
    
};

Application::~Application()
{
    
};
