#include<bits/stdc++.h>
using namespace std;
#include "volunteer_portal.h"

VolunteerPortal::VolunteerPortal(const User& user){
    currentUser=user;
}


void VolunteerPortal::logActivity(const string& completedTaskLine){

    stringstream ss(completedTaskLine);
    string volId, volName, petId, petName, taskDesc;
    getline(ss,volId,',');
    getline(ss,volName,',');
    getline(ss,petId,',');
    getline(ss,petName,',');
    getline(ss,taskDesc,',');

    ofstream logFile("interactions.csv", ios::app);
    if(logFile.is_open()){
        logFile<<petId<<","<<petName<<","<<taskDesc<<","<<volId<<","<<volName<<"\n";
        logFile.close();
    }
}

void VolunteerPortal::markTaskAsComplete(int taskNumber, const vector<string>& userTasks){
    string lineToComplete = userTasks[taskNumber-1];

    logActivity(lineToComplete);

    ifstream inputFile("tasks.csv");
    ofstream tempFile("temp.csv");
    string line;
    while(getline(inputFile,line)){
        if(line!=lineToComplete){
            tempFile<<line<<"\n";
        }
    }
    inputFile.close();
    tempFile.close();
    remove("tasks.csv");
    rename("temp.csv","tasks.csv");
    cout<<"Task marked as complete and logged!\n";
}


void VolunteerPortal::viewMyTasks(){
    ifstream taskFile("tasks.csv");
    if(!taskFile.is_open()){ cout<<"\nNo tasks assigned yet.\n"; return; }

    vector<string> myPendingTasks;
    string line;

    while(getline(taskFile,line)){
        stringstream ss(line);
        string volId;
        getline(ss,volId,',');

        if(volId==to_string(currentUser.id)){
            myPendingTasks.push_back(line);
        }
    }
    taskFile.close();

    if(myPendingTasks.empty()){
        cout<<"\nYou have no pending tasks. Great job!\n";
        return;
    }

    cout<<"\n---- Your Pending Tasks ----\n";
    for(int i=0; i<myPendingTasks.size(); ++i){
        stringstream ss(myPendingTasks[i]);
        string volId, volName, petId, petName, taskDesc;
        getline(ss,volId,','); getline(ss,volName,',');
        getline(ss,petId,','); getline(ss,petName,',');
        getline(ss,taskDesc,',');
        cout<<i+1<<". "<<taskDesc<<" for pet "<<petName<<" (ID: "<<petId<<")\n";
    }

    cout<<"\nEnter task number to mark as complete (or 0 to go back): ";
    int taskChoice=0;
    cin>>taskChoice;
    if(taskChoice>0 && taskChoice<=myPendingTasks.size()){
        markTaskAsComplete(taskChoice, myPendingTasks);
    }
}



void VolunteerPortal::viewKennelOccupancy(){
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
            stringstream ss(line);
            string Tspecies;
            string Tid;
            string Tname;
            getline(ss,Tid,',');
            getline(ss,Tname,',');
            getline(ss,Tspecies,',');
            currentCounts[Tspecies]++;
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
    cout<<endl<<"The Kennel Number is According to their Respective Pet IDs!!!"<<endl;
}



// Version for Pet Care of function overloaded here
void VolunteerPortal::logActivity(Pet* pet, const string& activity){
    ofstream logFile("interactions.csv", ios::app);
    if(logFile.is_open()){
        logFile<<pet->getID()<<","<<pet->getNAME()<<",\""<<activity<<"\","<<currentUser.id<<","<<currentUser.name<<"\n";
        logFile.close();
    }
}

Pet* VolunteerPortal::findAndCreatePet(const string& petId){
    ifstream petFile("pets.csv");
    string line;
    if(!petFile.is_open()){return nullptr;}
    while(getline(petFile,line)){
        stringstream ss(line);
        string id,name,species,breed,ageStr;
        int age;
        getline(ss,id,',');
        if(id==petId){
            getline(ss,name,','); getline(ss,species,','); getline(ss,breed,','); getline(ss,ageStr,',');
            age=stoi(ageStr);
            petFile.close();
            if(species=="Dog") return new Dog(id,name,breed,age);
            if(species=="Cat") return new Cat(id,name,breed,age);
            if(species=="Cow") return new Cow(id,name,breed,age);
            if(species=="Horse") return new Horse(id,name,breed,age);
            if(species=="Parrot") return new Parrot(id,name,breed,age);
            if(species=="Turtle") return new Turtle(id,name,breed,age);
            if(species=="Rabbit") return new Rabbit(id,name,breed,age);
            return nullptr;
        }
    }
    petFile.close();
    return nullptr;
}


void VolunteerPortal::performPetCare(){
    string searchId;
    cout<<"\nEnter the ID of the pet you want to interact with: ";
    cin>>searchId;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    Pet* pet=findAndCreatePet(searchId);
    if(pet==nullptr){
        cout<<"Sorry, no pet found with ID: "<<searchId<<endl;
        return;
    }

    string species=pet->getSPECIES();
    int choice=0;
    while(true){
        system("cls");
        cout<<"--- Interaction Session with "<<pet->getNAME()<<" ("<<species<<") ---\n";
        string activity="";

        if(species=="Dog"){
            cout<<"1. Walk the dog (20 mins)\n2. Play fetch\n3. Feed the dog\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Walked for 20 minutes";
            else if(choice==2) activity="Played fetch";
            else if(choice==3) activity="Was fed by user";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        else if(species=="Cat"){
            cout<<"1. Play with laser pointer\n2. Brush coat\n3. Feed the cat\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Played with laser pointer";
            else if(choice==2) activity="Brushed coat";
            else if(choice==3) activity="Was fed by user";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        else if(species=="Cow"){
            cout<<"1. Brush coat\n2. Lead to pasture\n3. Feed the cow\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Brushed coat";
            else if(choice==2) activity="Led to pasture";
            else if(choice==3) activity="Was fed by user";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        else if(species=="Horse"){
            cout<<"1. Brush coat\n2. Groom mane and tail\n3. Feed the horse\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Brushed coat";
            else if(choice==2) activity="Groomed mane and tail";
            else if(choice==3) activity="Was fed by user";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        else if(species=="Parrot"){
            cout<<"1. Clean Cage\n2. Provide fresh fruit\n3. Socialize / Talk\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Cleaned Cage";
            else if(choice==2) activity="Provided fresh fruit";
            else if(choice==3) activity="Socialized / Talked with";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        else if(species=="Turtle"){
            cout<<"1. Clean Tank\n2. Provide fresh vegetables\n3. Check water temperature\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Cleaned Tank";
            else if(choice==2) activity="Provided fresh vegetables";
            else if(choice==3) activity="Checked water temperature";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        else if(species=="Rabbit"){
            cout<<"1. Clean Hutch\n2. Provide fresh hay\n3. Socialize / Petting time\n4. Finish Session\nChoice: ";
            cin>>choice;
            if(choice==1) activity="Cleaned Hutch";
            else if(choice==2) activity="Provided fresh hay";
            else if(choice==3) activity="Socialized / Petting time";
            else if(choice==4) break;
            else{cout<<"Invalid choice.\n";}
        }
        
        if(!activity.empty()){
            logActivity(pet, activity);
            cout<<"\nActivity '"<<activity<<"' logged. Press Enter to continue...";
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin.get();
    }
    delete pet; // Clean up the temporary object
}


void VolunteerPortal::runVolunteerMenu(){
    int choice=0;
    while(true){
        system("cls");
        cout<<"----- VOLUNTEER PORTAL -----\n";
        cout<<"Welcome, "<<currentUser.name<<"!\n\n";
        cout<<"1. View & Complete My Assigned Tasks\n";
        cout<<"2. Perform Pet Care\n";
        cout<<"3. View Kennel Occupancy\n";
        cout<<"4. Exit\n";
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
        else{
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch(choice){
                case 1: viewMyTasks(); break;
                case 2: performPetCare(); break;
                case 3: viewKennelOccupancy(); break;
                case 4: return; // This will exit the volunteer's menu loop
                default: cout<<"Invalid choice.\n"; break;
            }
        }
        cout<<"\nPress Enter to continue...";
        cin.get();
    }
}
