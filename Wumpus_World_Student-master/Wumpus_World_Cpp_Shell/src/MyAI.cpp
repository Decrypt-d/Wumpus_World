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
#include <algorithm>

using namespace std;
MyAI::MyAI() : Agent()
{
    //meta info
    movesInAdvance = 6;
    totalMoves = 0;
    currentScore = 0;
    wumpusKilled = false;
    goldGrabbed = false;

    //position info
    orientation = 2;
    shouldModifyPos = false;
    currentxValue = 1;
    currentyValue = 1;

    //wall and boundary info
    topWall = -1;
    rightWall = -1;

    //backtracking info
    turningAround = false;
    turningComplete = false;
}

void MyAI::handlePositionChange(const bool &bump)
{
    if (shouldModifyPos && !bump)
    {
        if (orientation == NORTH)
            currentyValue += 1;
        else if (orientation == SOUTH)
            currentyValue -= 1;
        else if (orientation == EAST)
            currentxValue += 1;
        else if (orientation == WEST)
            currentxValue -= 1;
        shouldModifyPos = false;
    }
}


void MyAI::handleBump()
{

}


vector<int> MyAI::determineWalls(int currentX, int currentY)
{
    vector<int> availableDirections;
    vector<int>:: iterator iter;
    iter = availableDirections.begin();

    availableDirections.insert(iter, WEST);
    availableDirections.insert(iter + 1, EAST);
    availableDirections.insert(iter + 2, NORTH);
    availableDirections.insert(iter + 3, SOUTH);

    if (currentX == 1)
        remove(availableDirections.begin(), availableDirections.end(), WEST);
    
    if (currentY == 1)
       remove(availableDirections.begin(), availableDirections.end(), SOUTH);
    
    if (currentX == rightWall)
        remove(availableDirections.begin(), availableDirections.end(), EAST);
    
    if (currentX == topWall)
        remove(availableDirections.begin(), availableDirections.end(), NORTH);
    
    return availableDirections;
}

bool MyAI::tileExist(const int &x, const int &y)
{
    map<int, std::map<int, tile>>::iterator iter = worldMap.find(x);
    map<int, tile>::iterator iter2 = worldMap[x].find(y);
    return iter != worldMap.end() && iter2 != worldMap[x].end() ? true : false;
}

void MyAI::addNewTile(bool glitter,bool stench,bool breeze)
{
    if (tileExist(currentxValue, currentyValue))
    {
        tile currentTile;
        currentTile.glitter = glitter;
        currentTile.stench = stench;
        currentTile.breeze = breeze;
        worldMap[currentxValue][currentyValue] = currentTile;
    }
}

int MyAI::adjustDirection(int orientation, int chosenDirection)
{
    switch (orientation)
    {
    case 1:
        switch (chosenDirection)
        {
        case 2:
            return TURN_RIGHT;
        case 3:
           return TURN_RIGHT;
        case 4:
           return TURN_LEFT;
        }
    case 2:
        switch (chosenDirection)
        {
        case 1:
            return TURN_LEFT;
        case 3:
            return TURN_RIGHT;
        case 4:
           return TURN_RIGHT;
        }           
    case 3:
        switch (chosenDirection)
        {
        case 1:
            return TURN_RIGHT;
        case 2:
            return TURN_RIGHT;
        case 4:
            return TURN_LEFT;
        }
   case 4:
       switch (chosenDirection)
       {
       case 1:
           return TURN_RIGHT;
       case 2:
          return TURN_RIGHT;
       case 3:
           return TURN_LEFT;
       }
    }
}

//a little bit suspicious on logic here
int MyAI::backtrackAction()
{
    // lastAction = trail.top();
    // trail.pop();
    //     //gets to last relevant action
    // while (lastAction == SHOOT || lastAction == GRAB) || lastAction == CLIMB)
    // {
    //     trail.pop();
    //     lastAction = trail.top();
    // }
    //     if (lastAction == TURN_LEFT)
    //         return TURN_RIGHT;
    //     else if (lastAction == TURN_RIGHT && turningAround == false)
    //         return TURN_LEFT;
    //     else if (lastAction == TURN_RIGHT && turningAround == true && turningComplete == false)
    //         return TURN_RIGHT;
    //     else //case for going backward (retracing step) (lastAction == TURN_RIGHT && turningAround == true && turningComplete == true)
    //     {
    //         turningAround = false;
    //         turningComplete = false;
    //         return FOWARD;
    //     }
    //     stack<pair<int,int>>
}



//main method
Agent::Action MyAI::getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream)
{
    // heuristic that if you sense these things intially chance is too high for failure, just climb out to minimize damage
    // add action to stack so we know where we are in order to backtrack correctly
    addNewTile(glitter,stench,breeze);
    // no matter what move chosen moves increases by 1 and score decreases by 1
    // totalMoves++;
    // currentScore--;
    // if (currentTile.glitter == true)
    // {
    //    goldGrabbed = true;
    //    trail.push(GRAB);
    //    currentScore += 1000;
    //    return GRAB;
    // }
    // //basic case 1: you leave the cave b/c of danger
    // if (currentTile.breeze == true || currentTile.stench == true) 

    //    //this case you need to start backtracking to climb out
    //    if (currentTile.xvalue != 1 && currentTile.yvalue != 1) 
    //       return backtrackAction();
    //    //this is case where we are back at original sqaure so we climb out
    //    else
    //    {
    //       trail.push(CLIMB);
    //       return CLIMB;
    //    }
    // //basic case 2: no danger so you proceed
    // else
    // {
    //     vector<int> availableDirections = determineWalls(currentX, currentY);
    //     int randomIndex = rand() % availableDirections.size();
    //     int chosenDirection = availableDirections[randomIndex];
    //     if (orientation == chosenDirection)
    //     {
    //         trail.push(FOWARD);
    //         return FOWARD;
    //     }
    //     else
    //     {
    //         int action = adjustDirection(orientation, chosenDirection);
    //         trail.push(action);
    //         return action;
    //     }
    // }
}
