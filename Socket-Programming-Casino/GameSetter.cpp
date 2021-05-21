#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int main(){
    int sockfd, portNo;
    struct sockaddr_in serv_addr;

    portNo = 8080;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cout<<"Error opening socket"<<endl;
        exit(-1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNo);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        cout<<"Error connecting"<<endl;
        exit(-1);
    }
    cout<<"Welcome Tommy Shelby!! You are the Game Setter of The Birmingham Gamblers."<<endl;
    int p,r;
    cout<<"Enter number of players: ";
    cin>>p;
    while(p>17){
        cout<<"Please make sure to keep number of players less than or equal to 17."<<endl;
        cout<<"Enter number of players again: ";
        cin>>p;
    }
    cout<<"Enter number of rounds: ";
    cin>>r;
    while(r%p==0){
        cout<<"Please make sure that number of rounds is not divisible by number of players."<<endl;
        cout<<"Enter number of rounds again: ";
        cin>>r;
    }
    write(sockfd,&p,sizeof(p));
    write(sockfd,&r,sizeof(r));
    close(sockfd);
    return 0;
}