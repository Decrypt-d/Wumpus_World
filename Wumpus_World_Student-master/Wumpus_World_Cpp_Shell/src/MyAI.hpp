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
	bool goldGrabbed;

	//position info
	bool shouldModifyPos;
	int orientation; 
	int currentxValue;
	int currentyValue;

	//boundary and wall variables
	enum direction {NORTH,EAST,SOUTH,WEST};
	int topWall;
	int rightWall;

	//backtracking functionality
	stack<int> trail;
	bool turningAround;
	bool turningComplete;
	
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
	};

	//vector that will contain all tiles
	map<int,map<int,tile>> worldMap;
	
	//main method
	Action getAction(bool stench, bool breeze, bool glitter, bool bump, bool scream);

    //position related methods
	void handlePositionChange(const bool & bump);
    vector<int> determineWalls(int currentX, int currrentY);
	int adjustDirection(int orientation, int chosenDirection);
    int backtrackAction();

	//tile related methods
    bool tileExist(const int & x,const int & y);
    void addNewTile(bool glitter, bool stench, bool breeze);
	void handleBump();
};

#endif