#define WHITE 10
#define BLACK 0
#define CENTER 5
task main()
{
int dist = getUSDistance(S2)
if (dist < 10)
	 setMotorSpeed (motorA,0);
   setMotorSpeed (motorB,0);




 while(1)
 {
 int error = getColorReflected(S1)-CENTER;

   setMotorSpeed (motorA,50 + error);
   setMotorSpeed (motorB,50 - error);

 }
}
