#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include <iostream>
#include <string>

using namespace std;

bool handleMoveCommand(int targetFloor,int targetRoom,int& currentFloor,int& currentRoom,int& energy,int& time,const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],const char inventory[MAX_INVENTORY_CAPACITY],int inventoryCount,int& emergencyTripsLeft,bool lift2Operational){
if(targetFloor<1 || targetFloor>NUM_FLOORS || targetRoom<1 || targetRoom>ROOMS_PER_FLOOR){
  cout<<"Invalid room coordinates.\n";
  return false;
}
if(targetFloor==currentFloor && targetRoom==currentRoom){
  cout<<"You are already in F"<<currentFloor<<"-R"<<currentRoom<<".\n";
  return false;
}
char connType=CONN_NONE;
if(!isDirectlyConnected(currentFloor-1,currentRoom-1,targetFloor-1,targetRoom-1,horizontalConn,connType)){
  if(currentFloor==targetFloor && (currentRoom-targetRoom==1 || targetRoom-currentRoom==1)){
    int minR=(currentRoom<targetRoom)?(currentRoom-1):(targetRoom-1);
    if(horizontalConn[currentFloor-1][minR]==CONN_SOLID_WALL){
      cout<<"Blocked by a solid wall.\n";
      return false;
    }
  }
  cout<<"No direct path to F"<<targetFloor<<"-R"<<targetRoom<<".\n";
  return false;
}
if(!canTraverse(targetFloor-1,targetRoom-1,connType,inventory,inventoryCount,emergencyTripsLeft,lift2Operational)){
  if(connType==CONN_LOCKED_DOOR){
    cout<<"Door is locked! You need the Master Key (<).\n";
  }
  else if(connType==CONN_LIFT2){
    cout<<"Lift 2 is inactive! Install the Fuse & get the Lift Pass, or use an Emergency Card.\n";
  }
  else{
    cout<<"Cannot traverse this path.\n";
  }
  return false;
}
if(connType==CONN_LIFT2 && !canUseLift2(currentFloor,targetFloor,lift2Operational,emergencyTripsLeft)){
  cout<<"Lift 2 cannot travel between these floors.\n";
  return false;
}
currentFloor=targetFloor;
currentRoom=targetRoom;
if(connType==CONN_DOOR || connType==CONN_LOCKED_DOOR || connType==CONN_STAIR){
  applyResourceDelta(energy,time,COST_MOVE_ORDINARY_ENERGY,COST_MOVE_ORDINARY_TIME);
}
else if(connType==CONN_WINDOW){
  applyResourceDelta(energy,time,COST_MOVE_WINDOW_ENERGY,COST_MOVE_WINDOW_TIME);
}
else if(connType==CONN_LIFT1){
  applyResourceDelta(energy,time,COST_MOVE_LIFT_ENERGY,COST_MOVE_LIFT_TIME);
}
else if(connType==CONN_LIFT2){
  applyResourceDelta(energy,time,COST_MOVE_LIFT_ENERGY,COST_MOVE_LIFT_TIME);
  bool normalAccess=lift2Operational && hasItem(inventory,inventoryCount,ENT_LIFT_PASS);
  if(!normalAccess && emergencyTripsLeft>0){
    emergencyTripsLeft--;
    cout<<"Used 1 emergency lift trip. Trips left: "<<emergencyTripsLeft<<"\n";
  }
}
cout<<"Moved to F"<<currentFloor<<"-R"<<currentRoom<<".\n";
return true;
}

bool handleInspectCommand(int currentFloor,int currentRoom,int& energy,int& time,const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCount,bool objectives[NUM_OBJECTIVES],int& score){
applyResourceDelta(energy,time,COST_INSPECT_ENERGY,COST_INSPECT_TIME);
renderRoomDetails(currentFloor,currentRoom,roomEntities,entityCount);
for(int i=0;i<entityCount;i++){
  char ent=roomEntities[currentFloor-1][currentRoom-1][i];
  if(ent==ENT_CLUE_4 && !objectives[OBJ_CLUE_4]){
    objectives[OBJ_CLUE_4]=true;
    applyScoreDelta(score,SCORE_CLUE_FOUND);
    cout<<"Discovered Clue Digit 4! (+"<<SCORE_CLUE_FOUND<<" Score)\n";
  }
  else if(ent==ENT_CLUE_2 && !objectives[OBJ_CLUE_2]){
    objectives[OBJ_CLUE_2]=true;
    applyScoreDelta(score,SCORE_CLUE_FOUND);
    cout<<"Discovered Clue Digit 2! (+"<<SCORE_CLUE_FOUND<<" Score)\n";
  }
  else if(ent==ENT_CLUE_7 && !objectives[OBJ_CLUE_7]){
    objectives[OBJ_CLUE_7]=true;
    applyScoreDelta(score,SCORE_CLUE_FOUND);
    cout<<"Discovered Clue Digit 7! (+"<<SCORE_CLUE_FOUND<<" Score)\n";
  }
}
return true;
}

bool handleCollectCommand(char itemSymbol,int currentFloor,int currentRoom,int& energy,int& time,int& score,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int& entityCount,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES]){
int foundIdx=-1;
for(int i=0;i<entityCount;i++){
  if(roomEntities[currentFloor-1][currentRoom-1][i]==itemSymbol){
    foundIdx=i;
    break;
  }
}
if(foundIdx==-1){
  cout<<"Item '"<<itemSymbol<<"' is not in this room.\n";
  return false;
}
if(itemSymbol==ENT_CONTROL_PANEL || itemSymbol==ENT_CAT || itemSymbol==ENT_PASSENGER || itemSymbol==ENT_CLUE_4 || itemSymbol==ENT_CLUE_2 || itemSymbol==ENT_CLUE_7){
  cout<<"You cannot pick up '"<<itemSymbol<<"' directly.\n";
  return false;
}
if(itemSymbol==ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED]){
  cout<<"Security cabinet is locked! Unlock it with the 3-digit code first.\n";
  return false;
}
if(itemSymbol==ENT_ENERGY_PACK || itemSymbol==ENT_EMERGENCY_TOOLKIT || itemSymbol==ENT_OBSERVATION_TOKEN){
  for(int j=foundIdx;j<entityCount-1;j++){
    roomEntities[currentFloor-1][currentRoom-1][j]=roomEntities[currentFloor-1][currentRoom-1][j+1];
  }
  entityCount--;
  if(itemSymbol==ENT_ENERGY_PACK){
    applyResourceDelta(energy,time,BONUS_ENERGY_PACK,0);
    cout<<"Energy Pack applied! (+"<<BONUS_ENERGY_PACK<<" Energy)\n";
  }
  else if(itemSymbol==ENT_EMERGENCY_TOOLKIT){
    applyResourceDelta(energy,time,BONUS_EMERGENCY_TOOLKIT,0);
    cout<<"Emergency Toolkit applied! (+"<<BONUS_EMERGENCY_TOOLKIT<<" Energy)\n";
  }
  else{
    applyResourceDelta(energy,time,0,BONUS_OBSERVATION_TOKEN);
    cout<<"Observation Token applied! (+"<<BONUS_OBSERVATION_TOKEN<<" Time)\n";
  }
  return true;
}
if(!addItem(inventory,inventoryCount,itemSymbol)){
  cout<<"Inventory is full!\n";
  return false;
}
for(int j=foundIdx;j<entityCount-1;j++){
  roomEntities[currentFloor-1][currentRoom-1][j]=roomEntities[currentFloor-1][currentRoom-1][j+1];
}
entityCount--;
applyResourceDelta(energy,time,COST_ITEM_PICKUP_ENERGY,COST_ITEM_PICKUP_TIME);
cout<<"Collected '"<<itemSymbol<<"'.\n";
if(itemSymbol==ENT_MASTER_KEY && !objectives[OBJ_MASTER_KEY]){
  objectives[OBJ_MASTER_KEY]=true;
  applyScoreDelta(score,SCORE_KEY_COLLECT);
  cout<<"Master Key acquired! (+"<<SCORE_KEY_COLLECT<<" Score)\n";
}
else if(itemSymbol==ENT_FUSE){
  objectives[OBJ_FUSE_COLLECTED]=true;
}
else if(itemSymbol==ENT_LIFT_PASS && !objectives[OBJ_LIFT_PASS]){
  objectives[OBJ_LIFT_PASS]=true;
  applyScoreDelta(score,SCORE_LIFT_PASS);
  cout<<"Lift Pass acquired! (+"<<SCORE_LIFT_PASS<<" Score)\n";
  if(objectives[OBJ_FUSE_INSTALLED]){
    objectives[OBJ_LIFT2_ACTIVE]=true;
    cout<<"Lift 2 is now fully operational!\n";
  }
}
else if(itemSymbol==ENT_CAT_CARRIER){
  objectives[OBJ_CAT_CARRIER]=true;
}
else if(itemSymbol==ENT_CABINET_TRAIN_KEY && !objectives[OBJ_TRAIN_KEY]){
  objectives[OBJ_TRAIN_KEY]=true;
  applyScoreDelta(score,SCORE_TRAIN_KEY);
  cout<<"Train Key acquired! (+"<<SCORE_TRAIN_KEY<<" Score)\n";
}
return true;
}

bool handleUseCommand(char itemSymbol,int currentFloor,int currentRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES]){
if(!hasItem(inventory,inventoryCount,itemSymbol)){
  cout<<"You don't have '"<<itemSymbol<<"' in your inventory.\n";
  return false;
}
if(itemSymbol==ENT_FUSE && currentFloor==3 && currentRoom==4){
  if(installFuseControlPanel(inventory,inventoryCount,objectives,score)){
    applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
    cout<<"Fuse installed at Control Panel! (+"<<SCORE_INSTALL_FUSE<<" Score)\n";
    if(objectives[OBJ_LIFT2_ACTIVE]){
      cout<<"Lift 2 is now fully operational!\n";
    }
    return true;
  }
  cout<<"Fuse already installed.\n";
  return false;
}
if(itemSymbol==ENT_MEDICAL_KIT && currentFloor==4 && currentRoom==1 && !objectives[OBJ_PASSENGER_RESCUED]){
  removeItem(inventory,inventoryCount,ENT_MEDICAL_KIT);
  objectives[OBJ_PASSENGER_RESCUED]=true;
  applyScoreDelta(score,SCORE_RESCUE_PASSENGER);
  applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
  cout<<"Injured passenger treated and rescued! (+"<<SCORE_RESCUE_PASSENGER<<" Score)\n";
  return true;
}
if(itemSymbol==ENT_CAT_CARRIER && currentFloor==3 && currentRoom==3 && !objectives[OBJ_CAT_SECURED]){
  removeItem(inventory,inventoryCount,ENT_CAT_CARRIER);
  objectives[OBJ_CAT_SECURED]=true;
  applyScoreDelta(score,SCORE_SECURE_CAT);
  applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
  cout<<"Stranded cat safely secured! (+"<<SCORE_SECURE_CAT<<" Score)\n";
  return true;
}
if(itemSymbol==ENT_CABINET_TRAIN_KEY && currentFloor==5 && currentRoom==2 && !objectives[OBJ_TRAIN_PREPARED]){
  if(!objectives[OBJ_PASSENGER_RESCUED] || !objectives[OBJ_CAT_SECURED]){
    cout<<"Rescue the passenger (F4-R1) and secure the cat (F3-R3) before departing!\n";
    return false;
  }
  objectives[OBJ_TRAIN_PREPARED]=true;
  applyScoreDelta(score,SCORE_PREPARE_TRAIN);
  objectives[OBJ_MISSION_WON]=true;
  applyScoreDelta(score,SCORE_MISSION_VICTORY);
  applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
  cout<<"Train prepared and boarded! MISSION VICTORY! (+"<<(SCORE_PREPARE_TRAIN+SCORE_MISSION_VICTORY)<<" Score)\n";
  return true;
}
cout<<"You cannot use '"<<itemSymbol<<"' in F"<<currentFloor<<"-R"<<currentRoom<<".\n";
return false;
}

bool handleUnlockCabinetCommand(const char* code,int currentFloor,int currentRoom,bool objectives[NUM_OBJECTIVES],int& score,char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int& entityCount){
if(currentFloor!=5 || currentRoom!=1){
  cout<<"There is no security cabinet in this room.\n";
  return false;
}
if(objectives[OBJ_CABINET_UNLOCKED]){
  cout<<"Cabinet is already unlocked.\n";
  return false;
}
bool hasCabinet=false;
for(int i=0;i<entityCount;i++){
  if(roomEntities[currentFloor-1][currentRoom-1][i]==ENT_CABINET_TRAIN_KEY){
    hasCabinet=true;
    break;
  }
}
if(!hasCabinet){
  cout<<"Security cabinet not found.\n";
  return false;
}
if(!verifyCabinetCode(code,objectives)){
  cout<<"Unlock failed! Find all 3 clues (4, 2, 7) and enter the valid code.\n";
  return false;
}
objectives[OBJ_CABINET_UNLOCKED]=true;
applyScoreDelta(score,SCORE_CABINET_UNLOCK);
cout<<"Cabinet unlocked! You can now collect the Train Key (^). (+"<<SCORE_CABINET_UNLOCK<<" Score)\n";
return true;
}

bool checkEndConditions(int energy,int time,const bool objectives[NUM_OBJECTIVES]){
if(objectives[OBJ_MISSION_WON] || objectives[OBJ_MISSION_FAILED]){
  return true;
}
if(energy<=0 || time<=0){
  return true;
}
return false;
}

void runSimulation(int& playerFloor,int& playerRoom,int& energy,int& time,int& score,char inventory[MAX_INVENTORY_CAPACITY],int& inventoryCount,bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft,char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR-1],char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
while(!checkEndConditions(energy,time,objectives)){
  renderDashboard(playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,emergencyTripsLeft);
  cout<<"Commands: move <F> <R> | inspect | collect <sym> | use <sym> | unlock <code> | map | obj | save | load | quit\n> ";
  string command;
  if(!(cin>>command)){
    break;
  }
  if(command=="move"){
    int targetF=0;
    int targetR=0;
    if(!(cin>>targetF>>targetR)){
      cin.clear();
      cin.ignore(1000,'\n');
      cout<<"Usage: move <floor> <room>\n";
      continue;
    }
    handleMoveCommand(targetF,targetR,playerFloor,playerRoom,energy,time,horizontalConn,inventory,inventoryCount,emergencyTripsLeft,objectives[OBJ_LIFT2_ACTIVE]);
  }
  else if(command=="inspect"){
    handleInspectCommand(playerFloor,playerRoom,energy,time,roomEntities,entityCounts[playerFloor-1][playerRoom-1],objectives,score);
  }
  else if(command=="collect"){
    char sym=' ';
    cin>>sym;
    if(handleCollectCommand(sym,playerFloor,playerRoom,energy,time,score,roomEntities,entityCounts[playerFloor-1][playerRoom-1],inventory,inventoryCount,objectives)){
      if(sym==ENT_EMERGENCY_CARD){
        emergencyTripsLeft=3;
        cout<<"Emergency Access Card activated! (3 emergency Lift 2 trips)\n";
      }
    }
  }
  else if(command=="use"){
    char sym=' ';
    cin>>sym;
    handleUseCommand(sym,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives);
  }
  else if(command=="unlock"){
    string code;
    cin>>code;
    if(handleUnlockCabinetCommand(code.c_str(),playerFloor,playerRoom,objectives,score,roomEntities,entityCounts[playerFloor-1][playerRoom-1])){
      applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
    }
  }
  else if(command=="map"){
    renderAsciiBuilding(playerFloor,playerRoom);
  }
  else if(command=="obj"){
    renderObjectivesChecklist(objectives);
  }
  else if(command=="save"){
    if(saveGameStateToFile("./out/savegame.txt",playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts)){
      cout<<"Game saved to ./out/savegame.txt\n";
    }
    else{
      cout<<"Error saving game. Make sure the 'out' folder exists.\n";
    }
  }
  else if(command=="load"){
    if(loadGameStateFromFile("./out/savegame.txt",playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts)){
      cout<<"Game loaded from ./out/savegame.txt\n";
    }
    else{
      cout<<"No save file found at ./out/savegame.txt\n";
    }
  }
  else if(command=="quit"){
    cout<<"Exiting simulation...\n";
    break;
  }
  else{
    cout<<"Invalid command.\n";
  }
}
if((energy<=0 || time<=0) && !objectives[OBJ_MISSION_WON]){
  objectives[OBJ_MISSION_FAILED]=true;
  cout<<"\nGAME OVER! You ran out of resources.\n";
}
writeGameSummaryReport("./out/game_summary.txt",score,energy,time,objectives,objectives[OBJ_MISSION_WON]);
cout<<"Summary written to ./out/game_summary.txt\n";
}