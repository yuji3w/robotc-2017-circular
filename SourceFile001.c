#pragma config(Sensor, S1,     ,               sensorEV3_Color)
#pragma config(Sensor, S4,     ,               sensorEV3_Color)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define FIELDX 73
#define FIELDY 61
#define SPEED 30
#define FINT 65
#define OINT 15
#define FAVG 35
#define WMAX 120
#define HMAX 120
#define OUT 0
#define IN 1

#define WHEEL 18
#define AXLE 12.5

#define ALTS(A) ((A)==0 ? 3 : 0)
#define MEDIAN(a,b,c) ((a-b)*(b-c) > 1 ? b : ((a-b)*(a-c) < -1 ? a : c))

short distance[500];
int Motor1[500];
int isolate;
int iarr[8];
int totalDist;

void moveRob(float distance, int speed);
void rotateRob(float angle, int speed);
void edgeRob(int speed, int sensor);
bool analyzeArray(int flag);
bool posRob(int sensor, int inOrOut);
void clearMem();
void min(int s);
void min2(int s);
void movetoCenter();
void gotoRadius();
void bubbleSort();
void encircle();
void align();

typedef struct coordinate{
	int x[2];
	int y[2];
};
coordinate coordinates[4];

task main(){
	setMotorSpeed(motorA,0);
	for(int i = 0; i < 4; i++){
		clearMem();
		resetMotorEncoder(motorA);
		edgeRob(SPEED,S4);
		moveRob(7,SPEED);
		moveRob(-7,SPEED);
		setMotorSpeed(motorA,0);
		setMotorSpeed(motorB,0);
		rotateRob(-90,SPEED);
		sleep(200);
	}
	moveRob(-3,SPEED);
	align();
	moveRob(9.7+(AXLE/2)+2 ,SPEED);
	rotateRob(-45,SPEED);
	gotoRadius();
	encircle();
	rotateRob(-90,SPEED);
	/*
	movetoCenter();
	moveRob((FIELDY/2-17-11),SPEED);
	setMotorSpeed(motorC,100);
	sleep(500);*/
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

void edgeRob(int speed , int sensor){
	int flag = 0;
	while(posRob(ALTS(sensor),IN)){
		while(posRob(ALTS(sensor),IN)){
			if(sensor){
				setMotorSpeed(motorA,speed-0.25*(FAVG-getColorReflected(sensor)));
				setMotorSpeed(motorD,speed+0.25*(FAVG-getColorReflected(sensor)));
				}else{
				setMotorSpeed(motorA,speed+0.25*(FAVG-getColorReflected(sensor)));
				setMotorSpeed(motorD,speed-0.25*(FAVG-getColorReflected(sensor)));
			}
			flag++;
			if(!(flag%200)){
				distance[flag/200]=getUSDistance(S3);
				Motor1[flag/200]=getMotorEncoder(motorA);
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
	if(getColorReflected(sensor)<FAVG){
		return(OUT==inOrOut);
		}else if(getColorReflected(sensor)>FAVG){
		return(IN==inOrOut);
	}
	return false;
}

void clearMem(){
	for(int i = 0;i < 500; i++){
		distance[i] = 0;
	}
}

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
			radius[i][1] = sqrt(pow((coordinates[i].x[0]-(FIELDX/2)),2) + pow((coordinates[i].y[0]-(FIELDY/2)),2));
		}
		if(i%2){
			radius[i][0] = sqrt(pow((coordinates[i].y[0]-(FIELDX/2)),2) + pow((coordinates[i].x[0]-(FIELDY/2)),2));
			radius[i][1] = sqrt(pow((coordinates[i].y[0]-(FIELDX/2)),2) + pow((coordinates[i].x[0]-(FIELDY/2)),2));
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
	rotateRob(-90,SPEED);
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
	float ratio = ((distance+AXLE/2)/(distance-AXLE/2));
	setMotorSpeed(motorA,SPEED*pow(ratio,-1));
	setMotorSpeed(motorD,SPEED*ratio);
	int temp;
	totalDist = getMotorEncoder(motorD);
	while(1){
		if(getUSDistance(S2)<13){
			temp = getMotorEncoder(motorA);
			while(getColorReflected(S1)>FAVG && getColorReflected(S4)>FAVG){
				setMotorSpeed(motorA,SPEED);
				setMotorSpeed(motorD,SPEED);
			}
			setMotorSpeed(motorA,0);
			setMotorSpeed(motorD,0);
			while(getMotorEncoder(motorA)>temp){
				setMotorSpeed(motorA,-SPEED);
				setMotorSpeed(motorD,-SPEED);
			}
			setMotorSpeed(motorA,0);
			setMotorSpeed(motorD,0);
			setMotorSpeed(motorA,SPEED*pow(ratio,-1));
			setMotorSpeed(motorD,SPEED*ratio);
			break;
		}
	}
	while(1){
		if(getUSDistance(S2)<13){
			totalDist = getMotorEncoder(motorD) - totalDist;
			temp = getMotorEncoder(motorA);
			while(getColorReflected(S1)>FAVG && getColorReflected(S4)>FAVG){
				setMotorSpeed(motorA,SPEED);
				setMotorSpeed(motorD,SPEED);
			}
			setMotorSpeed(motorA,0);
			setMotorSpeed(motorD,0);
			while(getMotorEncoder(motorA)>temp){
				setMotorSpeed(motorA,-SPEED);
				setMotorSpeed(motorD,-SPEED);
			}
			setMotorSpeed(motorA,0);
			setMotorSpeed(motorD,0);
			setMotorSpeed(motorA,SPEED*pow(ratio,-1));
			setMotorSpeed(motorD,SPEED*ratio);
			break;
		}
	}
}

void align(){
	int i = 0;
	while(1){
		setMotorSpeed(motorA,(FAVG-getColorReflected(S1))/3);
		setMotorSpeed(motorD,(FAVG-getColorReflected(S4))/3);
		if(++i>=1000){
			break;
		}
	}
}
