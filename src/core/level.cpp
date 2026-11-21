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
        while(len!='\0'){len++;}
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



