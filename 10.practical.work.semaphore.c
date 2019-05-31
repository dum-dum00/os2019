# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>

# define buffer_size 2

typedef struct {
    char *name; // package name
    char *version; // version of package
    char *author; // package creator
} pkg;

pkg buffer[buffer_size]; 
int first = 0;
int last = 0;
sem_t sem;

void produce (pkg *i);
pkg receive (void);
pkg pack (char *name, char *version, char *author);
void garbage (pkg *box);

void *factory_thread (void *arg);
void *user_thread (void *arg);

// producing
void produce (pkg *i) {
    while ((first + 1) % buffer_size == last) {
        printf("no free queue");
    }
    
    sem_wait(&sem);
    memcpy(&buffer[first], i, sizeof(pkg));
    first = (first + 1) % buffer_size;
    sem_post(&sem);
    printf("Log of send package: \nFirst: %d\nLast: %d\n",first,last);
}

// receiving
pkg receive (void) {
    pkg i;
    while (first == last) {
        ; // wait for package put to queue
    }

    sem_wait(&sem);
    memcpy(&i, &buffer[last], sizeof(pkg));
    last = (last + 1) % buffer_size;
    sem_post(&sem);
    printf("Log of recieve package: \nFirst: %d\nLast: %d\n",first,last);
    return i;
}

// send to user
pkg pack (char *name, char *version, char *author) {
    pkg box;
    box.name = name;
    box.version = version;
    box.author = author;
    return box;
}

// factory thread produce package
void *factory_thread (void *arg) {
    pkg sbox1 = pack("ls","1.0","anonymous");
    pkg sbox2 = pack("bc","2.1","anonymous");
    pkg sbox3 = pack("ls","beta","anonymous");
    produce(&sbox1);
    produce(&sbox2);
    produce(&sbox3);
    return NULL;
}

// user thread receive package
void *user_thread (void *arg) {
    for (int i =0; i<2; i++) {
        pkg rbox = receive();
        printf("\nthe package number %d:\n",i+1);
        printf ("name: %s\n",rbox.name);
        printf ("version: %s\n",rbox.version);
        printf ("author: %s\n",rbox.author);
    }
    return NULL;
}

int main(){
    sem_init(&sem,0,1);

    pthread_t factory_addr;
    pthread_t user_addr;
    pthread_create (&factory_addr, NULL, factory_thread, NULL);
    pthread_create (&user_addr, NULL, user_thread, NULL);
    pthread_join(factory_addr,NULL);
    pthread_join(user_addr,NULL);

    sem_destroy(&sem);
    
    return 0;
}