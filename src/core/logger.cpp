#include "logger.h"
#include <fstream>

using namespace std;

bool saveGameStateToFile(const char* filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,const bool objectives[NUM_OBJECTIVES],int emergencyTripsLeft,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
ofstream out(filename);
if(!out){
  return false;
}
out<<playerFloor<<" "<<playerRoom<<" "<<energy<<" "<<time<<" "<<score<<" "<<emergencyTripsLeft<<"\n";
out<<inventoryCount<<"\n";
for(int i=0;i<inventoryCount;i++){
  out<<inventory[i]<<" ";
}
out<<"\n";
for(int i=0;i<NUM_OBJECTIVES;i++){
  out<<objectives[i]<<" ";
}
out<<"\n";
for(int f=0;f<NUM_FLOORS;f++){
  for(int r=0;r<ROOMS_PER_FLOOR;r++){
    out<<entityCounts[f][r];
    for(int e=0;e<entityCounts[f][r];e++){
      out<<" "<<roomEntities[f][r][e];
    }
    out<<"\n";
  }
}
out.close();
return true;
}

bool loadGameStateFromFile(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
ifstream in(filename);
if(!in){
  return false;
}
in>>playerFloor>>playerRoom>>energy>>time>>score>>emergencyTripsLeft;
in>>inventoryCount;
for(int i=0;i<inventoryCount;i++){
  in>>inventory[i];
}
for(int i=0;i<NUM_OBJECTIVES;i++){
  in>>objectives[i];
}
for(int f=0;f<NUM_FLOORS;f++){
  for(int r=0;r<ROOMS_PER_FLOOR;r++){
    in>>entityCounts[f][r];
    for(int e=0;e<entityCounts[f][r];e++){
      in>>roomEntities[f][r][e];
    }
  }
}
in.close();
return true;
}

void writeGameSummaryReport(const char* filename,int score,int remainingEnergy,int remainingTime,const bool objectives[NUM_OBJECTIVES],bool victory){
ofstream out(filename);
if(!out){
  return;
}
int completed=0;
for(int i=0;i<NUM_OBJECTIVES;i++){
  if(objectives[i]){
    completed++;
  }
}
out<<"=== THE LAST TRAIN - GAME SUMMARY ===\n";
out<<"Result: "<<(victory?"VICTORY":"GAME OVER")<<"\n";
out<<"Final Score: "<<score<<"\n";
out<<"Remaining Energy: "<<remainingEnergy<<"\n";
out<<"Remaining Time: "<<remainingTime<<"\n";
out<<"Objectives Completed: "<<completed<<"/"<<NUM_OBJECTIVES<<"\n";
out.close();
}