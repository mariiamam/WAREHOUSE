#include <string>
#include <vector>
#include <iostream>
#include "Customer.h"
#include <WareHouse.h>
#include <Action.h>
#include "Volunteer.h"
using std::string;
using std::vector;
using namespace std;




bool isBaked=false;

        BaseAction:: BaseAction() :errorMsg(),status(){}

        ActionStatus BaseAction:: getStatus() const{
            return status;
        }


        void BaseAction:: complete(){
          this->status = ActionStatus::COMPLETED;
        }


       void BaseAction:: error(string errorMsg){
            this->status = ActionStatus::ERROR;
            this->errorMsg= errorMsg;

        }

        string BaseAction:: getErrorMsg() const{
           return errorMsg;
        }

        string BaseAction:: statusToString (ActionStatus status) const {
               if(status == ActionStatus::COMPLETED)
                  return "COMPLETED";
                else 
                   return "ERROR";
               

         }

        
         


//     private:
//         string errorMsg;
//         ActionStatus status;
// };





        SimulateStep::SimulateStep(int numOfSteps):
          numOfSteps(numOfSteps){}


        void SimulateStep::act( WareHouse &wareHouse) {
            for (int i=0; i<numOfSteps;i++)
            {
                wareHouse.step();
            }
            this->complete(); 
            wareHouse.addAction(this);
      }/// why did we delete override


        string SimulateStep::toString() const {
                return "simulateStep "+ to_string(numOfSteps)+" "  + statusToString(getStatus());
        }


        SimulateStep* SimulateStep::clone() const{
                  return new SimulateStep(*this);

        
        }







// class AddOrder : public BaseAction {
//     public:

        AddOrder :: AddOrder(int id): 
        customerId(id){}




 void AddOrder :: act(WareHouse &wareHouse) {
        int x=wareHouse.getCustomerCounter();
        if (x <= customerId){
        error("cannot place this order\n");
        std::cout << getErrorMsg() ;
        }else{
        Customer& temp (wareHouse.getCustomer(customerId));
        int i = wareHouse.getIdCounter(); /// to give the order id 
        if (temp.addOrder(i) != -1){
        Order* order= new Order(i, (temp).getId(), (temp).getCustomerDistance() );
        wareHouse.addOrder(order);
        complete();
       
        }else{
        error("cannot place this order\n");
        std::cout << getErrorMsg() ;
       } 
        }
     wareHouse.addAction(this);
}


        string AddOrder:: toString() const {
                return "order "+ std::to_string(customerId) +" "+statusToString(getStatus());
        }


        AddOrder *AddOrder :: clone() const{
                 return new AddOrder(*this);

        }


//     private:
//         const int customerId;
// };





// class AddCustomer : public BaseAction {

       

        AddCustomer::AddCustomer(string customerName,string customerType, int distance, int maxOrders)
        :customerName(customerName),customerType(stringToCustomerType(customerType)),distance(distance),maxOrders(maxOrders){}

      const CustomerType AddCustomer::stringToCustomerType( string CustomerType) const {
            if(CustomerType=="soldier"){
            return CustomerType::Soldier;
            }else{
            return CustomerType::Civilian;
            }
            }


        void AddCustomer :: act(WareHouse &wareHouse) {

            int i = wareHouse.getCustomerCounter();
            if (customerType==CustomerType::Civilian)
            {
                 Customer *newcustomer = new  CivilianCustomer(i,customerName,distance,maxOrders);
                   wareHouse.Add_Customer(newcustomer);
            }

        else{
                  Customer *newcustomer = new  SoldierCustomer(i,customerName,distance,maxOrders);
                 wareHouse.Add_Customer(newcustomer);
  
        } 
           complete();
           wareHouse.addAction(this); 
            
          
            
     }

        AddCustomer * AddCustomer ::  clone() const{
                return new AddCustomer(*(this));


        }
        string AddCustomer:: toString() const {

      if(customerType==CustomerType::Civilian){
        return("Customer"+customerName + "civilian" +std::to_string(distance)+ std::to_string(maxOrders)) + statusToString(getStatus());
         }
        return("Customer "+ customerName+ "soldier"  +std::to_string(distance)+ std::to_string(maxOrders))+statusToString(getStatus());
}
//     private:
//         const string customerName;
//         const CustomerType customerType;
//         const int distance;
//         const int maxOrders;
// };






// class PrintOrderStatus : public BaseAction {
//     public:
        PrintOrderStatus::PrintOrderStatus(int id):orderId(id){}

        void PrintOrderStatus:: act(WareHouse &wareHouse) {
        if(orderId < wareHouse.getIdCounter()){
          Order temporder ( wareHouse.getOrder(orderId));
          
          std::cout <<  temporder.toString();
          complete();
        
        }
        else{ 
                error("Order doesnt exist");
                 std::cout << getErrorMsg() ;
                

        }
          wareHouse.addAction(this);  
        }

        PrintOrderStatus * PrintOrderStatus:: clone() const {
                return new PrintOrderStatus(*(this));

        }
        string PrintOrderStatus:: toString() const {
            
                 return "orderStatus " + std:: to_string(orderId)+ " "+ statusToString(getStatus());
        }



        
       
 

        PrintCustomerStatus:: PrintCustomerStatus(int customerId): 
        customerId(customerId){}

        void PrintCustomerStatus:: act(WareHouse &wareHouse) {
                if(customerId<wareHouse.getCustomerCounter()){
               Customer &tempCustomer( wareHouse.getCustomer(customerId)); 
               vector <int> orderId (tempCustomer.getOrdersIds());
               int numOfOrders=tempCustomer.getMaxOrders()-tempCustomer.getNumOrders();
                std::cout << "CustomerID:" + std::to_string(tempCustomer.getId()) + "\n";
               for(int order : orderId){
                 Order &tempOrder(wareHouse.getOrder(order)); 
                 cout << "OrderID:" + std::to_string(order) + "\n";
                 cout << "OrderStatus:" + 
                 tempOrder.fromOrderToString(tempOrder.getStatus()) + "\n" ;
               }
                    cout<<"numOrderLeft: " + std::to_string(numOfOrders) + "\n";
                   complete();
 }
    else
    {
        error("customer doesn't exist");
        std::cout << getErrorMsg() ;
        
 
        } 

    wareHouse.addAction(this);

}



        
        PrintCustomerStatus * PrintCustomerStatus:: clone() const {
                return new PrintCustomerStatus(*this);
        }
        string PrintCustomerStatus:: toString() const {
                return "customerStatus "+ std::to_string(customerId)+" "+ statusToString(getStatus());

        }


  


//     private:
//         const int customerId;
// };






// class PrintVolunteerStatus : public BaseAction {
//     public:
       PrintVolunteerStatus:: PrintVolunteerStatus(int id)
        : volunteerId(id){}

        void PrintVolunteerStatus::act(WareHouse &wareHouse) {


 if(volunteerId < wareHouse.getCustomerCounter()){               
         Volunteer &tempvolunteer=wareHouse.getVolunteer(volunteerId);
           cout << (tempvolunteer.toString());
         
         complete(); 
          
   }else{
      error("volunteer doesn't exist");
       cout << getErrorMsg() ;
   }

 wareHouse.addAction(this);

    


        }

        
        PrintVolunteerStatus * PrintVolunteerStatus :: clone() const{
                return new  PrintVolunteerStatus(*this);
        }

        string PrintVolunteerStatus :: toString() const{
                return"VolunteerStatus " +std::to_string(volunteerId)+" "+ statusToString(getStatus()) ;
        }

//     private:
//         const int volunteerId;
// };






// class PrintActionsLog : public BaseAction {
//     public:
        PrintActionsLog:: PrintActionsLog(){}


        void PrintActionsLog:: act(WareHouse &wareHouse) {
                vector<BaseAction*> act = wareHouse.getActions();
                for(BaseAction* action : act){
                 cout << (action->toString()) + "\n";
            
                }
                complete();
                wareHouse.addAction(this);
                 

   }

        PrintActionsLog * PrintActionsLog:: clone() const {
                return new PrintActionsLog(*this);
        }

        string PrintActionsLog:: toString() const{
                return "log "+ statusToString(getStatus()) ;
        }
//     private:
// };

// class Close : public BaseAction {
//     public:


       Close :: Close(){}

        void Close:: act(WareHouse &wareHouse){
                wareHouse.close();
                 complete();
                 wareHouse.addAction(this); 
                //  ~wareHouse; 

 
           
        }

        Close *  Close ::clone() const{
                return new Close(*this);

        }
        string  Close :: toString() const {
                return "close";
        }






        BackupWareHouse :: BackupWareHouse(){}
        void BackupWareHouse :: act(WareHouse &wareHouse){

                if(isBaked)
                        delete backup;
                        //  backup = new WareHouse(wareHouse); // this bad const cant change i think 
                        // complete();
                        // wareHouse.addAction(this); 
          


               backup = new WareHouse(wareHouse); //this bad const cant change
                complete();
              wareHouse.addAction(this);
              isBaked=true; 
         
        }


        BackupWareHouse * BackupWareHouse :: clone() const {
                return new BackupWareHouse (*this);
        }
        string BackupWareHouse ::toString() const{
                return "backup"+ statusToString(getStatus());}



       RestoreWareHouse:: RestoreWareHouse(){}

        void  RestoreWareHouse:: act(WareHouse &wareHouse){
                if(isBaked){
               wareHouse = *backup;
               complete();
              
                }else{
                      error("no backup available");
                      cout << getErrorMsg() ;

                }
               wareHouse.addAction(this); 

                
        }
        RestoreWareHouse *  RestoreWareHouse::clone() const{
                return new RestoreWareHouse(*this);

        } 
        string  RestoreWareHouse:: toString() const {
                return "restor" + statusToString(getStatus()); 

        }