#ifndef ORDER_H_
#define ORDER_H_

#pragma once
#include <string>
#include <vector>

/* ============================
          Enums & Macros
============================ */

enum class OrderStatus {
    PENDING,
    COLLECTING,
    DELIVERING,
    COMPLETED
};

#define NO_VOLUNTEER -1

/* ============================
             Order
============================ */

class Order {
public:
    Order(int id, int customerId, int distance);

    // Getters
    int getId() const;
    int getCustomerId() const;
    int getCollectorId() const;
    int getDriverId() const;
    int getDistance() const;
    OrderStatus getStatus() const;

    // Setters
    void setStatus(OrderStatus status);
    void setCollectorId(int collectorId);
    void setDriverId(int driverId);

    // Helpers
    const std::string toString() const;
    const std::string fromOrderToString(OrderStatus status) const;

    virtual ~Order() = default;

private:
    const int id;
    const int customerId;
    const int distance;
    OrderStatus status;

    int collectorId; // Initialized to NO_VOLUNTEER if none assigned
    int driverId;    // Initialized to NO_VOLUNTEER if none assigned
};

#endif // ORDER_H_
