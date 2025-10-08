#ifndef FACILITY_MANAGER_H
#define FACILITY_MANAGER_H

#include<bits/stdc++.h>
#include "Authentication.h"
using namespace std;

class FacilityManager{
public:
    FacilityManager(const User& user);
    void showFacilityManagementMenu();

private:
    User currentUser;

    void scheduleKennelCleaning();
    bool findVolunteerById(const string& volId, string& outName);
    bool findPetById(const string& petId, string& outName, string& outSpecies);


    // Manager-specific methods
    void assignNewMaintenanceRequest();
    void viewAllPendingRequests();

    // Staff-specific methods
    void viewMyAssignedRequests();
    void markRequestAsComplete(const string& requestId);

    // Shared method
    void viewMaintenanceHistory();

    // Helper methods
    bool findStaffById(const string& staffId, string& outName);
    string generateNewRequestId();

    void viewKennelOccupancy(); 
};

#endif