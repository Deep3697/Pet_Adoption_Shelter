#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include<bits/stdc++.h>
using namespace std;

enum UserRole{
    Manager,
    Staff,
    Volunteer,
    INVALID
};

struct User{    
    int id;
    string name;
    UserRole role;
};

class Authentication{
private:
    User verify_User(const string& id,const string& password);
public:
    User login();
};



#endif