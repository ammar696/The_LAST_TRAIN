#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include <iostream>
#include <string>

using namespace std;

void runSimulation(int& playerFloor,int& playerRoom,int& energy,
int& time,int& score,char inventory[],int& inventoryCount,bool objectives[],
int& emergencyTripsLeft,char horizontalConn[][ROOMS_PER_FLOOR-1],
char roomEntities[][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
int entityCounts[][ROOMS_PER_FLOOR]){
    
while(energy>0 && time>0){
  cout<<"\n----------------------------------------\n";
  cout<<"Location: Floor "<<playerFloor<<", Room "<<playerRoom<<"\n";
  cout<<"Energy: "<<energy<<" | Time: "<<time<<"\n";
  cout<<"----------------------------------------\n";
  cout<<"Enter a command (move, collect, inspect, use): ";
  string command;
  cin>>command;
  if(command=="move"){
    cout<<"You chose to move!\n";
  }
  else if(command=="collect"){
    cout<<"You chose to collect an item!\n";
  }
  else if(command=="inspect"){
    cout<<"You look around the room...\n";
  }
  else{
    cout<<"Invalid command.\n";
  }
  energy=energy-2;
  time=time-1;
}
if(energy<=0 || time<=0){
cout<<"\nGAME OVER! You ran out of resources.\n";
}
}