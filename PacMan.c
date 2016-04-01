void forward(){
	motor(0,100);
	msleep(25);
	ao();
	motor(2,100);
	msleep(25);
	ao();
}

void backUp(){
	motor(0,-100);
	msleep(50);
	ao();
	motor(2,-100);
	msleep(50);
	ao();
}

void pullUp(){
	int i;
	for(i = 0; i < 20; i++){
		forward();
	}
}

void left(){
	motor(0,-100);
	msleep(55);
	ao();
	motor(2,100);
	msleep(55);
	ao();
}
void right(){
	motor(0,100);
	msleep(55);
	ao();
	motor(2,-100);
	msleep(55);
	ao();
}

// This turns the robot to the left
void turnLeft(){
	int i;
	for(i = 0; i < 5; i++){
		left();
	}
}

// This turns the robot to the right
void turnRight(){
	int i;
	for(i = 0; i < 5; i++){
		right();
	}
}

// This turns the robot around to the right 180 degrees
void turnAround(){
	int i; 
	for(i = 0; i < 11; i++){
		right();
	}
}

void stop(){
	motor(0,-100);
	msleep(10);
	ao();
	motor(2,-100);
	msleep(10);
	ao();
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

int goal(){
	return 0;
}

int solveMaze(int lastTurn){
	if (digital(13) == 1){
		printf("Front Bumper\n");
		backUp();
		backUp();
		right();
		return lastTurn;
	}
	else if(digital(14) == 0){
		printf("Left Bumper\n");
		backUp();
		right();
		backUp();
		right();
		return lastTurn;
	}
	else if(digital(15) == 0){
		printf("Right Bumper\n");
		backUp();
		left();
		backUp();
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
	//camera_open();
	camera_update();
	// Color config
	// Hue: 44 to 64
	// Saturation: 40 to 255
	// Value: 150 to 255
	// printf("checkExit");
	if (get_object_count(0) > 0){
		printf("Number of Objects: %d\n", get_object_count(0));
		int h = get_object_bbox(0,0).height;
		int w = get_object_bbox(0,0).width;
		if (h > 40 && w > 100){
			//camera_close();
			pullUp();
			return 1;
		} else if (get_object_count(0) > 15){
			pullUp();
			return 1;
		}
		else {
			//camera_close();
			return 0;
		}
	} 
	else {
		//camera_close();
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
		for (i = 0; i < 5; i++){
			lastTurn = solveMaze(lastTurn);
		}
		if(checkExit()){
			printf ("Exit Found!");
			break;
		}
	}
	camera_close();
}
