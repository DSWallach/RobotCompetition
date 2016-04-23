void forward(){//int left, int right){
	int charge = get_create_battery_voltage ();
	printf ("current charge: %d", charge);
	motor(0,100);
	motor(2,92);
	msleep(25);
}

void backUp(){
	motor(0,-100);
	motor(2,-100);
	msleep(25);
}

void pullUp(){
	int i;
	for(i = 0; i < 20; i++){
		forward();
	}
}

void left(){
	motor(0,-100);
	motor(2,100);
	msleep(50);
}

void right(){
	motor(0,100);
	motor(2,-100);
	msleep(50);
}

// This turns the robot to the left
void turnLeft(){
	int i;
	for(i = 0; i < 10; i++){
		left();
	}
}

// This turns the robot to the right
void turnRight(){
	int i;
	for(i = 0; i < 10; i++){
		right();
	}
}

// This turns the robot around to the right 180 degrees
void turnAround(){
	int i; 
	for(i = 0; i < 20; i++){
		right();
	}
}

void stop(){
	motor(0,-100);
	motor(2,-100);
	msleep(10);
}


//Detects an object in front and returns 1 for true and 0 for false
int wallFront(){
	if(analog_et(0) > 300){
		return 1;
	}
	else{
		return 0;
	}
}

//Detects an object to the left and returns 1 for true and 0 for false
int wallLeft(){
	if(analog_et(1) > 250){
		return 1;
	}
	else{
		return 0;
	}
}

//Detects an object to the right and returns 1 for true and 0 for false
int wallRight(){
	if(analog_et(2) > 250){
		return 1;
	}
	else{
		return 0;
	}
}

int checkExit(){
	camera_update();
	// Color config for recognizing the shiny green ball
	// Hue: 122 to 172
	// Saturation: 134 to 255
	// Value: 50 to 255
	// printf("checkExit");
	if (get_object_count(0) > 0){
		int h = get_object_bbox(0,0).height;
		int w = get_object_bbox(0,0).width;
		printf("Number of Objects: %d\n Biggest object height: %d, width: %d\n", get_object_count(1),h,w);
		if ((h >= 15 && w >= 23) || (h >= 23 && w >= 15)){
			//pullUp();
			return 1;
			} else if (get_object_count(1) > 15){
			//pullUp();
			return 1;
		}
		else {
			return 0;
		}
	} 
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
	int tempArray[2];
	int *turnDir = &tempArray[0];
	turnDir[0] = lastTurnDir[0];
	turnDir[1] = lastTurnDir[1];
	if (digital(13) == 1 || digital (12) == 1){
		printf("Front Bumper\n");
		backUp();
		backUp();
		right();
		return turnDir;
	}
	else if(digital(14) == 0){
		printf("Left Bumper\n");
		backUp();
		backUp();
		right();
		backUp();
		right();
		right();
		return turnDir;
	}
	else if(digital(15) == 0){
		printf("Right Bumper\n");
		backUp();
		backUp();
		left();
		backUp();
		left();
		left();
		return turnDir;
	}
	else if(wallFront() == 0){
		if (wallRight() == 0 && wallLeft() == 0){
			printf("T intersection \n"); 
			if(analog_et(3) < 100){
				printf("PullUp\n");
				pullUp();
				turnDir[0] = 2;
				return turnDir;
			}
			else if (analog_et(4) < 100 && turnDir[0] != 0){
				printf("TurnRight\n");
				turnRight();
				turnDir[0] = 0;
				turnDir[1] += 1;
				return turnDir;
			}
			else if (analog_et(6) < 100 && turnDir[0] != 1){
				printf("TurnLeft\n");
				turnLeft();
				turnDir[0] = 1;
				turnDir[1] -= 1;
				return turnDir;
			}
			else{	
				pullUp();
				turnDir[0] = 2;
				return turnDir;
			}
		}
		else if (checkExit())
		{
			turnDir [0] = 5;
			return turnDir;		
		}
		else if(wallRight() == 0){
			printf(" L intersection \n");
			if(analog_et(3) < 100){
				printf("PullUp\n");
				pullUp();
				turnDir[0] = 2;
				return turnDir;
			}
			else if (analog_et(4) < 100 && turnDir[0] != 0){
				printf("TurnRight\n");
				turnRight();
				turnDir[0] = 0;
				turnDir[1] += 1;
				return turnDir;
			}
			else{
				printf("PullUp\n");
				pullUp();
				turnDir[0] = 2;
				return turnDir;
			}
		}
		else if(wallLeft() == 0){
			printf("_| intersection \n");
			if(analog_et(3) < 100){
				printf("PullUp\n");
				pullUp();
				return turnDir;
			}
			else if (analog_et(6) < 100 && turnDir[0] != 1){
				printf("TurnLeft\n");
				turnLeft();
				turnDir [0] = 1;
				turnDir [1] -= 1;
				return turnDir;
			}
			else{
				printf("PullUp\n");
				pullUp();
				turnDir [0] = 2;
				return turnDir;
			}
		} 
		else {
			forward();
			return turnDir;
		}
	}
	else if(wallRight() == 0 && wallLeft() == 0){
		printf(" -- intersection \n");
		if(analog_et(4) < 100 && turnDir[0] != 0){
			printf("TurnRight\n");
			turnRight();
			turnDir [0] = 0;
			turnDir [1] += 1;
			return turnDir;
		}
		else if (analog_et(6) < 100 && turnDir[0] != 1){
			printf("TurnLeft\n");
			turnLeft();
			turnDir [0] = 1;
			turnDir [1] -= 1;
			return turnDir;
		}
		else{
			printf("TurnRight\n");
			turnRight();
			turnDir [0] = 0;
			turnDir [1] += 1;
			return turnDir;
		}
	}
	else if (wallRight() == 0 && turnDir[0] != 0){
		printf("Right intersection \n");
		turnRight();
		turnDir[0] = 0;
		turnDir[1] += 1;
		return turnDir;
	}
	else if (wallLeft() == 0 && turnDir[0] != 1){
		printf(" Left intersection \n");
		turnLeft();
		turnDir [0] = 1;
		turnDir [1] -= 1;
		return turnDir;
	}
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
	forward ();
	return turnDir;
}

int main(){
	int i, 
	j,
	turnArray [2],
	*tempP;
	int pastDirArray [5] = {-1,-1,-1,-1,-1};
	turnArray[0] = 2;
	turnArray[1] = 0;
	//int h = 0;
	//int w = 0;
	camera_open();
	while(1){
		/*camera_update();
		if (get_object_count(0) > 0){
		h = get_object_bbox(0,0).height;
		w = get_object_bbox(0,0).width;
		printf("Height: %d, Width: %d\n",h,w);
		}*/
		for (i = 0; i < 3; i++){
			tempP = solveMaze(turnArray);
			
			turnArray [0] = tempP [0];
			
			if (tempP [1] == 5)
				turnArray [1] = 0;
			else if (tempP [1] == -1)
				turnArray [1] = 3;
			else
				turnArray [1] = tempP [1];
			
			for (j = 4; j > -1; j--)
				pastDirArray[j] = pastDirArray[j - 1];
			pastDirArray [0] = turnArray [1];
			
			if (turnArray[0] == 5)
			printf ("PacMan Found!\n Firing Weapons Array\n PacMan Neutralized!");
			freeze (0);
			freeze (2);
			break;
		}
		if (turnArray[0] == 5)
		break;
		forward ();
		
		if(checkExit()){
			printf ("PacMan Found!\n Firing Weapons Array\n PacMan Neutralized!");
			freeze (0);
			freeze (2);
			break;
		}
	}
	camera_close();
}
