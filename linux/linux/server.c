#include "cs537.h"
#include "request.h"
#include "threads.h"


int pop = 0;
int use = 0;
int poolH;
int bufferH;

int *buffer;

int c = 0;

pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

void put(int val) {
				buffer[pop] = val;
				pop = ( pop + 1) % bufferH;
				c++;
}

int get() {
				int tmp = buffer[use];
				use = (use + 1) % bufferH;
				c--;
				return tmp;
}

void getargs(int *port, int *th,int *bufs,int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *th = atoi(argv[2]);
    *bufs = atoi(argv[3]);
}





void* consumer(void *args){
    while(1){
        int confd;
        Pthread_mutex_lock(&mutex);
        while(c == 0)
            pthread_cond_wait(&fill, &mutex);
        confd=get();
        Pthread_mutex_unlock(&mutex);
        requestHandle(confd);
        Close(confd);
    }
}
void thread_pool(int sz, int bsize){
    int i;
    pthread_t pool[sz];
    for(i=0;i<sz;i++){
        pthread_create(&pool[i],NULL,consumer,NULL);
    }
    poolH = sz;
    bufferH = bsize;
    buffer = (int*)malloc(bsize * sizeof(int));
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, buffers, threads;
    struct sockaddr_in clientaddr;

    getargs(&port, &threads, &buffers, argc, argv);
    thread_pool(threads,buffers);

    listenfd = Open_listenfd(port);

    while (1) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

		Pthread_mutex_lock(&mutex);
		if(c == bufferH){
			Pthread_mutex_unlock(&mutex);
			Close(connfd);
		}
		else {
			put(connfd);
			pthread_cond_signal(&fill);
			Pthread_mutex_unlock(&mutex);
		}
    }

}


    


 
