// Global Variables
int counter = 2; 	// A counter tracking the number of turns made since the PacMan was trapped.
int trap = 0;		// 1 if the PacMan is trapped, 0 otherwise

// Makes the robot move forward.
void forward(){
	motor(0,95);
	motor(2,100);
	msleep(25);
}

// Makes the robot back up.
void backUp(){
	motor(0,-100);
	motor(2,-100);
	msleep(25);
}

// Makes the robot move forward a lot.
void pullUp(){
	int i;
	for(i = 0; i < 20; i++){
		forward();
	}
}

// Makes the robot turn slightly left.
void left(){
	motor(0,-100);
	motor(2,100);
	msleep(50);
}

// Makes the robot turn slightly right.
void right(){
	motor(0,100);
	motor(2,-100);
	msleep(50);
}


// This turns the robot to the left 90 degrees, and increments counter.
void turnLeft(){
	int i;
	counter++;
	printf("counter = %d", counter);
	for(i = 0; i < 8; i++){
		left();
	}
	pullUp();
	ao();
}

// This turns the robot to the right 90 degrees, and increments counter.
void turnRight(){
	int i;
	counter++;
	printf("counter = %d \n", counter);
	for(i = 0; i < 8; i++){
		right();
	}
	pullUp();
	ao();
}

// This turns the robot to the right 180 degrees
void turnAround(){
	int i; 
	for(i = 0; i < 17; i++){
		right();
	}
	ao();
}

// Reset trap and counter.ao()
void reset(){
	if (trap == 1 && counter > 2){
		printf("reset \n");
		trap = 0;
	}
}

//Detects an object in front and returns 1 for true and 0 for false
int wallFront(){
	if(analog_et(0) > 400){
		if (analog_et(3) < 100){
			printf("trap \n");
			trap = 1;
			counter = 0;
			printf("counter = %d \n", counter);
		}
		return 1;
	}
	else{
		return 0;
	}
}

//Detects an object to the left and returns 1 for true and 0 for false
int wallLeft(){
	if(analog_et(1) > 200){
		return 1;
	}
	else{
		return 0;
	}
}

//Detects an object to the right and returns 1 for true and 0 for false
int wallRight(){
	if(analog_et(2) > 200){
		return 1;
	}
	else{
		return 0;
	}
}

// This function performs the maze solving algorithm.
// NOTE: All print statements are for troubleshooting purposes.
int solveMaze(int lastTurn){
	
	// If either sensor in the front bumper is hit, back up and turn left 90 degrees.
	if (digital(13) == 1 || digital(12) == 1){
		printf("Front Bumper\n");
		backUp();
		backUp();
		turnLeft();
		return lastTurn;
	}
	// Else if the left bumper is hit, back up and turn slightly right.
	else if(digital(14) == 0){
		printf("Left Bumper\n");
		backUp();
		right();
		return lastTurn;
	}
	
	// Else if the right bumper is hit, back up and turn slightly left.
	else if(digital(15) == 1){
		printf("Right Bumper\n");
		backUp();
		left();
		return lastTurn;
	}
	
	// Else if there is no wall in front...
	else if(wallFront() == 0){
		
		// If there is no wall on either side...
		if (wallRight() == 0 && wallLeft() == 0){
			printf("T intersection \n"); 
			
			// If the IR beacon detects the other beacon straight ahead, move forward, return 2 (forward).
			if(analog_et(3) < 100 ){
				printf("PullUp\n");
				pullUp();
				pullUp();
				return 2;
			}
			
			// Else if the IR beacon detects the other beacon to the right,
			// and the last turn was not right, and the robot is not trapped,
			// turn right, return 0 (right).
			else if (analog_et(4) < 100 && lastTurn != 0 && trap != 1){
				printf("TurnRight\n");
				turnRight();
				return 0;
			}
			
			// Else if the IR beacon detects the other beacon to the left,
			// and the last turn was not left, and the robot is trapped,
			// turn left, return 1 (left).
			else if (analog_et(6) < 100 && lastTurn != 1 && trap != 0){
				printf("TurnLeft\n");
				turnLeft();
				return 1;
			}
			
			// Else if the robot is trapped and the lastTurn was not right, turn right.
			else if (trap == 1 && lastTurn != 1){
				printf("Trapped Turnright\n");
				turnRight();
				return 0;
			}
			
			// Otherwise go foward.
			else{	
				pullUp();
				return 2;
			}
		}
		
		// Else if there is no wall on the right...
		else if(wallRight() == 0){
			printf(" L intersection \n");
			
			// If the IR beacon detects the other beacon sraight ahead,
			// and the robot is not trapped, go forward.
			if(analog_et(3) < 100 && trap != 1){
				printf("PullUp\n");
				pullUp();
				return 2;
			}
			
			// Else if the IR beacon detects the other beacon to the right,
			// and the last turn was not right, and the robot is not trapped,
			// turn right, return 0 (right).
			else if (analog_et(4) < 100 && lastTurn != 0 && trap != 1){
				printf("TurnRight\n");
				turnRight();
				return 0;
			}
			
			// Else if the robot is trapped, turn right.
			else if (trap == 1){
				printf("Trapped TurnRight\n");
				turnRight();
				return 0;
			}
			
			// Otherwise pull up.
			else{
				printf("PullUp\n");
				pullUp();
				return 2;
			}
		}
		
		// Else if there is no wall on the left...
		else if(wallLeft() == 0){
			printf("_| intersection \n");
			
			// If the IR beacon detects the other beacon sraight ahead,
			// and the robot is not trapped, go forward.
			if(analog_et(3) < 100 && trap != 1){
				printf("PullUp\n");
				pullUp();
				return lastTurn;
			}
			
			// Else if the IR beacon detects the other beacon to the left,
			// and the robot is trapped, turn left.
			else if (analog_et(6) < 100 || trap == 1){ 
				printf("TurnLeft\n");
				turnLeft();
				return 1;
			}
			
			// Otherwise go forward.
			else{
				printf("PullUp\n");
				pullUp();
				return 2;
			}
		} 
		
		// Otherwise go slightly forward.
		else {
			forward();
			return lastTurn;
		}
	}
	
	// Else if there is no wall on the left or the right. 
	else if(wallRight() == 0 && wallLeft() == 0){
		printf(" -- intersection \n");
		
		// Else if the IR beacon detects the other beacon to the right,
		// and the last turn was not right, and the robot is not trapped,
		// turn right, return 0 (right).
		if(analog_et(4) < 100 && lastTurn != 0 && trap != 0){
			printf("TurnRight\n");
			turnRight();
			return 0;
		}
		
		// Else if the IR beacon detects the other beacon to the left,
		// and the last turn was not left, or the robot is trapped,
		// turn left, return 1 (left).
		else if ((analog_et(6) < 100 && lastTurn != 1) || trap == 1){
			printf("TurnLeft\n");
			turnLeft();
			return 1;
		}
		
		// Otherwise turn right.
		else{
			printf("TurnRight\n");
			turnRight();
			return 0;
		}
	}
	
	// Else if there is no wall on the right, and the last turn was not right, turn right. 
	else if (wallRight() == 0 && lastTurn != 0){
		printf("Right intersection \n");
		turnRight();
		return 0;
	}
	
	// Else if there is no wall on the left. and the last turn was not left, turn left.
	else if (wallLeft() == 0 && lastTurn != 1){
		printf(" Left intersection \n");
		turnLeft();
		return 1;
	}
	
	// Else if there is a wall in front and on both sides...
	else if (wallFront() == 1 && wallRight() == 1 && wallLeft() == 1){
		printf("Dead End \n");
		
		// If the IR beacon detects the other beacon straight ahead.
		if (analog_et(3) < 100){
			trap = 1;
			counter = 0;
			printf("Trapped \n");
		}
		
		// Turn around.
		turnAround();
		return 4;
	}
	
	// Otherwise go forward
	else
		forward();
		return lastTurn;
}

// Color config
// Hue: 44 to 64
// Saturation: 40 to 255
// Value: 150 to 255

// Function for checking for the maze exit. 
int checkExit(){
	
	// Update the camera.
	camera_update();
	
	// If there is at least one object on channel 0...
	if (get_object_count(0) > 0){
		printf("Biggest Object Size: %d by %d\n", get_object_bbox(0,0).height, get_object_bbox(0,0).width);
		printf("Number of Objects: %d\n", get_object_count(0));
		
		// Get the height and width of the largest object on that channel.
		int h = get_object_bbox(0,0).height;
		int w = get_object_bbox(0,0).width;
		
		// If the height is greater then 40 and the width is greater than 100, return true.
		if (h > 40 && w > 100){
			pullUp();
			return 1;
			
			// Else if there are more than 50 objects detected on that channel, return true.
			} else if (get_object_count(0) > 50){
			pullUp();
			return 1;
		}
		
		// Otherwise return false.
		else
		return 0;
	} 
	// Otherwise return false.
	else
	return 0;
}

// MAIN
int main(){ 
	int lastTurn = 2,		// Tracks the last turn made by PacMan, initially 2 (forward).
	i;					// Used in for loop.
	
	// Setup the camera.
	camera_open();
	
	// Loop until "break" is called.
	while(1){
		
		// Run solveMaze three times.
		for (i = 0; i < 1; i++)
		lastTurn = solveMaze(lastTurn);
		
		// Reset trapped and counter.
		reset();
		
		// Check for the exit, if found break from the loop.
		if(checkExit()){
			printf ("Exit Found!");
			break;
		}
	}
	
	// Shutdown the camera.
	camera_close();
}	
