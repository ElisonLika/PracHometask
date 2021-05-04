//Оно рабтает!!!!!!!

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
#include <sys/wait.h>
#define PORT 8888
#define BUFLEN 1024

using namespace std;


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
            char *str[]={cgi+9,(char*)0};
            setenv("QUERY_STRING", oth, 0);
            setenv("SCRIPT_NAME", cgi, 0);
            setenv("REMOTE_ADDR", "127.0.0.1",0);
            fd = tmpfile();
            strcpy(cod,"200");
            strcpy(comm,"It's CGI");
            //перенапривить вывод
            int fdin[2], fdout[2];
	
            fdin[0] = fdin[1] = fdout[0] = fdout[1] = -1;
            if (pipe(fdin) != 0 || pipe(fdout) != 0)
            {
                cout << "Cannot create CGI pipe";
                exit(1);
            }

            // Duplicate stdin and stdout file descriptors
            int oldin = dup(fileno(stdin));
            int oldout = dup(fileno(stdout));

            // Duplicate end of pipe to stdout and stdin file descriptors
            if ((dup2(fdout[1], fileno(stdout)) == -1) || (dup2(fdin[0], fileno(stdin)) == -1))
                exit(1);

            // Close original end of pipe
            close(fdin[0]);
            close(fdout[1]);
            pid_t pid;
            pid = fork();
            if(pid > 0){}
            else if (pid < 0){
                perror("Bad pid");
                exit(1);
            }
            else{
                execv(cgi, str);
            }
            // Duplicate copy of original stdin an stdout back into stdout
                dup2(oldin, fileno(stdin));
                dup2(oldout, fileno(stdout));

                // Close duplicate copy of original stdin and stdout
                close(oldin);
                close(oldout);
                // вернуть вывод

            while (1)
                {
                    //Читаем ответ от дочернего процесса
                    char bufferOut[100000];
                    int n = read(fdout[0], bufferOut, 100000);
                    if (n > 0)
                    {
                        //Выводим ответ на экран
                        fwrite(bufferOut, sizeof(char), n, fd);
                    }
                    //Если дочерний процесс завершился, то завершаем и родительский процесс
                    int status;
                    if (waitpid(pid, &status, WNOHANG) > 0) break;
                }
            //len=filesize(fd);
            //sprintf(buf,"%s %s", cod, comm);
            //len=strlen(buf);
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
        
        sprintf(http,"HTTP/1.1\n%s %s\n%s\nServer: Model HTTP Server/0.1\nAllow: GET,HEAD\n",cod,comm,DateS);
        char str1[BUFLEN];
        if(!flag)
        {
            sprintf(str1,"Content Type: %s\nContent Lenght: %d\n",type, len);
            strcat(http,str1);
            if(!strcmp(cod,"200")&&(!fl))
            {
                sprintf(str1,"%s\n\n",LastS);
                strcat(http,str1); 
            }
            else strcat(http,"\n");
        }
        else strcat(http,"\n");

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
            else{
                    while ((l = fread(buf, sizeof(char), BUFLEN, fd)) > 0) {
                        send(ClientSocket, &buf, l, 0);
                    }
                    fclose(fd);
                }

        }


        if (shutdown(ClientSocket, 1) < 0) {
            perror("Error calling shutdown");
            exit(1);
        }
        close (ClientSocket);
    }
};

int main(int argc, char **argv, char** env)
{
    /*fd_set master;    // главный сет дескрипторов
    fd_set read_fds;  // временный сет дескрипторов для select()
    //fd_set write_fds;
    int fdmax;        // макс. число дескрипторов
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int i;
    
    FD_ZERO(&master);    // очищаем оба сета
    FD_ZERO(&read_fds);*/

    Server server(8900);
    server.socket_bind();
    server.socket_listen();
    Client client;
    while (1) {    
         /*       read_fds = master; // копируем его
        //write_fds = master;
        
        if (select(fdmax+1, &read_fds, NULL, NULL, &tv) == -1) {
        perror("Select");
        exit(4);
        }

        
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // есть!
                if (i == server.get_socket()) {
                // обрабатываем новые соединения
                int fd = client.socket_accept(server.get_socket());
                FD_SET(fd, &master); // добавляем в мастер-сет
                if (fd > fdmax) {    // продолжаем отслеживать самый большой номер дескиптора
                    fdmax = fd;
                }
                } else {
                    // обрабатываем данные клиента
                    //if (FD_ISSET(i, &master)) {
                        client.ProcessClientRequest(i);
                        FD_CLR(i, &master); // удаляем из мастер-сета
                    //}
                    } else {
                        // у нас есть какие-то данные от клиента
                        for(j = 0; j <= fdmax; j++) {
                            // отсылаем данные всем!
                            if (FD_ISSET(j, &master)) {
                                // кроме слушающего сокета и клиента, от которого данные пришли
                                if (j != server.get_socket() && j != i) {
                                    if (send(j, buf, nbytes, 0) == —1) {
                                    perror("send");
                                    }
                                }
                            }
                        }
                    } // Закончили обрабатывать данные от клиента
            } // Закончили обрабатывать новое входящее соединение
        } // Закончили цикл по дескрипторам*/

        int fd = client.socket_accept(server.get_socket());
        client.ProcessClientRequest(fd);
    }
    return 0;
}