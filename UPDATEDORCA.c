#pragma config(Sensor, S1,     ,               sensorEV3_Color)
#pragma config(Sensor, S4,     ,               sensorEV3_Color)
#pragma config(Motor,  motorA,          left,          tmotorEV3_Medium, PIDControl, reversed, driveLeft, encoder)
#pragma config(Motor,  motorD,          right,         tmotorEV3_Medium, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define FIELDX 75
#define FIELDY 114.4
#define SPEED 40
#define FINT 70
#define OINT 10
#define FAVG 50
#define WMAX 120
#define HMAX 120
#define OUT 0
#define IN 1

#define WHEEL 18
#define AXLE 27.69
#define AXLE2 26.69

#define ALTS(A) ((A)==0 ? 3 : 0)
#define MEDIAN(a,b,c) ((a-b)*(b-c) > 1 ? b : ((a-b)*(a-c) < -1 ? a : c))

short distance[2];
int Motor1[2];
int isolate;
int iarr[8];
int totalDist;
int ACTUALRADIUS;

void moveRob(float distance, int speed);
void rotateRob(float angle, int speed);
void edgeRob(int speed, int sensor, int no);
//bool analyzeArray(int flag);
bool posRob(int sensor, int inOrOut);
//void clearMem();
void min(int s);
void min2(int s);
void movetoCenter();
void gotoRadius();
void bubbleSort();
void encircle();
void align();
void inputValue();
void pushOff();
void checkList();

typedef struct coordinate{
	int x[2];
	int y[2];
};
coordinate coordinates[4];

void inputValue(){
	int i,k;
	sleep(1000);
	for(i = 1; i < 6; i++){
		if(getButtonPress(i) == 1){
			break;
		}
		if(i==5){
			i=0;
		}
	}
	ACTUALRADIUS = (i)*10;
	sleep(600);
	for(k = 1; k < 3; k++){
		if(getButtonPress(k) == 1){
			break;
		}
		if(k==2){
			k=0;
		}
	}
	k--;
	ACTUALRADIUS += k*5;
	displayCenteredBigTextLine(2, " %d", ACTUALRADIUS);
}

task main(){
	/*setMotorSpeed(motorC,-25);
	sleep(1000);
	setMotorSpeed(motorC,-3)*/
	setMotorBrakeMode(motorB,motorBrake);
	resetMotorEncoder(motorB);
	setMotorSpeed(motorB,0);
	sleep(500);
	moveRob((FIELDY-(FIELDY-FIELDX)/2-AXLE)+7-5,SPEED);
	moveMotorTarget(motorC, -180, -20);
	rotateRob(-90,-SPEED);
	moveRob(FIELDY-AXLE/2+22.5+5,SPEED);
	sleep(500);
	rotateRob(-90,-SPEED);
	moveRob((FIELDY-(FIELDY-FIELDX)/2-AXLE)+7+2,SPEED);
	rotateRob(-90,-SPEED);
	moveRob(FIELDY-AXLE+20,SPEED);
	moveRob(-20,SPEED);
	rotateRob(-90,-SPEED);
	moveRob(FIELDY/2+18+15+5,SPEED);
	moveRob(-30,SPEED);
	setMotorSpeed(motorC,10);
	rotateRob(-90,-SPEED);
	moveMotorTarget(motorB, -15, -10);
	moveRob(FIELDY/2-AXLE/2,SPEED);
	sleep(500);
  setMotorSpeed(motorB, 100);
  sleep(500);
  setMotorBrakeMode(motorB,motorCoast);
  setMotorSpeed(motorB,0);
  setMotorSpeed(motorC,-25);
	moveRob(-FIELDY/2+AXLE/2,SPEED);
	rotateRob(90,-SPEED);
	moveRob(-(FIELDX/2+18+4),SPEED);
}

void moveRob(float distance, int speed){
	int encoder[2];
	encoder[0] = getMotorEncoder(motorA);
	encoder[1] = getMotorEncoder(motorD);
	setMotorSpeed(motorA,speed*sgn(distance));
	setMotorSpeed(motorD,speed*sgn(distance));
	distance = distance * (360 / WHEEL);
	encoder[0] += distance;
	encoder[1] += distance;
	int flag = sgn(encoder[0]-getMotorEncoder(motorA));
	while(flag+sgn(encoder[0]-getMotorEncoder(motorA)) && flag+sgn(encoder[1]-getMotorEncoder(motorD))){}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}

void rotateRob(float angle, int speed){
	int encoder[2];
	encoder[0] = getMotorEncoder(motorA);
	encoder[1] = getMotorEncoder(motorD);
	setMotorSpeed(motorA,speed*sgn(angle));
	setMotorSpeed(motorD,speed*-sgn(angle));
	float distance = angle * (AXLE * PI * (360.0 / WHEEL)) / 360.0;
	encoder[0] += distance;
	encoder[1] -= distance;
	int flag[2];
	flag[0] = sgn(encoder[0]-getMotorEncoder(motorA));
	flag[1] = sgn(encoder[1]-getMotorEncoder(motorA));
	while(flag[0]+sgn(encoder[0]-getMotorEncoder(motorA)) && flag[1]+sgn(encoder[1]-getMotorEncoder(motorB))){}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}

void edgeRob(int speed , int sensor, int no){
	resetMotorEncoder(motorA);
	float distance = FIELDY/2;
	float limit = (FIELDY/2)>(FIELDX/2) ? (FIELDX/2) : (FIELDY/2);
	distance = distance * (360 / WHEEL);
	limit = limit * (360 / WHEEL);
	while(posRob(ALTS(sensor),IN)){
		while(posRob(ALTS(sensor),IN)){
			if(sensor){
				setMotorSpeed(motorA,speed-.3*(FAVG-getColorReflected(sensor)));
				setMotorSpeed(motorD,speed+.4*(FAVG-getColorReflected(sensor)));
				}else{
				setMotorSpeed(motorA,speed+.3*(FAVG-getColorReflected(sensor)));
				setMotorSpeed(motorD,speed-.4*(FAVG-getColorReflected(sensor)));
			}
			if(no == 3){
				displayBigTextLine(2, "%d", getMotorEncoder(motorA));
				if(getMotorEncoder(motorA)>distance){
					setMotorSpeed(motorA,0);
					setMotorSpeed(motorD,0);
					return;
				}
			}
			sleep(30);
		}
		sleep(30);
	}
}

int median(int * number, int n)
{
	int i, j, a;
	for (i = 0; i < n; ++i)
	{
		for (j = i + 1; j < n; ++j)
		{
			if (number[i] > number[j])
			{
				a =  number[i];
				number[i] = number[j];
				number[j] = a;
			}
		}
	}
	return(number[(n/2)]);
}

bool posRob(int sensor, int inOrOut){
	if(getColorReflected(sensor)<OINT){
		return(OUT==inOrOut);
		}else if(getColorReflected(sensor)>OINT){
		return(IN==inOrOut);
	}
	return false;
}

/*
void clearMem(){
for(int i = 0;i < 500; i++){
distance[i] = 0;
}
}
*/

void min(int s){
	int smallest = distance[1];
	for (int i = 2; distance[i]!=0; i++) {
		if (distance[i] < smallest) {
			smallest = distance[i];
		}
	}
	for(int i = 2; distance[i]!=0; i++){
		if (smallest==distance[i]){
			isolate = i;
		}
	}
	coordinates[s].y[0]=distance[isolate];
	coordinates[s].x[0]=Motor1[isolate];
}

void min2(int s){
	int smallest = distance[1];
	for (int i = 2; distance[i]!=0; i++) {
		if(i<isolate-5 || i>isolate+5){
			continue;
		}
		if (distance[i] < smallest) {
			smallest = distance[i];
		}
	}
	coordinates[s].y[1]=distance[isolate];
	coordinates[s].x[1]=Motor1[isolate];
}

void movetoCenter(){
	resetMotorEncoder(motorA);
	while(getMotorEncoder(motorA)<((FIELDX/2) * (360 / WHEEL))){
		setMotorSpeed(motorA,SPEED-0.4*(FAVG-getColorReflected(S4)));
		setMotorSpeed(motorD,SPEED+0.4*(FAVG-getColorReflected(S4)));
	}
	rotateRob(90,SPEED);
	setMotorSpeed(motorA,-30);
	setMotorSpeed(motorD,-30);
	while(getColorReflected(S1) > OINT || getColorReflected(S4) > OINT){
		setMotorSpeed(motorA,(FAVG-getColorReflected(S1))/2);
		setMotorSpeed(motorD,(FAVG-getColorReflected(S4))/2);
	}
}

void gotoRadius(){
	int radius[4][2];
	for(int i = 0; i < 4; i++){
		if(!(i%2)){
			radius[i][0] = sqrt(pow((coordinates[i].x[0]-(FIELDX/2)),2) + pow((coordinates[i].y[0]-(FIELDY/2)),2));
			radius[i][1] = sqrt(pow((coordinates[i].x[1]-(FIELDX/2)),2) + pow((coordinates[i].y[1]-(FIELDY/2)),2));
		}
		if(i%2){
			radius[i][0] = sqrt(pow((coordinates[i].y[0]-(FIELDX/2)),2) + pow((coordinates[i].x[0]-(FIELDY/2)),2));
			radius[i][1] = sqrt(pow((coordinates[i].y[1]-(FIELDX/2)),2) + pow((coordinates[i].x[1]-(FIELDY/2)),2));
		}
	}
	for(int i = 0; i < 4; i++){
		for(int k = 0; k < 2; k++){
			iarr[(i*2)+k] = radius[i][k];
		}
	}
	bubbleSort();
	int distance = sqrt(pow(FIELDX/2,2) + pow(FIELDY/2,2)) - iarr[3];
	moveRob(distance, SPEED);
	rotateRob(90,SPEED);
}

void bubbleSort(){
	int num = 8;
	for(int z = 0; z < 7; z++){
		for(int i = 0; i < 8; i++){
			if(iarr[i] >= FIELDX || iarr[i] >= FIELDY){
				iarr[i] = iarr[(i+1)%8];
			}
		}
	}
	int i, j, temp;
	for (i = 1; i < num; i++) {
		for (j = 0; j < num - 1; j++) {
			if (iarr[j] > iarr[j + 1]) {
				temp = iarr[j];
				iarr[j] = iarr[j + 1];
				iarr[j + 1] = temp;
			}
		}
	}
}

void encircle(){
	setMotorSpeed(motorD,40);
	setMotorSpeed(motorA,40*(ACTUALRADIUS-AXLE/2)/(ACTUALRADIUS+AXLE/2));
	resetMotorEncoder(motorD);
	totalDist = getMotorEncoder(motorD);
	pushOff();
	pushOff();
	while(getMotorEncoder(motorD)<=(ACTUALRADIUS+AXLE/2)*2*PI*1.1*(360/WHEEL));
	int temp = getMotorEncoder(motorA);
	while(getColorReflected(S1)>FAVG || getColorReflected(S4)>FAVG){
		setMotorSpeed(motorA,50);
		setMotorSpeed(motorD,50);
	}
	sleep(500);
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
	sleep(50);
	while(getMotorEncoder(motorA)>temp){
		setMotorSpeed(motorA,-50);
		setMotorSpeed(motorD,-50);
	}
	setMotorSpeed(motorA,0);
	setMotorSpeed(motorD,0);
}

void align(){
	time1[T1] = 0;
	while(time1[T1]<=3000){
		setMotorSpeed(motorA,(FAVG-getColorReflected(S1))/3);
		setMotorSpeed(motorD,(FAVG-getColorReflected(S4))/3);
	}
}

void pushOff(){
	int temp;
	while(getMotorEncoder(motorD)<=(ACTUALRADIUS+AXLE/2)*2*PI*1.1*(360/WHEEL)){
		if(getUSDistance(S2)<15){
			temp = getMotorEncoder(motorA);
			while(getColorReflected(S1)>FAVG || getColorReflected(S4)>FAVG){
				setMotorSpeed(motorA,50);
				setMotorSpeed(motorD,50);
			}
			sleep(500);
			setMotorSpeed(motorA,0);
			setMotorSpeed(motorD,0);
			sleep(50);
			while(getMotorEncoder(motorA)>temp){
				setMotorSpeed(motorA,-50);
				setMotorSpeed(motorD,-50);
			}
			setMotorSpeed(motorA,0);
			setMotorSpeed(motorD,0);
			setMotorSpeed(motorD,40);
			setMotorSpeed(motorA,40*(ACTUALRADIUS-AXLE/2)/(ACTUALRADIUS+AXLE/2));
			break;
		}
	}
}

void checkList(){
	getMotorEncoder(motorA);
	getMotorEncoder(motorC);
	getMotorEncoder(motorD);
	getColorReflected(S1);
	getColorReflected(S4);
	getUSDistance(S2);
	playSound(soundBeepBeep);
}
