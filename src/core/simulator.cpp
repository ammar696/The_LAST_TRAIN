#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include <iostream>
#include <string>

using namespace std;

void runSimulation(int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[],int& inventoryCount,bool objectives[],int& emergencyTripsLeft,char horizontalConn[][ROOMS_PER_FLOOR-1],char roomEntities[][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[][ROOMS_PER_FLOOR]){
while(energy>0 && time>0){
  cout<<"\n----------------------------------------\n";
  cout<<"Location: Floor "<<playerFloor<<", Room "<<playerRoom<<"\n";
  cout<<"Energy: "<<energy<<" | Time: "<<time<<"\n";
  cout<<"Inventory: ";
  if(inventoryCount==0){
    cout<<"Empty";
  }
  else{
    for(int i=0;i<inventoryCount;i++){
      cout<<inventory[i]<<" ";
    }
  }
  cout<<"\n----------------------------------------\n";
  cout<<"Enter a command (move, collect, inspect, use): ";
  string command;
  cin>>command;
  if(command=="move"){
    cout<<"You chose to move!\n";
  }
  else if(command=="collect"){
    cout<<"Enter item symbol (e.g. K for Key, E for Energy): ";
    char item;
    cin>>item;
    if(inventoryCount<MAX_INVENTORY_CAPACITY){
      inventory[inventoryCount]=item;
      inventoryCount++;
      cout<<"Item '"<<item<<"' collected successfully!\n";
    }
    else{
      cout<<"Inventory is full!\n";
    }
  }
  else if(command=="use"){
    cout<<"Enter item to use: ";
    char itemToUse;
    cin>>itemToUse;
    bool found=false;
    for(int i=0;i<inventoryCount;i++){
      if(inventory[i]==itemToUse){
        found=true;
        if(itemToUse=='M'){
          cout<<"You used the Master Key!\n";
          objectives[OBJ_MASTER_KEY_COLLECTED]=true;
        }
        else if(itemToUse=='E'){
          cout<<"You used an Energy Pack! (+25 Energy)\n";
          energy=energy+25;
        }
        else{
          cout<<"You used '"<<itemToUse<<"' but nothing happened.\n";
        }
        break;
      }
    }
    if(!found){
      cout<<"You don't have that item in your inventory.\n";
    }
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