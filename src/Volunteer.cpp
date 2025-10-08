#include <string>
#include <vector>
#include <iostream>
#include "Order.h"
#include "Volunteer.h"

using std::string;
using std::vector;
using namespace std;

#define NO_ORDER -1

/* ============================
            Volunteer
============================ */

Volunteer::Volunteer(int id, const string &name)
    : completedOrderId(-1),
      activeOrderId(-1),
      id(id),
      name(name) {}

int Volunteer::getId() const { return id; }

const string& Volunteer::getName() const { return name; }

int Volunteer::getActiveOrderId() const { return activeOrderId; }

int Volunteer::getCompletedOrderId() const { return completedOrderId; }

bool Volunteer::isBusy() const {
    // Signal whether the volunteer is currently processing an order
    return (getActiveOrderId() != NO_ORDER);
}

/* ============================
       CollectorVolunteer
============================ */

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name),
      coolDown(coolDown),
      timeLeft(coolDown) {}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    // If finished, move active -> completed and free the volunteer
    if (decreaseCoolDown()) {
        this->completedOrderId = this->activeOrderId;
        this->activeOrderId = NO_ORDER;
    }
}

// Simulate volunteer step: if the volunteer finished the order, transfer
// activeOrderId to completedOrderId

int CollectorVolunteer::getCoolDown() const { return coolDown; }

int CollectorVolunteer::getTimeLeft() const { return timeLeft; }

bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft != 0) this->timeLeft--;
    return (timeLeft == 0);
}
// Decrease timeLeft by 1, return true if timeLeft == 0, false otherwise

bool CollectorVolunteer::hasOrdersLeft() const { return true; }

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return (!Volunteer::isBusy() && (order.getStatus() == OrderStatus::PENDING));
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    this->activeOrderId = order.getId();
    this->timeLeft = this->coolDown;
}

// Prepare for new order (reset activeOrderId/timeLeft as needed)

string CollectorVolunteer::toString() const {
    string start =
        "volunteerID: " + std::to_string(getId()) + "\n" +
        "isBusy: ";

    if (isBusy()) {
        start += string("True\n") +
                 "OrderID: " + std::to_string(getActiveOrderId()) + "\n" +
                 "timeLeft: " + std::to_string(getTimeLeft()) + "\n" +
                 "OrdersLeft: No Limit\n";
    } else {
        start += string("False\n") +
                 "OrderID: None\n" +
                 "timeLeft: None\n" +
                 "OrdersLeft: No Limit\n";
    }
    return start;
}

/* ============================
   LimitedCollectorVolunteer
============================ */

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name,
                                                     int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown),
      maxOrders(maxOrders),
      ordersLeft(maxOrders) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    // True if the volunteer didn't reach orders limit
    return ordersLeft != 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return (hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order));
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}
// Prepare for new order (same as collector) + decrease ordersLeft

int LimitedCollectorVolunteer::getMaxOrders() const { return maxOrders; }

int LimitedCollectorVolunteer::getNumOrdersLeft() const { return ordersLeft; }

string LimitedCollectorVolunteer::toString() const {
    string start =
        "volunteerID: " + std::to_string(getId()) + "\n" +
        "isBusy: ";

    if (isBusy()) {
        start += string("True\n") +
                 "OrderID: " + std::to_string(getActiveOrderId()) + "\n" +
                 "timeLeft: " + std::to_string(getTimeLeft()) + "\n" +
                 "OrdersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";
    } else {
        start += string("False\n") +
                 "OrderID: None\n" +
                 "timeLeft: None\n" +
                 "OrdersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";
    }
    return start;
}

/* ============================
        DriverVolunteer
============================ */

DriverVolunteer::DriverVolunteer(int id, const string &name,
                                 int maxDistance, int distancePerStep)
    : Volunteer(id, name),
      maxDistance(maxDistance),
      distancePerStep(distancePerStep),
      distanceLeft(0) {}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const { return distanceLeft; }

int DriverVolunteer::getMaxDistance() const { return maxDistance; }

int DriverVolunteer::getDistancePerStep() const { return distancePerStep; }

bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft = distanceLeft - distancePerStep;
    if (distanceLeft <= distancePerStep) {
        distanceLeft = 0;
        return true;
    }
    return false;
}
// Decrease distanceLeft by distancePerStep, return true if distanceLeft <= 0

bool DriverVolunteer::hasOrdersLeft() const { return true; }

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    if (order.getStatus() == OrderStatus::COLLECTING) {
        if (!Volunteer::isBusy() && (order.getDistance() <= maxDistance)) {
            return true;
        }
    }
    return false;
}
// Signal if the volunteer is not busy and the order is within maxDistance

void DriverVolunteer::acceptOrder(const Order &order) {
    this->distanceLeft = order.getDistance();
    this->activeOrderId = order.getId();
} // Assign distanceLeft to order's distance

void DriverVolunteer::step() {
    if (decreaseDistanceLeft()) {
        this->completedOrderId = activeOrderId;
        this->activeOrderId = NO_ORDER;
    }
} // Decrease distanceLeft by distancePerStep

string DriverVolunteer::toString() const {
    string start =
        "volunteerID: " + std::to_string(getId()) + "\n" +
        "isBusy: ";

    if (isBusy()) {
        start += string("True\n") +
                 "OrderID: " + std::to_string(getActiveOrderId()) + "\n" +
                 "distanceLeft: " + std::to_string(getDistanceLeft()) + "\n" +
                 "OrdersLeft: No Limit\n";
    } else {
        start += string("False\n") +
                 "OrderID: None\n" +
                 "distanceLeft: None\n" +
                 "OrdersLeft: No Limit\n";
    }
    return start;
}

/* ============================
     LimitedDriverVolunteer
============================ */

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name,
                                               int maxDistance, int distancePerStep,
                                               int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep),
      maxOrders(maxOrders),
      ordersLeft(maxOrders) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const { return maxOrders; }

int LimitedDriverVolunteer::getNumOrdersLeft() const { return ordersLeft; }

bool LimitedDriverVolunteer::hasOrdersLeft() const { return ordersLeft > 0; }

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return (ordersLeft > 0 && DriverVolunteer::canTakeOrder(order));
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}
// Assign distanceLeft to order's distance and decrease ordersLeft

string LimitedDriverVolunteer::toString() const {
    string start =
        "volunteerID: " + std::to_string(getId()) + "\n" +
        "isBusy: ";

    if (isBusy()) {
        start += string("True\n") +
                 "OrderID: " + std::to_string(getActiveOrderId()) + "\n" +
                 "distanceLeft: " + std::to_string(getDistanceLeft()) + "\n" +
                 "OrdersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";
    } else {
        start += string("False\n") +
                 "OrderID: None\n" +
                 "distanceLeft: None\n" +
                 "OrdersLeft: " + std::to_string(getNumOrdersLeft()) + "\n";
    }
    return start;
}
