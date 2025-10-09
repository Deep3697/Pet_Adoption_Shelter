#ifndef PEOPLE_MANAGER_H
#define PEOPLE_MANAGER_H

#include "Authentication.h"

class PeopleManager{
public:
    void showPeopleManagementMenu(const User& loggedInUser);

private:
    void registerNewPerson();
    void registerNewAdopter();
    void registerNewWorker();
    void listAllAdopters();
    void listAllVolunteers();
    string generateNewPersonId(const string& filename,const string& prefix);
    int getLastIdFromFile(const string& filename,const string& prefix);

    void viewAndEditPersonProfile();
    void displayInteractionHistory(const string& personId);
    void displayAdoptionHistory(const string& adopterId);
    void editPersonDetails(const string& personId, const string& filename);

    void assignTaskToVolunteer();
    bool findVolunteerById(const string& volId, string& outName);

    bool findPersonById(const string& personId, const string& filename, string& outName);
    bool findPetById(const string& petId, string& outName, string& outSpecies);
};

#endif