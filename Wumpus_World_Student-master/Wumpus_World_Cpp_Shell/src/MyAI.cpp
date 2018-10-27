// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"

using namespace std;
MyAI::MyAI() : Agent()
{
   //here would be intializing variables and shit, not sure if should put global type variables here or in the hpp file
   movesInAdvance = 6;
   totalMoves = 0;
   currentScore = 0;
   currentxValue = 1;
   currentyValue = 1;
   wumpusKilled = 0;
   orientation = 2;
   topWall = NULL;
   rightWall = NULL;
   shouldModifyPos = false;
}

void MyAI::handlePositionChange(const bool & bump)
{
    if (shouldModifyPos && !bump)
    {
        if (orientation == NORTH)
            currentyValue += 1;
        else if (orientation == SOUTH)
            currentyValue -= 1;
        else if (orientation == EAST)
            currentxValue += 1;
        else if (oreintation == WEST)
            curentxValue -= 1;
        shouldModifyPos = false;
    }
}

vector<int> MyAI::determineWalls(int currentX, int currrentY)
{
    vector<int> availableDirections;
    availableDirections.insert(WEST);
    availableDirections.insert(EAST);
    availableDirections.insert(NORTH);
    availableDirections.insert(SOUTH);
    if(currentX == 1)
       availableDirections.remove(WEST);
    if(currentY == 1)
       availableDirections.remove(SOUTH);
    if(currentX == rightWall)
       availableDirections.remove(EAST);
    if(currentX == topWall)
       availableDirections.remove(NORTH);
    return availableDirections;
}

bool MyAI::tileExist(const int & x,const int & y)
{
   map<int, std::map<int, tile>>::iterator iter = worldMap.find(x);
   map<int, tile>::iterator iter2 = worldMap[x].find(y);
   return 
}


//workhorse function, world passes these input in (all the bools as parameters) in line 105 of world.cpp) parameters is how it communicates the world info
Agent::Action MyAI::getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream)
{
   //STEP 1: IF NEEDED MAKE NEW TILE BASED ON INFO PASSED FROM PARAMETERS (NOT NEEDED IF SHOOT OR SOMETHING)
       //will be looping through multiple times concerned about object existence a bit here make sure creating distinct objects, not sure if need new keyword
   
   //add info to stack so we know where we are in order to backtrack correctly
   if (iter == worldMap.end() || iter2 == worldMap[currentxValue].end())
   {
       tile currentTile;
       currentTile.glitter = glitter;
       currentTile.stench = stench;
       currentTile.breeze = breeze;
       worldMap[currentxValue][currentyValue] = currentTile;
   }   
   



   //heuristic that if you sense these things intially chance is too high for failure, just climb out to minimize damage
   if(currentTile.breeze == true || currentTile.stench == true)
      if(currentTile.glitter == true)
         trail.push(GRAB);
         return GRAB;
      else if(currentTile.xvalue != 0 && currentTile.yvalue != 0)
         #back track to previous sqaure
      else //this is case where we are back at original sqaure
         trail.push(CLIMB);
         return CLIMB;
   else
      int randomIndex = rand() % determineWalls(currentX, currentY).size();

      
      #randomly move in one of those directions
      #trail.push(theaction)



   //STEP 3: update info about neighbors, if neighbors don't exist yet instantiate them

   //STEP 4: based on state info choose greediest course of action

	
	return CLIMB;
}
