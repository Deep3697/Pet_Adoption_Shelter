#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
#include "pet.h"
#include "Authentication.h"
#include "people_manager.h"
#include"volunteer_portal.h"
#include "adoption_manager.h"
#include "facility_manager.h"



void display_MainMenu(const User &current_User){     //if const not used there will be different reference fot .h and .cpp and there will be change in name for whole program instead of one time
    cout<<"Welcome to the HAPPY PET SHELTER"<<endl;
    cout<<"Logged in as:"<<current_User.name<<" (ID:"<<current_User.id<<")"<<endl;

    UserRole role=current_User.role;
    if(role==Volunteer){
        cout<<"------- MAIN MENU-------"<<endl;
        cout<<"Your Volunteer Menu"<<endl;
        cout<<
        "1. My Assigned Tasks"<<endl<<
        "2. Animal Care"<<endl<<
        "3. View Kennel Occupancy"<<endl<<
        "4. Exit"<<endl<<endl;
        cout<<"Enter Your Choice:";
        return;
    }
    
    cout<<"------- MAIN MENU-------"<<endl;
    cout<<
    "1. Pet Management"<<endl<<
    "2. People Management"<<endl<<
    "3. Adoption Workflow"<<endl<<
    "4. Facility Management"<<endl;
    if(role==Manager){
        cout<<
        "5. Inventory & Financials"<<endl<<
        "6. Events & Fostering "<<endl<<
        "7. Lost & Found"<<endl<<
        "8. Reports & Analytics"<<endl<<
        "9. Exit"<<endl;
        cout<<"Enter Your Choice:";
    }
    else if(role==Staff){
        cout<<
        "5. Exit"<<endl;
        cout<<"Enter Your Choice:";
    }
    return;
}

int main(){
    Authentication auth_User;
    User current_User=auth_User.login();
    AdoptionShelter shelter(current_User);
    PeopleManager peopleMgr;
    AdoptionManager adoptionMgr(current_User,shelter);
    FacilityManager facilityMgr(current_User);

    if(current_User.role==INVALID){
        cout<<"ERROR..Invalid Username! or Password is Incorrect!"<<endl;
        return 1;
    }

    
    if(current_User.role==Manager || current_User.role==Staff){
        int choice=0;
        while(true){
            system("cls");
            display_MainMenu(current_User);
            cin>>choice;

            if(cin.fail()){
                cout<<"Invalid Input.Please Enter the Valid Nubmber."<<endl;
                cin.clear();
                cout<<"Press Enter to Continue..."<<endl;
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cin.get();
                continue;
            }

            if((current_User.role==Manager && choice==9)||
                (current_User.role==Staff && choice==5)||
                (current_User.role==Volunteer && choice==4)){
                break;
            }
            cin.ignore(numeric_limits<streamsize>::max(),'\n');

            if(current_User.role==Manager){
                switch(choice){
                    case 1:
                        shelter.showPetManagementMenu();
                        break;
                    case 2:
                        peopleMgr.showPeopleManagementMenu(current_User);
                        break;
                    case 3:
                        adoptionMgr.showAdoptionWorkflowMenu();
                        break;
                    case 4:
                        facilityMgr.showFacilityManagementMenu();
                        break;
                    case 5:
                        //Inventory & Financials
                        break;
                    case 6:
                        //Events & Fostering
                        break;
                    case 7:
                        //Lost & Found
                        break;
                    case 8:
                        //Reports & Analytics
                        break;
                    case 9:
                        //Exit
                        break;
                    default:
                        //ERROR
                        break;
                }
            }
            else if(current_User.role==Staff){
                switch(choice){
                    case 1:
                        shelter.showPetManagementMenu();
                        break;
                    case 2:
                        peopleMgr.showPeopleManagementMenu(current_User);
                        break;
                    case 3:
                        adoptionMgr.showAdoptionWorkflowMenu();
                        break;
                    case 4:
                        facilityMgr.showFacilityManagementMenu();
                        break;
                    case 5:
                        //Exit
                        break;
                    default:
                        //ERROR
                        break;
                }
            }
        }
        cout<<"Press Enter to Continue..."<<endl;
        cin.get();
    }
    else if(current_User.role==Volunteer){
        VolunteerPortal portal(current_User);
        portal.runVolunteerMenu();
    }
    

    cout<<endl<<endl<<"Thank You for Visiting our Site."<<endl;
    Sleep(3000);
    system("cls");

    return 0;
}