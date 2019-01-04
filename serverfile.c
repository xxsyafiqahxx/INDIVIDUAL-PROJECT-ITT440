#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <math.h>

#define PORT 20000 
#define BACKLOG 5
#define LENGTH 512 

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

long int multiplyNumbers(int z);

long int multiplyNumbers(int z)
{
    if (z >= 1)
        return z*multiplyNumbers(z-1);
    else
        return 1;
}


int main ()
{
	/* Defining Variables */
	int sockfd; 
	int nsockfd; 
	int num;
	int sin_size; 
	time_t waktu;
	waktu = time(NULL);
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
	char revbuf[LENGTH]; // Receiver buffer
	int i, n, t1 = 0, t2 = 1, nextTerm;
	int space, x,y, star=0;
	int z;
	int f, j, k;
	
     
      
      
	/* Get the Socket file descriptor */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
		exit(1);
	}
	else 
		printf("[Server] Obtaining socket descriptor successfully.\n");

	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET; // Protocol Family
	addr_local.sin_port = htons(PORT); // Port number
	addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
	bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

	/* Bind a special Port */
	if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
		exit(1);
	}
	else 
		printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

	/* Listen remote connect/calling */
	if(listen(sockfd,BACKLOG) == -1)
	{
		fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
		exit(1);
	}
	else
		printf ("[Server] Listening the port %d successfully.\n", PORT);
	
	int ii=0;
	printf ("[Server] Time connected %s\n", ctime(&waktu));

	int success = 0;
	while(success == 0)
	{
		sin_size = sizeof(struct sockaddr_in);

		/* Wait a connection, and obtain a new socket file despriptor for single connection */
		if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		{
		    fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else 
			printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));
		
		
		
		read(nsockfd,&ii,sizeof(ii));	
		z=ntohl(ii);
		fprintf(stdout,"\n");
		fprintf(stdout,"%d\n",z);

  	  
	


		printf("Factorial of %d = %ld", z, multiplyNumbers(z));
		

		read(nsockfd,&y,sizeof(y));	
		k=ntohl(y);
		fprintf(stdout,"\n");
		fprintf(stdout,"%d\n",k);

		for(f=k/2; f<=k; f+=2)
    {
        for(j=1; j<k-f; j+=2)
        {
            printf(" ");
        }

        for(j=1; j<=f; j++)
        {
            printf("*");
        }

        for(j=1; j<=k-f; j++)
        {
            printf(" ");
        }

        for(j=1; j<=f; j++)
        {
            printf("*");
        }

        printf("\n");
    }

    for(f=k; f>=1; f--)
    {
        for(j=f; j<k; j++)
        {
            printf(" ");
        }

        for(j=1; j<=(f*2)-1; j++)
        {
            printf("*");
        }

        printf("\n");
    }	
		
			
     
		/*Receive File from Client */
		char* fr_name = "receive.txt";
		FILE *fr = fopen(fr_name, "w+");
		if(fr == NULL)
			printf("File %s Cannot be opened file on server.\n", fr_name);
		else
		{
			bzero(revbuf, LENGTH); 
			int fr_block_sz = 0;
			while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
			{
			    int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
				if(write_sz < fr_block_sz)
			    {
			        error("File write failed on server.\n");
			    }
				bzero(revbuf, LENGTH);
				if (fr_block_sz == 0 || fr_block_sz != 512) 
				{
					break;
				}
			}
			if(fr_block_sz < 0)
		    {
		        if (errno == EAGAIN)
	        	{
	                printf("recv() timed out.\n");
	            }
	            else
	            {
	                fprintf(stderr, "recv() failed due to errno = %d\n", errno);
					exit(1);
	            }
        	}
			printf("Ok received from client!\n");
			fclose(fr); 
		}


	FILE *fdr,*fdw;
	char filename[25],ch,sendline[100];
	int op;
	printf("Enter yes[y] or no[n] to decrypt :");
	scanf("%s",filename);
	if ((strcmp(filename,"y")==0)||(strcmp(filename,"Y")==0)){
	fdr=fopen("receive.txt","r");
	fdw=fopen("decrypt.txt","w+");
	ch=fgetc(fdr);
	while(ch!=EOF){
		printf("%c",ch);
		op=(int)ch;
			op=op+5;
			fprintf(fdw,"%c",op);
		ch=fgetc(fdr);	
	}
	fclose(fdw);
	fclose(fdr);
	close(sockfd);}
	else 
		close(sockfd);
	}

}
