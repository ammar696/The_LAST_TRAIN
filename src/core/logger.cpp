#include "logger.h"
#include <fstream>
#include "config.h"
#include "state.h"

using namespace std;

bool saveGameStateToFile(const char* filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,const bool objectives[NUM_OBJECTIVES],int emergencyTripsLeft,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
if(inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY){return false;}
ofstream out(filename);
if(!out){return false;}
out<<playerFloor<<" "<<playerRoom<<" "<<energy<<" "<<time<<" "<<score<<" "<<emergencyTripsLeft<<"\n"<<inventoryCount<<"\n";
for(int i=0;i<inventoryCount;i++){out<<inventory[i]<<" ";}
out<<"\n";
for(int i=0;i<NUM_OBJECTIVES;i++){out<<objectives[i]<<" ";}
out<<"\n";
for(int f=0;f<NUM_FLOORS;f++){
  for(int r=0;r<ROOMS_PER_FLOOR;r++){
    int c=entityCounts[f][r];
    if(c<0 || c>MAX_ENTITIES_PER_ROOM){return false;}
    out<<c;
    for(int e=0;e<c;e++){out<<" "<<roomEntities[f][r][e];}
    out<<"\n";
  }
}
return out.good();
}

bool loadGameStateFromFile(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
ifstream in(filename);
if(!in){return false;}
if(!(in>>playerFloor>>playerRoom>>energy>>time>>score>>emergencyTripsLeft)){return false;}
if(playerFloor<1 || playerFloor>NUM_FLOORS || playerRoom<1 || playerRoom>ROOMS_PER_FLOOR){return false;}
if(!(in>>inventoryCount) || inventoryCount<0 || inventoryCount>MAX_INVENTORY_CAPACITY){return false;}
for(int i=0;i<inventoryCount;i++){
  if(!(in>>inventory[i])){return false;}
}
for(int i=0;i<NUM_OBJECTIVES;i++){
  if(!(in>>objectives[i])){return false;}
}
for(int f=0;f<NUM_FLOORS;f++){
  for(int r=0;r<ROOMS_PER_FLOOR;r++){
    if(!(in>>entityCounts[f][r]) || entityCounts[f][r]<0 || entityCounts[f][r]>MAX_ENTITIES_PER_ROOM){return false;}
    for(int e=0;e<entityCounts[f][r];e++){
      if(!(in>>roomEntities[f][r][e])){return false;}
    }
  }
}
return true;
}

void writeGameSummaryReport(const char* filename,int score,int remainingEnergy,int remainingTime,const bool objectives[NUM_OBJECTIVES],bool victory){
ofstream out(filename);
if(!out){return;}
const char* names[OBJ_MISSION_FAILED]={"Master Key","Clue 4","Clue 2","Clue 7","Cabinet Unlocked","Train Key","Fuse Collected","Fuse Installed","Lift Pass","Lift 2 Active","Passenger Rescued","Cat Carrier","Cat Secured","Train Prepared","Mission Victory"};
int completed=0;
for(int i=0;i<OBJ_MISSION_FAILED;i++){
  if(objectives[i]){completed++;}
}
out<<"=== THE LAST TRAIN - GAME SUMMARY ===\n";
out<<"Result: "<<(victory?"VICTORY":"GAME OVER")<<"\n";
out<<"Final Score: "<<score<<"/520\n";
out<<"Remaining Energy: "<<remainingEnergy<<"/"<<MAX_ENERGY<<"\n";
out<<"Remaining Time: "<<remainingTime<<"/"<<MAX_TIME<<"\n";
out<<"Objectives Completed: "<<completed<<"/"<<OBJ_MISSION_FAILED<<"\n\nOBJECTIVE CHECKLIST:\n";
for(int i=0;i<OBJ_MISSION_FAILED;i++){
  out<<(objectives[i]?"[DONE] ":"[TODO] ")<<names[i]<<"\n";
}
out<<"\nAudit: "<<(victory?"Mission completed successfully.":(remainingEnergy<=0 || remainingTime<=0?"Failed: depleted resources.":"Ended early by player."))<<"\n";
}