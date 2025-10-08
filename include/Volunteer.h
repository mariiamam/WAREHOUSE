#ifndef VOLUNTEER_H_
#define VOLUNTEER_H_

#pragma once
#include <string>
#include <vector>
#include "Order.h"

/* ============================
          Constants
============================ */
#define NO_ORDER -1

/* ============================
          Volunteer
============================ */

class Volunteer {
public:
    Volunteer(int id, const std::string& name);

    // Getters
    int getId() const;
    const std::string& getName() const;
    int getActiveOrderId() const;
    int getCompletedOrderId() const;

    // State checks
    bool isBusy() const; // True if currently processing an order
    virtual bool hasOrdersLeft() const = 0; // True if volunteer can still take new orders
    virtual bool canTakeOrder(const Order& order) const = 0; // Checks if volunteer can take this order

    // Order handling
    virtual void acceptOrder(const Order& order) = 0; // Prepare for a new order
    virtual void step() = 0; // Simulate a single time step

    // Polymorphic interface
    virtual std::string toString() const = 0;
    virtual Volunteer* clone() const = 0;

    virtual ~Volunteer() = default;

protected:
    int completedOrderId; // NO_ORDER if no order completed yet
    int activeOrderId;    // NO_ORDER if idle

private:
    const int id;
    const std::string name;
};

/* ============================
       CollectorVolunteer
============================ */

class CollectorVolunteer : public Volunteer {
public:
    CollectorVolunteer(int id, const std::string& name, int coolDown);

    CollectorVolunteer* clone() const override;
    void step() override;

    // Accessors
    int getCoolDown() const;
    int getTimeLeft() const;

    // Behavior
    bool decreaseCoolDown(); // Decrease timeLeft by 1, return true if finished
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order& order) const override;
    void acceptOrder(const Order& order) override;
    std::string toString() const override;

    ~CollectorVolunteer() override = default;

private:
    const int coolDown; // Time it takes to process one order
    int timeLeft;       // Time remaining for the current order
};

/* ============================
   LimitedCollectorVolunteer
============================ */

class LimitedCollectorVolunteer : public CollectorVolunteer {
public:
    LimitedCollectorVolunteer(int id, const std::string& name, int coolDown, int maxOrders);

    LimitedCollectorVolunteer* clone() const override;

    // Behavior
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order& order) const override;
    void acceptOrder(const Order& order) override;

    // Accessors
    int getMaxOrders() const;
    int getNumOrdersLeft() const;
    std::string toString() const override;

    ~LimitedCollectorVolunteer() override = default;

private:
    const int maxOrders; // Total orders this volunteer can complete
    int ordersLeft;      // Remaining number of orders they can take
};

/* ============================
        DriverVolunteer
============================ */

class DriverVolunteer : public Volunteer {
public:
    DriverVolunteer(int id, const std::string& name, int maxDistance, int distancePerStep);

    DriverVolunteer* clone() const override;

    // Accessors
    int getDistanceLeft() const;
    int getMaxDistance() const;
    int getDistancePerStep() const;

    // Behavior
    bool decreaseDistanceLeft(); // Returns true if order completed
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order& order) const override;
    void acceptOrder(const Order& order) override;
    void step() override;
    std::string toString() const override;

    ~DriverVolunteer() override = default;

private:
    const int maxDistance;      // Max distance this volunteer can handle
    const int distancePerStep;  // Distance covered per step
    int distanceLeft;           // Remaining distance for current order
};

/* ============================
   LimitedDriverVolunteer
============================ */

class LimitedDriverVolunteer : public DriverVolunteer {
public:
    LimitedDriverVolunteer(int id, const std::string& name,
                           int maxDistance, int distancePerStep, int maxOrders);

    LimitedDriverVolunteer* clone() const override;

    // Accessors
    int getMaxOrders() const;
    int getNumOrdersLeft() const;

    // Behavior
    bool hasOrdersLeft() const override;
    bool canTakeOrder(const Order& order) const override;
    void acceptOrder(const Order& order) override;
    std::string toString() const override;

    ~LimitedDriverVolunteer() override = default;

private:
    const int maxOrders;
    int ordersLeft;
};

#endif // VOLUNTEER_H_
