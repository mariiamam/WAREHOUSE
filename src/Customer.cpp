#include <iostream>
#include <string>
#include <vector>
#include "Customer.h"

using std::string;
using std::vector;

/* ============================
           Customer
============================ */

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
    : id(id),
      name(name),
      locationDistance(locationDistance),
      maxOrders(maxOrders),
      ordersId() {}

const string& Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {
    return static_cast<int>(ordersId.size());
}

bool Customer::canMakeOrder() const {
    // True if the customer did not reach max orders yet
    return getMaxOrders() != getNumOrders();
}

const vector<int>& Customer::getOrdersIds() const {
    // Note: the trailing 'const' means this method does not modify the object,
    // and the returned reference is to a const vector (caller can't modify it).
    return ordersId;
}

int Customer::addOrder(int orderId) {
    // Return orderId if added successfully, -1 otherwise
    if (canMakeOrder()) {
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

/* ============================
       SoldierCustomer
============================ */

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer* SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

/* ============================
       CivilianCustomer
============================ */

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer* CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
