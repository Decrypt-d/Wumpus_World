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
#include <map>
#include <stack> 
#include <vector>
#include <iostream>


using namespace std;

class MyAI : public Agent
{
public:
	MyAI ( void );

	//meta info
	int movesInAdvance;
	int totalMoves;
	int currentScore;
	bool wumpusKilled;

	//position info
	int orientation; 
	int currentxValue;
	int currentyValue;

	//boundary and wall variables
	enum direction {NORTH,EAST,SOUTH,WEST};
	int topWall;
	int rightWall;

	//backtracking functionality
	stack<Agent::Action> trail;
	bool backTrackingOn;
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

	//vector that will contain all tiles
	map<int,map<int,tile>> worldMap;
	
	//main method
	Action getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream);

    //position related methods
	void handlePositionChange();
    vector<MyAI::direction> determineWalls(int currentX, int currrentY);
	Agent::Action adjustDirection(int chosenDirection);
	Agent::Action backtrackAction();
	MyAI::direction resolveNewOrientation(const int & action);
	Agent::Action resolveDirection(int chosenDirection);

	//tile related methods
    bool tileExist(const int & x,const int & y);
    void addNewTile(bool glitter, bool stench, bool breeze);
	void handleBump();
};

#endif