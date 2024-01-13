#pragma once
class Application
{
public:
    Application() {};
    ~Application();
    virtual void Run();
protected:
    virtual void Startup();
    virtual void Loop();
    virtual void InputLoop();
    

protected:
    bool IsRunning = false;
};

