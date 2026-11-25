#include "physics.h"
#include "util.h"

// Movement validation and resource adjustments without structures
// indices are 0 based
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char &outConnType)
{
    outConnType = CONN_NONE;
    if (fromF < 0 || fromF >= NUM_FLOORS || toF < 0 || toF >= NUM_FLOORS || fromR < 0 || fromR >= ROOMS_PER_FLOOR || toR < 0 || toR >= ROOMS_PER_FLOOR)
    {
        return false;
    }
    if (fromF == toF && fromR == toR)
    {
        return false;
    }
    if (fromF == toF)
    {
        int diffR = toR - fromR;
        if (diffR == 1 || diffR == -1)
        {
            int minR = (fromR < toR) ? fromR : toR;
            char c = horizontalConn[fromF][minR];
            if (c == CONN_DOOR || c == CONN_WINDOW || c == CONN_LOCKED_DOOR)
            {
                outConnType = c;
                return true;
            }
            return false;
        }
        return false;
    }
    // Different floors: Staircases in Room 1 (index 0)
    if (fromR == 0 && toR == 0)
    {
        bool stair12 = (fromF == 0 && toF == 1) || (fromF == 1 && toF == 0);
        bool stair23 = (fromF == 1 && toF == 2) || (fromF == 2 && toF == 1);
        bool stair45 = (fromF == 3 && toF == 4) || (fromF == 4 && toF == 3);
        if (stair12 || stair23 || stair45)
        {
            outConnType = CONN_STAIR;
            return true;
        }
    }
    // Different floors: Lift 1 between Floor 1 (0) and Floor 2 (1) in Room 2 (1) or Room 3 (2)
    if ((fromR == 1 || fromR == 2) && fromR == toR && ((fromF == 0 && toF == 1) || (fromF == 1 && toF == 0)))
    {
        if ((fromF == 0 && toF == 1) || (fromF == 1 && toF == 0))
        {
            outConnType = CONN_LIFT1;
            return true;
        }
    }
    if (fromR == 3 && toR == 3)
    {
        bool validF1 = (fromF == 1 || fromF == 3 || fromF == 4);
        bool validF2 = (toF == 1 || toF == 3 || toF == 4);
        if (validF1 && validF2)
        {
            outConnType = CONN_LIFT2;
            return true;
        }
    }
    return false;
}

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
{

    (void)targetFloor;
    (void)targetRoom;

    if (connType == CONN_NONE || connType == CONN_SOLID_WALL)
    {
        return false;
    }
    if (connType == CONN_DOOR || connType == CONN_WINDOW || connType == CONN_STAIR || connType == CONN_LIFT1)
    {
        return true;
    }
    if (connType == CONN_LOCKED_DOOR)
    {
        return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);
    }
    if (connType == CONN_LIFT2)
    {
        bool normalAccess = lift2Operational && hasItem(inventory, inventoryCount, ENT_LIFT_PASS);
        bool emergencyAccess = hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD) && (emergencyTripsLeft > 0);
        return normalAccess || emergencyAccess;
    }
    return false;
}

void applyResourceDelta(int &energy, int &time, int deltaenergy, int deltatime)
{
    energy += deltaenergy;
    time += deltatime;
    energy = clampInt(energy, 0, MAX_ENERGY);
    time = clampInt(time, 0, MAX_TIME);
}
void applyScoreDelta(int &score, int deltascore)
{
    score += deltascore;
}
bool hasItem(const char inventory[], int inventorycount, char itemsymbol)
{
    for (int i = 0; i < inventorycount; i++)
    {
        if (inventory[i] == itemsymbol)
        {
            return true;
        }
    }
    return false;
}
bool addItem(char inventory[], int &inventorycount, char itemsymbol)
{
    if (inventorycount >= MAX_INVENTORY_CAPACITY)
    {
        return false;
    }
    inventory[inventorycount] = itemsymbol;
    inventorycount++;
    return true;
}
bool removeItem(char inventory[], int &inventorycount, char itemsymbol)
{
    int idx = -1;
    for (int i = 0; i < inventorycount; i++)
    {
        if (inventory[i] == itemsymbol)
        {
            idx = i;
            break;
        }
    }
    if (idx == -1)
    {
        return false;
    }
    for (int j = idx; j < inventorycount - 1; j++)
    {
        inventory[j] = inventory[j + 1];
    }
    inventorycount--;
    return true;
}