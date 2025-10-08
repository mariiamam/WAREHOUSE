#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <vector>

// Forward declarations (avoid heavy includes in headers)
class WareHouse;
class Volunteer;

// Global backup pointer (defined in main.cpp)
extern WareHouse* backup;

/* ============================
          Enums
============================ */

enum class ActionStatus { COMPLETED, ERROR };
enum class CustomerType { Soldier, Civilian };

/* ============================
        BaseAction
============================ */

class BaseAction {
public:
    BaseAction();
    virtual ~BaseAction() = default;

    ActionStatus getStatus() const;

    virtual void act(WareHouse& wareHouse) = 0;
    virtual std::string toString() const = 0;
    virtual BaseAction* clone() const = 0;

protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
    std::string statusToString(ActionStatus status) const;

private:
    std::string  errorMsg;
    ActionStatus status;
};

/* ============================
        SimulateStep
============================ */

class SimulateStep : public BaseAction {
public:
    explicit SimulateStep(int numOfSteps);
    void act(WareHouse& wareHouse) override;
    std::string toString() const override;
    SimulateStep* clone() const override;

private:
    const int numOfSteps;
};

/* ============================
          AddOrder
============================ */

class AddOrder : public BaseAction {
public:
    explicit AddOrder(int id);
    void act(WareHouse& wareHouse) override;
    std::string toString() const override;
    AddOrder* clone() const override;

private:
    const int customerId;
};

/* ============================
         AddCustomer
============================ */

class AddCustomer : public BaseAction {
public:
    AddCustomer(std::string customerName, std::string customerType,
                int distance, int maxOrders);

    void act(WareHouse& wareHouse) override;
    AddCustomer* clone() const override;
    std::string toString() const override;

    const CustomerType stringToCustomerType(std::string customerType) const;

private:
    const std::string  customerName;
    const CustomerType customerType;
    const int          distance;
    const int          maxOrders;
};

/* ============================
       PrintOrderStatus
============================ */

class PrintOrderStatus : public BaseAction {
public:
    explicit PrintOrderStatus(int id);
    void act(WareHouse& wareHouse) override;
    PrintOrderStatus* clone() const override;
    std::string toString() const override;

private:
    const int orderId;
};

/* ============================
     PrintCustomerStatus
============================ */

class PrintCustomerStatus : public BaseAction {
public:
    explicit PrintCustomerStatus(int customerId);
    void act(WareHouse& wareHouse) override;
    PrintCustomerStatus* clone() const override;
    std::string toString() const override;

private:
    const int customerId;
};

/* ============================
     PrintVolunteerStatus
============================ */

class PrintVolunteerStatus : public BaseAction {
public:
    explicit PrintVolunteerStatus(int id);
    void act(WareHouse& wareHouse) override;
    PrintVolunteerStatus* clone() const override;
    std::string toString() const override;

private:
    const int volunteerId;
};

/* ============================
       PrintActionsLog
============================ */

class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(WareHouse& wareHouse) override;
    PrintActionsLog* clone() const override;
    std::string toString() const override;
};

/* ============================
            Close
============================ */

class Close : public BaseAction {
public:
    Close();
    void act(WareHouse& wareHouse) override;
    Close* clone() const override;
    std::string toString() const override;
};

/* ============================
       BackupWareHouse
============================ */

class BackupWareHouse : public BaseAction {
public:
    BackupWareHouse();
    void act(WareHouse& wareHouse) override;
    BackupWareHouse* clone() const override;
    std::string toString() const override;
};

/* ============================
       RestoreWareHouse
============================ */

class RestoreWareHouse : public BaseAction {
public:
    RestoreWareHouse();
    void act(WareHouse& wareHouse) override;
    RestoreWareHouse* clone() const override;
    std::string toString() const override;
};

#endif // ACTION_H_
