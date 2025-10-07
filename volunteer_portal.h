#ifndef VOLUNTEER_PORTAL_H
#define VOLUNTEER_PORTAL_H

#include "Authentication.h"
#include "pet.h"
#include <vector>
#include <string>

class VolunteerPortal{
public:
    VolunteerPortal(const User& user);
    void runVolunteerMenu();

private:
    User currentUser;
    void viewMyTasks();
    void logActivity(const string& lineToLog);
    void markTaskAsComplete(int taskNumber, const vector<string>& userTasks);

    void viewKennelOccupancy();
    
    void performPetCare();
    Pet* findAndCreatePet(const string& petId);
    void logActivity(Pet* pet, const string& activity); //Fxn. overloading
};

#endif 