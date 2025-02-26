#include "kernel/types.h"
#include "user/user.h"

#define stdout 1
#define stderr 2

#define READ 0
#define WRITE 1

int main(){
    int fdp2c[2] , fdc2p[2], pid;
    char p2c[64], c2p[64];
    
    pipe(fdp2c);
    pipe(fdc2p);

    if((pid=fork())<0){
        fprintf(stderr,"fork error...\n");
        exit(1);
    }else if(pid==0){ // child process,
        close(fdp2c[WRITE]);
        close(fdc2p[READ]);
        
        read(fdp2c[READ],p2c,sizeof(p2c)); //write the byte on the pipe to the parent, and exit; the parent should read the byte from the child, print "<pid>: received pong
        close(fdp2c[READ]);
        
        printf("%d: received %s\n",getpid(),p2c); 
        write(fdc2p[WRITE],"pong",4);// The parent should send a byte to the child; the child should print "<pid>: received ping"
        close(fdc2p[WRITE]);
        exit(0);
    }else{ // parent process
        close(fdc2p[WRITE]);
        close(fdp2c[READ]);

        write(fdp2c[WRITE],"ping",4); // The parent should send a byte to the child; the child should print "<pid>: received ping"
        close(fdp2c[WRITE]);

        read(fdc2p[READ],c2p,sizeof(c2p)); //write the byte on the pipe to the parent, and exit; the parent should read the byte from the child, print "<pid>: received pong
        close(fdc2p[READ]);

        printf("%d: received %s\n",getpid(),c2p);

        exit(0);
    }


}