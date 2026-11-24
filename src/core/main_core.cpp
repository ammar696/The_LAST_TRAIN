#include "simulator.h"
#include "level.h"
#include "logger.h"
#include "viewer_iface.h"
#include <iostream>

using namespace std;

int main(){
int playerFloor=1;
int playerRoom=1;
int energy=INITIAL_ENERGY;
int time=INITIAL_TIME;
int score=INITIAL_SCORE;
char inventory[MAX_INVENTORY_CAPACITY]={};
int inventoryCount=0;
int emergencyTripsLeft=0;
bool objectives[NUM_OBJECTIVES]={};
char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1]={};
char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM]={};
int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]={};
const char* mapFile="./data/building.txt";
const char* initStateFile="./data/initial-state.txt";
if(!loadBuildingMap(mapFile,horizontalConn,roomEntities,entityCounts)){
  cerr<<"Error: Could not load "<<mapFile<<"\n";
  return 1;
}
if(!loadInitialState(initStateFile,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft)){
  cerr<<"Error: Could not load "<<initStateFile<<"\n";
  return 1;
}
if(playerFloor<1 || playerFloor>NUM_FLOORS || playerRoom<1 || playerRoom>ROOMS_PER_FLOOR){
  cerr<<"Error: Invalid starting room F"<<playerFloor<<"-R"<<playerRoom<<"\n";
  return 1;
}
cout<<"===========================================\n";
cout<<" THE LAST TRAIN - SIMULATION INITIALIZED   \n";
cout<<"===========================================\n";
runSimulation(playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,horizontalConn,roomEntities,entityCounts);
return 0;
}