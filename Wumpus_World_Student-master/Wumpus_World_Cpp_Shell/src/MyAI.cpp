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
#include <iostream>


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
    orientation = EAST;
    currentxValue = 1;
    currentyValue = 1;

    //wall and boundary info
    topWall = -1;
    rightWall = -1;

    //backtracking info
    turningAround = false;
    turningAroundComplete = false;

    shouldClimbOut = false;
    wumpusLocation.first = -1;
    wumpusLocation.second = -1; 
}

void MyAI::handlePositionChange()
{
    if (orientation == NORTH)
        currentyValue += 1;
    else if (orientation == SOUTH)
        currentyValue -= 1;
    else if (orientation == EAST)
        currentxValue += 1;
    else if (orientation == WEST)
        currentxValue -= 1;
}


void MyAI::removeAllUnexploredAtBoundary(const bool & removeFromRightWall)
{
    if (removeFromRightWall)
    {
        for (unordered_map<int,unordered_set<int>>::iterator beg = unexploredTile.begin(); beg != unexploredTile.end(); ++beg)
            if ((*beg).first > rightWall)
            {
                unexploredTile.erase(beg);
                return;
            }
    }
    else
    {
        for (unordered_map<int,unordered_set<int>>::iterator beg = unexploredTile.begin(); beg != unexploredTile.end(); ++beg)
        {
            unordered_set<int>::iterator toRemove = (*beg).second.find(topWall + 1);
            if (toRemove != (*beg).second.end())
                (*beg).second.erase(toRemove);
        }
    }
}


void MyAI::handleBump()
{
    if (orientation == EAST)
    {
        
        currentxValue -= 1;
        rightWall = currentxValue;
        //  std::cout << "Right Wall " << rightWall << std::endl; 
        removeAllUnexploredAtBoundary(true);
    }
    else if (orientation == NORTH)
    {
        currentyValue -= 1;
        topWall = currentyValue;
        //  std::cout << "Right Wall " << rightWall << std::endl; 
        removeAllUnexploredAtBoundary(false);
    }
}

void MyAI::handleWumpus(bool scream)
{
    if (shootWumpusX1Y1 && scream)
    {
        std::cout << "Wumpus Killed" << std::endl;
        wumpusKilled = true;   
    }
    else if (shootWumpusX1Y1 && !scream)
    {
        std::cout << "Wumpus Alive" << std::endl;
        wumpusLocation.first = 1;
        wumpusLocation.second = 2;
    }
}

template <typename T1>
void removeFromVector(std::vector<T1> & toRemoveFrom, T1 toRemove)
{
    for (typename std::vector<T1>::iterator beg = toRemoveFrom.begin(); beg != toRemoveFrom.end(); ++beg)
        if (*beg == toRemove)
        {
            toRemoveFrom.erase(beg); 
            return;
        }
}

vector<MyAI::direction> MyAI::determineWalls(int currentX, int currentY)
{
    vector<MyAI::direction> availableDirections;
    availableDirections.push_back(WEST);
    availableDirections.push_back(EAST);
    availableDirections.push_back(NORTH);
    availableDirections.push_back(SOUTH);

    if (currentX == 1)
        removeFromVector(availableDirections, WEST);

    if (currentY == 1)
        removeFromVector(availableDirections, SOUTH);

    if (currentX == rightWall)
        removeFromVector(availableDirections, EAST);

    if (currentY == topWall)
        removeFromVector(availableDirections, NORTH);
    
    return availableDirections;
}

bool MyAI::tileExist(const int &x, const int &y)
{
    unordered_map<int, std::unordered_map<int, tile>>::iterator iter = worldMap.find(x);
    if (iter == worldMap.end()) return false;
    unordered_map<int, tile>::iterator iter2 = worldMap[x].find(y);
    return iter2 != worldMap[x].end() ? true : false;
}


bool MyAI::allowTravelToTile(const int & x, const int & y)
{
    if (tileExist(x,y))
        return true;
    vector<pair<int,int>> tilesToCheck;
    for (int i = -1; i < 2; i += 2)
    {
        // if (tileExist(x + i,y) && (!(wumpusLocation.first == x + i && wumpusLocation.second == y) || wumpusKilled))
        if (tileExist(x + i,y))
            tilesToCheck.push_back(pair<int,int>(x + i,y));
        // if (tileExist(x, y + i) && (!(wumpusLocation.first == x && wumpusLocation.second == y + i) || wumpusKilled))
        if (tileExist(x, y + i))
            tilesToCheck.push_back(pair<int,int>(x,y + i));
    }

    for (int i = 0; i < tilesToCheck.size(); ++i)
        if (!(worldMap[tilesToCheck[i].first][tilesToCheck[i].second].breeze || worldMap[tilesToCheck[i].first][tilesToCheck[i].second].stench))
            return true;


    if (tilesToCheck.size() > 1)
    {
        int breezeCount = 0;
        int stenchCount = 0;
        for (int i = 0; i < tilesToCheck.size(); ++i)
        {
            if (worldMap[tilesToCheck[i].first][tilesToCheck[i].second].breeze)
                ++breezeCount;
            if (worldMap[tilesToCheck[i].first][tilesToCheck[i].second].stench)
                ++stenchCount;
        }   
        if (stenchCount < tilesToCheck.size() && breezeCount < tilesToCheck.size())
            return true;
    }
    return false;
}

bool MyAI::posExceedBoundary(const int & x, const int & y)
{
    if (x < 1)
        return true;
    if (y < 1)
        return true;
    if (rightWall != - 1 && x > rightWall)
        return true;
    if (topWall != -1 && y > topWall)
        return true;
    return false;
}

void MyAI::removeFromUnexplored(const int & currentX,const int & currentY)
{
    unordered_map<int,unordered_set<int>>::iterator xPos = unexploredTile.find(currentX);
    if (xPos != unexploredTile.end())
    {
        unordered_set<int>::iterator yPos = (*xPos).second.find(currentY);
        if (yPos != (*xPos).second.end())
            (*xPos).second.erase(yPos);
        if ((*xPos).second.size() <= 0)
            unexploredTile.erase(xPos);
    }
}

void MyAI::addUnexploredTilePos(const int & currentX, const int & currentY)
{
    vector<pair<int,int>> unexploredPos;
    for (int i = -1; i < 2; i += 2)
    {
        if (!(tileExist(currentX + i,currentY)))
            if (!posExceedBoundary(currentX + i, currentY) && allowTravelToTile(currentX + i,currentY))
                unexploredPos.push_back(pair<int,int>(currentX + i,currentY));
        if (!(tileExist(currentX,currentY + i)))
            if (!posExceedBoundary(currentX, currentY + i) && allowTravelToTile(currentX,currentY + i))
                unexploredPos.push_back(pair<int,int>(currentX,currentY + i));
    }
    for (int i = 0; i < unexploredPos.size(); ++i)
    {
        unexploredTile[unexploredPos[i].first].insert(unexploredPos[i].second);  
    }
}

void MyAI::addNewTile(bool glitter,bool stench,bool breeze)
{
    if (!tileExist(currentxValue, currentyValue))
    {
        tile currentTile;
        currentTile.glitter = glitter;
        currentTile.stench = stench;
        currentTile.breeze = breeze;
        worldMap[currentxValue][currentyValue] = currentTile;
        addUnexploredTilePos(currentxValue,currentyValue);
        removeFromUnexplored(currentxValue,currentyValue);
    }
}


MyAI::direction MyAI::resolveNewOrientation(const MyAI::direction & orientation,const Agent::Action & action)
{
    MyAI::direction newOrientation;
    if (action == TURN_RIGHT)
        newOrientation = (MyAI::direction)((orientation + 1) % 4);
    else if (action == TURN_LEFT && (orientation - 1) >= 0)
        newOrientation = (MyAI::direction)((orientation - 1) % 4);
    else if (action == TURN_LEFT && (orientation - 1) < 0)
        newOrientation = (MyAI::direction)((4 + (orientation - 1)) % 4);
    return newOrientation;
}

Agent::Action MyAI::adjustDirection(const MyAI::direction & orientation,const MyAI::direction & chosenDirection)
{
    int numRotation1 = 0;
    for (int i = 1; i < 4; ++i)
    {
        ++numRotation1;
        if (((orientation + i) % 4) == chosenDirection)
            break;
    }
    int numRotation2 = 0;
    for (int i = 1; i < 4; ++i)
    {
        ++numRotation2;
        if ((orientation - i) >= 0 &&((orientation - i) % 4) == chosenDirection)
            break;
        else if ((orientation - i) < 0 &&( (4 + (orientation - i)) % 4) == chosenDirection)
            break;
    }
    Agent::Action toTurn;
    if (numRotation1 > numRotation2)
        toTurn = TURN_LEFT;
    else   
        toTurn = TURN_RIGHT;
    return toTurn;
}


void MyAI::createSequenceOfAction(const MyAI::direction & chosenDirection)
{
    MyAI::direction mockOrientation = orientation;
    while (mockOrientation != chosenDirection)
    {
       Agent::Action action = adjustDirection(mockOrientation,chosenDirection);
       sequenceOfActions.push(action);  
       mockOrientation = resolveNewOrientation(mockOrientation, action);
    }
    // sequenceOfActions.push(FORWARD);
}

MyAI::direction MyAI::determineBackTrackDirection(const int & destinationxValue,const int & destinationyValue)
{
    if(currentxValue == destinationxValue)
    {
       if(currentyValue > destinationyValue)
          return SOUTH;
       else
          return NORTH;
    }
    else //case where currentyValue == destinationyValue
    {
       if(currentxValue > destinationxValue)
          return WEST;
       else
          return EAST;
    }
}

Agent::Action MyAI::Turn_Left(){
    orientation = resolveNewOrientation(orientation,TURN_LEFT);
    return TURN_LEFT;
}

Agent::Action MyAI::Turn_Right(){
    orientation = resolveNewOrientation(orientation,TURN_RIGHT);
    return TURN_RIGHT;
}

Agent::Action MyAI::Forward(){
    handlePositionChange();
    return FORWARD;
}

Agent::Action MyAI::resolveAction(Agent::Action action)
{
    if (action == TURN_LEFT)
        return Turn_Left();
    else if (action == TURN_RIGHT)
        return Turn_Right();
    else
        return Forward();    
}

//Djikstra for next path ---------------------

void MyAI::updateActionNodeCost(actionNode & toUpdate)
{
    if (toUpdate.previousState == nullptr)
        toUpdate.cost = 0;
    else
        toUpdate.cost = (toUpdate.previousState)->cost + (toUpdate.actionToGetHere == AROUND_MOVE ? 2 : 1 );
}

void MyAI::resolveActionNodeOrientation(actionNode & toUpdate,const MyAI::direction & curOrientation)
{
    if (toUpdate.actionToGetHere == AROUND_MOVE)
        toUpdate.currentOrientation = (MyAI::direction) ((curOrientation + 2) % 4); 
    else if (toUpdate.actionToGetHere == LEFT_MOVE)
        toUpdate.currentOrientation = resolveNewOrientation(curOrientation, TURN_LEFT);
    else if (toUpdate.actionToGetHere == RIGHT_MOVE)
        toUpdate.currentOrientation = resolveNewOrientation(curOrientation, TURN_RIGHT);
}


pair<int,int> MyAI::determinePositionAfterForward(const int & currentX, const int & currentY, const MyAI::direction & curOrientation)
{
    pair<int,int> result;
    result.first = currentX;
    result.second = currentY;
    if ( curOrientation == NORTH)
        result.second += 1;
    else if ( curOrientation == SOUTH)
        result.second -= 1;
    else if ( curOrientation == EAST)
        result.first += 1;
    else if ( curOrientation == WEST)
        result.first -= 1;
    return result;
}


template <typename T>
bool locationExistInVector(const vector<T*> & vecToCheck,const pair<int,int> & location)
{
    for (typename vector<T*>::const_iterator beg = vecToCheck.begin(); beg != vecToCheck.end(); ++beg)
    {
        if ((*(*beg)).location.first == location.first && (*(*beg)).location.second == location.second)
            return true; 
    }
    return false;
}

void MyAI::generateNextActionNodes(std::priority_queue<actionNode * ,std::vector<actionNode *>,actionNode::actionNodeComparitor> & pq,vector<actionNode * > & exploredSet, vector<actionNode * > & currentNodes, const actionNode & previousState)
{
    if (previousState.allowTurn)
    {
        for (int i = 0; i < 3; ++i)
        {
            actionNode * newNode = new actionNode();
            (*newNode).location = previousState.location;
            (*newNode).actionToGetHere = (MyAI::movement) i;
            (*newNode).previousState = &previousState;
            (*newNode).allowTurn = false; 
            updateActionNodeCost(*newNode);
            resolveActionNodeOrientation(*newNode,previousState.currentOrientation);
            currentNodes.push_back(newNode);
            pq.push(newNode);
        }
    }
    pair<int,int> newLocIfForward = determinePositionAfterForward(previousState.location.first,previousState.location.second,previousState.currentOrientation);

    if (!locationExistInVector<actionNode>(exploredSet,newLocIfForward))
        if (!posExceedBoundary(newLocIfForward.first, newLocIfForward.second) && allowTravelToTile(newLocIfForward.first,newLocIfForward.second))
        {
           
            actionNode * newNode = new actionNode();
            (*newNode).location = newLocIfForward;
            (*newNode).actionToGetHere = FORWARD_MOVE;
            (*newNode).previousState = &previousState;
            (*newNode).allowTurn = true; 
            (*newNode).currentOrientation = previousState.currentOrientation;
            updateActionNodeCost(*newNode);
            currentNodes.push_back(newNode);
            pq.push(newNode);
            exploredSet.push_back(newNode);
        }
}

template <typename T>
void cleanUpPointersVector(vector<T> & toCleanUp)
{
    for (typename vector<T>::iterator beg = toCleanUp.begin(); beg != toCleanUp.end(); ++beg)
        delete (*beg);
} 

template <typename T>
std::ostream & operator<< (std::ostream & out, const vector<T> & toPrint)
{
    for (typename vector<T>::const_iterator beg = toPrint.begin(); beg != toPrint.end(); ++beg)
        std::cout << (*beg) << std::endl; 
    return out;
}

void MyAI::reconstructPath(std::stack<Agent::Action> & actionStack,const actionNode * source)
{
    const actionNode * currentNode = source;
    while (currentNode->previousState != nullptr)
    {
        if (currentNode->actionToGetHere == FORWARD_MOVE)
            actionStack.push(FORWARD);
        else if (currentNode->actionToGetHere == LEFT_MOVE)
            actionStack.push(TURN_LEFT);
        else if (currentNode->actionToGetHere == RIGHT_MOVE)
            actionStack.push(TURN_RIGHT);
        else if (currentNode->actionToGetHere == AROUND_MOVE)
        {
            actionStack.push(TURN_LEFT);
            actionStack.push(TURN_LEFT);
        }
        currentNode = currentNode->previousState;
    }

    int size = actionStack.size();
    for (int i = 0; i < size; ++i)
    {
        sequenceOfActions.push(actionStack.top());
        actionStack.pop();
    }
}

bool MyAI::findNextPath(const bool & shouldReturnToStart)
{
    actionNode * source = new actionNode();
    (*source).currentOrientation = orientation;
    (*source).location = pair<int,int>(currentxValue,currentyValue);
    (*source).cost = 0;
    vector<actionNode *> exploredSet;
    vector<actionNode *> currentNodes;
    std::priority_queue<actionNode * ,std::vector<actionNode *>,actionNode::actionNodeComparitor> pq;
    pq.push(source);
    exploredSet.push_back(source);
    currentNodes.push_back(source);
    actionNode * topNode = pq.top();
    if (!shouldReturnToStart)
    {
        while (!pq.empty())
        {
            topNode = pq.top();
            pq.pop();
            if (!(tileExist(topNode->location.first,topNode->location.second)))
                break;
            generateNextActionNodes(pq,exploredSet,currentNodes,*topNode);
        }
    }
    else
    {
        while (topNode->location.first != 1 || topNode->location.second != 1)
        {
            topNode = pq.top();
            pq.pop();
            generateNextActionNodes(pq,exploredSet,currentNodes,*topNode);
        }
    }

    // std::cout << currentNodes << std::endl;
    // std::cout << "---------------BEST NODES------------------" << std::endl;
    // std::cout << topNode << std::endl;
    std::stack<Agent::Action> actionStack;
    reconstructPath(actionStack,topNode);
    cleanUpPointersVector(currentNodes);
    return true;
}

//--------------------------


//main method
Agent::Action MyAI::getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream)
{
    //updates wall location information
    if (bump)
        handleBump();

    // handleWumpus(scream);
    //adding tile
    addNewTile(glitter,stench,breeze);
    tile currentTile = worldMap[currentxValue][currentyValue];

    // std::cout << "Current X Y " << currentxValue << " " << currentyValue << std::endl;
 
    int unexploredSize = 0;
    for (unordered_map<int,unordered_set<int>>::iterator beg = unexploredTile.begin(); beg != unexploredTile.end(); ++beg)
        for (unordered_set<int>::iterator beg2 = ((*beg).second).begin(); beg2 != ((*beg).second).end(); ++beg2)
        {
            //  std::cout << "Unexplored Pos: " << ((*beg).first) << " , " << (*beg2) << std::endl;
             unexploredSize += ((*beg).second).size();
        }
    
    // std::cout << "Unexplored Size " << unexploredSize << std::endl;
  


    //***************************************************Action Logic***************************************************
    if((currentxValue == 1 && currentyValue == 1) && (breeze ||  goldGrabbed || shouldClimbOut || stench))
       return CLIMB;

    // if ((currentxValue == 1 && currentyValue == 1) && stench && (!wumpusKilled && wumpusLocation.first == -1 && wumpusLocation.second == -1))
    // {
    //     shootWumpusX1Y1 = true;
    //     return SHOOT;
    // }
       
    if (currentTile.glitter && !goldGrabbed)
    {
        goldGrabbed = true;
        return GRAB;
    }

    if (unexploredSize <= 0 || goldGrabbed){
        findNextPath(true);
        if (sequenceOfActions.size() != 0)
        {
            shouldClimbOut = true;
            Agent::Action action = sequenceOfActions.front();
            sequenceOfActions.pop();
            return resolveAction(action);
        }
    }


    //continues to resolve desired direction if needed
    if (sequenceOfActions.size() != 0)
    {
        Agent::Action action = sequenceOfActions.front();
        sequenceOfActions.pop();
       return resolveAction(action);
    }  
    
    if (unexploredTile.size() > 0)
    {
        findNextPath(false);
        if (sequenceOfActions.size() != 0)
        {
            Agent::Action action = sequenceOfActions.front();
            sequenceOfActions.pop();
            return resolveAction(action);
        }
    }  


}




//need function for no unexplored edges
//need function to check if tile is safe
//need function to check adjacent tiles
//need backtracking + djsktra


/*
    //always check tile for gold and grab if possible
    if (currentTile.glitter && !goldGrabbed)
    {
       currentScore += 1000;
       goldGrabbed = true;
       backTrackingOn = true;
       return GRAB;
    }

    //handle backtracking decision
    if (backTrackingOn && (currentxValue != 1 || currentyValue != 1))
    {
        backtrackAction();
        Agent::Action action = sequenceOfActions.front();
        sequenceOfActions.pop();
        return resolveAction(action);
    }
    else if (backTrackingOn && currentxValue == 1 && currentyValue == 1)
        return CLIMB;

    //BASE CASE 1: Leaving: you leave when you have the gold or you sense danger
    if (currentTile.breeze == true || currentTile.stench == true) 
    {
        if (trail.size() == 1 && currentxValue == 1 && currentyValue == 1)
            return CLIMB;
        else
        {
           backTrackingOn = true;
           backtrackAction();
           Agent::Action action = sequenceOfActions.front();
           sequenceOfActions.pop();
           return resolveAction(action);
        }
    }
    //BASE CASE 2: No danger, move forward in a random direction
    else 
    {
        vector<MyAI::direction> availableDirections = determineWalls(currentxValue, currentyValue);
        int randomIndex = rand() % availableDirections.size();
        MyAI::direction chosenDirection = availableDirections[randomIndex];
        createSequenceOfAction(chosenDirection);
        // std::cout << "Chosen Direction " << chosenDirection << std::endl;
        Agent::Action action = sequenceOfActions.front();
        sequenceOfActions.pop();
       return resolveAction(action);
    }  */




//     if((currentxValue == 1 && currentyValue == 1) && (breeze = true ||  goldGrabbed || no unexplored edges))
//        return CLIMB
    
//     if(currentxValue == 1 && currentyValue == 1 && stench == true)
//        -shoot and move in appropriate direction
    
//     if(breeze || stench)
//        -get all possible directions
//        -Check if that tile that we are going to has an adjacent tile that is explored and is safe
//         if (it has an adjacent tile that is explored and is safe, we can go to that tile)
   
//    if (safe and unexplored)
//      -forward
//    else
//      Backtrack to previous tile. - remove the direction which we came from when we backtrack
//      Djikstra for backtracking