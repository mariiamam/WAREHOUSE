#include "Order.h"
#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using namespace std;

/* ============================
              Order
============================ */

Order::Order(int id, int customerId, int distance)
    : id(id),
      customerId(customerId),
      distance(distance),
      status(OrderStatus::PENDING),
      collectorId(NO_VOLUNTEER),
      driverId(NO_VOLUNTEER) {}

/* ---------- Getters ---------- */

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

int Order::getDistance() const {
    return distance;
}

OrderStatus Order::getStatus() const {
    return status;
}

/* ---------- Setters ---------- */

void Order::setStatus(OrderStatus status) {
    this->status = status;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

/* ---------- Helpers ---------- */

const string Order::fromOrderToString(OrderStatus status) const {
    if (status == OrderStatus::PENDING)
        return "Pending";
    else if (status == OrderStatus::COLLECTING)
        return "Collecting";
    else if (status == OrderStatus::DELIVERING)
        return "Delivering";
    else
        return "Completed";
}

/* ---------- String Representation ---------- */

const string Order::toString() const {
    string strStatus = fromOrderToString(getStatus());
    string start =
        "OrderId: " + to_string(id) + "\n"
        "OrderStatus: " + strStatus + "\n"
        "CustomerID: " + to_string(customerId) + "\n";

    if (status == OrderStatus::DELIVERING) {
        return start +
               "Collector: " + to_string(collectorId) + "\n" +
               "Driver: " + to_string(driverId) + "\n";
    }
    else if (status == OrderStatus::COLLECTING) {
        return start +
               "Collector: " + to_string(collectorId) + "\n" +
               "Driver: None\n";
    }
    else if (status == OrderStatus::COMPLETED) {
        return start +
               "Collector: " + to_string(collectorId) + "\n" +
               "Driver: " + to_string(driverId) + "\n";
    }

    return start + "Collector: None\nDriver: None\n";
}
