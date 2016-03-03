void forward(){
	motor(0,100);
	msleep(50);
	ao();
	motor(2,100);
	msleep(50);
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
	msleep(50);
	ao();
	motor(2,100);
	msleep(50);
	ao();
}
void right(){
	motor(0,100);
	msleep(50);
	ao();
	motor(2,-100);
	ao();
}
// I tried set_analog_pullup(0,0)
// it may be for an older version of the KISS
// software. 
// I think the function 
// analog_et(0) 
// that Nick mentioned may work. Even if it doesn't the code
// for the bump sensor is pretty much the same.
int main(){ 
	int i;
	int count = 0;
	while(count < 5){
		if(analog(0) > 0){
			count++;
			for(i = 0; i < 5; i++){
				backup();
			}
			left();
			left();
		} else {
			forward();
		}
	}
	printf("Hit 5 objects, I'm done.");
}
