#include "level.h"
#include "util.h"
#include<fstream>

using namespace std;




static char charAt(const char* line, int col) {
    int len = 0;
    while (line[len] != '\0') { len++; }
    if (col<0 || col>=len) {
         return ' ';
         }                         
    return line[col];
}

bool loadBuildingMap(const char* filename,char horizontalConn[][ROOMS_PER_FLOOR-1],char roomEntities[][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],int entityCounts[][ROOMS_PER_FLOOR]){

   
    if(filename==nullptr){
        return false;
    }
    ifstream file(filename);
    if(!file.is_open()){                               // opening map file
       
        return false;
    }
    for(int f=0;f<NUM_FLOORS;f++){
        for(int r=0;r<ROOMS_PER_FLOOR;r++){
            entityCounts[f][r]=0;
            for(int e=0;e<MAX_ENTITIES_PER_ROOM;e++){
                roomEntities[f][r][e]=' ';
            }
        }
        for(int r=0;r<ROOMS_PER_FLOOR-1;r++){
            horizontalConn[f][r]=CONN_NONE;

        }
    }

    char lines[16][MAX_LINE_LEN];
    for(int i=0;i<16;i++){
        
        if(!file.getline(lines[i],MAX_LINE_LEN)){
            return false;
        }
        int len=0;
         while(lines[i][len]!='\0'){len++;}
        if(len>0 && lines[i][len-1] == '\r'){
            lines[i][len - 1] = '\0';
        }
    }
    
    for(int f=1;f<=5 ;f++){
        int labelline=2+3*(5-f);
        int itemline=labelline+1;

        for (int k = 1; k <= 3; k++) {
        int col = 8+12*k;                                  
        char c = charAt(lines[labelline], col);
        if (c == CONN_DOOR || c == CONN_WINDOW || c == CONN_SOLID_WALL || c == CONN_LOCKED_DOOR) {  
        horizontalConn[f - 1][k-1] = c;              
        }
     }
        for(int r=1;r<=4;r++){
           int startcol=9+12*(r-1);
           int endcol=19+12*(r-1);

           for(int col=startcol;col<=endcol;col++){
            char c=charAt(lines[itemline],col);
            if(c!=' ' && entityCounts[f-1][r-1]<MAX_ENTITIES_PER_ROOM){
                roomEntities[f-1][r-1][entityCounts[f-1][r-1]]=c;
                entityCounts[f-1][r-1]++;
            }
           }
        }
    }
    if(entityCounts[0][3] < MAX_ENTITIES_PER_ROOM){
        roomEntities[0][3][entityCounts[0][3]]=ENT_ENERGY_PACK;
        entityCounts[0][3]++;
    }
return true;
}

static bool parseNumber(const char* text, int& result) {
    if (text[0] == '\0') { return false; }          // empty text
    result = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] < '0' || text[i] > '9') { return false; }   // not a digit
        result = result * 10 + (text[i] - '0');               // the character '0'
    }
    return true;
}

bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft) {

     if(filename==nullptr){
        return false;
    }
    ifstream file(filename);
    if(!file.is_open()){                               // opening map file
       
        return false;
    }
    playerFloor=1;
    playerRoom=1;
    emergencyTripsLeft=0;
    inventoryCount=0;

    for(int i=0;i<MAX_INVENTORY_CAPACITY;i++){
        inventory[i]=' ';
    }

    for(int i=0;i< NUM_OBJECTIVES;i++){
        objectives[i]=false;
    }
    int fileenergy=0;
    int filetime=0;
    int filescore=0;
    bool gotenergy=false;
    bool gottime=false;
    bool gotscore=false;

    char line[MAX_LINE_LEN];
    while (file.getline(line,MAX_LINE_LEN))
    {
       trimWhitespace(line);
       if(line[0]=='\0'){
        continue;
       }
       int colon=0;
       while(line[colon]!='\0' && line[colon]!=':'){
        colon++;
       }
       if(line[colon]=='\0'){
        continue;
       }
       line[colon]='\0';
       char* key=line;
       char* value=line+colon+1;
       trimWhitespace(key);
       trimWhitespace(value);

       if(stringEquals(key, "INITIAL_ENERGY")){
        if (!parseNumber(value, fileenergy)) { return false; }
    
        gotenergy=true;
       }
       if(stringEquals(key, "INITIAL_TIME")){
        if (!parseNumber(value, filetime)) { return false; }
        
        gottime=true;
       }
       if(stringEquals(key, "INITIAL_SCORE")){
        if (!parseNumber(value, filescore)) { return false; }
        
        gotscore=true;
       }
    }
      if(!gotenergy || !gotscore || !gottime){
        return false;
      }
      if(fileenergy<0 || fileenergy>MAX_ENERGY || filetime<0 || filetime>MAX_TIME){
        return false;
      }
      energy=fileenergy;
      time=filetime;
      score=filescore;

    
     
    return true;
}

const char* getEntityName(char entitySymbol) {
    switch (entitySymbol) {
        case ENT_MASTER_KEY:   return "Master Key";
        case ENT_ENERGY_PACK:  return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT:	return "Emergency Toolkit";
        case ENT_CLUE_4:	return "Clue Digit 4";
        case ENT_FUSE:	return "Fuse";
        case ENT_CAT:	return "Cat";
        case ENT_CONTROL_PANEL:	return "Control Panel";
        case ENT_LIFT_PASS:	return "Lift Pass";
        case ENT_PASSENGER:	return "Injured Passenger";
        case ENT_MEDICAL_KIT:	return "Medical Kit";
        case ENT_EMERGENCY_CARD:	return "Emergency Access Card";
        case ENT_CLUE_2:	return "Clue Digit 2";
        case ENT_CLUE_7:	return "Clue Digit 7";
        case ENT_CABINET_TRAIN_KEY:	return "Cabinet / Train Key";
        case ENT_CAT_CARRIER:	return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN:	return "Observation Token";
        default:               return "Unknown";
    }
}

