#include <bits/stdc++.h>
#include "PID.h"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "turtlesim/Pose.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <random>

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

using namespace cv;
using namespace std;

struct _Node{
    int selfX;
    int selfY;
    int parentX;
    int parentY;
};

void generateRandom(int i, int *random, vector <_Node> TREE);
int extend(void *POINTER, int *x);

int REACHED = 0;
int rand_prev1 = 0;
string FILENAME ="src/path_find/src/motion1.PNG";
Mat image = imread(FILENAME, IMREAD_COLOR);
Mat binary;
int **arr;
int iter = 10000;
int startX = 50, startY = 40, reachX = 570, reachY = 535;
int STRIDE = 5, r = 1;
vector<_Node> TREE_A, TREE_B; // Trees from start and goal points
vector<pair<_Node, int>> COST;
int m1, m2; // Size of Image
double y=1.0, d=1.0, n=25.0; //Logarithmic function for subset of TREE
_Node _source = {startX, startY, -1, -1};
_Node _goal = {reachX, reachY, -1, -1};
_Node _new;

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

int distance(_Node nearest, int *x){

	double sq = pow((double)(nearest.selfX-x[0]),2)+pow((double)(nearest.selfY-x[1]),2);
    int d = (int)sqrt(sq);
	return d;
}

void _nearest(vector <_Node> TREE, int x[], _Node *nearest){

	*nearest = TREE[0];
	int c = distance(*nearest, x);
	for(int i=1;i<TREE.size();i++){

		if(distance(TREE[i], x)<c){
			c = distance(TREE[i], x);
			*nearest = TREE[i];
		}
	}
}

void steer(_Node nearest, int x[], _Node *_new){

	double y = x[0] - nearest.selfY;
	double _x = x[1] - nearest.selfX;
	double r = sqrt(pow(_x,2) + pow(y,2));
	double _sin = y/r;
	double _cos = _x/r;
	if(nearest.selfX - x[0]==0){
		_sin = 0;
		_cos = 1;
	}
	else if(nearest.selfY - x[1]==0){
		_sin = 1;
		_cos = 0;
	}
	_new->selfX = nearest.selfX + (int)(STRIDE*_cos);
	_new->selfY = nearest.selfY + (int)(STRIDE*_sin);
	if(_new->selfX>x[0])
		_new->selfX=x[0];
	if(_new->selfY>x[1])
		_new->selfY=x[1];	
}

bool noObstacles(_Node nearest, _Node _new){

	if(_new.selfX < 0 || _new.selfX >m1 || _new.selfY<0 || _new.selfY>m2 || arr[_new.selfX][_new.selfY]==255){
		return false;
	}
	double y = nearest.selfY - _new.selfY;
	double _x = nearest.selfX - _new.selfX;
	double r = sqrt(pow(_x,2) + pow(y,2));
	double _sin = y/r;
	double _cos = _x/r;
	if(_new.selfX - nearest.selfX==0){
		_sin = 0;
		_cos = 1;
	}
	else if(_new.selfY - nearest.selfY==00){
		_sin = 0;
		_cos = 1;
	}
    int x[2];
	for(int i=0; i<=STRIDE; i++){

		x[0] = nearest.selfX + (int)(i*_cos);
	    x[1] = nearest.selfY + (int)(i*_sin);
		for(int j=-r;j<r;j++){
            for(int k=-r;k<r;k++){
                if ((x[0]+j)>0 && (x[0]+j)<m1 && (x[1]+k)>0 && (x[1]+k)<m2){
					if( arr[x[0]+j][x[1]+k]== 255)
						return false;
				}
				else 
					return false;
            }
		}
	}
	return true;
}

void _near(vector <_Node> TREE, vector <_Node> *NEAR, _Node _n, int k){

	for(int i=0; i<TREE.size(); i++){
		for(int j=-k; j<k; j++){
			for(int q=-k; q<k; q++){
				if(TREE[i].selfX==(_n.selfX + j) && TREE[i].selfY==(_n.selfY + q)){
					NEAR->push_back(TREE[i]);
				}
			}
		}
	}
}

int dist_cost(_Node nearest, _Node _new){
    double sq = pow((double)(nearest.selfX - _new.selfX),2)+pow((double)(nearest.selfY - _new.selfY),2);
    int d = (int)sqrt(sq);
	return d;
}


bool notInTree(_Node _new, vector <_Node> TREE){
    int k1 = _new.selfX;
    int k2 = _new.selfY;
    for(int i=0;i<TREE.size();i++){
        if(k1==TREE[i].selfX && k2==TREE[i].selfY)
            return false;
    }
    return true;
}

void generateRandom(int i, int *random, vector <_Node> TREE){

	int num1 , num2;

	mt19937 eng;
	random_device r1, r2;
	seed_seq seed{r1(), r2()};
	eng.seed(seed);

	uniform_int_distribution<> dist1(0, m1-1);
	uniform_int_distribution<> dist2(0, m2-1);
	num1 = (dist1(eng) * r1())%m1;
	num2 = (dist2(eng) * r2())%m2;

	if(i%3==0){
		num1 = TREE[0].selfX;
		num2 = TREE[0].selfY;
	}
	random[0]=num1;
	random[1]=num2;
}

int extend(void *POINTER, int *x){
	vector<_Node> *TREE;
	TREE = (vector<_Node>*)POINTER;
	_Node nearest;
	_nearest(*TREE, x, &nearest);
	steer(nearest, x, &_new);

	if(noObstacles(nearest, _new) && notInTree(_new, *TREE)){
		TREE->push_back(_new);
		_Node _min;
		_min = nearest;
		int k = (int)min(y*pow((log(TREE->size()))/TREE->size(),d) , n);
		vector <_Node> NEAR;
		_near(*TREE, &NEAR, _new, k);
        int i;
        for(i=0;i<COST.size();i++){
            if(nearest.selfX==COST[i].first.selfX && nearest.selfY==COST[i].first.selfY){
                break;
            }
        }
		int c_min = COST[0].second + dist_cost(nearest, _new);
		for(int i=0;i<NEAR.size();i++){
            _Node near;
			near = NEAR[i];
            int j;
            for(j=0;j<COST.size();j++){
                if(near.selfX==COST[j].first.selfX && near.selfY==COST[j].first.selfY){
                    break;
                }
            }
			if(noObstacles(near, _new) && (COST[j].second + dist_cost(near, _new))<c_min){
				_min = near;
				c_min = COST[j].second + dist_cost(near, _new);
			}			
		}
		_new.parentX = _min.selfX;
		_new.parentY = _min.selfY;
        COST.push_back(make_pair(_new, c_min));
        TREE->pop_back();
        TREE->push_back(_new);
		cout<<TREE->at(TREE->size()-1).selfX<<" "<<TREE->at(TREE->size()-1).selfY<<" "<<TREE->at(TREE->size()-1).parentX<<" "<<TREE->at(TREE->size()-1).parentY<<endl;

		for(int i=0;i<NEAR.size();i++){
			_Node _near;
			_near = NEAR[i];
            int j;
            for(j=0;j<COST.size();j++){
                if(_new.selfX==COST[j].first.selfX && _new.selfY==COST[j].first.selfY){
                    break;
                }
            }
            int q;
            for(q=0;q<COST.size();q++){
                if(_near.selfX==COST[j].first.selfX && _near.selfY==COST[j].first.selfY){
                    break;
                }
            }
			if(noObstacles(_near, _new) && (COST[j].second + dist_cost(_near, _new)) < COST[q].second){

				TREE->at(q).parentX = _new.selfX;
				TREE->at(q).parentY = _new.selfY;
                COST[q].second = COST[j].second + dist_cost(_near, _new);
			}
		}
		Point p1(TREE->at(TREE->size()-1).selfX, TREE->at(TREE->size()-1).selfY), p2(TREE->at(TREE->size()-1).parentX, TREE->at(TREE->size()-1).parentY);
		line(image, p1, p2, Scalar(255, 0, 0),1, LINE_8);
		imshow("TEST IMAGE", image);
		waitKey(1);
		if(_new.selfX == x[0] && _new.selfY == x[1]){
			return 0;
		}
		else{
			return 1;
		}
	}
	return -1;
}

int connect(vector <_Node> *TREE, _Node x){
	int s;
	int *x_directed;
	x_directed = new int[2];
	x_directed[0] = x.selfX; 
	x_directed[1] = x.selfY;
	while(s==1){
		s = extend(TREE, x_directed);
	}
	return s;
}

void _swap(vector <_Node> *V1, vector <_Node> *V2){
	vector <_Node> V;
	for (int i=0; i<V1->size(); i++)
        V.push_back(V1->at(i));
	V1->clear();
	for (int i=0; i<V2->size(); i++)
        V1->push_back(V2->at(i));
	V2->clear();
	for (int i=0; i<V.size(); i++)
        V2->push_back(V[i]);	
}

bool _equal(_Node A, _Node B){
	if(A.selfX!=B.selfX)
		return false;
	if(A.selfY!=B.selfY)
		return false;
	if(A.parentX!=B.parentX)
		return false;
	if(A.parentY!=B.parentY)
		return false;
	return true;
}

void rrt_star_connect(){
	TREE_A.push_back(_source);
	TREE_B.push_back(_goal);
	COST.push_back(make_pair(TREE_A[0], 0));
	COST.push_back(make_pair(TREE_B[0], 0));
	int *_rand;
	_rand = new int[2];
	for(int i=0;i<iter || REACHED != 1;i++){

		generateRandom(i, _rand, TREE_B);
		int z=1;
		while(z==1){
			z = extend(&TREE_A, _rand);
		}
		if(z!=-1)
			connect(&TREE_B, _new);
		_swap(&TREE_A, &TREE_B);
		
		vector <_Node> A_OPTIMUM, B_OPTIMUM;
		for(int q=0; q<TREE_A.size(); q++){
			for(int j=0; j<TREE_B.size(); j++){
				if(TREE_A[q].selfX==TREE_B[j].selfX && TREE_A[q].selfY==TREE_B[j].selfY){
					A_OPTIMUM.push_back(TREE_A[q]);
					B_OPTIMUM.push_back(TREE_B[j]);
				}
			}
		}
		if(A_OPTIMUM.size()>0){
			REACHED =1;
			_Node A_opt, B_opt;
			A_opt = A_OPTIMUM[0];
			B_opt = B_OPTIMUM[0];
			int c1=-1, c2=-1;
			for(int j=0;j<COST.size();j++){
				if(_equal(A_opt, COST[j].first)){
					c1 = j;
				}
				else if(_equal(B_opt, COST[j].first)){
					c2 = j;
					break;
				}
			}
			int min = COST[c1].second + COST[c2].second;
			for(int q=1;q<A_OPTIMUM.size();q++){
				c1 = -1;
				c2 = -1;
				for(int j=0;j<COST.size();j++){
					if(_equal(A_OPTIMUM[q], COST[j].first)){
						c1 = j;
					}
					else if(_equal(B_OPTIMUM[q], COST[j].first)){
						c2 = j;
						break;
					}
				}
				if(COST[c1].second + COST[c2].second < min){
					min = COST[c1].second + COST[c2].second;
					A_opt = A_OPTIMUM[q];
					B_opt = B_OPTIMUM[q];
				}
			}
			vector<pair<int,int>> ROUTE;
			_Node tmp;
			tmp = A_opt;
			int temp[2];
			temp[0] = tmp.selfX;
			temp[1] = tmp.selfY;
			while(temp[0]!=-1 || temp[1]!=-1){
				ROUTE.insert(ROUTE.begin(), make_pair(temp[0], temp[1]));
				temp[0] = tmp.parentX;
				temp[1] = tmp.parentY;
				for(int j = 0;j <TREE_A.size();j++){
					if(temp[0]==TREE_A[j].selfX && temp[1]==TREE_A[j].selfY){
						tmp = TREE_A[j];
					}
				}
			}
			tmp = B_opt;
			temp[0] = tmp.parentX;
			temp[1] = tmp.parentY;
			while(temp[0]!=-1 || temp[1]!=-1){
				ROUTE.push_back(make_pair(temp[0], temp[1]));
				temp[0] = tmp.parentX;
				temp[1] = tmp.parentY;
				for(int j = 0;j <TREE_B.size();j++){
					if(temp[0]==TREE_B[j].selfX && temp[1]==TREE_B[j].selfY){
						tmp = TREE_B[j];
					}
				}
			}
			for(int j=0; j<ROUTE.size()-1; j++){
				cout<<ROUTE[j].first<<" "<<ROUTE[j].second<<endl;
				V.push_back(make_pair((float)ROUTE[j].first/m1*15.425*11.0, (float)ROUTE[j].second/m2*16.854*11.0));
				Point p1(ROUTE[j].first, ROUTE[j].second), p2(ROUTE[j+1].first, ROUTE[j+1].second);
				line(image, p1, p2, Scalar(0, 255, 0),2, LINE_8);
				imshow("TEST IMAGE", image);
				waitKey(1);
			}
		}
	}
}

int main(int argc, char**argv){
	
	namedWindow("TEST IMAGE", WINDOW_NORMAL);
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	threshold( gray, binary, 180,255,THRESH_BINARY );

	m1 = binary.rows;
	m2 = binary.cols;
	arr = new int*[m1];
	for(int i = 0; i < m1; i++){
		arr[i] = new int[m2];
	}

	for(int i = 0; i < m1; i++){
		for(int j = 0; j < m2; j++){
			arr[i][j]=binary.at<u_char>(i, j);
		}
		cout<<endl;
	}

	measurement = new float[2];
	setGoal = new float[2];

	/* Initialise PID controller */
	V = {{2.0, 3.0},{2.5, 3.6},{3.7, 3.8}};
    PID_CONTROLLER pid;

    PID_CONTROLLER_init(&pid);

    ros::init(argc, argv, "Turtle");
    ros::NodeHandle node;
    ros::Publisher vel_publish = node.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    ros::Subscriber sub = node.subscribe("/turtle1/pose", 1000, pose_callback);
    int t=0;
	rrt_star_connect();
    setGoal[0] = V[0].first;
    setGoal[1] = V[0].second;
    ros::Rate loopRate(10);
    geometry_msgs::Twist vel_msg;

    while(t<V.size()){

        PID_CONTROLLER_update(&pid);
	    vel_msg.linear.x = pid.outX;
        vel_msg.linear.y = pid.outY;
        //cout<<pid.outX<<" "<<pid.outY<<endl;
        vel_msg.linear.z = 0.0f;
	    vel_msg.angular.x = 0.0f;
	    vel_msg.angular.y = 0.0f;
	    vel_msg.angular.z = 0.0f;
        vel_publish.publish(vel_msg);
        ros::spinOnce();
        loopRate.sleep();
		Point p1((int)pid.prevMeasurementX*m1/(15.425*11), (int)pid.prevMeasurementY*m2/(16.854*11)), p2((int)measurement[0]*m1/(15.425*11), (int)measurement[1]*m2/(16.854*11));
		line(image, p1, p2, Scalar(127, 127, 127),2, LINE_8);
		imshow("TEST IMAGE", image);
		waitKey(1);
        if(abs(measurement[0]-setGoal[0])<0.01 || abs(measurement[1]-setGoal[1])<0.01){
        	t++;
        	setGoal[0] = V[t].first;
        	setGoal[1] = V[t].second;
        }
    }
    
    vel_msg.linear.x = 0.0f;
    vel_msg.linear.y = 0.0f;
	vel_publish.publish(vel_msg);
	imshow("TEST IMAGE", image);
	waitKey(0);
    return 0;
}


