#include "PID_Control.h"

/* GLOBAL */
struct timespec now;
		
// Set tunable constants
void init_PID(PID *p, double Kp, double Ki, double Kd) {
	p->kp = Kp;//proportionality constant
	p->ki = Ki;//integration constant 
	p->kd = Kd;//damping constant
}

void reset_PID(PID *p){
	int j;
	p->lastInput = 0.0;
	
	for(j = 0; j < 8; j++) p->errSum[j] = 0.0;
	
	p->i = 0;
	clock_gettime(CLOCK_REALTIME, &(p->lastTime));
}

double Compute(PID *p, double error) {//this method must be called repetitively, either regularly or irregularly
	//declare variables:
	double  dError,
		sum,
		Output,
		dt;
	int	j;//cursor
	
	clock_gettime(CLOCK_REALTIME, &now);//get time from clockcycles
   
   	//compute difference in time since last compute- theoretically down to the nanosecond. 
	dt = (double)(now.tv_sec - p->lastTime.tv_sec) + (double)(now.tv_nsec - p->lastTime.tv_nsec)/1000000000.0;
	
	/* catch any timer errors, normalize to 1 second.  I suppose this is meant to deal with timer overflow.*/
	if(dt > 1.0) dt = 1.0;
	if(dt < 0.0) dt = 1.0;
	
	/*Compute all the working error variables*/
	//error = Setpoint - Input;//proportional
	p->errSum[p->i] = error * dt;//integral
	dError = (error - p->lastError) / dt;//derivative
	
	sum = 0.0;
	for(j = 0; j < 8; j++) sum += p->errSum[j];//sum last i parts of intrgral.  

	//printf("dt = %f\terror = %f\terrSum = %f\tdInput = %f\n", dt, error, sum, dInput);//diagnostic
	
	/*Compute PID Output*/
	Output = (p->kp * error) + (p->ki * sum) + (p->kd * dInput);//sum P I and D

	/*Remember some variables for next time*/
	p->lastError = error;
	p->lastTime.tv_sec = now.tv_sec;
	p->lastTime.tv_nsec = now.tv_nsec;	
	p->i++;
	if(p->i == 8) p->i = 0;
	
	return Output;
}