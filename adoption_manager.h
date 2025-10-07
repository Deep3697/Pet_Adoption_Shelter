#ifndef ADOPTION_MANAGER_H
#define ADOPTION_MANAGER_H

#include "Authentication.h" 
#include "pet.h"

class AdoptionManager{
public:
    AdoptionManager(const User& user, AdoptionShelter& shelter);
    void showAdoptionWorkflowMenu(); 

private:
    User currentUser;
    AdoptionShelter& shelter;
    
    void submitNewApplication();
    bool isAdopterValid(const string& adopterId, string& outAdopterName);
    bool isPetAvailable(const string& petId, string& outPetName);
    string generateNewAppId();

    void viewPendingApplications();

    void reviewApplication();
    void updateApplicationStatus(const string& appId, const string& newStatus);

    void viewAdoptionHistory();

};

#endif