#include "driver.h"

void Driver::Activate() {}
int Driver::Reset() { return 0; }
void Driver::Deactivate() {}

void DriverManager::AddDriver(Driver* driver)
{
    drivers[numDrivers++] = driver;
}

void DriverManager::ActivateAll()
{
    for (int i = 0; i < numDrivers; ++i) {
        drivers[i]->Activate();
    }
}