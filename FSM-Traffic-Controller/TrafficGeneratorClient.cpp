#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

void random0_1(int randArray[]);
int main()
{    
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    int i, temp;

    char buffer[256];
    portno = 10000;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        cout<<"Error opening socket"<<endl;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        cout<<"Error connecting"<<endl;

    int t;
    read(sockfd, &t, sizeof(t));
    for(int i=0;i<t;i++){
        int randArray[8];
        random0_1(randArray);
        cout<<"Sending traffic: ";
        for(int j=0;j<8;j++){
            cout<<randArray[j]<<" ";
        }
        cout<<endl;
        write(sockfd,&randArray,sizeof(randArray));
    }
    close(sockfd);
    return 0;
}

void random0_1(int randArray[])
{
    int index = 0;
	int n;
    for (int i = 0; i < 8; i++)
    {	
		do{
            srand(time(0));
			n = rand()%100;
			for( int j = 0; j < index; j++ ){
				if( n == randArray[j] ){
					n = 0;
					break;
				}
			}
		}while( n == 0 );
        
        randArray[index] = n;
        index++;
    }
    for (int i = 0; i < 8; i++){
        if(randArray[i]<50)
            randArray[i]=0;
        else
            randArray[i]=1;
    }
}