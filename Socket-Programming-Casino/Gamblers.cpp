#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int _round(int cards[], int card[]);

int main()
{    
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    int i, temp;

    portno = 8080;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        cout<<"Error opening socket"<<endl;
        exit(-1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout<<"Error connecting"<<endl;
        exit(-1);
    }
    cout<<"\n\t  ****** Welcome to The Birmingham Gamblers ******   "<<endl<<endl;
    int p, r;
    read(sockfd, &r, sizeof(r));
    read(sockfd, &p, sizeof(p));
    char welcome[100];
    read(sockfd, &welcome, sizeof(welcome));
    cout<<welcome<<endl;
    cout<<"There will be "<<r<<" rounds."<<endl;
    cout<<"You have "<<p-1<<" opponents."<<endl<<endl;
    
    int q=0;
    while(q<r){
        int a[3], b[3];
        read(sockfd, &a, sizeof(a));
        cout<<"Your Cards are: ";
        for(int i=0;i<3;i++){
            cout<<a[i]<<" ";
        }
        cout<<endl;
        int k = _round(a, b);
        cout<<"Your chosen card is: "<<a[k]<<endl<<endl;
        send(sockfd, &b[k], sizeof(b[k]), 0);
        q++;
    }
    char str[100];
    memset ( str, 0, 100 );
    read(sockfd, &str, sizeof(str));
    cout<<str<<endl;
    close(sockfd);
}

int _round(int cards[], int card[])
{
    for(int i=0;i<3;i++){
        card[i]=cards[i]%13;
        if(card[i]==0){
            card[i]=13;
        }
    }
	int k=(max_element(card, card+3)-card);
    return k;
}