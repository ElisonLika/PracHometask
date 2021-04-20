#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstring>
#include <netinet/in.h>
#include <memory.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <signal.h>
#define PORT 8888
#define BUFLEN 1024

using namespace std;

int SocDesk;

void SigHndlr(int s){
    cout<<endl<<"Ending"<<endl;
    if (shutdown(SocDesk, 1) < 0) {
        perror("Error calling shutdown");
        exit(1);
    }
    close (SocDesk);
    signal(SIGINT,SIG_DFL); 
}

class Server {
    int ServerSocket;
    socklen_t ServerLen;
    sockaddr_in ServerAddress;
    int port;
public:
    Server (int port = PORT) {
        if ((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("Can't create socket");
            exit(1);
        }
        ServerAddress.sin_family = AF_INET;
        ServerAddress.sin_port = htons(PORT);
        ServerAddress.sin_addr.s_addr = inet_addr ("127.0.0.1");
        this->port = port;
        ServerLen = sizeof(ServerAddress);
    }
    void set_port (int port) {
        ServerAddress.sin_port = htons(port);
    }
    int get_port () const {
        return port;
    }
    int get_socket () const {
        return ServerSocket;
    }
    void socket_bind () const {
        if (bind(ServerSocket, (struct sockaddr*) &ServerAddress, ServerLen) < 0) {
            perror("Can't bind socket!");
            exit(1);
        }
        else cout << "Bind is OK" << endl;
    }
    void socket_listen () const {
        if ((listen(ServerSocket, 5)) < 0) {
            perror("Can't listen socket!");
            exit(1);
        }
        else cout << "Listeting completed" << endl;
    }
};

class Client {
    sockaddr_in ClientAddress;
    int ClientSocket;
    socklen_t ClientAddrLen;
public:
    Client () {
        ClientAddrLen = sizeof(ClientAddress);
    }
    int socket_accept (int ServerSocket) {
        ClientSocket = accept(ServerSocket, (struct sockaddr*) &ClientAddress, &ClientAddrLen);
        if (ClientSocket < 0) {
            perror("Can't accept");
            exit(1);
        }
        else cout << "Accept is OK" << endl;
        return ClientSocket;
    }
    int get_client () const {
        return ClientSocket;
    }
    void ProcessClientRequest(int ClientSocket) {
        char buff[BUFLEN];
        memset(buff,0,BUFLEN);
        int rc = recv(ClientSocket, buff, BUFLEN, 0);
        if (rc < 0) {
            if (shutdown(ClientSocket, 1) < 0) {
                perror("Error calling shutdown");
                exit(1);
            }
            close (ClientSocket);
            perror("Can't receive data");
            exit(1);
        }
        cout << "Request to the server : " << endl;
        cout << buff;     
        
        int n = strlen(buff);
        char met[n], file[n], oth[n], cgi[n];
        memset(met,0,n);memset(file,0,n);memset(oth,0,n);memset(cgi,0,n);
        int i = 0, beg, fl = 0;
        while((buff[i]!=' ')&&(buff[i]!='\n')&&(i<=n)){ 
            met[i] = buff[i];
            i++;
        }
        met[i]='\0';
        i++;
        beg=i;
        if(!strncmp(buff+i,"/cgi-bin",7)){
            while((buff[i]!=' ')&&(buff[i]!='\n')&&(buff[i]!='?')&&(i<=n)){ 
                cgi[i-beg] = buff[i];
                i++;
            }
            cgi[i-beg]='\0';
            i++;
            beg=i;
            fl=1;
        }
        else {
            i++; beg++;
            while((buff[i]!=' ')&&(buff[i]!='\n')&&(i<=n)){ 
                file[i-beg] = buff[i];
                i++;
            }
            file[i-beg]='\0';
            i++;
            beg=i;
        }
        while((buff[i]!='\n')&&(i<=(n))){ 
            oth[i-beg] = buff[i];
            i++;
        }
        oth[i-beg]='\0';

        FILE *fd;
        struct tm *Date,*Last;
        long timeT,timeL;
        char DateS[50], LastS[60], type[30]; 
        char buf[BUFLEN], http[BUFLEN];
        struct stat stbuf;
        int l = 0, len, flag = strcmp(met,"GET\0");

        char cod[5], comm[22];

        if (flag && strcmp("HEAD\0",met)){
            strcpy(cod,"501");
            strcpy(comm,"Not Implemented");
            sprintf(buf,"%s %s", cod, comm);
            len=strlen(buf);
        }
        /*else if (!fl && strcmp(oth,"HTTP/1.1")){
            strcpy(cod,"400");
            strcpy(comm,"Bad request");
            sprintf(buf,"%s %s", cod, comm);
            len=strlen(buf);
        }*/
        else if (fl){
            strcpy(cod,"200");
            strcpy(comm,"It's CGI");
            sprintf(buf,"%s %s", cod, comm);
            len=strlen(buf);
        }
        else if ((stat(file,&stbuf))==-1){
            strcpy(cod,"404");
            strcpy(comm,"Not found file");
            sprintf(buf,"%s %s: %s", cod, comm, file);
            len=strlen(buf);
        }
        /*else if ((stbuf.st_mode & S_IREAD)){
            strcpy(cod,"403\0");
            strcpy(comm,"Forbidden\0");
            sprintf(buf,"%s %s", cod, comm);
            len=strlen(buf);
        }*/
        else{
            strcpy(cod,"200");
            strcpy(comm,"Ok");
            fd=fopen(file,"rb");
            len = stbuf.st_size / sizeof(char);
            if (!flag){
                timeL=stbuf.st_mtime;
                Last=gmtime(&timeL);
                strftime(LastS,50,"Last modified: %a, %d %b %Y %H:%M:%S GMT",Last);
            }

        }
        time(&timeT);
        Date=gmtime(&timeT);
        strftime(DateS,50,"Date: %a, %d %b %Y %H:%M:%S GMT",Date);
        strcpy(type,"text/html");
        if (!flag){
            if((strstr(file,"jpg"))!=NULL) strcpy(type,"image/jpeg");
        }     
        
        sprintf(http,"HTTP/1.1\n%s %s\n%s\nHost: 127.0.0.1:%d\nServer: Model Server/0.1\nAllow: GET,HEAD\n",cod,comm,DateS,PORT);
        char str1[BUFLEN];
        if(!flag)
        {
            sprintf(str1,"Content Type: %s\nContent Lenght: %d\n",type, len);
            strcat(http,str1);
            if(!strcmp(cod,"200"))
            {
                sprintf(str1,"%s\n",LastS);
                strcat(http,str1); 
            }
        }
        send(ClientSocket,&http,strlen(http),0);
        cout << http << endl;

        if(!flag)
        {
            if(strcmp(cod,"200")) {
                //fd = fopen ("Error.txt", "rb");
                //stat(file,&stbuf);
                //len = stbuf.st_size / sizeof(char);
                send(ClientSocket,&buf,len,0);
            }
            else 
                    while ((l = fread(buf, sizeof(char), BUFLEN, fd)) > 0) {
                        send(ClientSocket, &buf, l, 0);
                        //fclose(fd);
                    }
        }


        if (shutdown(ClientSocket, 1) < 0) {
            perror("Error calling shutdown");
            exit(1);
        }
        close (ClientSocket);
    }
};

int main()
{
    Server server(8900);
    server.socket_bind();
    server.socket_listen();
    Client client;
    signal(SIGINT,SigHndlr);
    while (1) {    
        int fd = client.socket_accept(server.get_socket());
        SocDesk = client.get_client();
        client.ProcessClientRequest(fd);
    }
    return 0;
}