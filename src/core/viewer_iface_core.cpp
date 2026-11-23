#include "viewer_iface.h"
#include "util.h"

// Terminal UI and dashboard rendering using primitive parameters
using namespace std;


int ZeroBasedFloor(int floorVal){
   
    if(floorVal>=0 && floorVal<NUM_FLOORS){
        return floorVal;
    }
    if(floorVal==NUM_FLOORS){
        return NUM_FLOORS-1;
    }
    return 0;
}

int ZeroBasedRoom(int roomVal){
   
    if(roomVal>=0 && roomVal<ROOMS_PER_FLOOR){
        return roomVal;
    }
    if(roomVal==ROOMS_PER_FLOOR){
        return ROOMS_PER_FLOOR-1;
    }
    return 0;
}
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft) {
    
    cout << "Floor: " << playerFloor << " | Room: " << playerRoom <<endl;
    cout << "Energy: " << energy << "/" << MAX_ENERGY<< " | Time: " << time << "/" << MAX_TIME<< " | Score: " << score <<endl;

    if(emergencyTripsLeft>0){
        cout<<"Emergency Lift 2 Trips left"<<emergencyTripsLeft<<endl;
    }

    cout<<"Inventory ("<<inventoryCount<<"/"<<MAX_INVENTORY_CAPACITY<<"):"<<endl;
    if(inventory==nullptr || inventoryCount<=0){
        cout<<" Empty"<<endl;
    }
    else{
        for(int i=0;i<inventoryCount && i<MAX_INVENTORY_CAPACITY;i++){
            char sm=inventory[i];
            cout<<"["<<sm<<" "<< getEntityName(sm)<<"]"<<endl;
            if(i+1 < inventoryCount && i+1 <MAX_INVENTORY_CAPACITY){
                cout<<",";

            }

        }
        cout<<"\n";
    }








int main(){}