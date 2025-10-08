//#include "WareHouse.h"
#include <iostream>
#include "WareHouse.h"
using namespace std;

WareHouse* backup ;



int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: warehouse </home/spl211/Downloads/configFileExample>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    WareHouse wareHouse(configurationFile);
    wareHouse.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    
    CivilianCustomer r(1,"j",2,3);
    cout <<r.getId() << endl;
    return 0;
    
}