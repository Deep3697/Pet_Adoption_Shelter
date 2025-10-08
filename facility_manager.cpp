#include<bits/stdc++.h>
using namespace std;
#include "facility_manager.h"

FacilityManager::FacilityManager(const User& user):currentUser(user){}

//HELPER to find pet and volunteer
bool FacilityManager::findVolunteerById(const string& volId, string& outName){
    ifstream userFile("users.csv");
    if(!userFile.is_open()){return false;}
    string line;
    while(getline(userFile,line)){
        stringstream ss(line);
        string id, password, name, role;
        getline(ss,id,','); getline(ss,password,','); getline(ss,name,','); getline(ss,role,',');
        if(id==volId && role=="Volunteer"){ // Correctly checks for Volunteer
            outName=name;
            userFile.close();
            return true;
        }
    }
    userFile.close();
    return false;
}

bool FacilityManager::findPetById(const string& petId, string& outName, string& outSpecies){
    ifstream file("pets.csv");
    if(!file.is_open()){
        return false;
    }
    string line;
    while(getline(file,line)){
        if(line.empty()){
            continue;
        }
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

void FacilityManager::scheduleKennelCleaning(){
    string volId, petId, volName, petName, petSpecies;
    cout<<"\n--- Schedule Kennel Cleaning ---\n";
    cout<<"Enter ID of Volunteer to assign cleaning task to: ";
    getline(cin,volId);
    if(!findVolunteerById(volId, volName)){
        cout<<"Error: Volunteer with ID "<<volId<<" not found.\n";
        return;
    }

    cout<<"Enter ID of Pet (Kennel) to be cleaned: ";
    getline(cin,petId);
    if(!findPetById(petId, petName, petSpecies)){
        cout<<"Error: Pet with ID "<<petId<<" not found.\n";
        return;
    }

    string taskDescription="Clean Enclosure";

    ofstream taskFile("tasks.csv",ios::app);
    if(taskFile.is_open()){
        taskFile<<volId<<","<<volName<<","<<petId<<","<<petName<<","<<taskDescription<<endl;
        taskFile.close();
        cout<<"\nTask '"<<taskDescription<<"' for pet "<<petName<<" has been assigned to "<<volName<<".\n";
    }else{
        cout<<"\nError: Could not open tasks.csv.\n";
    }
}



void FacilityManager::viewKennelOccupancy(){
    map<string, int> capacities;
    capacities["Dog"]=200;
    capacities["Cat"]=200;
    capacities["Cow"]=150;
    capacities["Rabbit"]=100;
    capacities["Turtle"]=50;
    capacities["Horse"]=50;
    
    map<string, int> currentCounts;
    ifstream petFile("pets.csv");
    string line;
    if(petFile.is_open()){
        while(getline(petFile,line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string id, name, species;
            getline(ss,id,',');
            getline(ss,name,',');
            getline(ss,species,',');
            currentCounts[species]++;
        }
    }
    petFile.close();

    cout<<"\n----- Kennel Capacity Summary -----\n";
    cout<<"-------------------------------------------------------------------------\n";
    cout<<left<<setw(15)<<"Species"<<setw(15)<<"Occupied"<<setw(15)<<"Available"<<"\n";
    cout<<"-------------------------------------------------------------------------\n";

    for(const auto& pair:capacities){
        string species=pair.first;
        int capacity=pair.second;
        int occupied=currentCounts[species];
        int available=capacity-occupied;
        cout<<left<<setw(15)<<species<<setw(15)<<occupied<<setw(15)<<available<<"\n";
    }
    cout<<"-------------------------------------------------------------------------\n";
}



// --- Shared Method ---
void FacilityManager::viewMaintenanceHistory(){
    ifstream historyFile("maintenance_history.csv");
    if(!historyFile.is_open()){cout<<"\nNo maintenance history found.\n"; return;}
    cout<<"\n----- Completed Maintenance History -----\n";
    string line;
    while(getline(historyFile,line)){
        cout<<line<<endl;
    }
    historyFile.close();
}

// --- Helper Methods ---
bool FacilityManager::findStaffById(const string& staffId, string& outName){
    ifstream userFile("users.csv");
    if(!userFile.is_open()){return false;}
    string line;
    while(getline(userFile,line)){
        stringstream ss(line);
        string id, password, name, role;
        getline(ss,id,',');
        getline(ss,password,',');
        getline(ss,name,',');
        getline(ss,role,',');
        if(id==staffId && (role=="Staff" || role=="Manager")){
            outName=name;
            userFile.close();
            return true;
        }
    }
    userFile.close();
    return false;
}

string FacilityManager::generateNewRequestId(){
    // Simple ID generator for maintenance requests
    ifstream file("maintenance_pending.csv");
    int maxId=0;
    string line;
    if(file.is_open()){
        while(getline(file,line)){
            string id;
            getline(stringstream(line),id,',');
            if(id.rfind("M",0)==0){
                try{ maxId=max(maxId,stoi(id.substr(1)));} catch(...){}
            }
        }
    }
    return "M"+to_string(maxId==0 ? 101 : maxId+1);
}




void FacilityManager::markRequestAsComplete(const string& requestId){
    string lineToComplete;
    ifstream inFile("maintenance_pending.csv");
    ofstream outFile("temp.csv");
    string line;
    while(getline(inFile,line)){
        string currentId;
        getline(stringstream(line),currentId,',');
        if(currentId==requestId){
            lineToComplete=line; // We found the line to move
        }else{
            outFile<<line<<"\n"; // Copy other lines
        }
    }
    inFile.close();
    outFile.close();
    remove("maintenance_pending.csv");
    rename("temp.csv","maintenance_pending.csv");

    if(!lineToComplete.empty()){
        ofstream historyFile("maintenance_history.csv",ios::app);
        if(historyFile.is_open()){
            // Format: RequestLine,CompletedByUserID,CompletedByUserName
            historyFile<<lineToComplete<<","<<currentUser.id<<","<<currentUser.name<<"\n";
            historyFile.close();
            cout<<"Request "<<requestId<<" marked as completed.\n";
        }
    }
}

void FacilityManager::viewMyAssignedRequests(){
    ifstream pendingFile("maintenance_pending.csv");
    if(!pendingFile.is_open()){cout<<"\nYou have no assigned requests.\n"; return;}

    vector<string> myRequests;
    string line;
    while(getline(pendingFile,line)){
        stringstream ss(line);
        string reqId, staffId;
        getline(ss,reqId,',');
        getline(ss,staffId,',');
        if(staffId==to_string(currentUser.id)){
            myRequests.push_back(line);
        }
    }
    pendingFile.close();

    if(myRequests.empty()){cout<<"\nYou have no pending requests.\n"; return;}

    cout<<"\n----- Your Assigned Maintenance Requests -----\n";
    for(int i=0; i<myRequests.size(); ++i){
        cout<<i+1<<". "<<myRequests[i]<<endl;
    }

    cout<<"\nEnter request number to mark as complete (or 0 to go back): ";
    int choice=0;
    cin>>choice;
    if(choice>0 && choice<=myRequests.size()){
        stringstream ss(myRequests[choice-1]);
        string requestId;
        getline(ss,requestId,',');
        markRequestAsComplete(requestId);
    }
}


void FacilityManager::viewAllPendingRequests(){
    ifstream pendingFile("maintenance_pending.csv");
    if(!pendingFile.is_open()){cout<<"\nNo pending maintenance requests.\n"; return;}
    cout<<"\n----- All Pending Maintenance Requests -----\n";
    string line;
    while(getline(pendingFile,line)){
        cout<<line<<endl;
    }
    pendingFile.close();
}

void FacilityManager::assignNewMaintenanceRequest(){
    string staffId, staffName, description;
    cout<<"\n---- Assign New Maintenance Request ----\n";
    cout<<"Enter ID of Staff member to assign request to: ";
    getline(cin,staffId);
    if(!findStaffById(staffId, staffName)){
        cout<<"Error: Staff with ID "<<staffId<<" not found.\n";
        return;
    }
    cout<<"Enter description of the maintenance issue: ";
    getline(cin,description);

    string requestId=generateNewRequestId();
    ofstream pendingFile("maintenance_pending.csv",ios::app);
    if(pendingFile.is_open()){
        // Format: RequestID,StaffID,StaffName,"Description"
        pendingFile<<requestId<<","<<staffId<<","<<staffName<<",\""<<description<<"\"\n";
        pendingFile.close();
        cout<<"\nMaintenance request assigned successfully with ID: "<<requestId<<endl;
    }else{
        cout<<"\nError: Could not open maintenance_pending.csv.\n";
    }
}


void FacilityManager::showFacilityManagementMenu(){
    // The menu is different depending on the user's role
    if(currentUser.role==Manager){
        int choice=0;
        while(true){
            system("cls");
            cout<<"------ FACILITY MANAGEMENT (Manager View) ------\n";
            cout<<"1. Schedule Kennel Cleaning for Volunteer\n";
            cout<<"2. Assign New Maintenance Request\n";
            cout<<"3. View All Pending Maintenance Requests\n";
            cout<<"4. View Maintenance History\n";
            cout<<"5. View Kennel Occupenncy\n";
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
                case 1: scheduleKennelCleaning(); break;
                case 2: assignNewMaintenanceRequest(); break;
                case 3: viewAllPendingRequests(); break;
                case 4: viewMaintenanceHistory(); break;
                case 5: viewKennelOccupancy(); break;
                case 6: return;
                default: cout<<"Invalid Choice.\n"; break;
            }
            cout<<"\nPress Enter to continue...";
            cin.get();
        }
    }else if(currentUser.role==Staff){
        int choice=0;
        while(true){
            system("cls");
            cout<<"------ FACILITY MANAGEMENT (Staff View) ------\n";
            cout<<"1. Schedule Kennel Cleaning for Volunteer\n";
            cout<<"2. View & Complete My Assigned Requests\n";
            cout<<"3. View Maintenance History\n";
            cout<<"4. View Kennel Occupency\n";
            cout<<"5. Return to Main Menu\n";
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
                case 1: scheduleKennelCleaning(); break;
                case 2: viewMyAssignedRequests(); break;
                case 3: viewMaintenanceHistory(); break;
                case 4: viewKennelOccupancy(); break;
                case 5: return;
                default: cout<<"Invalid Choice.\n"; break;
            }
            cout<<"\nPress Enter to continue...";
            cin.get();
        }
    }
}
