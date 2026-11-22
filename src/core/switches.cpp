#include "switches.h"
#include "util.h"

bool canUseLift1(int fromFloor,int toFloor){
if((fromFloor==1 && toFloor==2) || (fromFloor==2 && toFloor==1)){
  return true;
}
return false;
}

bool canUseLift2(int fromFloor,int toFloor,bool lift2Operational,int emergencyTripsLeft){
if(fromFloor==toFloor){
  return false;
}
bool validFrom=(fromFloor==2 || fromFloor==4 || fromFloor==5);
bool validTo=(toFloor==2 || toFloor==4 || toFloor==5);
if(!validFrom || !validTo){
  return false;
}
if(lift2Operational || emergencyTripsLeft>0){
  return true;
}
return false;
}

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount){
for(int i=0;i<inventoryCount;i++){
  if(inventory[i]==ENT_MASTER_KEY){
    return true;
  }
}
return false;
}

bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& score){
if(objectives[OBJ_FUSE_INSTALLED]){
  return false;
}
for(int i=0;i<inventoryCount;i++){
  if(inventory[i]==ENT_FUSE){
    for(int j=i;j<inventoryCount-1;j++){
      inventory[j]=inventory[j+1];
    }
    inventoryCount--;
    objectives[OBJ_FUSE_INSTALLED]=true;
    score=score+SCORE_INSTALL_FUSE;
    if(objectives[OBJ_LIFT_PASS]){
      objectives[OBJ_LIFT2_ACTIVE]=true;
    }
    return true;
  }
}
return false;
}

bool verifyCabinetCode(const char* enteredCode,const bool objectives[NUM_OBJECTIVES]){
if(!objectives[OBJ_CLUE_4] || !objectives[OBJ_CLUE_2] || !objectives[OBJ_CLUE_7]){
  return false;
}
return stringEquals(enteredCode,"427");
}