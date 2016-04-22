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

int solveMaze(int lastTurn){
	if (digital(13) == 1 || digital (12) == 1){
		printf("Front Bumper\n");
		backUp();
		backUp();
		right();
		return lastTurn;
	}
	else if(digital(14) == 0){
		printf("Left Bumper\n");
		backUp();
		backUp();
		right();
		backUp();
		right();
		right();
		return lastTurn;
	}
	else if(digital(15) == 0){
		printf("Right Bumper\n");
		backUp();
		backUp();
		left();
		backUp();
		left();
		left();
		return lastTurn;
	}
	else if(wallFront() == 0){
		if (wallRight() == 0 && wallLeft() == 0){
			printf("T intersection \n"); 
			if(analog_et(3) < 100){
				printf("PullUp\n");
				pullUp();
				return 2;
			}
			else if (analog_et(4) < 100 && lastTurn != 0){
				printf("TurnRight\n");
				turnRight();
				return 0;
			}
			else if (analog_et(6) < 100 && lastTurn != 1){
				printf("TurnLeft\n");
				turnLeft();
				return 1;
			}
			else{	
				pullUp();
				return 2;
			}
		}
		else if (checkExit ())
		return 5;		
		else if(wallRight() == 0){
			printf(" L intersection \n");
			if(analog_et(3) < 100){
				printf("PullUp\n");
				pullUp();
				return 2;
			}
			else if (analog_et(4) < 100 && lastTurn != 0){
				printf("TurnRight\n");
				turnRight();
				return 0;
			}
			else{
				printf("PullUp\n");
				pullUp();
				return 2;
			}
		}
		else if(wallLeft() == 0){
			printf("_| intersection \n");
			if(analog_et(3) < 100){
				printf("PullUp\n");
				pullUp();
				return lastTurn;
			}
			else if (analog_et(6) < 100 && lastTurn != 1){
				printf("TurnLeft\n");
				turnLeft();
				return 1;
			}
			else{
				printf("PullUp\n");
				pullUp();
				return 2;
			}
		} 
		else {
			forward();
			return lastTurn;
		}
	}
	else if(wallRight() == 0 && wallLeft() == 0){
		printf(" -- intersection \n");
		if(analog_et(4) < 100 && lastTurn != 0){
			printf("TurnRight\n");
			turnRight();
			return 0;
		}
		else if (analog_et(6) < 100 && lastTurn != 1){
			printf("TurnLeft\n");
			turnLeft();
			return 1;
		}
		else{
			printf("TurnRight\n");
			turnRight();
			return 0;
		}
	}
	else if (wallRight() == 0 && lastTurn != 0){
		printf("Right intersection \n");
		turnRight();
		return 0;
	}
	else if (wallLeft() == 0 && lastTurn != 1){
		printf(" Left intersection \n");
		turnLeft();
		return 1;
	}
	else if (wallFront() == 1 && wallRight() == 1 && wallLeft() == 1){
		printf("Dead End \n");
		turnAround();
		return 4;
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

int main(){ 
	int lastTurn = 2;
	int h = 0;
	int w = 0;
	camera_open();
	while(1){
		/*camera_update();
		if (get_object_count(0) > 0){
		h = get_object_bbox(0,0).height;
		w = get_object_bbox(0,0).width;
		printf("Height: %d, Width: %d\n",h,w);
		}*/
		int i;
		for (i = 0; i < 3; i++){
			lastTurn = solveMaze(lastTurn);
			if (lastTurn == 5)
			printf ("PacMan Found!\n Firing Weapons Array\n PacMan Neutralized!");
			freeze (0);
			freeze (2);
			break;
		}
		if (lastTurn == 5)
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
