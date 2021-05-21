#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

struct terminal
{
	int socket;
	thread th;
};

vector<terminal> clients(17);
int seed=0;
int p,r;

void winRound(int cards[], int win[]);
int Winner(int win[], int arr[]);
void random(int cards[], int p);
void Play(int i, int cards[], int gameCards[]);

int main()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
     
    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
      cout<<"Error in opening socket"<<endl;
      exit(-1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    portno = 8080;
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        cout<<"Error in binding"<<endl;
        exit(-1);
    }
    listen(sockfd,SOMAXCONN);

    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0){
        cout<<"Error in accepting GameSetter."<<endl;
        exit(-1);
    }
    read(newsockfd, &p, sizeof(p));
    read(newsockfd, &r, sizeof(r));
    cout<<"\n\t  ****** Welcome to The Birmingham Gamblers ******   "<<endl;
    cout<<"The number of players is "<<p<<"."<<endl;
    cout<<"The number of rounds is "<<r<<"."<<endl;
    close(newsockfd);

    struct sockaddr_in clientAdd;
	socklen_t len=sizeof(sockaddr_in);// typedef unsigned int socklen_t

    int client;

    //Accepting all the clients here, first.
    while(seed<p)
    {
        client=accept(sockfd,(struct sockaddr *)&clientAdd, &len);
        if(client==-1)
        {
            cout<<"Error in accepting Client socket."<<endl;
            exit(-1);
        }
        cout<<"Player "<<seed+1<<" has joined."<<endl;
        write(client, &r, sizeof(r));
        write(client, &p, sizeof(p));
        char welcome[100];
        memset(welcome, 0, 100);
        string s = "You are Player " + to_string(seed+1) + ".";
        int i;
        for(i=0;i<s.size();i++){
            welcome[i] = s[i];
        }
        welcome[i] = '\0';
        write(client, &welcome, sizeof(welcome));
        clients[seed].socket=client;
        seed++;
    }
    cout<<endl;
    //Game begins from here.
    int q=0;
    int *cards, *gameCards;
    cards = new int[3*p];
    gameCards = new int[p];
    int win[p];
    for(int h=0;h<p;h++)
        win[h]=0;
    while(q<r){
        random(cards, p);
        for(int i=0;i<seed;i++){
            thread t(Play, i, cards, gameCards);
            clients[i].th=(move(t));
        }
        for(int i=0; i<clients.size(); i++)
        {
            if(clients[i].th.joinable())
                clients[i].th.join();
        }
        cout<<"Round "<<q+1<<" results."<<endl;
        winRound(gameCards, win);
        q++;
    }

    int arr[17];
    int k=Winner(win, arr);
    cout<<"\n\t*** Player Points ***"<<endl;
    for(int i=0;i<p;i++){
        cout<<"Player "<<i+1<<": "<<win[i]<<endl;
    }
    if(k==1){
        char str[100];
        memset(str, 0, 100);
        int h;
        cout<<endl;
        string s = "The Winner of Game is Player " + to_string(arr[0]) +".";
        cout<<s<<endl;
        for(h=0;h<s.size();h++){
            str[h] = s[h];
        }
        str[h] = '\0';
        for(int i=0;i<p;i++){
            if((i+1) != arr[0])
                write(clients[i].socket, &str, sizeof(str));
            else{
                char str1[100];
                memset(str1, 0, 100);
                string s1 = "Congrats!!! You Won.";
                for(h=0;h<s1.size();h++){
                    str1[h] = s1[h];
                }
                str1[h] = '\0';
                write(clients[i].socket, &str1, sizeof(str1));
            }
        }                  
    }
    else{
        string s="There was a draw. The winners are Players ";
        for(int i=0;i<k-1;i++)
            s = s + to_string(arr[i]) + ", ";
        s = s + "and " + to_string(arr[k-1]);
        cout<<s<<endl;
        char str[100];
        memset(str, 0, 100);
        int h;
        for(h=0;h<s.size();h++){
            str[h] = s[h];
        }
        str[h] = '\0';
        for(int i=0;i<p;i++){
            for(int j=0;j<k;j++){
                if((i+1)==arr[j]){
                    char str1[100];
                    memset(str1, 0, 100);
                    string s1 = "Congrats!!! You Won.";
                    for(h=0;h<s1.size();h++){
                        str1[h] = s1[h];
                    }
                    str1[h] = '\0';
                    write(clients[i].socket, &str1, sizeof(str1));
                }
                else
                    write(clients[i].socket, &str, sizeof(str));
            }
        }
    }
    for(int i=0;i<clients.size();i++){
        close(clients[i].socket);
    }
    close(sockfd);
    return 0;
}

void winRound(int cards[], int win[]){
    int max = *max_element(cards, (cards+p));
    for(int i=0;i<p;i++){
        if(max==cards[i]){
            win[i]++;
            cout<<"Player "<<i+1<<" won this round. He got 1 point."<<endl;
        }
    }
    cout<<endl;
}

int Winner(int win[], int arr[]){
    int max=*max_element(win, win+p);
    int k=0;
    for(int i=0;i<p;i++){
        if(max==win[i]){
            arr[k]=i+1;
            k++;
        }
    }
    return k;
}

void random(int cards[], int p){
    int index = 0;
	int n;
    for (int i = 0; i < 3*p; i++){	
		do{
            srand(time(0));
			n = rand()%52 + 1;
			for( int j = 0; j < index; j++ ){
				if( n == cards[j] ){
					n = 0;
					break;
				}
			}
		}while( n == 0 );
        
        cards[index] = n;
        index++;
    }
}

void Play(int i, int cards[], int gameCards[]){

    int pack[3];
    int k=0;
    for(int j=3*i;j<3*(i+1);j++){
        pack[k]=*(cards + j);
        k++;
    }
    write(clients[i].socket, &pack, sizeof(pack));
    int recv_card;
    read(clients[i].socket, &recv_card, sizeof(recv_card));
    gameCards[i] = recv_card;
}