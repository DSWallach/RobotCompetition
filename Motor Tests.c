void forward(){
	motor(0,100);
	msleep(25);
	ao();
	motor(2,100);
	msleep(25);
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
	msleep(53);
	ao();
	motor(2,100);
	msleep(53);
	ao();
}
void right(){
	motor(0,100);
	msleep(53);
	ao();
	motor(2,-100);
	msleep(53);
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
	

//Detects an object and returns 1 for true and 0 for false
int wallFront(){
	if(analog_et(0) > 300){
		return 1;
	}
	else{
		return 0;
	}
}

// I tried set_analog_pullup(0,0)
// it may be for an older version of the KISS
// software. 
// I think the function 
// analog_et(0) 
// that Nick mentioned may work. Even if it doesn't the code
// for the bump sensor is pretty much the same.
int main(){ 
	while(wallFront() == 0){
		forward();
	}
	stop();
	turnLeft();
}
