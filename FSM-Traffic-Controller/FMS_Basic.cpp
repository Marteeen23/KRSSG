#include<bits/stdc++.h>
using namespace std;

enum TrafficStates{
	STATE_0, STATE_1, STATE_2, STATE_3, STATE_4, STATE_5, STATE_6, STATE_7, STATE_8, STATE_9, STATE_10, 
	STATE_11, STATE_12, STATE_13, STATE_14, STATE_15, STATE_16, STATE_17, STATE_18, STATE_19, STATE_20 
};

void transition(TrafficStates _state, int cars[]);
void traffic(TrafficStates _state, int arr[]);
int max_(int a, int b, int c);
int tot(int a[]);

int main(){
	int cars[8]={0,0,0,0,0,0,0,0};
	TrafficStates _state=STATE_0;
	int t=1;
	int n;
	cout<<"Enter number of timesteps in which car will come: ";
	cin>>n;
	cout<<endl;
	int j=0;
	do{
		for(int i=0;i<8 && j<n;i++){
			int k;
			cin>>k;
			cars[i]+=k;
		}
		j++;
		cout<<"Timestep: "<<t<<endl;
		t++;
		cout<<"Initial Queue: ";
		for(int i=0;i<8;i++){
			cout<<cars[i]<<" ";
		}
		cout<<endl;
		transition(_state, cars);
		cout<<"Final Queue: ";
		for(int i=0;i<8;i++){
			cout<<cars[i]<<" ";
		}
		cout<<endl;
		cout<<endl;
	}while(tot(cars)>0 || j<n);
}

void transition(TrafficStates _state, int cars[]){
	if((max_element(cars, cars+8)-cars)==0){
		if(cars[1]==0 && cars[2]==0 && cars[7]==0){
			_state=STATE_13;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[2],cars[7])==cars[1]){
			_state=STATE_1;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[2],cars[7])==cars[2]){
			_state=STATE_2;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[2],cars[7])==cars[7]){
			_state=STATE_3;
			traffic(_state, cars);
		}
	}
	else if((max_element(cars, cars+8)-cars)==1){
		if(cars[0]==0 && cars[4]==0 && cars[3]==0){
			_state=STATE_14;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[4],cars[3])==cars[0]){
			_state=STATE_1;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[4],cars[3])==cars[4]){
			_state=STATE_4;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[4],cars[3])==cars[3]){
			_state=STATE_5;
			traffic(_state, cars);
		}
	}
	else if((max_element(cars, cars+8)-cars)==2){
		if(cars[0]==0 && cars[5]==0 && cars[3]==0){
			_state=STATE_15;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[5],cars[3])==cars[0]){
			_state=STATE_2;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[5],cars[3])==cars[5]){
			_state=STATE_6;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[5],cars[3])==cars[3]){
			_state=STATE_7;
			traffic(_state, cars);
		}
	}
	else if((max_element(cars, cars+8)-cars)==3){
		if(cars[1]==0 && cars[2]==0 && cars[6]==0){
			_state=STATE_16;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[2],cars[6])==cars[1]){
			_state=STATE_5;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[2],cars[6])==cars[2]){
			_state=STATE_7;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[2],cars[6])==cars[6]){
			_state=STATE_8;
			traffic(_state, cars);
		}
	}
	else if((max_element(cars, cars+8)-cars)==4){
		if(cars[1]==0 && cars[5]==0 && cars[6]==0){
			_state=STATE_17;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[5],cars[6])==cars[1]){
			_state=STATE_4;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[5],cars[6])==cars[5]){
			_state=STATE_9;
			traffic(_state, cars);
		}
		else if(max_(cars[1],cars[5],cars[6])==cars[6]){
			_state=STATE_10;
			traffic(_state, cars);
		}
	}
	else if((max_element(cars, cars+8)-cars)==5){
		if(cars[2]==0 && cars[4]==0 && cars[7]==0){
			_state=STATE_18;
			traffic(_state, cars);
		}
		else if(max_(cars[2],cars[4],cars[7])==cars[2]){
			_state=STATE_6;
			traffic(_state, cars);
		}
		else if(max_(cars[2],cars[4],cars[7])==cars[4]){
			_state=STATE_9;
			traffic(_state, cars);
		}
		else if(max_(cars[2],cars[4],cars[7])==cars[7]){
			_state=STATE_11;
			traffic(_state, cars);
		}
	}
	else if((max_element(cars, cars+8)-cars)==6){
		if(cars[3]==0 && cars[4]==0 && cars[7]==0){
			_state=STATE_19;
			traffic(_state, cars);
		}
		else if(max_(cars[3],cars[4],cars[7])==cars[3]){
			_state=STATE_8;
			traffic(_state, cars);
		}
		else if(max_(cars[3],cars[4],cars[7])==cars[4]){
			_state=STATE_10;
			traffic(_state, cars);
		}
		else if(max_(cars[3],cars[4],cars[7])==cars[7]){
			_state=STATE_12;
			traffic(_state, cars);;
		}
	}
	else if((max_element(cars, cars+8)-cars)==7){
		if(cars[0]==0 && cars[5]==0 && cars[6]==0){
			_state=STATE_20;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[5],cars[6])==cars[0]){
			_state=STATE_3;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[5],cars[6])==cars[5]){
			_state=STATE_11;
			traffic(_state, cars);
		}
		else if(max_(cars[0],cars[5],cars[6])==cars[6]){
			_state=STATE_12;
			traffic(_state, cars);
		}
	}
}


void traffic(TrafficStates _state, int arr[]){
	TrafficStates state = _state;

	switch(state){
		case STATE_0:
			break;

		case STATE_1:
            {arr[0]--;
            arr[1]--;
            cout<<"A: Go Straight, Go Right"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_2:
           {arr[0]--;
            arr[2]--;
            cout<<"A: Go Straight"<<endl;
            cout<<"B: Go Straight"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_3:
            {arr[0]--;
            arr[7]--;
            cout<<"A: Go Straight"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Go Right"<<endl;
			break;}

		case STATE_4:
            {arr[1]--;
            arr[4]--;
            cout<<"A: Go Right"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Straight"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_5:
            {arr[1]--;
            arr[3]--;
            cout<<"A: Go Right"<<endl;
            cout<<"B: Go Right"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_6:
            {arr[2]--;
            arr[5]--;
            cout<<"A: Go Straight"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Right"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_7:
           {arr[2]--;
            arr[3]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Go Straight, Go Right"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_8:
            {arr[3]--;
            arr[6]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Go Right"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Go Straight"<<endl;
			break;}

		case STATE_9:
            {arr[4]--;
            arr[5]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Straight, Go Right"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_10:
            {arr[4]--;
            arr[6]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Straight"<<endl;
            cout<<"D: Go Straight"<<endl;
			break;}

		case STATE_11:
            {arr[5]--;
            arr[7]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Right"<<endl;
            cout<<"D: Go Right"<<endl;
			break;}

		case STATE_12:
            {arr[6]--;
            arr[7]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Go Straight, Go Right"<<endl;
			break;}

		case STATE_13:
		    {arr[0]--;
            cout<<"A: Go Straight"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_14:
		    {arr[1]--;
            cout<<"A: Go Right"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_15:
		    {arr[2]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Go Straight"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_16:
		    {arr[3]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Go Right"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_17:
		    {arr[4]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Straight"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_18:
		    {arr[5]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Go Right"<<endl;
            cout<<"D: Stop"<<endl;
			break;}

		case STATE_19:
		    {arr[6]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Go Straight"<<endl;
			break;}

		case STATE_20:
		    {arr[7]--;
            cout<<"A: Stop"<<endl;
            cout<<"B: Stop"<<endl;
            cout<<"C: Stop"<<endl;
            cout<<"D: Go Right"<<endl;
			break;}

		default:
			cout<<"Error in States."<<endl;
			break;
	}
}

int tot(int a[]){
	int sum=0;
	for(int i=0;i<8;i++){
		sum=sum+a[i];
	}
	return sum;
}

int max_(int a, int b, int c){
	if(a>b && a>c) {
		return a;
	} 
	else if(b>a && b>c) {
		return b;
	}
	else {
		return c;
	}
}