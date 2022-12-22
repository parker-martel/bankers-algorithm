// Parker Martel
// CPSC 351 - Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// Initialize matrices
void initialization(int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]){

    printf("\n**********STARTING BANKER'S ALGORITHM WITH %d CUSTOMERS AND %d RESOURCES**********\n\n", NUMBER_OF_CUSTOMERS, NUMBER_OF_RESOURCES);
    
    // Print the matrices
    printf("ALLOCATION           MAXIMUM                 AVAILABLE\n");
    bool flag = true;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        printf("P%d  ", i);
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
        printf("%d ", allocation[i][j]);
        }
        printf("         P%d  ", i);
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
        printf("%d ", maximum[i][j]);
        }
        printf("             ");
        while(flag == true){
            for(int k = 0; k < NUMBER_OF_RESOURCES; k++){
                printf("%d ", available[k]);
            }
            flag = false;
        }
        
        printf("\n");
    }

    // Set need array
    printf("\n\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
		for (int j = 0; j < NUMBER_OF_RESOURCES; j++){
			need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }    
}

// Test to see if processes are in safe state or unsafe state
int safety_algorithm(){
    printf("\n\n**********CALCULATING SAFE STATE**********");

    int Work[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        Work[i] = available[i];
    }
    
    int Finish[NUMBER_OF_CUSTOMERS];
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        Finish[i] = 0;
    }

    // Test Banker's Algorithm
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        for(int j = 0; j< NUMBER_OF_RESOURCES; j++){
            if(Finish[i] == 0 && need[i][j] <= Work[j]){
                for (int k = 0; k < NUMBER_OF_RESOURCES; k++){
                    Work[k] = Work[k] + allocation[i][k];
                    Finish[i] = 1;
                }
            } 
        }
    }

    // Safe state if all processes "i" are safe (1)
    printf("\n\nFinish: %d %d %d %d %d", Finish[0], Finish[1], Finish[2], Finish[3], Finish[4]);
    int flag = 0;
    for(int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        if (Finish[i] == 1){
            flag = 1;
        }
        else{
            flag = 0;
            break;
        }
    }
    
    // Show updated matrices
    printf("\n\nALLOCATION           MAXIMUM                  NEED                 AVAILABLE\n");
    bool avail = true;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++){
        printf("P%d  ", i);
        for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
            printf("%d ", allocation[i][j]);
        }
        printf("         P%d  ", i);
            for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
                printf("%d ", maximum[i][j]);
            }
            printf("             ");
                for(int j = 0; j < NUMBER_OF_RESOURCES; j++){
                    printf("%d ", need[i][j]);
                }
                printf("             ");
                while(avail == true){
                    for(int k = 0; k < NUMBER_OF_RESOURCES; k++){
                        printf("%d ", available[k]);
                    }
                    avail = false;
                }
    
                printf("\n");
    }
    
    return flag;
}

// Have processes request resources
void request_resources(int customer_num, int request[]){
    
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (request[i] > need[customer_num][i]){
            printf("\n\n******ERROR, thread has exceeded its maximum claim******\n");
            break;
        }
    }
    
    // Test if there are not enough available resources for the request
    int flag = 1;
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (request[i] > available[i]){
            flag = 0;
        }       
    }
    // Update matrices
    if (flag == 1){
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
            available[i] -= request[i];
            allocation[customer_num][i] += request[i];
            need[customer_num][i] -= request[i];
        }
        // Use Banker's Algorithm to test safe state
        int safe = safety_algorithm();
        if (safe == 1){
            printf("\n\nSafe state. Resource request has been successfully granted.\n\n");
        } else if (safe == 0){
            printf("\n\nUNSAFE state. Resource request denied.\n\n");
        }
    } else {
        printf("\n\n******ERROR, NOT ENOUGH AVAILABLE RESOURCES TO REQUEST!******");
    }
    
}

// Have processes release resources
void release_resources(int customer_num, int release[]){

    // Test if trying to release more than allocated
    int flag = 1;
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
        if (release[i] > allocation[customer_num][i]){
            printf("\n\n******ERROR, thread cannot release more resources than it has allocated!******\n");
            flag = 0;
            break;
        }
    }
    
    // Update matrices
    if (flag == 1){
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++){
            available[i] += release[i];
            allocation[customer_num][i] -= release[i];
            need[customer_num][i] += release[i];
        }

        // Use Banker's Algorithm to test safe state
        int safe = safety_algorithm();
        if (safe == 1){
            printf("\n\nSafe state. Resources have been released successfully.\n\n");
        } else {
            printf("\n\nUNSAFE state. Resources cannot be released.\n\n");
        }
    }
} 

int main(int argc, const char * argv[]) {

    FILE *myFile;
    myFile = fopen("bankers_input.txt", "r");

    //read file into array
    int i = 0;
    int j = 0;

    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    // Initialize available matrix, from file
   for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        fscanf(myFile, "%d,", &available[i]);
   }

    // Initialize allocation and maximum matrices, from file
   for (int j = 0; j < NUMBER_OF_CUSTOMERS; j++){
        for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        fscanf(myFile, "%d,", &allocation[j][i]);
    }
        for (i = 0; i < NUMBER_OF_RESOURCES; i++){
        fscanf(myFile, "%d,", &maximum[j][i]);
 
    }
   
   }

   // Start the Banker's Algorithm
   initialization(allocation, maximum);
   int state = safety_algorithm();
   if (state == 1){
        printf("\nSafe state.");
    } else {
        printf("\nUNSAFE State. Possible deadlock!");
    }

   
   int req1[5] = {0,2,2,0};
   int req2[5] = {0,10,2,0};
   int req3[5] = {0,3,3,1};
   int req4[5] = {1,0,0,0};
   
   printf("\n\n\n\n**********PROCESS P1 REQUESTING (0,10,2,0) RESOURCES**********");
   request_resources(1, req2);

   printf("\n\n\n\n**********PROCESS P1 REQUESTING (0,2,2,0) RESOURCES**********");
   request_resources(1, req1);
   
   printf("\n\n\n\n**********PROCESS P4 REQUESTING (0,3,3,1) RESOURCES**********");
   request_resources(4, req3);
   
   printf("\n\n\n\n**********PROCESS P2 REQUESTING (1,0,0,0) RESOURCES**********");
   request_resources(2, req4);
   
   
   
   int rel1[5] = {0,2,2,0};
   int rel2[5] = {0,3,5,4};
   int rel3[5] = {1,1,0,0};
   
   printf("\n\n\n\n**********PROCESS P1 RELEASING (0,2,2,0) RESOURCES**********");
   release_resources(1, rel1);
   printf("\n\n\n\n**********PROCESS P2 RELEASING (0,3,5,4) RESOURCES**********");
   release_resources(2, rel2);
   printf("\n\n\n\n**********PROCESS P3 RELEASING (1,1,0,0) RESOURCES**********");
   release_resources(3, rel3);

    fclose(myFile);

    return 0;
}

/*  OUTPUT 

***********************************************
OUTPUT 1/2 WITH "bankers_input2.txt" INPUT FILE
***********************************************


osc@ubuntu:~/final-src-osc10e/ch8$ ./bankersalgorithm -c

**********STARTING BANKER'S ALGORITHM WITH 5 CUSTOMERS AND 3 RESOURCES**********

ALLOCATION           MAXIMUM              AVAILABLE
P0  1 0 2          P0  7 9 9              2 1 0 
P1  0 2 1          P1  9 9 9              
P2  0 0 0          P2  6 11 9              
P3  1 0 0          P3  9 1 22              
P4  1 0 5          P4  4 3 4              




**********CALCULATING SAFE STATE**********

Finish: 0 0 0 0 0

ALLOCATION         MAXIMUM                NEED               AVAILABLE
P0  1 0 2          P0  7 9 9              6 9 7              2 1 0 
P1  0 2 1          P1  9 9 9              9 7 8              
P2  0 0 0          P2  6 11 9             6 11 9              
P3  1 0 0          P3  9 1 22             8 1 22              
P4  1 0 5          P4  4 3 4              3 3 -1              

UNSAFE State. Possible deadlock!






**********************************************
OUTPUT 2/2 WITH "bankers_input.txt" INPUT FILE
**********************************************


osc@ubuntu:~/final-src-osc10e/ch8$ ./bankersalgorithm -c

**********STARTING BANKER'S ALGORITHM WITH 5 CUSTOMERS AND 4 RESOURCES**********

ALLOCATION           MAXIMUM                 AVAILABLE
P0  0 0 1 2          P0  0 0 1 2              1 5 2 0 
P1  1 0 0 0          P1  1 7 5 0              
P2  1 3 5 4          P2  2 3 5 6              
P3  0 6 3 2          P3  0 6 5 2              
P4  0 0 1 4          P4  0 6 5 6              




**********CALCULATING SAFE STATE**********

Finish: 1 1 1 1 1

ALLOCATION           MAXIMUM                  NEED                 AVAILABLE
P0  0 0 1 2          P0  0 0 1 2              0 0 0 0              1 5 2 0 
P1  1 0 0 0          P1  1 7 5 0              0 7 5 0              
P2  1 3 5 4          P2  2 3 5 6              1 0 0 2              
P3  0 6 3 2          P3  0 6 5 2              0 0 2 0              
P4  0 0 1 4          P4  0 6 5 6              0 6 4 2              

Safe state.



**********PROCESS P1 REQUESTING (0,10,2,0) RESOURCES**********

******ERROR, thread has exceeded its maximum claim******


******ERROR, NOT ENOUGH AVAILABLE RESOURCES TO REQUEST!******



**********PROCESS P1 REQUESTING (0,2,2,0) RESOURCES**********

**********CALCULATING SAFE STATE**********

Finish: 1 1 1 1 1

ALLOCATION           MAXIMUM                  NEED                 AVAILABLE
P0  0 0 1 2          P0  0 0 1 2              0 0 0 0              1 3 0 0 
P1  1 2 2 0          P1  1 7 5 0              0 5 3 0              
P2  1 3 5 4          P2  2 3 5 6              1 0 0 2              
P3  0 6 3 2          P3  0 6 5 2              0 0 2 0              
P4  0 0 1 4          P4  0 6 5 6              0 6 4 2              


Safe state. Resource request has been successfully granted.





**********PROCESS P4 REQUESTING (0,3,3,1) RESOURCES**********

******ERROR, NOT ENOUGH AVAILABLE RESOURCES TO REQUEST!******



**********PROCESS P2 REQUESTING (1,0,0,0) RESOURCES**********

**********CALCULATING SAFE STATE**********

Finish: 1 1 1 1 1

ALLOCATION           MAXIMUM                  NEED                 AVAILABLE
P0  0 0 1 2          P0  0 0 1 2              0 0 0 0              0 3 0 0 
P1  1 2 2 0          P1  1 7 5 0              0 5 3 0              
P2  2 3 5 4          P2  2 3 5 6              0 0 0 2              
P3  0 6 3 2          P3  0 6 5 2              0 0 2 0              
P4  0 0 1 4          P4  0 6 5 6              0 6 4 2              


Safe state. Resource request has been successfully granted.





**********PROCESS P1 RELEASING (0,2,2,0) RESOURCES**********

**********CALCULATING SAFE STATE**********

Finish: 1 1 1 1 1

ALLOCATION           MAXIMUM                  NEED                 AVAILABLE
P0  0 0 1 2          P0  0 0 1 2              0 0 0 0              0 5 2 0 
P1  1 0 0 0          P1  1 7 5 0              0 7 5 0              
P2  2 3 5 4          P2  2 3 5 6              0 0 0 2              
P3  0 6 3 2          P3  0 6 5 2              0 0 2 0              
P4  0 0 1 4          P4  0 6 5 6              0 6 4 2              


Safe state. Resources have been released successfully.





**********PROCESS P2 RELEASING (0,3,5,4) RESOURCES**********

**********CALCULATING SAFE STATE**********

Finish: 1 1 1 1 1

ALLOCATION           MAXIMUM                  NEED                 AVAILABLE
P0  0 0 1 2          P0  0 0 1 2              0 0 0 0              0 8 7 4 
P1  1 0 0 0          P1  1 7 5 0              0 7 5 0              
P2  2 0 0 0          P2  2 3 5 6              0 3 5 6              
P3  0 6 3 2          P3  0 6 5 2              0 0 2 0              
P4  0 0 1 4          P4  0 6 5 6              0 6 4 2              


Safe state. Resources have been released successfully.





**********PROCESS P3 RELEASING (1,1,0,0) RESOURCES**********

******ERROR, thread cannot release more resources than it has allocated!******
osc@ubuntu:~/final-src-osc10e/ch8$ 
*/