
#include "Order.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
using namespace std;
#include <iostream>



        Order::Order(int id, int customerId, int distance): 
         id(id), customerId(customerId),distance(distance),status(OrderStatus::PENDING)
        , collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
          {}


        int Order::getId() const{
            return id;
        }


        int Order::getCustomerId() const{
            return customerId ;
        }


        void Order::setStatus(OrderStatus status){
            this->status= status;
           
            }

        void Order:: setCollectorId(int collectorId){
            this->collectorId= collectorId;
        }

        void Order::setDriverId(int driverId){
            
            this->driverId= driverId;//// should add the this>?
        }
      

        int Order::getCollectorId() const{
            return collectorId;
        }


        int Order::getDriverId() const{
            return driverId ;
        }


        OrderStatus Order::getStatus() const{
            return status;
        }



  const string Order :: fromOrderToString(OrderStatus status) const {
    if (status == OrderStatus::PENDING)
        return "Pending";
    else if (status == OrderStatus::COLLECTING)
        return "Collecting";
    else if (status == OrderStatus::DELIVERING)
        return "Delivering";
    else 
        return "Completed";

    }



    const string Order::toString() const{
      string strStatus = fromOrderToString( getStatus());
      
      string start="OrderId:"+ std::to_string(id)+ "\n"
                   "OrderStatus:" + strStatus+ "\n"
                   "CustomerID:"+ std::to_string(customerId)+ "\n";

                if(status == OrderStatus::DELIVERING){
                    return (start  
                    +"Collector: "+ std::to_string(collectorId)+"\n"
                    + "Driver: "+ std::to_string(driverId))+"\n";
                }

                else if(status == OrderStatus::COLLECTING){
                    return start  
                    +"Collector: "+ std::to_string(collectorId)
                    +"\n"+ "Driver: "+ "None"+"\n";
                }
                 else if(status == OrderStatus::COMPLETED){
                    return start  
                    +"Collector: "+ std::to_string(collectorId)
                    +"\n"+ "Driver: "+ std::to_string(driverId)+"\n";
                }
                
                    return start + "Collector: "+ "None"+"\n"
                                 + "Driver: "+ "None"+"\n";
     
        }


        int Order::getDistance() const{
            return distance;
        } 


//     private:
//         const int id;
//         const int customerId;
//         const int distance;
//         OrderStatus status;
//         int collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
//         int driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
