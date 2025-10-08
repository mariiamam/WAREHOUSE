#include <string>
#include <vector>
#include <iostream>
#include "Customer.h"
#include "Volunteer.h"
#include "WareHouse.h"
#include "Action.h"
using std::string;
using std::vector;
using namespace std;

bool isBaked = false; // Global backup flag

/* ============================
        BaseAction
============================ */

BaseAction::BaseAction() : errorMsg(), status() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

string BaseAction::statusToString(ActionStatus status) const {
    return (status == ActionStatus::COMPLETED) ? "COMPLETED" : "ERROR";
}

/* ============================
        SimulateStep
============================ */

SimulateStep::SimulateStep(int numOfSteps)
    : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
    for (int i = 0; i < numOfSteps; i++)
        wareHouse.step();

    complete();
    wareHouse.addAction(this);
}

string SimulateStep::toString() const {
    return "simulateStep " + to_string(numOfSteps) + " " + statusToString(getStatus());
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

/* ============================
           AddOrder
============================ */

AddOrder::AddOrder(int id)
    : customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    int x = wareHouse.getCustomerCounter();

    if (x <= customerId) {
        error("cannot place this order\n");
        cout << getErrorMsg();
    } else {
        Customer &temp = wareHouse.getCustomer(customerId);
        int i = wareHouse.getIdCounter();

        if (temp.addOrder(i) != -1) {
            Order* order = new Order(i, temp.getId(), temp.getCustomerDistance());
            wareHouse.addOrder(order);
            complete();
        } else {
            error("cannot place this order\n");
            cout << getErrorMsg();
        }
    }
    wareHouse.addAction(this);
}

string AddOrder::toString() const {
    return "order " + to_string(customerId) + " " + statusToString(getStatus());
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

/* ============================
          AddCustomer
============================ */

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders)
    : customerName(customerName),
      customerType(stringToCustomerType(customerType)),
      distance(distance),
      maxOrders(maxOrders) {}

const CustomerType AddCustomer::stringToCustomerType(string CustomerType) const {
    return (CustomerType == "soldier") ? CustomerType::Soldier : CustomerType::Civilian;
}

void AddCustomer::act(WareHouse &wareHouse) {
    int i = wareHouse.getCustomerCounter();

    if (customerType == CustomerType::Civilian) {
        Customer *newcustomer = new CivilianCustomer(i, customerName, distance, maxOrders);
        wareHouse.Add_Customer(newcustomer);
    } else {
        Customer *newcustomer = new SoldierCustomer(i, customerName, distance, maxOrders);
        wareHouse.Add_Customer(newcustomer);
    }

    complete();
    wareHouse.addAction(this);
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    string type = (customerType == CustomerType::Civilian) ? "civilian" : "soldier";
    return "Customer " + customerName + " " + type + " " +
           to_string(distance) + " " + to_string(maxOrders) + " " +
           statusToString(getStatus());
}

/* ============================
       PrintOrderStatus
============================ */

PrintOrderStatus::PrintOrderStatus(int id)
    : orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    if (orderId < wareHouse.getIdCounter()) {
        Order tempOrder = wareHouse.getOrder(orderId);
        cout << tempOrder.toString();
        complete();
    } else {
        error("Order doesn't exist");
        cout << getErrorMsg();
    }

    wareHouse.addAction(this);
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
    return "orderStatus " + to_string(orderId) + " " + statusToString(getStatus());
}

/* ============================
      PrintCustomerStatus
============================ */

PrintCustomerStatus::PrintCustomerStatus(int customerId)
    : customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if (customerId < wareHouse.getCustomerCounter()) {
        Customer &tempCustomer = wareHouse.getCustomer(customerId);
        vector<int> orderIds = tempCustomer.getOrdersIds();
        int numOrdersLeft = tempCustomer.getMaxOrders() - tempCustomer.getNumOrders();

        cout << "CustomerID: " << tempCustomer.getId() << "\n";
        for (int order : orderIds) {
            Order &tempOrder = wareHouse.getOrder(order);
            cout << "OrderID: " << order << "\n";
            cout << "OrderStatus: " << tempOrder.fromOrderToString(tempOrder.getStatus()) << "\n";
        }
        cout << "numOrderLeft: " << numOrdersLeft << "\n";
        complete();
    } else {
        error("customer doesn't exist");
        cout << getErrorMsg();
    }

    wareHouse.addAction(this);
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const {
    return "customerStatus " + to_string(customerId) + " " + statusToString(getStatus());
}

/* ============================
     PrintVolunteerStatus
============================ */

PrintVolunteerStatus::PrintVolunteerStatus(int id)
    : volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    if (volunteerId < wareHouse.getCustomerCounter()) {
        Volunteer &tempVolunteer = wareHouse.getVolunteer(volunteerId);
        cout << tempVolunteer.toString();
        complete();
    } else {
        error("volunteer doesn't exist");
        cout << getErrorMsg();
    }

    wareHouse.addAction(this);
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    return "VolunteerStatus " + to_string(volunteerId) + " " + statusToString(getStatus());
}

/* ============================
       PrintActionsLog
============================ */

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    vector<BaseAction*> actions = wareHouse.getActions();
    for (BaseAction* action : actions)
        cout << action->toString() << "\n";

    complete();
    wareHouse.addAction(this);
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log " + statusToString(getStatus());
}

/* ============================
             Close
============================ */

Close::Close() {}

void Close::act(WareHouse &wareHouse) {
    wareHouse.close();
    complete();
    wareHouse.addAction(this);
}

Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close";
}

/* ============================
        BackupWareHouse
============================ */

BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (isBaked)
        delete backup;

    backup = new WareHouse(wareHouse);
    complete();
    wareHouse.addAction(this);
    isBaked = true;
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup " + statusToString(getStatus());
}

/* ============================
       RestoreWareHouse
============================ */

RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (isBaked) {
        wareHouse = *backup;
        complete();
    } else {
        error("no backup available");
        cout << getErrorMsg();
    }

    wareHouse.addAction(this);
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "restore " + statusToString(getStatus());
}
