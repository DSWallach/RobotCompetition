// Makes the robot move forward.
void forward(){
	motor(0,100);
	motor(2,100);
	msleep(25);
}

// Makes the robot back up.
void backUp(){
	motor(0,-100);
	motor(2,-100);
	msleep(25);
}

// Makes the robot move forward a large amount in one go.
void pullUp(){
	int i;
	for(i = 0; i < 20; i++){
		forward();
	}
}

// Function for making the robot turn slightly left.
void left(){
	motor(0,-100);
	motor(2,100);
	msleep(50);
}

// Function for making the robot turn slightly right
void right(){
	motor(0,100);
	motor(2,-100);
	msleep(50);
}

// Turns the robot left 90 degrees.
void turnLeft(){
	int i;
	for(i = 0; i < 10; i++){
		left();
	}
}

// Turns the robot right 90 degrees.
void turnRight(){
	int i;
	for(i = 0; i < 10; i++){
		right();
	}
}

// Turns the robot right 180 degrees.
void turnAround(){
	int i; 
	for(i = 0; i < 20; i++){
		right();
	}
}

// Detects an object in front and returns 1 for true and 0 for false
int wallFront(){
	if(analog_et(0) > 300){
		return 1;
	}
	else{
		return 0;
	}
}

// Detects an object to the left and returns 1 for true and 0 for false
int wallLeft(){
	if(analog_et(1) > 250){
		return 1;
	}
	else{
		return 0;
	}
}

// Detects an object to the right and returns 1 for true and 0 for false
int wallRight(){
	if(analog_et(2) > 250){
		return 1;
	}
	else{
		return 0;
	}
}

// Checks the camera for the PacMan.
int checkPacMan(){
	
	// Update the camera
	camera_update();
	
	// If there is at least one object on color_channel 0.
	if (get_object_count(0) > 0){
		// Get the objects height and width.
		int h = get_object_bbox(0,0).height;
		int w = get_object_bbox(0,0).width;
		
		// Print out the number of objects and the height and width.
		// Useful for troubleshooting.
		printf("Number of Objects: %d\n Biggest object height: %d, width: %d\n", get_object_count(1),h,w);
		
		// If the height/width is greater that 30/20, return 1 (PacMan found).
		if ((h >= 30 && w >= 20) || (h >= 20 && w >= 30))
		return 1;
		// If height/width is greater than 20/10, move forward then return 1 (PacMan found).
		else if ((h >= 20 && w >= 10) || (h >= 10 && w >= 20))
		{
			pullUp();
			printf ("Heaading to target\n");
			return 1;
		}
		// Otherwise return 0 (No PacMan).
		else {
			return 0;
		}
	} 
	// Otherwise return 0 (No PacMan).
	else {
		return 0;
	}
}

int *solveMaze(int lastTurnDir[]){
	// Implemented track of cardinal directions
	// 0 = North (relative to starting position)
	// 1 = East
	// 2 = South
	// 3 = West
	int tempArray[2];				// Used to store lastTurn and current direction.
	int *turnDir = &tempArray[0];	// Used to point to tempArray.
	turnDir[0] = lastTurnDir[0];	// Assign the first value to the first value of input array lastTurnDir.
	turnDir[1] = lastTurnDir[1];	// Same as above but for the second value.
	
	// NOTE: All printf statements are included for troubleshooting purposes.
	// If either of the two sensors that make up the front bumper are hit ...
	if (digital(13) == 1 || digital (12) == 1){
		printf("Front Bumper\n");	
		backUp();					// Back up three times.
		backUp();					
		backUp();
		turnRight();				// Turn right 90 degrees.
		return turnDir;				// Return turnDir unchanged.
	}
	// If the left bumper is hit ...
	else if(digital(14) == 0){
		printf("Left Bumper\n");	
		backUp();					// Back up and turn slightly right. 
		backUp();
		right();
		backUp();
		right();
		right();
		return turnDir;				// Return turnDir unchanged.
	}
	else if(digital(15) == 0){
		printf("Right Bumper\n");	
		backUp();					// Back up and turn slightly left.
		backUp();
		left();
		backUp();
		left();
		left();
		return turnDir;				// Return turnDir unchanged.
	}
	// If the IR detects no wall in front...
	else if(wallFront() == 0){
		
		// If the IRs detect no walls on either side...
		if (wallRight() == 0 && wallLeft() == 0){
			printf("T intersection \n"); 	
			printf("PullUp\n");		
			pullUp();				// Move forward 
			turnDir[0] = 2;			// Update turnDir 
			return turnDir;			
		}
		
		// Check for PacMan
		else if (checkPacMan())
		{
			turnDir [0] = 5; 		// Update turnDir to signal PacMan found.
			return turnDir;			
		}
		
		// Else if there is no wall on the right...
		else if(wallRight() == 0){
			printf(" L intersection \n");
			printf("PullUp\n");
			pullUp();				// Move forward
			turnDir[0] = 2;			// Update turnDir
			return turnDir;
		}
		
		// Else if there is no wall on the left...
		else if(wallLeft() == 0){
			printf("_| intersection \n");
			printf("PullUp\n");
			pullUp();				// Move Forward
			turnDir [0] = 2;		// Update turnDir
			return turnDir;
		} 
		else {
			forward();				// Move slightly forward
			return turnDir;			// Return turnDir unchanged
		}
	}
	
	// If there is no wall on the left or right...
	else if(wallRight() == 0 && wallLeft() == 0){
		printf(" -- intersection \n");
		
		// If lastTurn is not 0 (right), turn right and update turnDir.
		if(turnDir[0] != 0){
			printf("TurnRight\n");
			turnRight();
			turnDir [0] = 0;
			turnDir [1] += 1;
			return turnDir;
		}
		
		// Else if lastTurn is not 1 (left), turn left and update turnDir
		else if (turnDir[0] != 1){
			printf("TurnLeft\n");
			turnLeft();
			turnDir [0] = 1;
			turnDir [1] -= 1;
			return turnDir;
		}
		
		// Otherwise turn right and update turnDir
		else{
			printf("TurnRight\n");
			turnRight();
			turnDir [0] = 0;
			turnDir [1] += 1;
			return turnDir;
		}
	}
	
	// Else if there is no wall on the right and lastTurn is not 0 (right)
	// Turn right and update turnDir.
	else if (wallRight() == 0 && turnDir[0] != 0){
		printf("Right intersection \n");
		turnRight();
		turnDir[0] = 0;
		turnDir[1] += 1;
		return turnDir;
	}
	// Else if there is no wall on the left and lastTurn is not 1 (left)
	// Turn left and update turnDir.
	else if (wallLeft() == 0 && turnDir[0] != 1){
		printf(" Left intersection \n");
		turnLeft();
		turnDir [0] = 1;
		turnDir [1] -= 1;
		return turnDir;
	}
	
	// Else if the IRs detect walls on all sides, turn 180 degrees and update turnDir
	else if (wallFront() == 1 && wallRight() == 1 && wallLeft() == 1){
		printf("Dead End \n");
		turnAround();
		turnDir [0] = 4;
		if (turnDir [1] < 2)
		turnDir [1] += 2;
		else 
		turnDir [1] -= 2;
		return turnDir;
	}
	
	// Otherwise move slightly forward and return turnDir unchanged.
	else {
		forward ();
		return turnDir;
	}
}

int main(){
	int i,										// Used in for loop. 
	j, 											// Used in different for loop.
	turnArray [2], 								// Stores lastTurn, and current direction information.
	*tempP;										// Used to point to turnArray.
	int pastDirArray [5] = {-1,-1,-1,-1,-1};	// Initialize list of past directions. NOTE: This ended up not being used for anything.
	turnArray[0] = 2;							// Initialize lastTurn to 2 (forward).
	turnArray[1] = 0;							// Initialize current direction to 0 (North).
	
	// Setup the camera.
	camera_open();
	
	// Loop until "break" is called.
	while(1){					
		
		// Run solveMaze three times.
		for (i = 0; i < 3; i++){
			
			// Update pointer to turnArray.
			tempP = solveMaze(turnArray);
			
			// Update lastTurn
			turnArray [0] = tempP [0];
			
			// Update current direction.
			if (tempP [1] == 5)
			turnArray [1] = 0;
			else if (tempP [1] == -1)
			turnArray [1] = 3;
			else
			turnArray [1] = tempP [1];
			
			// Update pastDirArray
			for (j = 4; j > -1; j--) {
				pastDirArray[j] = pastDirArray[j - 1];
				pastDirArray [0] = turnArray [1];
			}
			
			// If PacMan was found freeze the motors and break out of the loop.
			if (turnArray[0] == 5)
			printf ("PacMan Found!\n Firing Weapons Array\n PacMan Neutralized!");
			freeze (0);
			freeze (2);
			break;
		}
		
		// Check for PacMan, if found freeze the motors and break out of the loop.
		if(checkPacMan()){
			printf ("PacMan Found!\n Firing Weapons Array\n PacMan Neutralized!");
			freeze (0);
			freeze (2);
			break;
		}
	}
	
	// Shutdown the Camera.
	camera_close();
	
	// Exit.
	return 0;
}
