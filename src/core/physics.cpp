#include "physics.h"
#include "util.h"

// Movement validation and resource adjustments without structures
void applyResourceDelta(int& energy,int& time,int deltaenergy,int deltatime){
    energy+=deltaenergy;
    time+=deltatime;
    energy=clampInt(energy,0,MAX_ENERGY);
    time=clampInt(time,0,MAX_TIME);

}
void applyScoreDelta(int& score,int deltascore){
    score+=deltascore;
}
bool hasItem(const char inventory[],int inventorycount,char itemsymbol){
    for(int i=0;i<inventorycount;i++){
        if(inventory[i]==itemsymbol){
            return true;
        }
        
    }
    return false;
}
bool addItem(char inventory[],int& inventorycount,char itemsymbol){
    if(inventorycount>=MAX_INVENTORY_CAPACITY){
        return false;
    }
    inventory[inventorycount]=itemsymbol;
    inventorycount++;
    return true;

}
bool removeItem(char inventory[],int& inventorycount,char itemsymbol){
    int idx=-1;
    for(int i=0;i<inventorycount;i++){
        if(inventory[i]==itemsymbol){
            idx=i;
            break;
        }
    }
    if(idx==-1){
        return false;
    }
    for(int j=idx;j<inventorycount-1;j++){
        inventory[j]=inventory[j+1];
    }
    inventorycount--;
    return true;
}