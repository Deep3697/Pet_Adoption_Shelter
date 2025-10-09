#ifndef FINANCIAL_MANAGER_H
#define FINANCIAL_MANAGER_H

#include "Authentication.h"

struct InventoryItem{
    string name;
    int quantity;
    int threshold;
    double price;
    double totalValue;
};

class FinancialManager{
public:
    FinancialManager(const User& user);
    void showFinancialMenu();

private:
    User currentUser;
    void viewCurrentInventory();

    void addOrUpdateStock();
    double getCurrentBalance();
    void addTransaction(const string& type, const string& description, double amount);
    bool findItemInInventory(const string& searchName, InventoryItem& outItem);
    void addNewItemToInventory(const string& itemName);
    void updateExistingItem(const InventoryItem& item);
    void logUsage(const string& itemName, int usedQty);
    string getCurrentDate();

    void viewSupplyUsage();

    void viewLowStockItems();

    void logFinancialDonation();

    void logExpense();

    void viewTransactionLedger();

    void generateFinancialSummary();
};

#endif 