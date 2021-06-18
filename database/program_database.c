#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    unsigned int key;
    char fname[100];
    char lname[100];
    unsigned int age;
} person_rec;

int open_record(char *filename){
    int fd;

    fd = open(filename, O_CREAT | O_RDWR | O_APPEND, 0644);
    if(fd == -1)
        perror("open_record");

    return fd;
}

void close_record(int fd){
    close(fd);
}

int insert_record(int fd, person_rec *rec){
    int ret;

    ret = write(fd, rec, sizeof(person_rec));
    return ret;
}

int get_record(int fd, person_rec *rec, int key){
    int ret;
    while( ret = read(fd, read, sizeof(person_rec)) != -1 ){
        // if u reading the file 
        if(ret==0){
            memset(rec, 0, sizeof(person_rec));
            break;
            return ret;
        }
        //if we read and there's smth
        else if(key == rec->key) return ret;
    }
    //to clear the record to 0
    memset(rec, 0, sizeof(person_rec));
    return ret;
}

int delete_record(int fd, int key){
    int ret;
    person_rec rec;

    off_t pos;
    pos = lseek(fd, 0, SEEK_SET);

    while( ret = read(fd, &rec, sizeof(person_rec)) != -1 ){
        if(ret==0){
            return ret;
        }
        else if(key == rec.key){
            lseek(fd, pos, SEEK_SET);
            rec.key = 0;
            ret = write(fd, &rec, sizeof(person_rec));

            return ret;
        }
        pos = lseek(fd, 0, SEEK_CUR); 
    }
    return ret;
}

int main(){

    // ---------------------------------- Coba socket ?
  int welcomeSocket, newSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

  /*---- Send message to the socket of the incoming connection ----*/
  strcpy(buffer,"Hello World\n");
  send(newSocket,buffer,13,0);
    
  
// ----------------------------------------------------- Database ? ://    
    int fd;
   // char chr(argv[2]);
    //person_rec rec_ret;
    person_rec rec;

    fd = open_record("data1");

    if(argc > 1){
        /* Insert */

        if(argc > 5 && !strcmp(argv[1], "insert")){
            rec.key =atoi(argv[2]);
            strcpy(rec.fname, argv[3]);
            strcpy(rec.lname, argv[4]);
            rec.age = atoi(argv[5]);
            insert_record(fd, &rec);
        }

        /* Delete */

        if(argc > 2 && !strcmp(argv[1], "delete"))
            delete_record(fd, atoi(argv[2]));

        /* Print */

        if(argc > 2 && !strcmp(argv[1], "print")){
            get_record(fd, &rec, atoi(argv[2]));

            printf("key = %d\n", rec.key);
            printf("First = %s\n", rec.fname);
            printf("Last = %s\n", rec.lname);
            printf("Age = %d\n", rec.age);
        }
    }
    close_record(fd);
    return 0;
}
