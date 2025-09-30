#ifndef PET_H
#define PET_H

#include<bits/stdc++.h>
using namespace std;

#include "Authentication.h"

class Pet{       //base class of Pet
protected:
    string id;
    string name;
    string species;
    string breed;
    int age;
public:
    Pet(string id,string name,string species,string breed,int age);
    virtual ~Pet();    //virtual destructor will first destruct the derived class and last it deleted

    string getID() const;
    string getNAME() const;
    string getSPECIES() const;
    string getBREED() const;
    int getAGE() const;

    void setNAME(const string& newName);
    void setBREED(const string& newBreed);
    void setAGE(const int& newAge);

    virtual void makeSound()=0;
    virtual void displayDetails();
};

class Dog:public Pet{
public:
    Dog(string id,string name,string breed,int age):Pet(id,name,"Dog",breed,age){}
    void makeSound() override{ 
        cout<<"Bhow Bhow!!"<<endl; 
    }
};
class Cat:public Pet{
public:
    Cat(string id,string name,string breed,int age):Pet(id,name,"Cat",breed,age){}
    void makeSound() override{
        cout<<"Meow Meow!!"<<endl;
    }
};
class Cow:public Pet{
public:
    Cow(string id,string name,string breed,int age):Pet(id,name,"Cow",breed,age){}
    void makeSound() override{
        cout<<"Moooooo!!"<<endl;
    }
};
class Horse:public Pet{
public:
    Horse(string id,string name,string breed,int age):Pet(id,name,"Horse",breed,age){}
    void makeSound() override{
        cout<<"Hin Hin Hin Hin!!"<<endl;
    }
};
class Parrot:public Pet{
public:
    Parrot(string id,string name,string breed,int age):Pet(id,name,"Parrot",breed,age){}
    void makeSound() override{
        cout<<"Tain tain!!"<<endl;
    }
};
class Turtle:public Pet{
public:
    Turtle(string id,string name,string breed,int age):Pet(id,name,"Turtle",breed,age){}
    void makeSound() override{
        cout<<"Hiss!!"<<endl;
    }
};
class Rabbit:public Pet{
public:
    Rabbit(string id,string name,string breed,int age):Pet(id,name,"Rabbit",breed,age){}
    void makeSound() override{
        cout<<"Muttering!!"<<endl;
    }
};


class PetID_Generator{    //ID generating separate class for pets
    map<string,string> prefixInfo;
public:
    PetID_Generator();
    int lastID(string &Prefix);
    string newID(string &Species);   
}; 


class AdoptionShelter{
private:
    vector<Pet*> pets;
    User current_User;
    PetID_Generator ID_Generator;
    void Load_Data_From_File();
    void save_File();
    void addPet();
    void listPets();
    void SearchPet();
    void editPetDetails();
    
public:
    AdoptionShelter(const User& user);
    ~AdoptionShelter(); 

    void showPetManagementMenu(const User &current_User);
   

};







// void addPet();
// void listPets();
// void SearchPet();
// void editPetDetails();

// void PlayWpets();
// void storePetInteraction(User &current_User);

#endif
