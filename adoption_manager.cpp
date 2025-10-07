#include<bits/stdc++.h>
using namespace std;
#include "adoption_manager.h"

AdoptionManager::AdoptionManager(const User& user, AdoptionShelter& shelterRef):currentUser(user), shelter(shelterRef){}


string AdoptionManager::generateNewAppId(){
    ifstream appFile("applications.csv");
    if(!appFile.is_open()){return "A101";}
    string line;
    int maxId=0;
    while(getline(appFile,line)){
        string id;
        getline(stringstream(line),id,',');
        if(id.rfind("A",0)==0){
            try{ maxId=max(maxId, stoi(id.substr(1))); } catch(...){}
        }
    }
    return "A"+to_string(maxId==0 ? 101 : maxId+1);
}

bool AdoptionManager::isPetAvailable(const string& petId, string& outPetName){
    // First, check if the pet exists in pets.csv
    ifstream petFile("pets.csv");
    if(!petFile.is_open()){return false;}
    string line;
    bool petExists=false;
    while(getline(petFile,line)){
        stringstream ss(line);
        string id, name;
        getline(ss,id,',');
        if(id==petId){
            getline(ss,name,',');
            outPetName=name;
            petExists=true;
            break;
        }
    }
    petFile.close();
    if(!petExists){return false;} // Pet doesn't exist at all

    // Second, check if there's already a "Pending" application for this pet
    ifstream appFile("applications.csv");
    if(!appFile.is_open()){return true;} // No applications file means no pending apps
    while(getline(appFile,line)){
        stringstream ss(line);
        string appId, adopterId, adopterName, currentPetId, petName, status;
        getline(ss,appId,',');
        getline(ss,adopterId,',');

        getline(ss,adopterName,',');
        getline(ss,currentPetId,',');
        getline(ss,petName,',');
        getline(ss,status,',');
        if(currentPetId==petId && status=="Pending"){
            appFile.close();
            return false; // Found a pending application, so pet is not available
        }
    }
    appFile.close();
    return true; // Pet exists and has no pending applications
}

bool AdoptionManager::isAdopterValid(const string& adopterId, string& outAdopterName){
    ifstream adopterFile("adopters.csv");
    if(!adopterFile.is_open()){return false;}
    string line;
    while(getline(adopterFile,line)){
        stringstream ss(line);
        string id, name;
        getline(ss,id,',');
        if(id==adopterId){
            getline(ss,name,',');
            outAdopterName=name;
            adopterFile.close();
            return true;
        }
    }
    adopterFile.close();
    return false;
}

void AdoptionManager::submitNewApplication(){
    string adopterId, petId, adopterName, petName;
    cout<<"\n--- Submit New Application ---\n";

    // 1. Validate Adopter
    cout<<"Enter the Adopter's ID (e.g., X101): ";
    getline(cin,adopterId);
    if(!isAdopterValid(adopterId, adopterName)){
        cout<<"\nError: Adopter with ID "<<adopterId<<" not found.\n";
        cout<<"Please register the adopter from the 'People Management' menu first.\n";
        return;
    }

    // 2. Validate Pet
    cout<<"Enter the Pet's ID (e.g., D101): ";
    getline(cin,petId);
    if(!isPetAvailable(petId, petName)){
        cout<<"\nError: Pet with ID "<<petId<<" not found or is already pending adoption.\n";
        return;
    }

    // 3. All checks passed, create the application
    string appId=generateNewAppId();
    ofstream appFile("applications.csv",ios::app);
    if(appFile.is_open()){
        appFile<<appId<<","<<adopterId<<","<<adopterName<<","<<petId<<","<<petName<<",Pending\n";
        appFile.close();
        cout<<"\nApplication for "<<petName<<" by "<<adopterName<<" submitted successfully with App ID: "<<appId<<".\n";
    }else{
        cout<<"\nError: Could not open applications.csv file.\n";
    }
}


void AdoptionManager::viewPendingApplications(){
    ifstream appFile("applications.csv");
    string line;
    int pendingCount=0;

    if(!appFile.is_open()){
        cout<<"\nError: Could not open applications.csv. No applications found.\n";
        return;
    }

    cout<<"\n----- List of All Pending Adoption Applications -----\n";
    cout<<"----------------------------------------------------------------------------------------\n";
    cout<<left<<setw(10)<<"App ID"<<setw(20)<<"Adopter Name"<<setw(20)<<"Pet Name"<<setw(10)<<"Pet ID"<<"\n";
    cout<<"----------------------------------------------------------------------------------------\n";

    while(getline(appFile,line)){
        stringstream ss(line);
        string appId, adopterId, adopterName, petId, petName, status;

        getline(ss,appId,',');
        getline(ss,adopterId,',');
        getline(ss,adopterName,',');
        getline(ss,petId,',');
        getline(ss,petName,',');
        getline(ss,status,',');

        // This is the key: only print the line if the status is "Pending"
        if(status=="Pending"){
            cout<<left<<setw(10)<<appId<<setw(20)<<adopterName<<setw(20)<<petName<<setw(10)<<petId<<"\n";
            pendingCount++;
        }
    }
    
    if(pendingCount==0){
        cout<<"No pending applications found.\n";
    }
    cout<<"----------------------------------------------------------------------------------------\n";
    appFile.close();
}


void AdoptionManager::updateApplicationStatus(const string& appId, const string& newStatus){
    ifstream inFile("applications.csv");
    ofstream outFile("temp.csv");
    string line;
    while(getline(inFile,line)){
        stringstream ss(line);
        string currentAppId;
        getline(ss,currentAppId,',');
        if(currentAppId==appId){
            // Rebuild the line with the new status
            stringstream line_ss(line);
            string app, adopterId, adopterName, petId, petName, oldStatus;
            getline(line_ss,app,','); getline(line_ss,adopterId,','); getline(line_ss,adopterName,',');
            getline(line_ss,petId,','); getline(line_ss,petName,',');
            outFile<<app<<","<<adopterId<<","<<adopterName<<","<<petId<<","<<petName<<","<<newStatus<<"\n";
        }else{
            outFile<<line<<"\n";
        }
    }
    inFile.close();
    outFile.close();
    remove("applications.csv");
    rename("temp.csv","applications.csv");
}

void AdoptionManager::reviewApplication(){
    string appId;
    cout<<"\nEnter the Application ID to review: ";
    getline(cin,appId);

    // Find the application and its details (petId specifically)
    ifstream appFile("applications.csv");
    string line, petIdToProcess, appLine;
    bool found=false;
    while(getline(appFile,line)){
        stringstream ss(line);
        string currentAppId, status;
        getline(ss,currentAppId,',');
        // This is complex: seek to the end to find the status
        string temp=line.substr(line.find_last_of(',')+1);

        if(currentAppId==appId && temp=="Pending"){
            found=true;
            appLine=line;
            // Parse out the petId
            stringstream line_ss(line);
            string field;
            getline(line_ss,field,','); // AppId
            getline(line_ss,field,','); // AdopterId
            getline(line_ss,field,','); // AdopterName
            getline(line_ss,petIdToProcess,','); // PetId
            break;
        }
    }
    appFile.close();

    if(!found){cout<<"Pending application with ID "<<appId<<" not found.\n"; return;}

    system("cls");
    cout<<"--- Reviewing Application "<<appId<<" ---\n";
    cout<<appLine<<endl;
    cout<<"\nChoose an action:\n1. Approve\n2. Reject\n3. Cancel\nChoice: ";
    int choice=0;
    cin>>choice;

    char confirm='n';
    if(choice==1){ // Approve
        cout<<"Are you sure you want to APPROVE this application? (y/n): ";
        cin>>confirm;
        if(confirm=='y' || confirm=='Y'){
            shelter.removePet(petIdToProcess); // Tell the shelter to remove the pet
            updateApplicationStatus(appId,"Approved");
            cout<<"Application Approved.\n";
        }else{
            cout<<"Approval cancelled.\n";
        }
    }else if(choice==2){ // Reject
        cout<<"Are you sure you want to REJECT this application? (y/n): ";
        cin>>confirm;
        if(confirm=='y' || confirm=='Y'){
            updateApplicationStatus(appId,"Rejected");
            cout<<"Application Rejected.\n";
        }else{
            cout<<"Rejection cancelled.\n";
        }
    }
}


void AdoptionManager::viewAdoptionHistory(){
    // This is the same logic as viewPendingApplications, but checks for "Approved"
    ifstream appFile("applications.csv");
    cout<<"\n----- Adoption History (Approved Applications) -----\n";
    string line;
    while(getline(appFile,line)){
        if(line.find(",Approved")!=string::npos){
            cout<<line<<endl;
        }
    }
    appFile.close();
}


void AdoptionManager::showAdoptionWorkflowMenu(){
    int choice=0;
    while(true){
        system("cls");
        cout<<"------ ADOPTION WORKFLOW ------\n";
        cout<<"1. Submit New Adoption Application\n";
        cout<<"2. View All Pending Applications\n";
        cout<<"3. Review an Application (Approve/Reject)\n";
        cout<<"4. View Adoption History\n";
        cout<<"5. Return to Main Menu\n";
        cout<<"Enter Your Choice: ";
        cin>>choice;

        if(cin.fail()){cin.clear(); cin.ignore(); continue;}
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        switch(choice){
            case 1:
                submitNewApplication();
                break;
            case 2:
                viewPendingApplications();
                break;
            case 3:
                reviewApplication();
                break;
            case 4:
                viewAdoptionHistory();
                break;
            case 5:
                return;
            default:
                cout<<"Invalid Choice.\n";
                break;
        }
        cout<<"\nPress Enter to continue...";
        cin.get();
    }
}