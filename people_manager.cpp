#include<bits/stdc++.h>
using namespace std;
#include "people_manager.h"


void PeopleManager::editPersonDetails(const string& personId,const string& filename){
    ifstream inputFile(filename);
    ofstream tempFile("temp.csv");
    if(!inputFile.is_open()||!tempFile.is_open()){cout<<"Error opening files.\n"; return;}

    string line;
    while(getline(inputFile,line)){
        string id;
        // Create a copy of the stream to check the ID first coz we are doing by visiting the profile for that ID
        stringstream ss_check(line);
        getline(ss_check,id,',');

        if(id==personId){
            string newName,newAddress,newPhone;
            cout<<"\n--- Editing Details for ID "<<id<<" ---\n";
            cout<<"Enter New Name: ";
            getline(cin,newName);

            if(filename=="adopters.csv"){
                cout<<"Enter New Address: ";
                getline(cin,newAddress);
                cout<<"Enter New Phone: ";
                getline(cin,newPhone);
                tempFile<<id<<","<<newName<<","<<newAddress<<","<<newPhone<<"\n";
            }
            else{
                stringstream ss_original(line);
                string oldId,oldPassword,oldName,oldRole;
                getline(ss_original,oldId,',');
                getline(ss_original,oldPassword,',');
                getline(ss_original,oldName,',');
                getline(ss_original,oldRole,',');
                tempFile<<id<<","<<oldPassword<<","<<newName<<","<<oldRole<<"\n";
            }
            cout<<"Details updated.\n";
        }else{
            tempFile<<line<<"\n"; // Copy unchanged line to the tempfile then we will rename it and delete old one
        }
    }
    inputFile.close();
    tempFile.close();
    remove(filename.c_str());
    rename("temp.csv",filename.c_str());
}

void PeopleManager::displayInteractionHistory(const string& personId){
    ifstream logFile("interactions.csv");
    if(!logFile.is_open()) return; 

    cout<<"\n---- Interaction History ----\n";
    string line;
    bool foundLogs=false;
    while(getline(logFile,line)){
        stringstream ss(line);
        string petId,petName,activity,userId,userName;
        getline(ss,petId,','); 
        getline(ss,petName,','); 
        getline(ss,activity,',');
        getline(ss,userId,',');

        if(userId==personId){
            foundLogs=true;
            getline(ss,userName,',');
            cout<<"- Activity: "<<activity<<" with Pet: "<<petName<<" (ID: "<<petId<<")"<<endl;
        }
    }
    if(!foundLogs){
        cout<<"No interactions logged for this person.\n";
    }
    logFile.close();
}



void PeopleManager::displayAdoptionHistory(const string& adopterId){
    ifstream appFile("applications.csv");
    if(!appFile.is_open()){ return; }

    cout<<"\n---- Adoption History ----\n";
    string line;
    bool foundHistory=false;
    while(getline(appFile,line)){
        stringstream ss(line);
        string appId, currentAdopterId, adopterName, petId, petName, status;
        getline(ss,appId,',');
        getline(ss,currentAdopterId,',');
        getline(ss,adopterName,',');
        getline(ss,petId,',');
        getline(ss,petName,',');
        getline(ss,status,',');

        if(currentAdopterId==adopterId && status=="Approved"){
            foundHistory=true;
            cout<<"- Adopted Pet: "<<petName<<" (ID: "<<petId<<")\n";
        }
    }
    if(!foundHistory){
        cout<<"No approved adoption history found for this person.\n";
    }
    appFile.close();
}

void PeopleManager::viewAndEditPersonProfile(){
    string searchId;
    cout<<"\nEnter the ID of the person to view (e.g., 101, X102): ";
    getline(cin,searchId);
    string filename;
    ifstream file;
    //if prefix ==x so open volunteers.csv file else users.csv
    if(searchId.rfind("X",0)==0){
        filename="adopters.csv";
    }else{
        filename="users.csv";
    }
    file.open(filename);

    if(!file.is_open()){
        cout<<"Error opening data file.\n";
        return;
    }

    string line;
    bool found=false;
    while(getline(file,line)){
        stringstream ss(line);
        string id;
        getline(ss,id,',');

        if(id==searchId){
            found=true;
            system("cls");
            cout<<"---- Profile: ----\n";
            if(filename=="adopters.csv"){
                string name,address,phone;
                getline(ss,name,','); getline(ss,address,','); getline(ss,phone,',');
                cout<<"ID:"<<id<<endl<<"Name: "<<name<<endl<<"Address: "<<address<<endl<<"Phone: "<<phone<<endl;
            }else{
                string password,name,role;
                getline(ss,password,','); getline(ss,name,','); getline(ss,role,',');
                cout<<"ID:"<<id<<endl<<"Name: "<<name<<endl<<"Role: "<<role<<endl;
            }

            // --- THIS IS THE CHANGED PART ---
            // It now checks which type of person it is and shows the correct history.
            if(filename=="adopters.csv"){
                displayAdoptionHistory(id);
            }else{
                displayInteractionHistory(id);
            }
            cout<<"\n---- Options: ----\n";
            cout<<"1. Edit This Person's Details\n";
            cout<<"2. Go Back\n";
            cout<<"Enter your choice: ";
            int choice=0;
            cin>>choice;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            if(choice==1){
                editPersonDetails(id,filename);
            }
            break; 
        }
    }
    file.close();
    if(!found){
        cout<<"Person with ID "<<searchId<<" not found.\n";
    }
}


void PeopleManager::listAllVolunteers(){
    ifstream userFile("users.csv");
    string line;

    if(!userFile.is_open()){
        cout<<"\nError: Could not open users.csv.\n";
        return;
    }

    cout<<"\n----- List of All Registered Volunteers -----\n";
    cout<<"--------------------------------------------------------------------\n";
    cout<<left<<setw(10)<<"ID"<<setw(30)<<"Name"<<setw(15)<<"Role"<<"\n";
    cout<<"--------------------------------------------------------------------\n";

    while(getline(userFile,line)){
        stringstream ss(line);
        string id,password,name,role;
        getline(ss,id,',');
        getline(ss,password,',');
        getline(ss,name,',');
        getline(ss,role,',');

        if(role=="Volunteer"){
            cout<<left<<setw(10)<<id<<setw(30)<<name<<setw(15)<<role<<"\n";
        }
    }
    cout<<"-------------------------------------------------------------------------\n";
    userFile.close();
}

void PeopleManager::listAllAdopters(){
    ifstream adopterFile("adopters.csv");
    string line;

    if(!adopterFile.is_open()){
        cout<<"\nError: Could not open adopters.csv. No adopters registered yet.\n";
        return;
    }

    cout<<"\n----- List of All Registered Adopters -----\n";
    cout<<"----------------------------------------------------------------------\n";
    cout<<left<<setw(10)<<"ID"<<setw(25)<<"Name"<<setw(30)<<"Address"<<setw(15)<<"Phone"<<"\n";
    cout<<"----------------------------------------------------------------------\n";

    while(getline(adopterFile,line)){
        stringstream ss(line);
        string id,name,address,phone;
        getline(ss,id,',');
        getline(ss,name,',');
        getline(ss,address,',');
        getline(ss,phone,',');

        cout<<left<<setw(10)<<id<<setw(25)<<name<<setw(30)<<address<<setw(15)<<phone<<"\n";
    }
    cout<<"-------------------------------------------------------------------------\n";
    adopterFile.close();
}

int PeopleManager::getLastIdFromFile(const string& filename, const string& prefix){
    ifstream file(filename);
    string line;
    int maxId=0;
    if(!file.is_open()){return 100;}
    while(getline(file,line)){
        if(line.empty() || line.rfind(prefix,0)!=0) continue;
        try{
            maxId=max(maxId,stoi(line.substr(prefix.length())));
        }catch(...){}
    }
    file.close();
    return maxId==0 ? 100 : maxId;
}

string PeopleManager::generateNewPersonId(const string& filename, const string& prefix){
    int lastId=getLastIdFromFile(filename, prefix);
    return prefix+to_string(lastId+1);
}


void PeopleManager::registerNewAdopter(){
    string name, address, phone;
    cout<<"\n----- Register New Adopter -----\n";
    cout<<"Enter Name: ";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    getline(cin,name);
    cout<<"Enter Address: ";
    getline(cin,address);
    cout<<"Enter Phone Number: ";
    getline(cin,phone);

    string newId=generateNewPersonId("adopters.csv","X");

    ofstream adopterFile("adopters.csv",ios::app);
    if(adopterFile.is_open()){
        adopterFile<<newId<<","<<name<<","<<address<<","<<phone<<"\n";
        adopterFile.close();
        cout<<"\nAdopter '"<<name<<"' registered successfully with ID: "<<newId<<endl;
    }else{
        cout<<"\nError: Could not open adopters.csv file.\n";
    }
}

void PeopleManager::registerNewWorker(){
    string name, password, roleStr;
    int roleChoice;

    cout<<"\n----- Register New Worker -----\n";
    cout<<"Enter Name: ";
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    getline(cin,name);
    cout<<"Enter a Password: ";
    getline(cin,password);
    cout<<"Select a Role:\n1. Manager\n2. Staff\n3. Volunteer\nChoice: ";
    cin>>roleChoice;

    switch(roleChoice){
        case 1: roleStr="Manager"; break;
        case 2: roleStr="Staff"; break;
        case 3: roleStr="Volunteer"; break;
        default: cout<<"Invalid role. Aborting.\n"; return;
    }

    string newId=generateNewPersonId("users.csv","");

    ofstream userFile("users.csv",ios::app);
    if(userFile.is_open()){
        userFile<<newId<<","<<password<<","<<name<<","<<roleStr<<"\n";
        userFile.close();
        cout<<"\nWorker '"<<name<<"' registered successfully with ID: "<<newId<<endl;
    }else{
        cout<<"\nError: Could not open users.csv file.\n";
    }
}

void PeopleManager::registerNewPerson(){
    int choice=0;
    system("cls");
    cout<<"----- Register New Person -----\n";
    cout<<"1. Register New Adopter\n";
    cout<<"2. Register New Worker (Manager/Staff/Volunteer)\n";
    cout<<"3. Go Back\n";
    cout<<"Enter your choice: ";
    cin>>choice;

    switch(choice){
        case 1:
            registerNewAdopter();
            break;
        case 2:
            registerNewWorker();
            break;
        case 3:
            return;
        default:
            cout<<"Invalid choice.\n";
            break;
    }
}

bool PeopleManager::findPersonById(const string& personId, const string& filename, string& outName){
    ifstream file(filename);
    if(!file.is_open()){return false;}
    string line;
    while(getline(file,line)){
        stringstream ss(line);
        string id, password, name;
        getline(ss,id,',');
        if(id==personId){
            getline(ss,password,',');
            getline(ss,name,',');
            outName=name;
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool PeopleManager::findPetById(const string& petId, string& outName, string& outSpecies){
    ifstream file("pets.csv");
    if(!file.is_open()){return false;}
    string line;
    while(getline(file,line)){
        stringstream ss(line);
        string id, name, species;
        getline(ss,id,',');
        if(id==petId){
            getline(ss,name,',');
            getline(ss,species,',');
            outName=name;
            outSpecies=species;
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void PeopleManager::assignTaskToVolunteer(){
    string volId, petId, volName, petName, petSpecies;
    cout<<"\n---- Assign Task ----\n";
    cout<<"Enter ID of Volunteer to assign task to: ";
    getline(cin,volId);
    if(!findPersonById(volId,"users.csv",volName)){
        cout<<"Error: Volunteer with ID "<<volId<<" not found."<<endl;
        return;
    }

    cout<<"Enter ID of Pet for the task: ";
    getline(cin,petId);
    if(!findPetById(petId, petName, petSpecies)){
        cout<<"Error: Pet with ID "<<petId<<" not found.\n";
        return;
    }

    int choice=0;
    string taskDescription;
    system("cls");
    cout<<"Assigning task for Pet: "<<petName<<" ("<<petSpecies<<")\n";
    cout<<"Select a task:\n";
    // --- DOG TASKS ---
    if(petSpecies=="Dog"){
        cout<<"1. Clean Enclosure\n";
        cout<<"2. Walk for 20 minutes\n";
        cout<<"3. Play fetch\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Enclosure";
        else if(choice==2) taskDescription="Walk for 20 minutes";
        else if(choice==3) taskDescription="Play fetch";
    }
    // --- CAT TASKS ---
    else if(petSpecies=="Cat"){
        cout<<"1. Clean Enclosure\n";
        cout<<"2. Brush coat\n";
        cout<<"3. Play with laser pointer\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Enclosure";
        else if(choice==2) taskDescription="Brush coat";
        else if(choice==3) taskDescription="Play with laser pointer";
    }
    // --- COW TASKS ---
    else if(petSpecies=="Cow"){
        cout<<"1. Clean Stall\n";
        cout<<"2. Brush coat\n";
        cout<<"3. Lead to pasture\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Stall";
        else if(choice==2) taskDescription="Brush coat";
        else if(choice==3) taskDescription="Lead to pasture";
    }
    // --- HORSE TASKS ---
    else if(petSpecies=="Horse"){
        cout<<"1. Clean Stall\n";
        cout<<"2. Brush coat\n";
        cout<<"3. Groom mane and tail\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Stall";
        else if(choice==2) taskDescription="Brush coat";
        else if(choice==3) taskDescription="Groom mane and tail";
    }
    // --- PARROT TASKS ---
    else if(petSpecies=="Parrot"){
        cout<<"1. Clean Cage\n";
        cout<<"2. Provide fresh fruit\n";
        cout<<"3. Socialize / Talk\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Cage";
        else if(choice==2) taskDescription="Provide fresh fruit";
        else if(choice==3) taskDescription="Socialize / Talk";
    }
    // --- TURTLE TASKS ---
    else if(petSpecies=="Turtle"){
        cout<<"1. Clean Tank\n";
        cout<<"2. Provide fresh vegetables\n";
        cout<<"3. Check water temperature\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Tank";
        else if(choice==2) taskDescription="Provide fresh vegetables";
        else if(choice==3) taskDescription="Check water temperature";
    }
    // --- RABBIT TASKS ---
    else if(petSpecies=="Rabbit"){
        cout<<"1. Clean Hutch\n";
        cout<<"2. Provide fresh hay\n";
        cout<<"3. Socialize / Petting time\n";
        cout<<"Choice: ";
        cin>>choice;
        if(choice==1) taskDescription="Clean Hutch";
        else if(choice==2) taskDescription="Provide fresh hay";
        else if(choice==3) taskDescription="Socialize / Petting time";
    }

    if(taskDescription.empty()){
        cout<<"Invalid task choice.\n";
        return;
    }


    ofstream taskFile("tasks.csv",ios::app);
    if(taskFile.is_open()){
        taskFile<<volId<<","<<volName<<","<<petId<<","<<petName<<","<<taskDescription<<endl;
        taskFile.close();
    }else{
        cout<<"\nError: Could not open tasks.csv.\n";
    }
}

void PeopleManager::showPeopleManagementMenu(const User& loggedInUser){
    int choice=0;
    while(true){
        system("cls");
        cout<<"------ PEOPLE MANAGEMENT ------"<<endl;
        cout<<"1. Register New Person\n";
        cout<<"2. List All Adopters\n";
        cout<<"3. List All Volunteers\n";
        cout<<"4. View/Edit Person Profile\n";
        cout<<"5. Assign Task to Volunteer\n";
        cout<<"6. Return to Main Menu\n";
        cout<<"Enter Your Choice: ";
        cin>>choice;

        if(cin.fail()){
            cout<<"Invalid Input.Please Enter the Valid Nubmber."<<endl;
            cin.clear();
            cout<<"Press Enter to Continue..."<<endl;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cin.get();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        switch(choice){
            case 1:
                registerNewPerson();
                break;
            case 2:
                listAllAdopters();
                break;
            case 3:
                listAllVolunteers();
                break;
            case 4:
                if(loggedInUser.role==Manager){
                    viewAndEditPersonProfile();
                }
                else{
                    cout<<"\nAccess Denied.This feature is for Managers only."<<endl;
                }
                break;
            case 5:
                if(loggedInUser.role==Manager || loggedInUser.role==Staff){
                    assignTaskToVolunteer();
                }
                else{
                    cout<<"\nAccess Denied.Volunteers Can not access it."<<endl;
                }
                break;
            case 6:
                return;
            default:
                cout<<"Invalid Choice.\n";
                break;
        }
        cout<<"Press Enter to continue..."<<endl;
        cin.get();
    }
}