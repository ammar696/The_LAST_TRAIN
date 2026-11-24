#include "viewer_iface.h"
#include "util.h"
#include<iostream>
#include "level.h"

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
                     }
    void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount) {

                        int fd=ZeroBasedFloor(floor);
                        int rd=ZeroBasedRoom(room);

                    cout<<"floor "<<fd+1<<"Room "<<rd+1<<endl;
                    if(roomEntities==nullptr || entityCount<=0){
                        cout<<"No visible items found or entities here"<<endl;
                     return;
                    }
                    int limit=entityCount;
                    if(limit>MAX_ENTITIES_PER_ROOM){
                        limit=MAX_ENTITIES_PER_ROOM;
                    }
                    for(int i=0;i<limit;i++){
                        char sm=roomEntities[fd][rd][i];
                        if(sm!=ENT_NONE){
                            cout<<"- "<<sm<<getEntityName(sm)<<endl;
                        }
                    }
                }
    
 void renderAsciiBuilding(int playerFloor, int playerRoom) {
    int FloorIdx = ZeroBasedFloor(playerFloor);
    int RoomIdx = ZeroBasedRoom(playerRoom);

  for(int f=NUM_FLOORS-1;f>=0;f--){
    cout<<"F "<<f+1<<":";
    for(int r=0;r<ROOMS_PER_FLOOR;r++){
        bool here=(f==FloorIdx && r==RoomIdx );
        cout<<"R "<<r+1<<(here?"*":" ")<<endl;
        if(r<ROOMS_PER_FLOOR-1){
            cout<<"-";
        }
    }
    cout<<"\n";
  }
  cout<<"(*marks current room)\n";
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {
    if (objectives == nullptr) {
        return;
    }

    const int obj[]={
        OBJ_MASTER_KEY,
        OBJ_CLUE_4,
        OBJ_CLUE_2,
        OBJ_CLUE_7,
        OBJ_CABINET_UNLOCKED,
        OBJ_TRAIN_KEY,
        OBJ_FUSE_COLLECTED,
        OBJ_FUSE_INSTALLED,
        OBJ_LIFT_PASS,
        OBJ_LIFT2_ACTIVE,
        OBJ_PASSENGER_RESCUED,
        OBJ_CAT_CARRIER,
        OBJ_CAT_SECURED,
        OBJ_TRAIN_PREPARED,
        OBJ_MISSION_WON,
        OBJ_MISSION_FAILED,

    };
    const char* const objLabels[] = {
        "Master Key Collected",
        "Clue Digit 4 Found",
        "Clue Digit 2 Found",
        "Clue Digit 7 Found",
        "Security Cabinet Unlocked",
        "Train Key Obtained",
        "Fuse Collected",
        "Fuse Installed at Control Panel",
        "Lift Pass Collected",
        "Lift 2 Active",
        "Injured Passenger Rescued",
        "Cat Carrier Collected",
        "Cat Secured",
        "Train Prepared",
        "Mission Won",
        "Mission Failed"
    };

    cout<<"Mission objective"<<endl;
    int total=sizeof(obj)/sizeof(obj[0]);
    for(int i=0;i<total;i++){
        int idx=obj[i];
        bool done=(idx>= 0 && idx<NUM_OBJECTIVES)? objectives[idx]:false;
        cout<<" ["<<(done?"x":" ")<<"] "<<objLabels[i]<<"\n";
    }
}




