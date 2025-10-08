#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#pragma once
#include <string>
#include <vector>

/* ============================
          Customer
============================ */

class Customer {
public:
    Customer(int id, const std::string& name, int locationDistance, int maxOrders);

    // Getters
    const std::string& getName() const;
    int getId() const;
    int getCustomerDistance() const;
    int getMaxOrders() const;
    int getNumOrders() const;
    bool canMakeOrder() const;
    const std::vector<int>& getOrdersIds() const;

    // Order management
    int addOrder(int orderId); // Returns orderId if added successfully, -1 otherwise

    // Polymorphic interface
    virtual Customer* clone() const = 0;
    virtual ~Customer() = default;

private:
    const int id;
    const std::string name;
    const int locationDistance;
    const int maxOrders;
    std::vector<int> ordersId;
};

/* ============================
       SoldierCustomer
============================ */

class SoldierCustomer : public Customer {
public:
    SoldierCustomer(int id, const std::string& name, int locationDistance, int maxOrders);
    SoldierCustomer* clone() const override;
    ~SoldierCustomer() override = default;
};

/* ============================
       CivilianCustomer
============================ */

class CivilianCustomer : public Customer {
public:
    CivilianCustomer(int id, const std::string& name, int locationDistance, int maxOrders);
    CivilianCustomer* clone() const override;
    ~CivilianCustomer() override = default;
};

#endif // CUSTOMER_H_
