#pragma once
#include <string>
#include <vector>
#include <Volunteer.h>
#include "Order.h"
#include "Customer.h"
#include <cstddef>


class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        ~WareHouse();// destructure
        WareHouse ( const WareHouse& other);// copy constructure
        WareHouse( WareHouse && other);///move copy
        WareHouse& operator=( WareHouse &&other);
        WareHouse& operator=(const WareHouse& other);
        
        

        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        int getCustomerCounter();/// i add 
        int getIdCounter();/// i add 
        void Add_Customer(Customer* coustomer);// i add, there is one on action so i added another one here
        void step(); 
        int getVolunteerCounter();

       //int getCustomerCounter();
       void AddVolunteer(Volunteer* volunteer);
       void sorting(Order* order);
       void sort();
       int indexof( Order* order);

    



    private:
        bool isOpen;
        int idCounter;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
       
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
       
        
       
        
       
        
        
};
