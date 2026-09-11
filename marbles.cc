#include "marbles.h"
using namespace std;

void marbles::handleMarbleFlags() {
    cout << "marbles!" << endl;
}

//NEEDED FUNCTIONS:
//register an action
//deregister an action
//alter an existing action's marble reward
//log an action
//view all actions w/ reward
//view marble jar
//recall by date to view actions logged on that date
//getWeeklySummary()
//deleteLoggedAction()

//internal function:
//calculate daily marble reward based on actions logged
    // note, will be done by day
//saveData
//loadData


/*
Category	        Task / Time Block	    Marble Value
Programming	        First 30-minute block	0.5
Every subsequent    30-minute block	        0.2
Upkeep & Life Goals	Primary Upkeep	        0.3
                    Secondary Upkeep        0.2
*/