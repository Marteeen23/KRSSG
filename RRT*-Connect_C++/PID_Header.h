#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct{

	//Controller constants
	float Kp, Ki, Kd;

	//Derivative low-pass filter time constant
	float tau;

	//Output Limits
	float limMax, limMin;

	//Integrator limits
	float limMaxInt, limMinInt;

	//Sample time
	float T;

	//Controller memory
	float integratorX, prevErrorX, differentiatorX, prevMeasurementX;
	float integratorY, prevErrorY, differentiatorY, prevMeasurementY;

	//Controller Output
	float outX, outY;

} PID_CONTROLLER;

void PID_CONTROLLER_init(PID_CONTROLLER *pid);
float PID_CONTROLLER_update(PID_CONTROLLER *pid);

#endif