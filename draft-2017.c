#define WHEEL 18
#define AXLE 12.5
#define MINDETECTIONS1 20
#define MINDETECTIONS2 22
#define TENNIS 1
#define BOTTLE 2

//TODO: code theta r polar -> cartesian

float GPSx = 0.0;
float GPSy = 0.0;

float theta = 0.0;

void updateGPS(float x,float y)
{
	GPSx = x;
	GPSy = y;
}

int detectionState()
{
	if(getUSDistance(S1)<MINDETECTIONS1)
	{
		if(getUSDistance(S2)<MINDETECTIONS2)
		{
			return BOTTLE;
		}else
		{
			return TENNIS;
		}
	}else
	{
		return 0;
	}
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
	GPSy += distance * sgn(speed) * sinDegrees(theta);
	GPSx += distance * sgn(speed) * sinDegrees(theta);
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
	theta += angle * sgn(speed);
}

void lineFollow(int speed)
{
	while(1)
	{
		int offset = getColorReflected(S1);
		setMotorSpeed(motorA, speed + pow(offset,3));
		setMotorSpeed(motorD, speed - pow(offset,3));
	}
}


task main()
{

}
