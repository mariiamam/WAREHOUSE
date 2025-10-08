
#include <string>
#include <vector>
#include "Volunteer.h"
#include "Order.h"
#include "Customer.h"
#include "Action.h"
#include <iostream>
#include <cstddef>
using namespace std;
#include <utility>
#include <fstream>
#include <sstream>
#include<algorithm>
#include<WareHouse.h>

using std:: vector;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.



       

        void WareHouse:: start(){
            open();
            std::cout << "Warehouse is open!"<<endl;
            string mission;


            // AddOrder :: act(WareHouse &wareHouse)
            
            while ((isOpen)){
                getline(std::cin,mission);
               istringstream iss(mission);
               vector<std::string> words;
               string word;

            while(iss>>word)
            {
               words.push_back(word);
            }

            if(words.at(0)=="order"){
                int coustomerID = std::stoi(words[1]);//// to convert string to int 
            BaseAction* curract = new AddOrder(coustomerID);
             (*curract).act(*this);///// sholdnt we send refrence 
  }
           if(words.at(0)=="orderStatus"){
                int orderId = std::stoi(words[1]);//// to convert string to int 
            BaseAction* curract = new PrintOrderStatus(orderId);
             (*curract).act(*this);///// sholdnt we send refrence 
  }
           if(words.at(0)=="volunteerStatus"){
                int volunteerId = std::stoi(words[1]);//// to convert string to int 
            BaseAction* curract = new PrintVolunteerStatus(volunteerId);
             (*curract).act(*this);///// sholdnt we send refrence 
  }


            if(words.at(0)=="customerStatus"){
                int customerId = std::stoi(words[1]);//// to convert string to int 
            BaseAction* curract = new PrintCustomerStatus(customerId);
             (*curract).act(*this);///// sholdnt we send refrence 
  }
         
          if(words.at(0)=="step"){
                int numOfSteps = std::stoi(words[1]);//// to convert string to int 
            BaseAction* curract = new SimulateStep(numOfSteps);
             (*curract).act(*this);///// sholdnt we send refrence 
  }   

  
          if(words.at(0)=="close"){
            BaseAction* curract = new Close();
             (*curract).act(*this);///// sholdnt we send refrence 
  }
 
          if(words.at(0)=="log"){
            BaseAction* curract = new  PrintActionsLog ();
            (*curract).act(*this);///// sholdnt we send refrence 
  }   

         if(words.at(0)=="backup"){
            BaseAction* curract = new BackupWareHouse();
             (*curract).act(*this);///// sholdnt we send refrence 
  }


         if(words.at(0)=="customer"){
             string name((words[1]));
              string type((words[2]));
             int distance=stoi(words[3]);
             int maxOrders=stoi(words[4]);
            AddCustomer current(name,type,distance,maxOrders);
            current.act(*this);
             
         
             ///// sholdnt we send refrence 
  }
           
           
         if(words.at(0)=="restore"){
            BaseAction* curract = new RestoreWareHouse ();
            (*curract).act(*this);///// sholdnt we send refrence 
  }   
            

        }
        }

void WareHouse::step(){  
           for(unsigned int index=0;index<pendingOrders.size();index++)
            { 
                bool found= false; 
             for(Volunteer* v:volunteers){
              
                 if(!found){

                if(v->canTakeOrder(*pendingOrders.at(index)))
                {
                  

                    found=true; 
                    v->acceptOrder((*pendingOrders.at(index)));
                   

                    if (pendingOrders.at(index)->getStatus()== OrderStatus::COLLECTING)
                    {
                        
                      (*pendingOrders.at(index)).setDriverId(v->getId());
                      (*pendingOrders.at(index)).setStatus(OrderStatus::DELIVERING);
                                         
                    }
                   else{
                    
                      (*pendingOrders.at(index)).setCollectorId(v->getId());
                      (*pendingOrders.at(index)).setStatus(OrderStatus::COLLECTING);
                   }
                     inProcessOrders.push_back((pendingOrders.at(index)));
                     pendingOrders.erase(pendingOrders.begin()+index);
                     index--;
               
                     }
                    

             }
             } 
            //if all the volunteers cant take this order then we move to the next one with saving his place 
     }
       
int index=0;
     for(Volunteer* vol:volunteers){
        if(vol->isBusy()){
           (*vol).step();
     
           if(vol->getActiveOrderId()==-1){
           
           int orderId = vol->getCompletedOrderId();
          Order* order = &getOrder(orderId);
          if(order->getStatus()==OrderStatus::DELIVERING){
            order->setStatus(OrderStatus::COMPLETED);
            completedOrders.push_back(order);
            int toremove=indexof(order);
            inProcessOrders.erase(inProcessOrders.begin()+toremove);
      }
      else{
            if(order->getDriverId()!=-1){
             order->setStatus(OrderStatus::COMPLETED);
            completedOrders.push_back(order);
            int toremove=indexof(order);
            inProcessOrders.erase(inProcessOrders.begin()+toremove);

            }else{
            pendingOrders.push_back(order);
            sort();
            int toremove=indexof(order);
         inProcessOrders.erase(inProcessOrders.begin()+toremove);
            }
      }

   }


 if(!(vol)->hasOrdersLeft()){ 
  volunteers.erase(volunteers.begin() + index);
  delete vol;
  vol=nullptr;
     }
     else{
        index++;
     }


      }
}

}





 int WareHouse::indexof( Order* order) {
    int index=0;

    for (Order* currorder : inProcessOrders){
if(order->getId()!=currorder->getId())
    index++;
else 

return index;
    }
    
    return 0;
}







static bool compare(const Order* order1,const Order* order2){
    return order1->getId()<order2->getId();
}
void WareHouse::  sort(){
std::sort(this->pendingOrders.begin(),this->pendingOrders.begin(),compare);
}

     void WareHouse:: addOrder(Order* order){
            pendingOrders.push_back(order);
            idCounter++;
        }


      void WareHouse:: addAction(BaseAction* action){
        actionsLog.push_back(action);

     }
////vector<BaseAction*> actionsLog;

    Customer & WareHouse::getCustomer(int customerId) const{
                return ( *(customers[customerId]));
      }
             


        ///// using seqal 






        Volunteer & WareHouse::getVolunteer(int volunteerId) const{

            for(Volunteer* volunteer: volunteers){
                if ( (volunteer)->getId()==volunteerId)
                
                    return *volunteer;

                }
                
                static CollectorVolunteer NOVOLUNTEER(0,"0",0);
                return NOVOLUNTEER;
            
        }



        Order & WareHouse::getOrder(int orderId) const{
            
       if(orderId < idCounter){
         for( Order* currorder : pendingOrders){
         
            if( (*currorder).getId()== orderId)
                return *currorder;
        }
            for( Order* currorder : inProcessOrders){
            if( (*currorder).getId()== orderId)
                return *currorder;
        }
          for( Order* currorder : completedOrders){
            if( (*currorder).getId()== orderId)
                return *currorder;
        }}   

          static Order NOORDER(0,0,0);
                return NOORDER;
     
        
        }

        const vector<BaseAction*> & WareHouse::getActions() const{
            return actionsLog;
        }

        void WareHouse:: close(){
            
            isOpen= false;

            for (Order* currorder : pendingOrders)
            {
                if((currorder->fromOrderToString(currorder->getStatus()) )=="PENDING"){

               std::cout << "OrderID:"+std::to_string(currorder->getId())+"," +"CustomerID: " +std::to_string(currorder->getCustomerId())+", " +  "Status:" +"PENDING"+ "\n";
               
            }else {
                std::cout <<  "OrderID:"+std::to_string(currorder->getId())+"," +"Customer: " + std::to_string(currorder->getCustomerId())+ ", " +  "Status:"+ "COLLECTING"+ "\n";
            
            }
            
        }
        for (Order* currorder:inProcessOrders)
            {
                  if((currorder->fromOrderToString(currorder->getStatus()) )=="COLLECTING"){

               std::cout << "OrderID:"+ std::to_string(currorder->getId()) +"," +"CustomerID: "+ std::to_string(currorder->getCustomerId()) + ", " +  "Status:"+  "COLLECTING"+ "\n";
               
            } else {
               std::cout << "OrderID:"+  std::to_string(currorder->getId())+"," +"CustomerID: "+ std::to_string(currorder->getCustomerId())+ ", " +  "Status:"+ "DELIVERING"+ "\n";
               


            }
          }
          for (Order* currorder:completedOrders)
            {
                std::cout <<  "OrderID: "+std::to_string(currorder->getId())+"," +" CustomerID: "+ std::to_string(currorder->getCustomerId())+ ", " +  "Status: "+  "COMPLETED"+ "\n";
          }
          

           }
            
          



        void WareHouse:: open(){
            isOpen= true;
        }

       

        int WareHouse:: getIdCounter(){
            return idCounter;
        }/// i add 



       void WareHouse:: Add_Customer(Customer* coustomer){
        customers.push_back(coustomer);
        customerCounter++;
       }

     void WareHouse :: AddVolunteer(Volunteer* volunteer){
        volunteers.push_back(volunteer);
        volunteerCounter++;
     }

     int WareHouse:: getCustomerCounter(){
        return customerCounter;
       }

  int WareHouse:: getVolunteerCounter(){
    return volunteerCounter;
  }
    

    
//     private:
//         bool isOpen;
//         vector<BaseAction*> actionsLog;
//         vector<Volunteer*> volunteers;
//         vector<Order*> pendingOrders;
//         vector<Order*> inProcessOrders;
//         vector<Order*> completedOrders;
//         vector<Customer*> customers;
//         int customerCounter; //For assigning unique customer IDs
//         int volunteerCounter; //For assigning unique volunteer IDs
//         int idCounter;///i add

// };


WareHouse:: WareHouse(const string &configFilePath):
       isOpen(false),idCounter(0),customerCounter(0),volunteerCounter(0),actionsLog(),volunteers(),
       pendingOrders(),inProcessOrders(),completedOrders()
       ,customers(){


        std::string line;
        ifstream inputFile(configFilePath);
        while(std::getline(inputFile,line)){
            int count=0;
            std::istringstream iss(line);
            std::vector<std::string>words;
            std::string word;

        while(iss>>word){
            count++;
            words.push_back(word);
        }
        if(count==0 || words.at(0)=="#" ){
        continue;
        }
        
        if(words.at(0)=="customer"){
            int distance= stoi(words.at(3));
            int maxOrders= stoi(words.at(4));
            if(words.at(2)=="civilian"){
        Customer* toAddCustomer= new CivilianCustomer(customerCounter,words.at(1),distance,maxOrders);
        Add_Customer(toAddCustomer);
 }else{
          Customer* toAddCustomer= new SoldierCustomer(customerCounter,words.at(1),distance,maxOrders);
          Add_Customer(toAddCustomer);
 }       
 ///we should use the addCustomer in warehouse beccuse the user didnt actually add it 
         
         }

          if(words.at(0)=="volunteer"){

        if(words.at(2)=="collector"){
        int coolDown= stoi(words.at(3));
        Volunteer* toAddvolunteer= new CollectorVolunteer(volunteerCounter,words.at(1),coolDown);
        AddVolunteer(toAddvolunteer);

 }else if(words.at(2)=="limited_collector"){
         int coolDown= stoi(words.at(3));
         int maxOrders=stoi(words.at(4));
        Volunteer* toAddvolunteer= new LimitedCollectorVolunteer(volunteerCounter,words.at(1),coolDown,maxOrders);
        AddVolunteer(toAddvolunteer);

}else if(words.at(2)=="driver"){
         int maxDistance= stoi(words.at(3));
         int distancePerStep= stoi(words.at(4));
        Volunteer* toAddvolunteer= new DriverVolunteer(volunteerCounter,words.at(1),maxDistance,distancePerStep);
        AddVolunteer(toAddvolunteer);

 }else if(words.at(2)=="limited_driver"){
         int maxDistance= stoi(words.at(3));
         int distancePerStep= stoi(words.at(4));
         int maxOrder=stoi(words.at(5));
        Volunteer* toAddvolunteer= new LimitedDriverVolunteer(volunteerCounter,words.at(1),maxDistance,distancePerStep,maxOrder);
        AddVolunteer(toAddvolunteer);

         
        }

        }
       

 }
 }




       WareHouse:: WareHouse ( const WareHouse& other)://copy cons

            isOpen(other.isOpen),idCounter(other.idCounter),customerCounter(other.customerCounter),
            volunteerCounter(other.volunteerCounter),actionsLog(),volunteers(),
       pendingOrders(),inProcessOrders(),completedOrders()
       ,customers(){

        if(this!=&other)
        {
            for(BaseAction* action : other.actionsLog)
            {
                actionsLog.push_back(action->clone());
            }
            
            for(Volunteer* volunteer : other.volunteers)
            {
                volunteers.push_back(volunteer->clone());
            }
            
            for(Order* order : other.pendingOrders)
            {
                pendingOrders.push_back(new Order(*order));
            }
            
           
            for(Order* order : other.inProcessOrders)
            {
                inProcessOrders.push_back(new Order(*order));
            }
        
            for(Order* order : other.completedOrders)
            {
                completedOrders.push_back(new Order(*order));
            }
            
            for(Customer* customer : other.customers)
            {
                customers.push_back(customer->clone());
            }
        }
        

                // for(unsigned int i=0 ; i<other.actionsLog.size();i++)
                //  actionsLog.push_back(other.actionsLog.at(i)-> clone());

                //     for(unsigned int i=0 ; i<other.volunteers.size();i++)
                // volunteers.push_back(other.volunteers.at(i)-> clone());

                //     for(unsigned int i=0 ; i<other.customers.size();i++)
                //  customers.push_back(other.customers.at(i)-> clone());

                //     for(unsigned int i=0 ; i<other.pendingOrders.size();i++)/// sholud we add clone 
                //  pendingOrders.push_back(other.pendingOrders.at(i));

                //     for(unsigned  int i=0 ; i<other.inProcessOrders.size();i++)
                // inProcessOrders.push_back(other.inProcessOrders.at(i));

                //     for( unsigned int i=0 ; i<other.completedOrders.size();i++)
                //  completedOrders.push_back(other.completedOrders.at(i));


            }
 
 WareHouse:: WareHouse( WareHouse && other):
            isOpen(false),idCounter(0),customerCounter(0),volunteerCounter(0),actionsLog(),volunteers(),
       pendingOrders(),inProcessOrders(),completedOrders()
       ,customers(){
        

           actionsLog= other.actionsLog;
           other.actionsLog.clear();

            volunteers=other.volunteers;
            other.volunteers.clear();

            inProcessOrders=other.inProcessOrders;
            other.inProcessOrders.clear();



            pendingOrders=other.pendingOrders;
            other.pendingOrders.clear();


            completedOrders=other.completedOrders;
            other.completedOrders.clear();

            customers=other.customers;
            other.customers.clear();

           
   }
 //assignment operator
      WareHouse& WareHouse::operator=( const WareHouse &other){
         if(&other!=this) {
            isOpen=other.isOpen;
            customerCounter=other.customerCounter;
            volunteerCounter=other.volunteerCounter;
            idCounter=other.idCounter;


            for (BaseAction* action : actionsLog) {
            delete action;}
            actionsLog.clear();
            for(BaseAction* action : actionsLog)
            {
                actionsLog.push_back(action->clone());
            }
            // for(unsigned int i=0;i<other.actionsLog.size();i++){
            //     actionsLog.push_back(other.actionsLog.at(i)->clone());}
 
           
            for (Volunteer* volunteer : volunteers) {
              delete volunteer;}
            volunteers.clear();
            for(Volunteer* volunteer : volunteers)
            {
                volunteers.push_back(volunteer->clone());
            }
            // for(unsigned int i=0;i<other.volunteers.size();i++){
            //     volunteers.push_back(other.volunteers.at(i)->clone());}
 
            for (Order* order : pendingOrders) {
                delete order;}
            pendingOrders.clear();  
            for(Order* order : pendingOrders)
            {
                pendingOrders.push_back(new Order(*order));
            }
            // for(unsigned int i=0;i<other.pendingOrders.size();i++){
            //     pendingOrders.push_back(other.pendingOrders.at(i));}

            for (Order* order : inProcessOrders) {
                delete order;}
            inProcessOrders.clear();
            for(Order* order : inProcessOrders)
            {
                inProcessOrders.push_back(new Order(*order));
            }
            //  for(unsigned int i=0;i<other.inProcessOrders.size();i++){
            //     inProcessOrders.push_back(other.inProcessOrders.at(i));} 
             

            for (Order* order : completedOrders) {
                delete order;}
            completedOrders.clear();
            for(Order* order : completedOrders)
            {
                completedOrders.push_back(new Order(*order));
            }
            // for(unsigned int i=0;i<other.completedOrders.size();i++){
            //     completedOrders.push_back(other.completedOrders.at(i));}


            for (Customer* customer : customers) {
                delete customer;}
            customers.clear();
            for(Customer* customer : customers)
            {
                customers.push_back(customer->clone());
            }
            // for(unsigned int i=0;i<other.customers.size();i++){
            //     customers.push_back(other.customers.at(i));}
                 }



               return *this;
               }


WareHouse& WareHouse::operator=(  WareHouse &&other){
         if(&other!=this) {
            isOpen=other.isOpen;
            customerCounter=other.customerCounter;
            volunteerCounter=other.volunteerCounter;
            idCounter=other.idCounter;


            for (BaseAction* action : actionsLog) {
            delete action;}
            actionsLog.clear();
            actionsLog= other.actionsLog;
           other.actionsLog.clear();
 
           
            for (Volunteer* volunteer : volunteers) {
              delete volunteer;}
            volunteers.clear();
            volunteers= other.volunteers;
           other.volunteers.clear();
 
            for (Order* order : pendingOrders) {
                delete order;}
            pendingOrders.clear();  
          pendingOrders= other.pendingOrders;
           other.pendingOrders.clear();

            for (Order* order : inProcessOrders) {
                delete order;}
            inProcessOrders.clear();
            inProcessOrders= other.inProcessOrders;
           other.inProcessOrders.clear(); 
             

            for (Order* order : completedOrders) {
                delete order;}
            completedOrders.clear();
             completedOrders= other.completedOrders;
           other.completedOrders.clear(); 
            

            for (Customer* customer : customers) {
                delete customer;}
            customers.clear();
            customers= other.customers;
           other.customers.clear(); 
          
         }


               return *this;
               
               
}


        
    WareHouse ::~ WareHouse(){


                for (Volunteer* act : volunteers) {
                    delete act ;}
                volunteers.clear();

                for (BaseAction* act : actionsLog) {
                    delete act ;}
                actionsLog.clear();
           
                for (Order* act : pendingOrders) {
                    delete act;}
                pendingOrders.clear();

                 
                for (Order* act : inProcessOrders) {
                    delete act;}
                inProcessOrders.clear();

                for(Order* act : completedOrders){
                delete act; }
                completedOrders.clear();


                for(Customer* act : customers){
                delete act;} 
                customers.clear();
    }