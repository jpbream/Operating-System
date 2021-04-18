#ifndef _DRIVER_H_
#define _DRIVER_H_

class Driver
{
public:

    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();

};

class DriverManager
{
private:
    Driver* drivers[255];
    int numDrivers;

public:
    void AddDriver(Driver* driver);
    void ActivateAll();
};

#endif