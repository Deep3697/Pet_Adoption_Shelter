#include<bits/stdc++.h>
using namespace std;
#include "Authentication.h"

User Authentication::login(){
    string INid,INpassword;
    cout<<"----SHELTER LOGIN SYSTEM----"<<endl;
    cout<<"Enter User ID:";
    cin>>INid;
    cout<<"Enter Password:";
    cin>>INpassword;

    return verify_User(INid,INpassword);
}

User Authentication::verify_User(const string& INid,const string& INpassword){

    ifstream userFile("users.csv");
    string line;

    if(!userFile.is_open()){
        cout<<"ERROR...File Could not opened."<<endl;
        return {-1,"",INVALID};
    }

    while(getline(userFile,line)){
        stringstream ss(line);
        string id,password,name,roleS;

        getline(ss,id,',');
        getline(ss,password,',');
        getline(ss,name,',');
        getline(ss,roleS,',');

        if(id==INid && password==INpassword){
            User got_User;
            got_User.id=stoi(id);
            got_User.name=name;
            if(roleS=="Manager") got_User.role=Manager;
            else if(roleS=="Staff") got_User.role=Staff;
            else if(roleS=="Volunteer") got_User.role=Volunteer;
            else if(roleS=="INVALID") got_User.role=INVALID;

            userFile.close();
            return got_User;
        }
    }
    userFile.close();
    return {-1,"",INVALID};
}