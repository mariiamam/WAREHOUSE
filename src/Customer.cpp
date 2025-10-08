#include <iostream>
#include <string>
#include <vector>
#include "Customer.h"
using std::string;
using std::vector;


 Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
        : id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders),ordersId() {}


    const string& Customer::getName() const{
        return name;
    }


    int Customer:: getId() const{
        return id;
    }


    int Customer:: getCustomerDistance() const{
        return locationDistance;
    }


    int Customer::getMaxOrders() const {
        return maxOrders;
    } // Returns maxOrders


    int Customer:: getNumOrders() const{
        return ordersId.size();
    }

    // Returns num of orders the customer has made so far


    bool Customer:: canMakeOrder() const{
        return getMaxOrders() != getNumOrders();

    } // Returns true if the customer didn't reach max orders


    const vector<int>& Customer::getOrdersIds() const { //what the second const for 
        return ordersId;
    }


    int Customer:: addOrder(int orderId){
        if (canMakeOrder())
        {
            ordersId.push_back(orderId);
            return orderId;
        }
        else

            return -1;

    } // return OrderId if order was added successfully, -1 otherwise



// private:
//     const int id;
//     const string name;
//     const int locationDistance;
//     const int maxOrders;
//     vector<int> ordersId;





   SoldierCustomer:: SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance,maxOrders ){}

    SoldierCustomer* SoldierCustomer::clone() const {
        return new SoldierCustomer(*this);
    }







     CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
        : Customer(id, name, locationDistance,maxOrders ){}

        
               CivilianCustomer* CivilianCustomer::clone() const {
                return new CivilianCustomer(*this);
              }


             
    





