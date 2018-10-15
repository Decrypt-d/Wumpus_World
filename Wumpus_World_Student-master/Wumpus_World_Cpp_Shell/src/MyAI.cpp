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

MyAI::MyAI() : Agent()
{
   //here would be intializing variables and shit, not sure if should put global type variables here or in the hpp file
   movesInAdvance = 6;
   totalMoves = 0;
   currentScore = 0;
   currentxValue = 1;
   currentyValue = 1;
   wumpusKilled = 0;
}

//workhorse function, world passes these input in (all the bools as parameters) in line 105 of world.cpp) parameters is how it communicates the world info
Agent::Action MyAI::getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream)
{
   //STEP 1: IF NEEDED MAKE NEW TILE BASED ON INFO PASSED FROM PARAMETERS (NOT NEEDED IF SHOOT OR SOMETHING)
       //will be looping through multiple times concerned about object existence a bit here make sure creating distinct objects, not sure if need new keyword
   tile currentTile;
   currentTile.glitter = glitter;
   currentTile.stench = stench;
   currentTile.breeze = breeze;
   currentTile.leftWall = NULL;
   currentTile.bottomWall = NULL;

   //STEP 2: add the new tile
   worldMap[currentxValue][currentyValue] = currentTile;

   //STEP 3: update info about neighbors, if neighbors don't exist yet instantiate them

   //STEP 4: based on state info choose greediest course of action

	
	return CLIMB;
}
