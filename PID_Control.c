#include "PID_Control.h"

/* GLOBAL */
struct timespec now;
		
// Set tunable proportionality constants
void SetTunings(PID *p, double Kp, double Ki, double Kd) {
	p->kp = Kp;
	p->ki = Ki;
	p->kd = Kd;
}

double Compute(PID *p, double Input, double Setpoint) {
	double 	error,
		dErr,
		Output,
		dt;
	
	clock_gettime(CLOCK_REALTIME, &now);
   
	dt = (double)(now.tv_sec - p->lastTime.tv_sec) + (double)(now.tv_nsec - p->lastTime.tv_nsec)/100000000.0;
	
	/*Compute all the working error variables*/
	error = Setpoint - Input;
	p->errSum += error * dt;
	dErr = (error - p->lastErr) / dt;

	/*Compute PID Output*/
	Output = p->kp * error + p->ki * p->errSum + p->kd * dErr;

	/*Remember some variables for next time*/
	p->lastErr = error;
	p->lastTime.tv_sec = now.tv_sec;
	p->lastTime.tv_nsec = now.tv_nsec;		
	
	return Output;
}