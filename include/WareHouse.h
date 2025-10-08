#ifndef WAREHOUSE_H_
#define WAREHOUSE_H_

#pragma once
#include <string>
#include <vector>

// Forward declarations to avoid circular includes
class BaseAction;
class Volunteer;
class Order;
class Customer;

/* ============================
            WareHouse
   - Manages Volunteers, Customers,
     Actions, and Orders.
============================ */

class WareHouse {
public:
    // Construction / Rule of Five
    explicit WareHouse(const std::string& configFilePath);
    ~WareHouse();                                   // destructor
    WareHouse(const WareHouse& other);              // copy constructor
    WareHouse(WareHouse&& other);                   // move constructor
    WareHouse& operator=(const WareHouse& other);   // copy assignment
    WareHouse& operator=(WareHouse&& other);        // move assignment

    // Runtime
    void start();
    void step();
    void open();
    void close();

    // Mutations
    void addOrder(Order* order);
    void addAction(BaseAction* action);
    void Add_Customer(Customer* customer);
    void AddVolunteer(Volunteer* volunteer);

    // Accessors
    Customer&  getCustomer(int customerId) const;
    Volunteer& getVolunteer(int volunteerId) const;
    Order&     getOrder(int orderId) const;
    const std::vector<BaseAction*>& getActions() const;

    // Counters
    int getCustomerCounter();   // for assigning unique customer IDs
    int getVolunteerCounter();  // for assigning unique volunteer IDs
    int getIdCounter();         // global order ID counter

    // Helpers
    void sort();                // sort pending orders by ID
    int  indexof(Order* order); // index of an order within inProcessOrders

private:
    // State
    bool isOpen = false;
    int  idCounter = 0;
    int  customerCounter = 0;
    int  volunteerCounter = 0;

    // Data
    std::vector<BaseAction*> actionsLog;
    std::vector<Volunteer*>  volunteers;
    std::vector<Order*>      pendingOrders;
    std::vector<Order*>      inProcessOrders;
    std::vector<Order*>      completedOrders;
    std::vector<Customer*>   customers;
};

#endif // WAREHOUSE_H_
