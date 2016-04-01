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
	for(i = 0; i < 40; i++){
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

void solveMaze(){
	if(wallFront() == 0){
		if(digital(14) == 0){
			backUp();
			right();
		}
		else if(digital(15) == 0){
			backUp();
			left();
		}
		else {
			forward();
		}
	}
	if(wallFront() != 0){
		pullUp();
	}
	
	stop();
	if(wallFront() == 0 && wallRight() == 0 && wallLeft() == 0){
		printf("T intersection \n"); 
		if(analog_et(3) < 100){
			printf("PullUp\n");
			pullUp();
		}
		else if (analog_et(4) < 100){
			printf("TurnRight\n");
			turnRight();
		}
		else if (analog_et(6) < 100){
			printf("TurnLeft\n");
			turnLeft();
		}
		else{
			
			pullUp();
		}
	}
	else if(wallFront() == 0 && wallRight() == 0){
		printf(" L intersection \n");
		if(analog_et(3) < 100){
			printf("PullUp\n");
			pullUp();
		}
		else if (analog_et(4) < 100){
			printf("TurnRight\n");
			turnRight();
		}
		else{
			printf("PullUp\n");
			pullUp();
		}
	}
	else if(wallFront() == 0 && wallLeft() == 0){
		printf("_| intersection \n");
		if(analog_et(3) < 100){
			printf("PullUp\n");
			pullUp();
		}
		else if (analog_et(6) < 100){
			printf("TurnLeft\n");
			turnLeft();
		}
		else{
			printf("PullUp\n");
			pullUp();
		}
	}
	else if(wallRight() == 0 && wallLeft() == 0){
		printf(" -- intersection \n");
		if(analog_et(4) < 100){
			printf("TurnRight\n");
			turnRight();
		}
		else if (analog_et(6) < 100){
			printf("TurnLeft\n");
			turnLeft();
		}
		else{
			printf("TurnRight\n");
			turnRight();
		}
	}
	else if (wallRight() == 0){
		printf("Right intersection \n");
		turnRight();
	}
	else if (wallLeft() == 0){
		printf(" Left intersection \n");
		turnLeft();
	}
	else if (wallFront() == 1 && wallRight() == 1 && wallLeft() == 1){
		printf("Dead End \n");
		turnAround();
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
		int h = get_object_bbox(0,0).height;
		int w = get_object_bbox(0,0).width;
		if (h > 40 && w > 100){
			//camera_close();
			forward();
			forward();
			forward();
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
			solveMaze();
		}
		if(checkExit()){
			printf ("Exit Found!");
			break;
		}
	}
	camera_close();
}
