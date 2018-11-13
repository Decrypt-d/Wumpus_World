// ======================================================================
// FILE:        MyAI.hpp
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

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <unordered_map>
#include <stack> 
#include <queue>
#include <vector>
#include <iostream>
#include <unordered_set>


using namespace std;

class MyAI : public Agent
{
public:
	MyAI ( void );

	//boundary and wall variables
	enum direction {NORTH,EAST,SOUTH,WEST};
	enum movement {LEFT_MOVE, RIGHT_MOVE, AROUND_MOVE, FORWARD_MOVE };
	int topWall;
	int rightWall;

	//meta info
	int movesInAdvance;
	int totalMoves;
	int currentScore;
	bool wumpusKilled;
	bool goldGrabbed;
	bool shouldClimbOut;
	pair<int,int> wumpusLocation;
	bool shootWumpusX1Y1 = false;

	//position info
	MyAI::direction orientation; 
	int currentxValue;
	int currentyValue;

	//backtracking and exploring functionality
	unordered_map<int,unordered_set<int>> unexploredTile;
	queue<Agent::Action> sequenceOfActions;
	bool turningAround;
	bool turningAroundComplete;
	
	
	class tile{
	
	   public:

		   //possible features on a given tile
		   bool glitter;
		   bool breeze;
		   bool stench;
        
		   //probability of given feature
		   double pit;
		   double wumpus;
		   double gold;
		   tile & operator = (const tile & second)
		   {
			   this->glitter = second.glitter;
			   this->breeze = second.breeze;
			   this->stench = second.stench;
			   this->pit = second.pit;
			   this->wumpus = second.wumpus;
			   this->gold = second.gold;
			   return *this;
		   }
	};

	struct actionNode
	{
		const actionNode * previousState = nullptr;
		MyAI::direction currentOrientation;
		int cost;
		pair<int,int> location;
		bool allowTurn = true;
		MyAI::movement actionToGetHere;

		friend std::ostream & operator<<(std::ostream & out, actionNode * s)
		{
			operator << (out, *s);
		}
		friend std::ostream & operator<<(std::ostream & out, const actionNode & s)
		{
			std::cout << "Previous Node Address " << s.previousState << std::endl;
			std::cout << "Current Orientation " << s.currentOrientation << std::endl;
			std::cout << "Cost " << s.cost << std::endl;
			std::cout << "Location " << s.location.first << " , " << s.location.second << std::endl;
			std::cout << "Allow Turn " << s.allowTurn << std::endl;
			std::cout << "Action To Get Here " << s.actionToGetHere << std::endl;
			return out;
		}



		struct actionNodeComparitor
		{
			bool operator()(const actionNode * const & lhs, const actionNode * const & rhs)
			{
				return lhs->cost > rhs->cost;
			}
		};
	};

	//vector that will contain all tiles
	unordered_map<int,unordered_map<int,tile>> worldMap;
	
	//main method
	Action getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream);

    //position related methods
	void handlePositionChange();
    vector<MyAI::direction> determineWalls(int currentX, int currrentY);
	Agent::Action adjustDirection(const MyAI::direction & orientation,const MyAI::direction & chosenDirection);
	MyAI::direction determineBackTrackDirection(const int & destinationxValue,const int & destinationyValue);
	MyAI::direction resolveNewOrientation(const MyAI::direction & orientation,const Agent::Action & action);
	bool allowTravelToTile(const int & X, const int & y);
	Agent::Action Forward();
	Agent::Action Turn_Left();
	Agent::Action Turn_Right();
	void createSequenceOfAction(const MyAI::direction & chosenDirection);
	Agent::Action resolveAction(Agent::Action action);
	bool posExceedBoundary(const int & x, const int & y);
	bool findNextPath(const bool & shouldReturnToStart);
	void generateNextActionNodes(std::priority_queue<actionNode *,std::vector<actionNode *>,actionNode::actionNodeComparitor> & pq,vector<actionNode * > & exploredSet, vector<actionNode * > & currentNode, const actionNode & previousState);
	pair<int,int> determinePositionAfterForward(const int & currentX, const int & currentY, const MyAI::direction & curOrientation);
	void updateActionNodeCost(actionNode & toUpdate);
	void resolveActionNodeOrientation(actionNode & toUpdate,const MyAI::direction & curOrientation);
	void reconstructPath(std::stack<Agent::Action> & actionStack,const actionNode * source);
	void handleWumpus(bool scream);


	//tile related methods
    bool tileExist(const int & x,const int & y);
    void addNewTile(bool glitter, bool stench, bool breeze);
	void handleBump();
	void addUnexploredTilePos(const int & currentX, const int & currentY);
	void removeFromUnexplored(const int & currentX,const int & currentY);
	void removeAllUnexploredAtBoundary(const bool & removeFromRightWall);
};

#endif