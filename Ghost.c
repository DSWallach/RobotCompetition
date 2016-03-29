void forward(int left, int right){
	motor(0,100);
	msleep(left);
	ao();
	motor(2,100);
	msleep(right);
	ao();
}

void backup(){
	motor(0,-100);
	msleep(50);
	ao();
	motor(2,-100);
	msleep(50);
	ao();
}
// Note: left() and right() may be reversed depending on
// how the motors are plugged in. I think the left motor
// was in motor port 0 and the right one in motor port 2,
// so this should be correct.

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
	if(analog_et(1) > 300){
		return 1;
	}
	else{
		return 0;
	}
}

//Detects an object to the right and returns 1 for true and 0 for false
int wallRight(){
	if(analog_et(2) > 300){
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
	int wFront = analog_et(0),
	wLeft = analog_et(1),
	wRight = analog_et(2);
	printf ("Front %d, Left %d, Right %d: ", wFront, wLeft, wRight);
	if( 1){//analog_et(0) < 300){
		
		forward (27, 20);
		
	} else if(analog_et(2) < 300){
		printf ("Right\n");
		turnRight();
	} 
	else if (analog_et(1) < 300){
		printf("left\n");
		turnLeft();
	} 
	else {
		printf("turn\n");
		turnAround();
	}
}

int main(){ 
	while(goal() == 0){
		solveMaze();
	}
}
