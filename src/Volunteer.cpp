
#include <string>
#include <vector>
#include "Order.h"
#include"Volunteer.h"
using std::string;
using std::vector;
using namespace std;
#include <iostream>

#define NO_ORDER -1



      Volunteer::Volunteer(int id, const string &name):
        completedOrderId(-1),activeOrderId(-1),id(id), name(name){} 


        int Volunteer :: getId() const
        {
            return id;
        }


        const string& Volunteer :: getName() const
        {
            return name;
        }


        int Volunteer :: getActiveOrderId() const{
            return activeOrderId;
        
        }


        int Volunteer ::getCompletedOrderId() const{
            return completedOrderId;
        
        }

        
        bool Volunteer :: isBusy() const{ 

            return (getActiveOrderId() != NO_ORDER );
            
        } 
       
        

        
        
        
        
        
        
        // Signal whether the volunteer is currently processing an order


     
    // protected:
    //     int completedOrdercId; //Initialized to NO_ORDER if no order has been completed yet
    //     int activeOrderId; //Initialized to NO_ORDER if no order is being processed
    
    // private:
    //     const int id;
    //     const string name;









 
      CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown):
        Volunteer(id,name),coolDown(coolDown),timeLeft(coolDown){}


         CollectorVolunteer* CollectorVolunteer :: clone() const {
            return new CollectorVolunteer(*this);
        }
        

    
        void CollectorVolunteer :: step() {////should we change the ststus of the order,and send it to the driver
         
         if(decreaseCoolDown()){
            this->completedOrderId = this->activeOrderId;
            this->activeOrderId = NO_ORDER;

  }

   }



//Simulate volunteer step,if the volunteer finished the order, transfer activeOrderId to completedOrderId

        int CollectorVolunteer::  CollectorVolunteer:: getCoolDown() const{
            return coolDown;
        }

        int CollectorVolunteer :: getTimeLeft() const{

            return timeLeft;
        }

        bool CollectorVolunteer :: decreaseCoolDown(){
           
         if(timeLeft!= 0)
         this->timeLeft--;
 

         if(timeLeft==0){
            return true;
            }

      else{
                return false;
            }
        }
        //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise

        bool CollectorVolunteer :: hasOrdersLeft() const {
            return true;
        }


        bool CollectorVolunteer :: canTakeOrder(const Order &order) const {
            return (!(Volunteer::isBusy()) && (order.getStatus()==OrderStatus::PENDING));
        }


        void CollectorVolunteer ::acceptOrder(const Order &order) { 
           
              this->activeOrderId = order.getId();
               this->timeLeft= this->coolDown;
             

        }
    
        
// Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)


        string CollectorVolunteer :: toString() const {
           string start = "volunteerID: " + std::to_string(getId())+"\n"+
           "isBusy: ";

           if(isBusy()){
            start = start +"True\n"+ "OrderID:" + std::to_string(getActiveOrderId())
            +"\n"+"timeLeft: " + std::to_string(getTimeLeft())+ "\n"
            "OrdersLeft: "+ "No Limit" "\n";
            
            }

        else {
             start = start+"False\n" +  "OrderID: " + "None" + "\n" 
             + "timeLeft: " +"None" + "\n"
              "OrdersLeft: "+ "No Limit" + "\n" ;
 
           }

           return start; 
        }





    LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders)
        : CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders),ordersLeft(maxOrders){}

        
    LimitedCollectorVolunteer* LimitedCollectorVolunteer :: clone() const {
         return new LimitedCollectorVolunteer(*this);
        }

        bool LimitedCollectorVolunteer::hasOrdersLeft() const {
            return ordersLeft!=0;


        }// Signal whether the volunteer didn't reach orders limit,Always true for CollectorVolunteer and DriverVolunteer


        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
            return ( hasOrdersLeft() && CollectorVolunteer :: canTakeOrder(order));

        }     

         
        void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
              
            
                CollectorVolunteer :: acceptOrder(order);
                ordersLeft --;
            //    }
            
        }// Prepare for new order(Reset activeOrderId,TimeLeft,DistanceLeft,OrdersLeft depends on the volunteer type)


        int LimitedCollectorVolunteer:: getMaxOrders() const{
            return maxOrders;
        }


        int LimitedCollectorVolunteer::getNumOrdersLeft() const{
            return ordersLeft;

        }

         
         string LimitedCollectorVolunteer:: toString() const {

           string start = "volunteerID:"+ std::to_string(getId())+"\n"+
           "isBusy: ";

           if(isBusy()){
            start = start + "True\n"+ "OrderID:" + std::to_string(getActiveOrderId())+"\n"
            +"timeLeft: " + std::to_string(getTimeLeft())+ "\n"
            "OrdersLeft: "+ std::to_string(getNumOrdersLeft() )+"\n";
            
            }

        else {
             start = start +"False\n"+  "OrderID:" + "None" + "\n" 
             + "timeLeft: " +"None" + "\n"
              "OrdersLeft: "+std::to_string(getNumOrdersLeft() )+"\n" ;
 
           }

           return start; 
        }

         

      
                    
    // private:
    //     const int maxOrders;                                                            ders; // The number of orders the volunteer can process in the whole simulation
    //     int ordersLeft; // The number of orders the volunteer can still take

















 

        DriverVolunteer:: DriverVolunteer(int id, const string &name, int maxDistance, int  distancePerStep)
        : Volunteer(id, name), maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(0){}

         DriverVolunteer* DriverVolunteer :: clone() const {
            return new DriverVolunteer(*this);
        }

        int  DriverVolunteer:: getDistanceLeft() const{
            return distanceLeft;
        }

        int DriverVolunteer:: getMaxDistance() const{

            return maxDistance;
        }

        int  DriverVolunteer::getDistancePerStep() const{

            return distancePerStep;
        }

        

        bool  DriverVolunteer:: decreaseDistanceLeft(){
           distanceLeft=distanceLeft-distancePerStep;
           if(distanceLeft<=distancePerStep){ 
            distanceLeft=0;
            return true;

            }
       else {
            
            return false; 
        }
        }
        
         //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool  DriverVolunteer:: hasOrdersLeft() const {
            return true;

        }

        bool  DriverVolunteer:: canTakeOrder(const Order &order) const {
            if(order.getStatus()== OrderStatus::COLLECTING)
            {
                if(!(Volunteer::isBusy()) && (( order.getDistance())<=maxDistance))
                {
                    return true;
                }
            }
                return false;


            //return ( !(Volunteer:: isBusy()) && (( order.getDistance())<=maxDistance) && order.getStatus()==OrderStatus::COLLECTING );
        } ///which one is right

        // Signal if the volunteer is not busy and the order is within the maxDistance

        void  DriverVolunteer::acceptOrder(const Order &order) {
            // if( canTakeOrder(order) )
            // {
           this->distanceLeft= order.getDistance();
            this->activeOrderId = order.getId();   
            // }
        } // Assign distanceLeft to order's distance

        void  DriverVolunteer::step() {
            
            if(decreaseDistanceLeft())
            {
                this->completedOrderId= activeOrderId;
                this->activeOrderId = NO_ORDER;
            }
         

         } // Decrease distanceLeft by distancePerStep


        string  DriverVolunteer::toString() const{

            string start = "volunteerID:"+ std::to_string(getId())+"\n"+
           "isBusy: ";

           if(isBusy()){
            start = start + "True\n" +  "OrderID: " + std::to_string(getActiveOrderId())
            +"\n"+"distanceLeft: " + std::to_string(getDistanceLeft())+ "\n"
            "OrdersLeft: "+ "No Limit" +"\n";
            
            }

        else {
             start = start + "False\n"+  "OrderID: " + "None" + "\n" 
             + "distanceLeft: " +"None" + "\n"
              "OrdersLeft: "+ "No Limit" +"\n" ;
 
           }

           return start; 
        }

          

         
    

    // private:
    //     const int maxDistance; // The maximum distance of ANY order the volunteer can take
    //     const int distancePerStep; // The distance the volunteer does in one step
    //     int distanceLeft; // Distance left until the volunteer finishes his current order















        LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders)
        : DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders)
            {}

        LimitedDriverVolunteer* LimitedDriverVolunteer:: clone() const {
            return new LimitedDriverVolunteer(*this);
        }

        int LimitedDriverVolunteer:: getMaxOrders() const{
            return maxOrders;
        }

        int  LimitedDriverVolunteer:: getNumOrdersLeft() const{
            return ordersLeft;
        }


        bool  LimitedDriverVolunteer::hasOrdersLeft() const {
            return ordersLeft >0;
        }

        bool  LimitedDriverVolunteer::canTakeOrder(const Order &order) const {

            return (ordersLeft >0 && DriverVolunteer :: canTakeOrder(order));
        } 
        
        
        // Signal if the volunteer is not busy, the order is within the maxDistance.
        void  LimitedDriverVolunteer::acceptOrder(const Order &order) {
            //  if (canTakeOrder(order)){
            DriverVolunteer :: acceptOrder(order);
            ordersLeft--;
            
            // }

        }
        
         // Assign distanceLeft to order's distance and decrease ordersLeft
        string  LimitedDriverVolunteer::toString() const {
    
     string start = "volunteerID:"+ std::to_string(getId())+"\n"+
           "isBusy: ";

           if(isBusy()){
            start = start + "True\n"+  "OrderID: " + std::to_string(getActiveOrderId())
            +"\n" + "distanceLeft: " + std::to_string(getDistanceLeft())+ "\n"
            "OrdersLeft: "+ std::to_string(getNumOrdersLeft())+"\n";
            
            }

        else {
             start = start + "False\n"+  "OrderID: " + "None" + "\n" 
             + "distanceLeft: " +"None" + "\n"
              "OrdersLeft: "+ std::to_string(getNumOrdersLeft()) +"\n" ;
 
           }

           return start; 
        }

            
       
    // private:
    //     const int maxOrders; // The number of orders the volunteer can process in the whole simulation
    //     int ordersLeft; // The number of orders the volunteer can still take
