int counter = 2;
int trap = 0;

void forward(){
	motor(0,95);
	motor(2,100);
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
	counter++;
	printf("counter = %d", counter);
	for(i = 0; i < 8; i++){
		left();
	}
	ao();
}

// This turns the robot to the right
void turnRight(){
	int i;
	counter++;
	printf("counter = %d \n", counter);
	for(i = 0; i < 8; i++){
		right();
		}
	ao();
}

// This turns the robot around to the right 180 degrees
void turnAround(){
	int i; 
	for(i = 0; i < 17; i++){
		right();
	}
	ao();
}

void stop(){
	motor(0,-100);
	msleep(10);
	ao();
	motor(2,-100);
	msleep(10);
	ao();
}

void reset(){
	if (trap == 1 && counter > 10){
		printf("reset \n");
		trap = 0;
	}
}

//Detects an object in front and returns 1 for true and 0 for false
int wallFront(){
	if(analog_et(0) > 400){
		return 1;
		if (analog_et(3) < 100){
			printf("trap \n");
			trap = 1;
			counter = 0;
			printf("counter = %d \n", counter);
		}
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
		if (analog_et(3) < 100) {
			printf("trap\n");
			 trap = 1;
			 counter = 0;
			 printf("counter = %d \n", counter);
		}
		int i;
		for (i = 0; i < 4; i++) {
			backUp();
			right();
		}
		return lastTurn;
	}
	else if(digital(14) == 0){
		printf("Left Bumper\n");
		int i;
		for (i = 0; i < 2; i++) {
			backUp();
			right();
		}
		return lastTurn;
	}
	else if(digital(15) == 1){
		printf("Right Bumper\n");
		int i;
		for (i = 0; i < 2; i++) {
			backUp();
			left();
		}
		return lastTurn;
	}
	else if(wallFront() == 0){
		if (wallRight() == 0 && wallLeft() == 0){
			printf("T intersection \n"); 
			stop();
			if(analog_et(3) < 100 && trap != 1){
				printf("PullUp\n");
				pullUp();
				return 2;
			}
			else if (analog_et(4) < 100 && lastTurn != 0 && trap != 1){
				printf("TurnRight\n");
				turnRight();
				return 0;
			}
			else if (analog_et(6) < 100 && lastTurn != 1 && trap != 1){
				printf("TurnLeft\n");
				turnLeft();
				return 1;
			}
			else if (trap == 1 && lastTurn != 1){
				printf("Trapped Turnright\n");
				turnRight();
				return 0;
			}
			else{	
				pullUp();
				return 2;
			}
		}
		else if(wallRight() == 0){
			printf(" L intersection \n");
			stop();
			if(analog_et(3) < 100 && trap != 1){
				printf("PullUp\n");
				pullUp();
				return 2;
			}
			else if (analog_et(4) < 100 && lastTurn != 0 && trap != 1){
				printf("TurnRight\n");
				turnRight();
				return 0;
			}
			else if (trap = 1){
				printf("Trapped TurnRight\n");
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
			stop();
			printf("_| intersection \n");
			if(analog_et(3) < 100 && trap != 1){
				printf("PullUp\n");
				pullUp();
				return lastTurn;
			}
			else if (analog_et(6) < 100 && lastTurn != 1 && trap != 1){
				printf("TurnLeft\n");
				turnLeft();
				return 1;
			}
			/*else if (trap == 1){
				printf("Trapped PullUpt\n");
				pullUp();
				return 1;
			}*/
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
		stop();
		printf(" -- intersection \n");
		if(analog_et(4) < 100 && lastTurn != 0 && trap != 0){
			printf("TurnRight\n");
			turnRight();
			return 0;
		}
		else if (analog_et(6) < 100 && lastTurn != 1 && trap != 0){
			printf("TurnLeft\n");
			turnLeft();
			return 1;
		}
		/*else if (trap = 1){
			printf("turnRight\n");
			turnRight();
			counter++
			return 0;*/
		else{
			printf("TurnRight\n");
			turnRight();
			return 0;
		}
	}
	else if (wallRight() == 0 && lastTurn != 0){
		stop();
		printf("Right intersection \n");
		turnRight();
		return 0;
	}
	else if (wallLeft() == 0 && lastTurn != 1){
		stop();
		printf(" Left intersection \n");
		turnLeft();
		return 1;
	}
	else if (wallFront() == 1 && wallRight() == 1 && wallLeft() == 1){
		printf("Dead End \n");
		if (analog_et(3) < 100){
			trap = 1;
			counter = 0;
			printf("Trapped \n");
		}
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
		printf("Biggest Object Size: %d by %d\n", get_object_bbox(0,0).height, get_object_bbox(0,0).width);
		printf("Number of Objects: %d\n", get_object_count(0));
		int h = get_object_bbox(0,0).height;
		int w = get_object_bbox(0,0).width;
		if (h > 40 && w > 100){
			//camera_close();
			pullUp();
			return 1;
		} else if (get_object_count(0) > 50){
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
		for (i = 0; i < 1; i++){
			lastTurn = solveMaze(lastTurn);
		}
		reset();
		if(checkExit()){
			printf ("Exit Found!");
			break;
		}
	}
	camera_close();
}	
