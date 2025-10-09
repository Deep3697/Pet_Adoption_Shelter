#include "financial_manager.h"
#include<bits/stdc++.h>

using namespace std;

FinancialManager::FinancialManager(const User& user):currentUser(user){}

string toLower(string s){
    transform(s.begin(),s.end(),s.begin(),[](unsigned char c){ return tolower(c); });
    return s;
}

void FinancialManager::viewCurrentInventory(){
    ifstream inventoryFile("inventory.csv");
    if(!inventoryFile.is_open()){
        cout<<"\nInventory is empty or file not found.\n";
        return;
    }
    vector<InventoryItem> items;
    string line;
    while(getline(inventoryFile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        InventoryItem item;
        string qtyStr, thresholdStr, priceStr, totalStr;

        getline(ss,item.name,',');
        getline(ss,qtyStr,',');
        getline(ss,thresholdStr,',');
        getline(ss,priceStr,',');
        getline(ss,totalStr,',');

        try{
            item.quantity=stoi(qtyStr);
            item.threshold=stoi(thresholdStr);
            item.price=stod(priceStr);
            item.totalValue=stod(totalStr);
            items.push_back(item);
        }catch(const invalid_argument &e){}
    }
    inventoryFile.close();

    sort(items.begin(),items.end(),[](const InventoryItem& a,const InventoryItem& b){
        return a.name < b.name;
    });

    cout<<"\n------ Current Shelter Inventory ------\n";
    cout<<"-------------------------------------------------------------------------------------------------\n";
    cout<<left<<setw(30)<<"Item Name"<<setw(12)<<"Quantity"<<setw(18)<<"Low Stock Level"<<setw(15)<<"Price/Unit"<<setw(15)<<"Total Value"<<"Alert\n";
    cout<<"-------------------------------------------------------------------------------------------------\n";

    if(items.empty()){
        cout<<"No items in inventory.\n";
    }else{
        for(const auto& item : items){
            cout<<left<<setw(30)<<item.name<<setw(12)<<item.quantity<<setw(18)<<item.threshold
                <<setw(15)<<fixed<<setprecision(2)<<item.price<<setw(15)<<item.totalValue;
            if(item.quantity<=item.threshold){
                cout<<"<< LOW STOCK!";
            }
            cout<<"\n";
        }
    }
    cout<<"-------------------------------------------------------------------------------------------------\n";
}


void FinancialManager::addOrUpdateStock(){
    string name;
    cout<<"\nEnter the name of the inventory item: ";
    getline(cin,name);

    InventoryItem foundItem;
    if(findItemInInventory(name, foundItem)){
        updateExistingItem(foundItem);
    }else{
        addNewItemToInventory(name);
    }
}

void FinancialManager::addNewItemToInventory(const string& itemName){
    cout<<"'"<<itemName<<"' is a new item. Please provide details.\n";
    int qty, threshold;
    double price;
    cout<<"Enter current quantity: ";
    cin>>qty;
    cout<<"Enter low-stock threshold: ";
    cin>>threshold;
    cout<<"Enter price per unit: ";
    cin>>price;

    double totalCost=qty*price;
    double currentBalance=getCurrentBalance();

    // INSUFFICIENT BALANCE CHECK
    if(currentBalance-totalCost < 1000.0){
        cout<<"\nError: Insufficient Balance! This purchase would drop the balance below 1000.\n";
        cout<<"Current Balance: "<<currentBalance<<"\nPurchase Cost: "<<totalCost<<endl;
        cout<<"\nPress Enter to acknowledge.";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    // Add to inventory
    ofstream invFile("inventory.csv",ios::app);
    if(invFile.is_open()){
        invFile<<itemName<<","<<qty<<","<<threshold<<","<<price<<","<<totalCost<<"\n";
        invFile.close();
        cout<<"\nItem '"<<itemName<<"' added to inventory.\n";
        // Log the expense
        addTransaction("Expense","Purchase of "+itemName, -totalCost);
    }else{
        cout<<"Error opening inventory file.\n";
    }
}



void FinancialManager::updateExistingItem(const InventoryItem& item){
    cout<<"\nItem '"<<item.name<<"' found (Current Quantity: "<<item.quantity<<").\n";
    cout<<"1. Add more stock\n";
    cout<<"2. Log usage\n";
    cout<<"Choice: ";
    int choice=0;
    cin>>choice;

    int changeQty=0;
    if(choice==1){
        cout<<"Enter quantity to add: "; cin>>changeQty;
        double price;
        cout<<"Enter price per unit for this purchase: "; cin>>price;
        double totalCost=changeQty*price;
        double currentBalance=getCurrentBalance();
        if(currentBalance-totalCost < 1000.0){
            cout<<"\nError: Insufficient Balance!\n";
            return;
        }
        addTransaction("Expense","Restock of "+item.name, -totalCost);
    }else if(choice==2){
        cout<<"Enter quantity used: "; cin>>changeQty;
        if(changeQty > item.quantity){
            cout<<"Error: Cannot use more than is in stock.\n";
            return;
        }
        logUsage(item.name, changeQty);
        changeQty = -changeQty; // Make it a negative number to subtract from stock
    }else{
        cout<<"Invalid choice.\n";
        return;
    }

    ifstream inFile("inventory.csv");
    ofstream outFile("temp.csv");
    string line;
    while(getline(inFile,line)){
        stringstream ss(line);
        string name;
        getline(ss,name,',');
        if(toLower(name)==toLower(item.name)){
            int newQty=item.quantity+changeQty;
            double newTotalValue=newQty*item.price;
            outFile<<item.name<<","<<newQty<<","<<item.threshold<<","<<item.price<<","<<newTotalValue<<"\n";
        }else{
            outFile<<line<<"\n";
        }
    }
    inFile.close();
    outFile.close();
    remove("inventory.csv");
    rename("temp.csv","inventory.csv");
    cout<<"Inventory updated successfully.\n";
}
// --- Helper Method Implementations ---

double FinancialManager::getCurrentBalance(){
    ifstream transFile("transactions.csv");
    double balance=0.0;
    string line;
    if(transFile.is_open()){
        while(getline(transFile,line)){
            stringstream ss(line);
            string date, type, desc, amountStr;
            getline(ss,date,',');
            getline(ss,type,',');
            getline(ss,desc,',');
            getline(ss,amountStr,',');
            try{ balance+=stod(amountStr); } catch(const invalid_argument &e){}
        }
    }
    return balance;
}

void FinancialManager::addTransaction(const string& type, const string& description, double amount){
    ofstream transFile("transactions.csv",ios::app);
    if(transFile.is_open()){
        transFile<<getCurrentDate()<<","<<type<<",\""<<description<<"\","<<amount<<"\n";
        transFile.close();
        cout<<"Financial transaction logged.\n";
    }
}

void FinancialManager::logUsage(const string& itemName, int usedQty){
    ofstream usageFile("usage_log.csv",ios::app);
    if(usageFile.is_open()){
        usageFile<<getCurrentDate()<<",\""<<itemName<<"\","<<usedQty<<"\n";
        usageFile.close();
    }
}



// --- The Complete findItemInInventory Method ---
bool FinancialManager::findItemInInventory(const string& searchName, InventoryItem& outItem){
    ifstream invFile("inventory.csv");
    if(!invFile.is_open()){return false;}

    string lowerSearchName=toLower(searchName);
    string line;

    while(getline(invFile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string name, qtyStr, thresholdStr, priceStr, totalStr;

        getline(ss,name,',');
        string lowerName=toLower(name);

        if(lowerName==lowerSearchName){
            // Match found! Populate the outItem struct with the data.
            getline(ss,qtyStr,',');
            getline(ss,thresholdStr,',');
            getline(ss,priceStr,',');
            getline(ss,totalStr,',');
            try{
                outItem.name=name; // Store the original name, not the lowercase one
                outItem.quantity=stoi(qtyStr);
                outItem.threshold=stoi(thresholdStr);
                outItem.price=stod(priceStr);
                outItem.totalValue=stod(totalStr);
            }catch(const invalid_argument &e){
                // If data is corrupted, treat it as not found
                invFile.close();
                return false;
            }
            invFile.close();
            return true; // Return true to signal the item was found
        }
    }
    invFile.close();
    return false; // Return false if the loop finishes and no match was found
}

string FinancialManager::getCurrentDate(){
    time_t now = time(0);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return string(buf);
}


void FinancialManager::viewSupplyUsage  (){
    ifstream usageFile("usage_log.csv");
    string line;

    if(!usageFile.is_open()){
        cout<<"\nNo supply usage has been logged yet.\n";
        return;
    }

    cout<<"\n----- Supply Usage History -----\n";
    cout<<"------------------------------------------------------------------------------\n";
    cout<<left<<setw(15)<<"Date"<<setw(30)<<"Item Name"<<setw(15)<<"Quantity Used"<<"\n";
    cout<<"------------------------------------------------------------------------------\n";

    while(getline(usageFile,line)){
        stringstream ss(line);
        string date, itemName, qtyUsed;

        getline(ss,date,',');
        getline(ss,itemName,',');
        getline(ss,qtyUsed,',');

        cout<<left<<setw(15)<<date<<setw(30)<<itemName<<setw(15)<<qtyUsed<<"\n";
    }
    cout<<"------------------------------------------------------------------------------\n";
    usageFile.close();
}

void FinancialManager::viewLowStockItems(){
    ifstream inventoryFile("inventory.csv");
    string line;
    int lowStockCount=0;

    if(!inventoryFile.is_open()){
        cout<<"\nInventory is empty or file not found.\n";
        return;
    }

    cout<<"\n--- Items Currently Low on Stock ---\n";
    cout<<"------------------------------------------------------------------------------\n";
    cout<<left<<setw(30)<<"Item Name"<<setw(15)<<"Quantity"<<setw(20)<<"Threshold"<<"\n";
    cout<<"------------------------------------------------------------------------------\n";

    while(getline(inventoryFile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string name, qtyStr, thresholdStr;
        int quantity=0, threshold=0;

        getline(ss,name,',');
        getline(ss,qtyStr,',');
        getline(ss,thresholdStr,',');

        try{
            quantity=stoi(qtyStr);
            threshold=stoi(thresholdStr);
        }catch(const invalid_argument &e){ continue; } // Skip malformed lines

        // This is the key: only print if quantity is at or below the threshold
        if(quantity<=threshold){
            cout<<left<<setw(30)<<name<<setw(15)<<quantity<<setw(20)<<threshold<<"\n";
            lowStockCount++;
        }
    }
    
    if(lowStockCount==0){
        cout<<"No items are currently low on stock.\n";
    }
    cout<<"------------------------------------------------------------------------------\n";
    inventoryFile.close();
}


void FinancialManager::logFinancialDonation(){
    string name, contact;
    double amount=0.0;

    cout<<"\n----- Log a Financial Donation -----\n";
    cout<<"Enter Donor's Full Name: ";
    getline(cin,name);
    cout<<"Enter Donor's Contact Details (Phone/Email): ";
    getline(cin,contact);
    cout<<"Enter Donation Amount: ";
    cin>>amount;

    if(cin.fail() || amount<=0){
        cout<<"Invalid amount entered.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        return;
    }

    string date=getCurrentDate();  //In built function

    // Save the detailed donation to donations.csv
    ofstream donationFile("donations.csv",ios::app);
    if(donationFile.is_open()){
        donationFile<<date<<",\""<<name<<"\",\""<<contact<<"\","<<amount<<"\n";
        donationFile.close();
    }else{
        cout<<"Error: Could not open donations log file.\n";
    }

    // Add the income to the main transaction ledger
    addTransaction("Income", "Donation from "+name, amount);

    cout<<"\nDonation from "<<name<<" logged successfully.\n";
}

void FinancialManager::logExpense(){
    string description;
    double amount=0.0;

    cout<<"\n------ Log an Expense ------\n";
    cout<<"Enter Expense Description (e.g., Vet Bill, Rent): ";
    getline(cin,description);
    cout<<"Enter Expense Amount: ";
    cin>>amount;

    if(cin.fail() || amount<=0){
        cout<<"Invalid amount entered.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        return;
    }
    double currentBalance=getCurrentBalance();

    if(currentBalance-amount < 1000.0){
        cout<<"\nError: Insufficient Balance! This expense would drop the balance below 1000.\n";
        cout<<"Current Balance: "<<currentBalance<<"\nExpense Cost: "<<amount<<endl;
        cout<<"\nPress Enter to acknowledge.";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }
    addTransaction("Expense", description, -amount);
}


void FinancialManager::viewTransactionLedger(){
    ifstream transFile("transactions.csv");
    string line;
    int recordCount=0;

    if(!transFile.is_open()){
        cout<<"\nNo transactions have been logged yet.\n";
        return;
    }

    cout<<"\n----- Full Transaction Ledger -----\n";
    cout<<"----------------------------------------------------------------------------------------\n";
    cout<<left<<setw(12)<<"Date"<<setw(10)<<"Type"<<setw(40)<<"Description"<<setw(10)<<"Amount"<<"\n";
    cout<<"----------------------------------------------------------------------------------------\n";

    while(getline(transFile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string date, type, desc, amountStr;
        double amount=0.0;

        getline(ss,date,',');
        getline(ss,type,',');
        getline(ss,desc,',');
        getline(ss,amountStr,',');
        
        try{ amount=stod(amountStr); }catch(const invalid_argument &e){}

        cout<<left<<setw(12)<<date<<setw(10)<<type<<setw(40)<<desc<<right<<setw(10)<<fixed<<setprecision(2)<<amount<<"\n";
        recordCount++;
    }
    if(recordCount==0){
        cout<<"No transactions found.\n";
    }
    cout<<"----------------------------------------------------------------------------------------\n";
    transFile.close();
}

void FinancialManager::generateFinancialSummary(){
    ifstream transFile("transactions.csv");
    string line;
    double totalIncome=0.0;
    double totalExpenses=0.0;

    if(!transFile.is_open()){
        cout<<"\nNo transactions have been logged yet.\n";
        return;
    }
    while(getline(transFile,line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string date, type, desc, amountStr;
        double amount=0.0;

        getline(ss,date,',');
        getline(ss,type,',');
        getline(ss,desc,',');
        getline(ss,amountStr,',');
        
        try{
            amount=stod(amountStr);
            if(type=="Income"){
                totalIncome+=amount;
            }else if(type=="Expense"){
                totalExpenses+=amount;
            }
        }catch(const invalid_argument &e){}
    }
    transFile.close();

    double netBalance=totalIncome+totalExpenses;

    cout<<"\n----- Financial Summary Report -----\n";
    cout<<"-------------------------------------------------\n";
    cout<<left<<setw(20)<<"Total Income:"<<right<<setw(10)<<fixed<<setprecision(2)<<totalIncome<<"\n";
    cout<<left<<setw(20)<<"Total Expenses:"<<right<<setw(10)<<fixed<<setprecision(2)<<totalExpenses<<"\n";
    cout<<"-------------------------------------------------\n";
    cout<<left<<setw(20)<<"Net Balance:"<<right<<setw(10)<<fixed<<setprecision(2)<<netBalance<<"\n";
    cout<<"-------------------------------------------------\n";
}


void FinancialManager::showFinancialMenu(){
    int choice=0;
    while(true){
        system("cls");
        cout<<"------ INVENTORY & FINANCIALS ------\n\n";
        cout<<"---- Inventory Management ----\n";
        cout<<"1. View Current Inventory\n";
        cout<<"2. Add/Update Stock\n";
        cout<<"3. View Supply Usage History\n"; 
        cout<<"4. View Low-Stock Items\n\n";
        cout<<"---- Financial Management ----\n";
        cout<<"5. Log a Financial Donation\n";
        cout<<"6. Log an Expense\n";
        cout<<"7. View Transaction Ledger\n";
        cout<<"8. Generate Financial Summary Report\n\n";
        cout<<"9. Return to Main Menu\n";
        cout<<"Enter Your Choice: ";
        cin>>choice;

        if(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Invalid Input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        switch(choice){
            case 1: viewCurrentInventory(); break;
            case 2: addOrUpdateStock(); break;
            case 3: viewSupplyUsage(); break;
            case 4: viewLowStockItems(); break;
            case 5: logFinancialDonation(); break;
            case 6: logExpense(); break;
            case 7: viewTransactionLedger(); break;
            case 8: generateFinancialSummary(); break;
            case 9: return;
            default: cout<<"Invalid Choice.\n"; break;
        }
        cin.clear();
        cout<<"Press Enter to continue"<<endl;
        cin.get();
    }
}
