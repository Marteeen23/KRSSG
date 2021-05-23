#include <bits/stdc++.h>
#include "PID.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "turtlesim/Pose.h"

/* Controller parameters */
#define PID_KP  0.5f
#define PID_KI  0.05f
#define PID_KD  0.025f

#define PID_TAU 0.02f

#define PID_LIM_MIN -1.0f
#define PID_LIM_MAX  1.0f

#define PID_LIM_MIN_INT -0.5f
#define PID_LIM_MAX_INT  0.5f

#define SAMPLE_TIME_S 0.01f

using namespace std;

float *measurement;

float *setGoal;

vector <pair<float, float>> V;

void PID_CONTROLLER_init(PID_CONTROLLER *pid){
	pid->Kp = PID_KP;
	pid->Ki = PID_KI;
	pid->Kd = PID_KD;

	pid->tau = PID_TAU;

	pid->limMax = PID_LIM_MAX;
	pid->limMin = PID_LIM_MIN;

	pid->limMaxInt = PID_LIM_MAX_INT;
	pid->limMinInt = PID_LIM_MIN_INT;

	pid->T = SAMPLE_TIME_S;

	pid->integratorX = 0.0f;
	pid->integratorY = 0.0f;
	pid->prevErrorX = 0.0f;
	pid->prevErrorY = 0.0f;

	pid->differentiatorX = 0.0f;
	pid->differentiatorY = 0.0f;
	pid->prevMeasurementX = 0.0f;
	pid->prevMeasurementY = 0.0f;

	pid->outX = 0.0f;
	pid->outY = 0.0f;

}

void PID_CONTROLLER_update(PID_CONTROLLER *pid){

	//Error signal
	float errorX = setGoal[0]-measurement[0];
	float errorY = setGoal[1]-measurement[1];

	//Proportional
	float proportionalX = pid->Kp * errorX;
	float proportionalY = pid->Kp * errorY;

	//Integral
	pid->integratorX += 0.5f * pid->Ki * pid->T *(errorX + pid->prevErrorX);
	pid->integratorY += 0.5f * pid->Ki * pid->T *(errorY + pid->prevErrorY);

	//Anti wind-up mechanism (Clamping)
	if (pid->integratorX > pid->limMaxInt){
        pid->integratorX = pid->limMaxInt;
	}

    else if (pid->integratorX < pid->limMinInt){
		pid->integratorX = pid->limMinInt;
    }

    if (pid->integratorY > pid->limMaxInt){
        pid->integratorY = pid->limMaxInt;
	}

    else if (pid->integratorY < pid->limMinInt){
		pid->integratorY = pid->limMinInt;
    }

	//Differential
	
	pid->differentiatorX = -(2.0f * pid->Kd * (measurement[0]-pid->prevMeasurementX) + (2.0f * pid->tau - pid->T) * pid->differentiatorX) / (2.0f * pid->tau + pid->T);
    pid->differentiatorY = -(2.0f * pid->Kd * (measurement[1]-pid->prevMeasurementY) + (2.0f * pid->tau - pid->T) * pid->differentiatorY) / (2.0f * pid->tau + pid->T);

	//Controller output
	pid->outX = proportionalX + pid->integratorX + pid->differentiatorX;
    pid->outY = proportionalY + pid->integratorY + pid->differentiatorY;

    float out = (float)sqrt(pow((double)pid->outX,2) + pow((double)pid->outY, 2));
	//Limit controller output
	if (pid->outX > sqrt(pid->limMax)){
		pid->outX = sqrt(pid->limMax);
    }

	else if (pid->outX < sqrt(pid->limMin)){
		pid->outX = sqrt(pid->limMin);
    }

	if (pid->outY > sqrt(pid->limMax)){
		pid->outY = sqrt(pid->limMax);
    }

	else if (pid->outY < sqrt(pid->limMin)){
		pid->outY = sqrt(pid->limMin);
	}
	// Store error and measurement for later use 
	pid->prevErrorX = errorX;
    pid->prevErrorY = errorY;
	pid->prevMeasurementX = measurement[0];
	pid->prevMeasurementY = measurement[1];

}

void pose_callback(const turtlesim::Pose& msgIn){
    measurement[0] = msgIn.x;
    measurement[1] = msgIn.y;
	cout<<measurement[0]<<" "<<measurement[1]<<endl;
}

int main(int argc, char**argv){
	measurement = new float[2];
	setGoal = new float[2];
	/* Initialise PID controller */
	V = { {2.0, 1.0}, {2.0, 5.0}, {3.0, 6.0}, {3.0, 6.4}, {3.6, 8.1}, {5.0, 9.0}, {7.0, 9.2} };
    PID_CONTROLLER pid;

    PID_CONTROLLER_init(&pid);

    ros::init(argc, argv, "Turtle");
    ros::NodeHandle node;
    ros::Publisher vel_publish = node.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    ros::Subscriber sub = node.subscribe("/turtle1/pose", 1000, pose_callback);
    int t=0;
    setGoal[0] = V[0].first;
    setGoal[1] = V[0].second;
    ros::Rate loopRate(10);
    geometry_msgs::Twist vel_msg;

    while(t<V.size()){

        PID_CONTROLLER_update(&pid);
	    vel_msg.linear.x = pid.outX;
        vel_msg.linear.y = pid.outY;
        cout<<pid.outX<<" "<<pid.outY<<endl;
        vel_msg.linear.z = 0.0f;
	    vel_msg.angular.x = 0.0f;
	    vel_msg.angular.y = 0.0f;
	    vel_msg.angular.z = 0.0f;
        vel_publish.publish(vel_msg);
        ros::spinOnce();
        loopRate.sleep();
        if(measurement[0]==setGoal[0] && measurement[1]==setGoal[1]){
        	t++;
        	setGoal[0] = V[t].first;
        	setGoal[1] = V[t].second;
        }
    }
    
    vel_msg.linear.x = 0.0f;
    vel_msg.linear.y = 0.0f;
	vel_publish.publish(vel_msg);

    return 0;
}


