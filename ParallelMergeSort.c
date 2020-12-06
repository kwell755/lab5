#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int array[SIZE]) {
for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int array[SIZE]) {
for(int i = 0; i<SIZE; i++) printf("%5d", array[i]);
printf("\n");
}

typedef struct StartEndIndexes {
int start;
int end;
} StartEndIndexes;

void merge(int left, int middle, int right)
{
    int x, y, z;
    int idx1 = middle - left + 1;
    int idx2 = right - middle;

    int leftArr[idx1], rightArr[idx2];

    for (x = 0; x < idx1; x++){
      leftArr[x] = array[left + x];  
    }
    for (y = 0; y < idx2; y++){
       rightArr[y] = array[middle + 1+ y]; 
    }
    
    x = 0; 
    y = 0; 
    z = left; 

    while (x < idx1 && y < idx2){

        if (leftArr[x] <= rightArr[y]){
            array[z] = leftArr[x];
            x++;
        }
        else{
            array[z] = rightArr[y];
            y++;
        }
        z++;
    }

    while (x < idx1){
        array[z] = leftArr[x];
        x++;
        z++;
    }

    while (y < idx2){
    array[z] = rightArr[y];
    y++;
    z++;
    }

}

void* mergeSort(void* args) {

    StartEndIndexes SEI = *((StartEndIndexes*)args);
    int leftErr, rightErr, mid;

    if (SEI.start < SEI.end){

        mid = (SEI.start + SEI.end) / 2;

        StartEndIndexes SEI1, SEI2;

        SEI1.start = SEI.start;
        SEI1.end = mid;
        SEI2.start = mid + 1;
        SEI2.end = SEI.end;

        pthread_t left_tid, right_tid;

        leftErr = pthread_create(&left_tid, NULL, mergeSort, &SEI1);
        rightErr = pthread_create(&right_tid, NULL, mergeSort, &SEI2);

        if (leftErr != 0){
            printf("Something went wrong in the left tree.\n"); 
        }

        if (rightErr != 0){
            printf("Something went wrong in the right tree.\n"); 
        }

        leftErr = pthread_join(left_tid, NULL);
        rightErr = pthread_join(right_tid, NULL);

        if (leftErr != 0){
            printf("Something went wrong in the left tree.\n"); 
        }

        if (rightErr != 0){
            printf("Something went wrong in the right tree.\n");  
        }

        merge(SEI.start, mid, SEI.end);

    }

    return NULL;
}

int main() {

    srand(time(0));
    StartEndIndexes SEI;
    SEI.start = 0;
    SEI.end = SIZE - 1;
    int error;

    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);

    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);

    // 3. Create a thread for merge sort.
    pthread_t tid;
    error = pthread_create(&(tid), NULL, &mergeSort, &SEI);
    if (error != 0){
        printf("Something went wrong creating the thread.\n");
    }
    
    // 4. Wait for mergesort to finish.
    error = pthread_join(tid, NULL);
    if (error != 0){
     printf("Something went wrong joining the thread.\n");
    }
    
    // 5. Print the sorted array.
    printf("Sorted array: ");
    printArray(array);

}

