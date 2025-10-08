#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Volunteer.h"
#include "Order.h"
#include "Customer.h"
#include "Action.h"
#include "WareHouse.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;

/* ===========================================
                Core Lifecycle
   =========================================== */

void WareHouse::start() {
    open();
    cout << "Warehouse is open!" << endl;

    string mission;
    while (isOpen) {
        if (!std::getline(std::cin, mission)) break;

        std::istringstream iss(mission);
        vector<string> words;
        string word;
        while (iss >> word) words.push_back(word);

        if (words.empty()) continue; // avoid out_of_range on words.at(0)

        const string &cmd = words[0];

        if (cmd == "order") {
            int customerID = std::stoi(words[1]);
            BaseAction* curr = new AddOrder(customerID);
            curr->act(*this);
        }
        else if (cmd == "orderStatus") {
            int orderId = std::stoi(words[1]);
            BaseAction* curr = new PrintOrderStatus(orderId);
            curr->act(*this);
        }
        else if (cmd == "volunteerStatus") {
            int volunteerId = std::stoi(words[1]);
            BaseAction* curr = new PrintVolunteerStatus(volunteerId);
            curr->act(*this);
        }
        else if (cmd == "customerStatus") {
            int customerId = std::stoi(words[1]);
            BaseAction* curr = new PrintCustomerStatus(customerId);
            curr->act(*this);
        }
        else if (cmd == "step") {
            int numSteps = std::stoi(words[1]);
            BaseAction* curr = new SimulateStep(numSteps);
            curr->act(*this);
        }
        else if (cmd == "close") {
            BaseAction* curr = new Close();
            curr->act(*this);
        }
        else if (cmd == "log") {
            BaseAction* curr = new PrintActionsLog();
            curr->act(*this);
        }
        else if (cmd == "backup") {
            BaseAction* curr = new BackupWareHouse();
            curr->act(*this);
        }
        else if (cmd == "customer") {
            string name = words[1];
            string type = words[2];
            int distance = std::stoi(words[3]);
            int maxOrders = std::stoi(words[4]);
            AddCustomer curr(name, type, distance, maxOrders);
            curr.act(*this);
        }
        else if (cmd == "restore") {
            BaseAction* curr = new RestoreWareHouse();
            curr->act(*this);
        }
        // else: unknown command -> ignore silently (or print help)
    }
}

void WareHouse::step() {
    // Try to assign pending orders to volunteers
    for (unsigned int index = 0; index < pendingOrders.size(); index++) {
        bool found = false;

        for (Volunteer* v : volunteers) {
            if (found) break;

            if (v->canTakeOrder(*pendingOrders.at(index))) {
                found = true;
                v->acceptOrder(*pendingOrders.at(index));

                if (pendingOrders.at(index)->getStatus() == OrderStatus::COLLECTING) {
                    pendingOrders.at(index)->setDriverId(v->getId());
                    pendingOrders.at(index)->setStatus(OrderStatus::DELIVERING);
                } else {
                    pendingOrders.at(index)->setCollectorId(v->getId());
                    pendingOrders.at(index)->setStatus(OrderStatus::COLLECTING);
                }

                inProcessOrders.push_back(pendingOrders.at(index));
                pendingOrders.erase(pendingOrders.begin() + index);
                index--;
            }
        }
        // if no volunteer can take it now, keep it in place and move on
    }

    // Advance volunteers & move finished orders
    int idx = 0;
    for (Volunteer* vol : volunteers) {
        if (vol->isBusy()) {
            vol->step();

            if (vol->getActiveOrderId() == -1) {
                int orderId = vol->getCompletedOrderId();
                Order* order = &getOrder(orderId);

                if (order->getStatus() == OrderStatus::DELIVERING) {
                    order->setStatus(OrderStatus::COMPLETED);
                    completedOrders.push_back(order);
                    int rm = indexof(order);
                    inProcessOrders.erase(inProcessOrders.begin() + rm);
                } else {
                    if (order->getDriverId() != -1) {
                        order->setStatus(OrderStatus::COMPLETED);
                        completedOrders.push_back(order);
                        int rm = indexof(order);
                        inProcessOrders.erase(inProcessOrders.begin() + rm);
                    } else {
                        pendingOrders.push_back(order);
                        sort();
                        int rm = indexof(order);
                        inProcessOrders.erase(inProcessOrders.begin() + rm);
                    }
                }
            }
        }

        if (!vol->hasOrdersLeft()) {
            volunteers.erase(volunteers.begin() + idx);
            delete vol;
            vol = nullptr;
        } else {
            idx++;
        }
    }
}

/* ===========================================
                Helpers / Utils
   =========================================== */

int WareHouse::indexof(Order* order) {
    int idx = 0;
    for (Order* curr : inProcessOrders) {
        if (order->getId() != curr->getId())
            idx++;
        else
            return idx;
    }
    return 0;
}

static bool compare(const Order* a, const Order* b) {
    return a->getId() < b->getId();
}

void WareHouse::sort() {
    // FIX: second iterator should be end(), not begin()
    std::sort(pendingOrders.begin(), pendingOrders.end(), compare);
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
    idCounter++;
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const {
    return *(customers[customerId]);
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer* v : volunteers)
        if (v->getId() == volunteerId)
            return *v;

    static CollectorVolunteer NOVOLUNTEER(0, "0", 0);
    return NOVOLUNTEER;
}

Order& WareHouse::getOrder(int orderId) const {
    if (orderId < idCounter) {
        for (Order* o : pendingOrders)      if (o->getId() == orderId) return *o;
        for (Order* o : inProcessOrders)    if (o->getId() == orderId) return *o;
        for (Order* o : completedOrders)    if (o->getId() == orderId) return *o;
    }
    static Order NOORDER(0, 0, 0);
    return NOORDER;
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
    isOpen = false;

    for (Order* o : pendingOrders) {
        if (o->fromOrderToString(o->getStatus()) == "PENDING") {
            cout << "OrderID:" << o->getId()
                 << ",CustomerID: " << o->getCustomerId()
                 << ", Status:PENDING\n";
        } else {
            cout << "OrderID:" << o->getId()
                 << ",Customer: " << o->getCustomerId()
                 << ", Status:COLLECTING\n";
        }
    }

    for (Order* o : inProcessOrders) {
        if (o->fromOrderToString(o->getStatus()) == "COLLECTING") {
            cout << "OrderID:" << o->getId()
                 << ",CustomerID: " << o->getCustomerId()
                 << ", Status:COLLECTING\n";
        } else {
            cout << "OrderID:" << o->getId()
                 << ",CustomerID: " << o->getCustomerId()
                 << ", Status:DELIVERING\n";
        }
    }

    for (Order* o : completedOrders) {
        cout << "OrderID: " << o->getId()
             << ", CustomerID: " << o->getCustomerId()
             << ", Status: COMPLETED\n";
    }
}

void WareHouse::open() { isOpen = true; }

int WareHouse::getIdCounter() { return idCounter; }

void WareHouse::Add_Customer(Customer* customer) {
    customers.push_back(customer);
    customerCounter++;
}

void WareHouse::AddVolunteer(Volunteer* volunteer) {
    volunteers.push_back(volunteer);
    volunteerCounter++;
}

int WareHouse::getCustomerCounter() { return customerCounter; }

int WareHouse::getVolunteerCounter() { return volunteerCounter; }

/* ===========================================
           Rule of Five / Construction
   =========================================== */

WareHouse::WareHouse(const string &configFilePath)
    : isOpen(false),
      idCounter(0),
      customerCounter(0),
      volunteerCounter(0),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers() {

    std::ifstream inputFile(configFilePath);
    string line;

    while (std::getline(inputFile, line)) {
        int count = 0;
        std::istringstream iss(line);
        vector<string> words;
        string w;

        while (iss >> w) {
            count++;
            words.push_back(w);
        }
        if (count == 0 || words[0] == "#") continue;

        if (words[0] == "customer") {
            int distance  = std::stoi(words[3]);
            int maxOrders = std::stoi(words[4]);

            if (words[2] == "civilian") {
                Customer* c = new CivilianCustomer(customerCounter, words[1], distance, maxOrders);
                Add_Customer(c);
            } else {
                Customer* c = new SoldierCustomer(customerCounter, words[1], distance, maxOrders);
                Add_Customer(c);
            }
        }

        if (words[0] == "volunteer") {
            if (words[2] == "collector") {
                int coolDown = std::stoi(words[3]);
                Volunteer* v = new CollectorVolunteer(volunteerCounter, words[1], coolDown);
                AddVolunteer(v);
            } else if (words[2] == "limited_collector") {
                int coolDown  = std::stoi(words[3]);
                int maxOrders = std::stoi(words[4]);
                Volunteer* v  = new LimitedCollectorVolunteer(volunteerCounter, words[1], coolDown, maxOrders);
                AddVolunteer(v);
            } else if (words[2] == "driver") {
                int maxDistance    = std::stoi(words[3]);
                int distancePerStep = std::stoi(words[4]);
                Volunteer* v = new DriverVolunteer(volunteerCounter, words[1], maxDistance, distancePerStep);
                AddVolunteer(v);
            } else if (words[2] == "limited_driver") {
                int maxDistance     = std::stoi(words[3]);
                int distancePerStep = std::stoi(words[4]);
                int maxOrder        = std::stoi(words[5]);
                Volunteer* v = new LimitedDriverVolunteer(volunteerCounter, words[1],
                                                          maxDistance, distancePerStep, maxOrder);
                AddVolunteer(v);
            }
        }
    }
}

WareHouse::WareHouse(const WareHouse& other)
    : isOpen(other.isOpen),
      idCounter(other.idCounter),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers() {

    if (this != &other) {
        for (BaseAction* a : other.actionsLog)     actionsLog.push_back(a->clone());
        for (Volunteer*  v : other.volunteers)     volunteers.push_back(v->clone());
        for (Order*     o : other.pendingOrders)   pendingOrders.push_back(new Order(*o));
        for (Order*     o : other.inProcessOrders) inProcessOrders.push_back(new Order(*o));
        for (Order*     o : other.completedOrders) completedOrders.push_back(new Order(*o));
        for (Customer*  c : other.customers)       customers.push_back(c->clone());
    }
}

WareHouse::WareHouse(WareHouse&& other)
    : isOpen(false),
      idCounter(0),
      customerCounter(0),
      volunteerCounter(0),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers() {

    actionsLog      = other.actionsLog;       other.actionsLog.clear();
    volunteers      = other.volunteers;       other.volunteers.clear();
    inProcessOrders = other.inProcessOrders;  other.inProcessOrders.clear();
    pendingOrders   = other.pendingOrders;    other.pendingOrders.clear();
    completedOrders = other.completedOrders;  other.completedOrders.clear();
    customers       = other.customers;        other.customers.clear();
}

WareHouse& WareHouse::operator=(const WareHouse &other) {
    if (&other != this) {
        isOpen          = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter= other.volunteerCounter;
        idCounter       = other.idCounter;

        for (BaseAction* a : actionsLog) delete a;
        actionsLog.clear();
        for (BaseAction* a : other.actionsLog) actionsLog.push_back(a->clone());

        for (Volunteer* v : volunteers) delete v;
        volunteers.clear();
        for (Volunteer* v : other.volunteers) volunteers.push_back(v->clone());

        for (Order* o : pendingOrders) delete o;
        pendingOrders.clear();
        for (Order* o : other.pendingOrders) pendingOrders.push_back(new Order(*o));

        for (Order* o : inProcessOrders) delete o;
        inProcessOrders.clear();
        for (Order* o : other.inProcessOrders) inProcessOrders.push_back(new Order(*o));

        for (Order* o : completedOrders) delete o;
        completedOrders.clear();
        for (Order* o : other.completedOrders) completedOrders.push_back(new Order(*o));

        for (Customer* c : customers) delete c;
        customers.clear();
        for (Customer* c : other.customers) customers.push_back(c->clone());
    }
    return *this;
}

WareHouse& WareHouse::operator=(WareHouse&& other) {
    if (&other != this) {
        isOpen          = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter= other.volunteerCounter;
        idCounter       = other.idCounter;

        for (BaseAction* a : actionsLog) delete a;
        actionsLog.clear();
        actionsLog = other.actionsLog; other.actionsLog.clear();

        for (Volunteer* v : volunteers) delete v;
        volunteers.clear();
        volunteers = other.volunteers; other.volunteers.clear();

        for (Order* o : pendingOrders) delete o;
        pendingOrders.clear();
        pendingOrders = other.pendingOrders; other.pendingOrders.clear();

        for (Order* o : inProcessOrders) delete o;
        inProcessOrders.clear();
        inProcessOrders = other.inProcessOrders; other.inProcessOrders.clear();

        for (Order* o : completedOrders) delete o;
        completedOrders.clear();
        completedOrders = other.completedOrders; other.completedOrders.clear();

        for (Customer* c : customers) delete c;
        customers.clear();
        customers = other.customers; other.customers.clear();
    }
    return *this;
}

WareHouse::~WareHouse() {
    for (Volunteer* v : volunteers)     delete v;
    volunteers.clear();

    for (BaseAction* a : actionsLog)    delete a;
    actionsLog.clear();

    for (Order* o : pendingOrders)      delete o;
    pendingOrders.clear();

    for (Order* o : inProcessOrders)    delete o;
    inProcessOrders.clear();

    for (Order* o : completedOrders)    delete o;
    completedOrders.clear();

    for (Customer* c : customers)       delete c;
    customers.clear();
}
