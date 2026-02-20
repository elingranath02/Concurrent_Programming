#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 20
#define maxSleep 12
#define maxSleepBath 4

sem_t *x, *y, *womenLock, *menLock;
int nrOfWomenInBathroom = 0;
int nrOfMenInBathroom = 0;
int nrOfWomenWaiting = 0;
int nrOfMenWaiting = 0;

const char sem_name_xSem[] = "./xSem";
const char sem_name_ySem[] = "./ySem";
const char sem_name_womenSem[] = "./womenSem";
const char sem_name_menSem[] = "./menSem";

int randomTime(int maxTime) {
    int r = 0;
    while (r == 0) {
        r = rand() % maxTime;
    }

    return r;
}

void enterWoman(int id) {
    printf("Woman %d enters bathroom\n", id);
    sleep(randomTime(maxSleepBath));
    printf("Woman %d leaves bathroom\n", id);
}
void enterMan(int id) {
    printf("Man %d enters bathroom\n", id);
    sleep(randomTime(maxSleepBath));
    printf("Man %d leaves bathroom\n", id);
}

void women(int id) {
    nrOfWomenWaiting++;
    printf("Woman %d placed in queue\n", id);
    if(nrOfWomenWaiting == 1 && nrOfMenInBathroom > 0){
        sem_wait(menLock);
    }
    sem_post(y);
    sem_wait(womenLock);
    nrOfWomenWaiting--;
    if(nrOfWomenInBathroom == 0 && nrOfMenWaiting == 0){
        sem_wait(menLock);
    }
    nrOfWomenInBathroom++;
    if(nrOfMenWaiting == 0){
        sem_post(womenLock);
    }
    enterWoman(id);
    nrOfWomenInBathroom--;
    if (nrOfWomenInBathroom == 0) {
        printf("\nNr of women in bathroom: %d\n", nrOfWomenInBathroom);
        printf("Nr of men in bathroom: %d\n\n", nrOfMenInBathroom);
        printf("Nr of men waiting: %d\n\n", nrOfMenWaiting);
        sem_post(menLock);
        printf("MenLock unlocked by woman: %d\n", id);
        int temp = (nrOfMenWaiting - 1);
        while (temp > 0) {
            sem_post(menLock);
            printf("MenLock unlocked by woman: %d\n", id);
            temp--;
        }
        printf("\nNr of women waiting: %d\n\n", nrOfWomenWaiting);
    }
}

void men(int id) {
    nrOfMenWaiting++;
    printf("Man %d placed in queue\n", id);
    if(nrOfMenWaiting == 1 && nrOfWomenInBathroom > 0){
        sem_wait(womenLock);
    }
    sem_post(y);
    sem_wait(menLock);
    nrOfMenWaiting--;
    if(nrOfMenInBathroom == 0 && nrOfWomenWaiting == 0){
        sem_wait(womenLock);
    }
    nrOfMenInBathroom++;
    if(nrOfWomenWaiting == 0){
        sem_post(menLock);
    }
    enterMan(id);
    nrOfMenInBathroom--;
    if (nrOfMenInBathroom == 0) {
        printf("\nNr of men in bathroom: %d\n", nrOfMenInBathroom);
        printf("Nr of women in bathroom: %d\n\n", nrOfWomenInBathroom);
        printf("Nr of women waiting: %d\n\n", nrOfWomenWaiting);
        sem_post(womenLock);
        printf("WomenLock unlocked by man: %d\n", id);
        int temp = (nrOfWomenWaiting - 1);
        while (temp > 0) {
            sem_post(womenLock);
            printf("WomenLock unlocked by man: %d\n", id);
            temp--;
        }
        printf("\nNr of men waiting: %d\n\n", nrOfMenWaiting);
    }
}

void enterBathroom(int id) {
    sem_wait(y);
    sem_post(x);

    if (id % 2 == 0) {
        women(id);
    } else {
        men(id);
    }
}

void *start(void *arg) {
    int id = *(int *)arg;

    while (1) {
        sleep(randomTime(maxSleep));
        sem_wait(x);
        enterBathroom(id);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_close(x);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/xSem");
    sem_unlink("/ySem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    x = sem_open("/xSem", O_CREAT, 0644, 1);
    y = sem_open("/ySem", O_CREAT, 0644, 1);
    womenLock = sem_open("/womenSem", O_CREAT, 0644, 1);
    menLock = sem_open("/menSem", O_CREAT, 0644, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, start, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_close(x);
    sem_close(y);
    sem_close(womenLock);
    sem_close(menLock);
    sem_unlink("/xSem");
    sem_unlink("/ySem");
    sem_unlink("/womenSem");
    sem_unlink("/menSem");

    return 0;
}
