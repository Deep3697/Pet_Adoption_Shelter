#include<bits/stdc++.h>
#include<chrono>
#include<ctime>
using namespace std;

#include "pet.h"


Pet::Pet(string id,string name,string species,string breed,int age){
    this->id=id;
    this->name=name;
    this->species=species;
    this->breed=breed;
    this->age=age;
}

Pet::~Pet(){};

string Pet::getID() const {return id;}
string Pet::getNAME() const {return name;}
string Pet::getSPECIES() const {return species;}
string Pet::getBREED() const {return breed;}
int Pet::getAGE() const {return age;}

void Pet::setNAME(const string& newName){name=newName;}
void Pet::setBREED(const string& newBreed){breed=newBreed;}
void Pet::setAGE(const int& newAge){age=newAge;}

void Pet::displayDetails(){
    cout<<left<<setw(10)<<id<<setw(20)<<name<<setw(15)<<species<<setw(20)<<breed<<setw(5)<<age<<endl;
}

void AdoptionShelter::removePet(const string& petId){
    // Find the pet with the matching ID
    auto it=std::remove_if(pets.begin(),pets.end(),[&](Pet* pet){
        if(pet->getID()==petId){
            delete pet; // Free the memory for the object
            return true; // Mark this element for removal
        }
        return false;
    });
    // Erase the now-invalid elements from the vector
    if(it!=pets.end()){
        pets.erase(it,pets.end());
        cout<<"\nNotification: Pet with ID "<<petId<<" has been adopted and removed from the shelter.\n";
    }
}


PetID_Generator::PetID_Generator(){
    prefixInfo["Dog"]="D";
    prefixInfo["Cat"]="C";
    prefixInfo["Cow"]="CO";
    prefixInfo["Horse"]="H";
    prefixInfo["Parrot"]="P";
    prefixInfo["Turtle"]="T";
    prefixInfo["Rabbit"]="R";
}

int PetID_Generator::lastID(string &Prefix){
    ifstream petFile("pets.csv");
    string line;
    int maxID=0;
    if(!petFile.is_open()){
        ofstream createFile("pets.csv");   //make new file if not exists.
        createFile.close();
        return 100;
    }
    while(getline(petFile,line)){
        stringstream ss(line);
        string id;
        getline(ss,id,',');

        if(id.rfind(Prefix,0)==0){         //id.rfind() return 0 if it find value or it will give npos(No Position found)
            try{           // try will try to do dangerous conversion
                int currentID=stoi(id.substr(Prefix.length()));
                maxID=max(maxID,currentID);
            }
            catch(const invalid_argument &e){   // catch-if stoi fails then code runs  and prints the warning and skip the bad line

            }  
        }
    }
    petFile.close();
    return maxID==0?100 : maxID;
}

string PetID_Generator::newID(string &Species){
    string Prefix;
    if(prefixInfo.count(Species)){
        Prefix=prefixInfo[Species];
    }   
    int last_Id=lastID(Prefix);
    int new_Id=last_Id+1;
    return Prefix+to_string(new_Id);
}

AdoptionShelter::AdoptionShelter(const User& user):current_User(user) {
    Load_Data_From_File();       //Construct will load all data to the objects.
}
AdoptionShelter::~AdoptionShelter(){
    save_File();        //Destructor save the edited file and then free memory.
    for(Pet* x:pets){
        delete x;
    }
}

void AdoptionShelter::Load_Data_From_File(){
    ifstream petFile("pets.csv");
    string line;
    if(!petFile.is_open()) return;

    while(getline(petFile,line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string id,name,species,breed,ageStr;
        int age;

        getline(ss,id,',');
        getline(ss,name,',');
        getline(ss,species,',');
        getline(ss,breed,',');
        getline(ss,ageStr,',');

        try{
            age=stoi(ageStr);
            if(species=="Dog") pets.push_back(new Dog(id,name,breed,age));
            else if (species == "Cat") pets.push_back(new Cat(id, name, breed, age));
            else if (species == "Cat") pets.push_back(new Cat(id, name, breed, age));
            else if (species == "Cow") pets.push_back(new Cow(id, name, breed, age));
            else if (species == "Horse") pets.push_back(new Horse(id, name, breed, age));
            else if (species == "Turtle") pets.push_back(new Turtle(id, name, breed, age));
            else if (species == "Rabbit") pets.push_back(new Rabbit(id, name, breed, age));
        } catch (const invalid_argument &e){ /* ignore malformed lines */ }
    }
    cout << pets.size() << " pets loaded from pets.csv." << endl;
    petFile.close();
}

void AdoptionShelter::save_File() {
    ofstream petFile("pets.csv");
    if (!petFile.is_open()) return;
    for (Pet* pet : pets) {
        petFile << pet->getID() << "," << pet->getNAME() << "," << pet->getSPECIES()<< "," << pet->getBREED() << "," << pet->getAGE() << "\n";
    }
}

void AdoptionShelter::listPets() {
    cout << "\n----- List of All Shelter Pets -----" << endl;
    
    if(pets.empty()){
        cout<<"The shelter is currently empty."<<endl;
        return;
    }

    map<string, vector<Pet*>> groupedPets;
    for (Pet* pet : this->pets) {
        groupedPets[pet->getSPECIES()].push_back(pet);
    }

    for(auto& pair:groupedPets){
        string species=pair.first;
        vector<Pet*>& speciesPets=pair.second; // Get a reference to the vector

        sort(speciesPets.begin(),speciesPets.end(),[](Pet* a,Pet* b){
            return a->getID()<b->getID();
        });

        cout << "\n--- "<< species <<" ---" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) << "Species" << setw(20) << "Breed" << setw(5) << "Age" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;

        for (Pet* pet : speciesPets) {
            pet->displayDetails();
        }
    }
    cout << "----------------------------------------------------------------------------------------------" << endl;
}


void AdoptionShelter::SearchPet() {
    string searchId;
    cout << "Enter the ID of the pet to search for:"<<endl;
    cin >> searchId;
    bool found = false;
    for (Pet* pet : pets) {
        if (pet->getID() == searchId) {
            cout << "\n------ Pet Found! ------" << endl;
            cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) << "Species" << setw(20) << "Breed" << setw(5) << "Age" << endl;
            cout << "----------------------------------------------------------------------------------------" << endl;
            pet->displayDetails();
            found = true;
            break;
        }
    }
    if(!found) { cout << "\nSorry, no pet found with ID: " << searchId << endl; }
}

void AdoptionShelter::addPet() {
    string name, species, breed;
    int age, choice;
    cout << "\n------ Add New Pet ------\nSelect Species:\n1. Dog\n2. Cat\n3. Cow\n4. Horse\n5. Parrot\n6. Turtle\n7. Rabbit\nChoice: ";
    cin >> choice;
    switch (choice) {
        case 1: species = "Dog"; break;
        case 2: species = "Cat"; break;
        case 3: species = "Cow"; break;
        case 4: species = "Horse"; break;
        case 5: species = "Parrot"; break;
        case 6: species = "Turtle"; break;
        case 7: species = "Rabbit"; break;
        default: cout << "Invalid choice." << endl; return;
    }
    cout << "Enter Name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, name);
    cout << "Enter Breed: "; getline(cin, breed);
    cout << "Enter Age: "; cin >> age;

    string id = ID_Generator.newID(species);
    if (species == "Dog") pets.push_back(new Dog(id, name, breed, age));
    else if (species == "Cat") pets.push_back(new Cat(id, name, breed, age));
    else if (species == "Cat") pets.push_back(new Cat(id, name, breed, age));
    else if (species == "Cow") pets.push_back(new Cow(id, name, breed, age));
    else if (species == "Horse") pets.push_back(new Horse(id, name, breed, age));
    else if (species == "Turtle") pets.push_back(new Turtle(id, name, breed, age));
    else if (species == "Rabbit") pets.push_back(new Rabbit(id, name, breed, age));
    cout << "\n Pet added to memory Successfully!" << endl;
}

void AdoptionShelter::editPetDetails() {
    string searchId;
    cout << "\nEnter the ID of the pet to edit: ";
    cin >> searchId;
    for (Pet* pet : pets) {
        if (pet->getID() == searchId) {
            cout << "Pet found! (" << pet->getNAME() << "). Enter new details:" << endl;
            string newName, newBreed;
            int newAge;
            cout << "Enter New Name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, newName);
            cout << "Enter New Breed: "; getline(cin, newBreed);
            cout << "Enter New Age: "; cin >> newAge;

            pet->setNAME(newName);
            pet->setBREED(newBreed);
            pet->setAGE(newAge);

            cout << "Pet details updated successfully." << endl;
            return;
        }
    }
    cout << "\nSorry, no pet found with ID: " << searchId << endl;
}

void AdoptionShelter::storeInLogBook(Pet* pet,const string& activity){
    ofstream logFile("interactions.csv",ios::app);
    if(logFile.is_open()){
        logFile<<pet->getID()<<","<<pet->getNAME()<<",\""<<activity<<"\","<<current_User.id<<","<<current_User.name<<"\n";
        logFile.close();
    }
}

Pet* AdoptionShelter::findPetById(const string& id){
    for(Pet* pet:pets){
        if(pet->getID()==id){
            return pet;
        }
    }
    return NULL;
}

void AdoptionShelter::InteractionwithPets(){
    string searchId;
    cout<<"\nEnter the ID of the pet you want to interact with: ";
    cin>>searchId;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    Pet* pet=findPetById(searchId);
    if(pet==nullptr){
        cout<<"Sorry, no pet found with ID: "<<searchId<<endl;
        return;
    }

    string species=pet->getSPECIES();
    int choice=0;
    while(true){
        system("cls");
        cout<<"--- Interaction Session with "<<pet->getNAME()<<" ("<<species<<") ---\n";
        // --- DOG MENU ---
        if(species=="Dog"){
            cout<<"1. Walk the dog (20 mins)\n2. Play fetch\n3. Feed the dog\n4. Finish Session\nChoice: ";
            cin>>choice;
            switch(choice){
                case 1: storeInLogBook(pet,"Walked for 20 minutes"); cout<<"You take "<<pet->getNAME()<<" for a nice walk.\n"; break;
                case 2: storeInLogBook(pet,"Played fetch"); cout<<pet->getNAME()<<" happily chases the ball!\n"; pet->makeSound(); break;
                case 3: storeInLogBook(pet,"Was fed by user"); cout<<"You give "<<pet->getNAME()<<" a bowl of food.\n"; break;
                case 4: return;
                default: cout<<"Invalid choice.\n"; break;
            }
        }
        // --- CAT MENU ---
        else if(species=="Cat"){
            cout<<"1. Play with laser pointer\n2. Brush coat\n3. Feed the cat\n4. Finish Session\nChoice: ";
            cin>>choice;
            switch(choice){
                case 1: storeInLogBook(pet,"Played with laser pointer"); cout<<pet->getNAME()<<" keenly chases the red dot!\n"; pet->makeSound(); break;
                case 2: storeInLogBook(pet,"Brushed coat"); cout<<"You gently brush "<<pet->getNAME()<<"'s fur.\n"; break;
                case 3: storeInLogBook(pet,"Was fed by user"); cout<<"You give "<<pet->getNAME()<<" a bowl of food.\n"; break;
                case 4: return;
                default: cout<<"Invalid choice.\n"; break;
            }
        }
        // --- OTHER SPECIES MENU (Generic) ---
        else {
            cout<<"1. Feed the animal\n2. Clean enclosure\n3. Observe the animal\n4. Finish Session\nChoice: ";
            cin>>choice;
            switch(choice){
                case 1: storeInLogBook(pet,"Was fed by user"); cout<<"You provide fresh food for "<<pet->getNAME()<<".\n"; break;
                case 2: storeInLogBook(pet,"Enclosure was cleaned"); cout<<"You clean the enclosure for "<<pet->getNAME()<<".\n"; break;
                case 3: storeInLogBook(pet,"Was observed by user"); cout<<pet->getNAME()<<" seems calm and content.\n"; break;
                case 4: return;
                default: cout<<"Invalid choice.\n"; break;
            }
        }
        cout<<"\nActivity logged. Press Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cin.get();
    }
}

void AdoptionShelter::logInteractions() {
    ifstream logFile("interactions.csv");
    string line;
    if (!logFile.is_open()) {
        cout << "\nInteraction log is empty or could not be opened." << endl;
        return;
    }
    cout << "\n--- Animal Interaction Log ---" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(10) << "Pet ID" << setw(20) << "Pet Name" << setw(30) << "Activity" << setw(10) << "User ID" << setw(20) << "User Name" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;
    while (getline(logFile, line)) {
        stringstream ss(line);
        string petId, petName, activity, userId, userName;
        getline(ss, petId, ',');
        getline(ss, petName, ',');
        getline(ss, activity, ',');
        getline(ss, userId, ',');
        getline(ss, userName, ',');
        cout << left << setw(10) << petId << setw(20) << petName << setw(30) << activity << setw(10) << userId << setw(20) << userName << endl;
    }
    cout << "----------------------------------------------------------------------------------------------------------" << endl;
    logFile.close();
}

void AdoptionShelter::showPetManagementMenu(){
    int choice=0;
    while(true){
        system("cls");
        cout<<"---PET MANAGEMENT MENU---"<<endl;
        cout<<
        "1. Add New Pet"<<endl<<
        "2. List All Pets"<<endl<<
        "3. Search For Pet"<<endl<<
        "4. Edit Pet Details"<<endl<<
        "5. Play with Pets"<<endl<<
        "6. Animal Interection logs"<<endl<<
        "7. return to MAIN MENU"<<endl;
        cout<<"Enter Your Choice:";
        cin>>choice;
        if(cin.fail()){
            cout<<"Invalid Input.Enter Valid Input."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Enter to continue."<<endl;
            cin.get();
            continue;
        }
        switch(choice){
            case 1:
                addPet();
                break;
            case 2:
                listPets();
                break;
            case 3:
                SearchPet();
                break;
            case 4:
                editPetDetails();
                break;
            case 5:
                InteractionwithPets();
                break;
            case 6:
                logInteractions();
                break;
            case 7:
                cout<<"Returning to MAIN MENU..."<<endl;
                return;
                break;
            default:
                cout<<"Invalid Input!"<<endl;
                break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Press Enter to continue"<<endl;
        cin.get();
    }
}


