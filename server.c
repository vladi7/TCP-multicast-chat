/*-------------------------------------------------------------*/
/* server.c - Vladislav Dubrovenski                            */
/*-------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "inet.h"
#include <stdlib.h>
#include <unistd.h>

#define MAX 1000

/*
 * Read up to "n" bytes from a descriptor.
 * Use in place of read() when fd is a stream socket.
 */

int
readn(fd, ptr, nbytes)
register int    fd;
register char   *ptr;
register int    nbytes;
{
    int nleft, nread;

    nleft = nbytes;
    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread < 0)
            return(nread);      /* error, return < 0 */
        else if (nread == 0)
            break;          /* EOF */

        nleft -= nread;
        ptr   += nread;
    }
    return(nbytes - nleft);     /* return >= 0 */
}


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/* Display menu and retrieve user's response */
int get_response()
{       
    int choice;
    printf("-------------------------------------------\n");
    printf("                1. Register a chat room(IP port topic)\n");
    printf("                2. Get the list of active chat rooms\n");
    printf("                3. Deregister the room\n");
    printf("-------------------------------------------\n");
    printf("               Choice (1-3), 4 to start Chat Directory Service:");

    char term;
    if( scanf("%d%c",&choice,&term)!=2||choice<1 || choice > 3|| term!='\n'){//protection against invalid input 
        return -1;
    }
    printf("===========================================\n");
    return(choice);

}


void appendToFile(){
    FILE *fp;
    char buf[MAX];
    
    char *p;
    printf("Enter \"IP port TopicName\"\n");

          if (fgets(buf, sizeof(buf), stdin) != NULL)
     {
         if ((p = strchr(buf, '\n')) != NULL)
                    *p = '\0';
            printf ("%s\n", buf);
       }


          fp = fopen("database.txt","a+");
              if (fp == NULL)
                  {
          printf("Error opening file!\n");
                exit(1);
                          }
              fprintf(fp, "%s\n", buf);

                  fclose(fp);
}

void appendToFileFromServer(char *p){
    FILE *fp;
    char buf[MAX];
                snprintf(buf,MAX,"%s",p);

          fp = fopen("database.txt","a+");
              if (fp == NULL)
                  {
          printf("Error opening file!\n");
                exit(1);
                          }
              fprintf(fp, "%s\n", buf);

                  fclose(fp);
}
void readFromFile(){
        FILE* file = fopen("database.txt", "r"); /* should check the result */
            char line[256];

            while (fgets(line, sizeof(line), file)) {
                        printf("%s", line);
                    printf("***********************\n");
                        }
                fclose(file);

}
void deleteFromFile(){
             FILE* file = fopen("database.txt", "r"); /* should check the result */
            char line[256];
            char buf[MAX];
            char resultBuf[MAX];
                char *p;

             printf("Enter Topic Name to delete:\n");

          if (fgets(buf, sizeof(buf), stdin) != NULL)
              {
         if ((p = strchr(buf, '\n')) != NULL)
                    *p = '\0';
            printf ("%s\n", buf);
                  }

            while (fgets(line, sizeof(line), file)) {
                        printf("%s", line);
                        if (strstr(line, buf) != NULL) {
                            continue ;                       }
                        else{
                            strcat(resultBuf, line);
                        }
                        }
                fclose(file);
        FILE* newTxtFile=NULL;

        newTxtFile=fopen("database.txt", "w+");
              fprintf(newTxtFile, "%s\n", resultBuf);
    
            }


int main(int argc, char **argv)
{
    int                 sockfd, newsockfd, childpid;
    unsigned int	clilen;
    struct sockaddr_in  cli_addr, serv_addr;
    struct tm           *timeptr;  /* pointer to time structure */
    time_t              clock;     /* clock value (in secs)     */
    char                s[MAX];
    char                s1[MAX];
    char                request;
    int                 nread;       /* number of characters */

    int                 response;


    while(1){
    if((response = get_response()) == -1){
        printf("\nStarting the server\n");
            break;
    }
            if(response ==1){appendToFile();}
            else if(response==2){readFromFile();}
            else if(response==3){deleteFromFile();}
}    


	srand((unsigned int)time(NULL)); 

    /* Create communication endpoint */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: can't open stream socket");
        exit(1);
    }

    /* Bind socket to local address */
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port        = htons(SERV_TCP_PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("server: can't bind local address");
        exit(1);
    }

    listen(sockfd, 5);

    for ( ; ; ) {
        /* Accept a new connection request. */
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("server: accept error");
            exit(1);
        }

        nread = readn (newsockfd, s1, MAX);


        if(strstr(s1, "client") != NULL){
              FILE* file = fopen("database.txt", "r"); /* should check the result */
            char line[100];
            char resultBuf[MAX];
            while (fgets(line, sizeof(line), file)) {
                            strcat(resultBuf, line);
                        }
                fclose(file);
            write (newsockfd, resultBuf, MAX);
        }



        printf ("%s\n", s1);
        appendToFileFromServer(s1);

	   close(newsockfd);     
    }
}
