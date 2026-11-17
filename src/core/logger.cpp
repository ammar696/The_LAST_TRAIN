#include "logger.h"
#include "util.h"

bool loadInitialState(const char* initStateFile,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[],int& inventoryCount,bool objectives[],int& emergencyTripsLeft){
return true;
}

bool saveGameStateToFile(const char* filename,int playerFloor,int playerRoom,int energy,int time,int score,const char inventory[],int inventoryCount,const bool objectives[],int emergencyTripsLeft,const char roomEntities[][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],const int entityCounts[][ROOMS_PER_FLOOR]){
return true;
}

bool loadGameStateFromFile(const char* filename,int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[],int& inventoryCount,bool objectives[],int& emergencyTripsLeft,char roomEntities[][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[][ROOMS_PER_FLOOR]){
return true;
}

void writeGameSummaryReport(const char* filename,int score,int remainingEnergy,int remainingTime,const bool objectives[],bool victory){
}