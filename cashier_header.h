#ifndef CASHIER_HEADER
#define CASHIER_HEADER

#define MAX_QUEUE_CAPACITY 10
#define NUM_CASHIERS 3
#define MAX_ITEMS_IN_CART 10
#define MAX_LINE_LENGTH 100
#include "constants.h"
#include "header.h"
#include "customer_header"
#include "semphores.h"
#include "shared_memories.h"

int get_total_cashiers();

volatile sig_atomic_t exit_program = 0;
volatile sig_atomic_t totalSalesAboveThreshold = 0;

typedef struct {
    int id;                     // Cashier ID
    int numPeopleInQueue;       // Number of people in the cashier's queue
    struct Customer queue[MAX_QUEUE_SIZE]; // Queue of customers
    int happiness;              // Happiness level of the cashier
    int scanningTime;
    int number_of_all_items ;// Time taken by the cashier to scan each item
    float total_sales;
} Cashier;

// Function to generate random scanning time based on config
int generate_scanning_time(int cashier_id) {



    srand((unsigned int)time(NULL) + 7*cashier_id);
    return rand() % c.scanning_time + 1; // Random number between 1 and scanning_time
}


int get_total_cashiers( int num_of_cashier_queue) {


    int shmid = shmget(total_num_cashiers_key, sizeof(total_cashiers), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    total_cashiers *shared_data = (total_cashiers *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code
    // Acquire the semaphore before modifying shared memory
    sem_t *cashier_mutex = get_semaphore(total_cashiers_key);

    lock_sem(cashier_mutex);

    // Increment the totalCustomers value

    int total_cash ;

    if(num_of_cashier_queue == 0){
        total_cash = shared_data->totalCashiers;
    }
    else if(num_of_cashier_queue == 1){
        total_cash = shared_data->scanning_time1;
    }else if(num_of_cashier_queue == 2){
        total_cash = shared_data->scanning_time2 ;
    }else if(num_of_cashier_queue == 3) {
        total_cash = shared_data->scanning_time3 ;
    }




    // Release the semaphore
    unlock_sem(cashier_mutex);
    close_semaphore(cashier_mutex);
    //end mutex code

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return total_cash;
}


void increment_total_cashiers(int num, int num_of_cashier_queue ) {
    // Get the shared memory ID
    int shmid = shmget(total_num_cashiers_key, sizeof(total_cashiers), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach to the shared memory
    total_cashiers *shared_data = (total_cashiers *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Acquire the semaphore before modifying shared memory
    sem_t *cashier_mutex = get_semaphore(total_cashiers_key);


    lock_sem(cashier_mutex);


    if(num_of_cashier_queue == 0){
        shared_data->totalCashiers = shared_data->totalCashiers + num;
    }
     else if(num_of_cashier_queue == 1){
        shared_data->scanning_time1 = num;
    }else if(num_of_cashier_queue == 2){
        shared_data->scanning_time2 = num;
     }else if(num_of_cashier_queue == 3) {
        shared_data->scanning_time3 = num;
    }





    // Release the semaphore
    unlock_sem(cashier_mutex);

    // Close the semaphore
    close_semaphore(cashier_mutex);

    // Detach from shared memory
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}


void change_score1_cashier1(float score1) {

    int shmid = shmget(cashier1_score_key, sizeof(cashier_score1_shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score1_shared_memory *shared_data = (cashier_score1_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    sem_t* score1_mutex = get_semaphore(cashSem1_score_key);

    lock_sem(score1_mutex);


     shared_data->score1 = score1;

    unlock_sem(score1_mutex);
    close_semaphore(score1_mutex);

    //end mutex code

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }


}

void change_score2_cashier2(float score2){

    int shmid = shmget(cashier2_score_key , sizeof(cashier_score2_shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score2_shared_memory *shared_data = (cashier_score2_shared_memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


    sem_t* score2_mutex = get_semaphore(cashSem2_score_key);

    lock_sem(score2_mutex);

    shared_data->score2 = score2;

    unlock_sem(score2_mutex);
    close_semaphore(score2_mutex);

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }



}

void change_score3_cashier3(float score3) {

    int shmid = shmget(cashier3_score_key, sizeof(cashier_score3_shared_memory), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_score3_shared_memory *shared_data = (cashier_score3_shared_memory *) shmat(shmid, NULL, 0);
    if (shared_data == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


    sem_t *score3_mutex = get_semaphore(cashSem3_score_key);

    lock_sem(score3_mutex);

    shared_data->score3 = score3;

    unlock_sem(score3_mutex);
    close_semaphore(score3_mutex);

    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

}


// num for people or  items to add
// queue_num_index for the queue number
// item_or_people_array_type 1 for people 2 for items

void write_score_att(int num ,int queue_num_index , int people_1_items_2){

    int shmid = shmget(score_atrributes_key, sizeof(score_atrributes), 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    score_atrributes *shared_data = (score_atrributes *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    sem_t* score_att = get_semaphore(score_atrributes_key_sem);

    lock_sem(score_att);

    // 1 for people 2 for items


    if(people_1_items_2 == 1){
        shared_data->total_waiting_customers[queue_num_index -1] = num + shared_data->total_waiting_customers[queue_num_index -1];
    }else if(people_1_items_2 == 2){
        shared_data->total_num_of_items[queue_num_index-1] += num;
    }



    unlock_sem(score_att);
    close_semaphore(score_att);

    //end mutex code

    if (shmdt(shared_data) == -1) {

        perror("shmdt");
        exit(EXIT_FAILURE);

    }


}

float get_total_sales(int num_of_cashier_queue){


    int shmid = shmget(total_sales_key, sizeof(total_sales), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    total_sales *shared_data = (total_sales *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


    // mutex code

    sem_t* total_sales_mutex = get_semaphore(totalsale_key_sem);

    lock_sem(total_sales_mutex);

    // 1 for people 2 for items
    float  sales = 0;

    if(num_of_cashier_queue == 1){
       sales = shared_data->total_sales1;

    }else if(num_of_cashier_queue == 2){
        sales = shared_data->total_sales2;

    }
    else if(num_of_cashier_queue == 3){
        sales = shared_data->total_sales3;

    }


    unlock_sem(total_sales_mutex);

    close_semaphore(total_sales_mutex);

    //end mutex code

    if (shmdt(shared_data) == -1) {

        perror("shmdt");
        exit(EXIT_FAILURE);

    }

    return sales;

}

void write_total(int num_of_cashier_queue , float sales){

    int shmid = shmget(total_sales_key, sizeof(total_sales), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    total_sales *shared_data = (total_sales *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    sem_t* total_sales_mutex = get_semaphore(totalsale_key_sem);

    lock_sem(total_sales_mutex);

    // 1 for people 2 for items

    if(num_of_cashier_queue == 1){
        shared_data->total_sales1 = sales;

    }else if(num_of_cashier_queue == 2){

        shared_data->total_sales2 = sales;

    }
    else if(num_of_cashier_queue == 3){

        shared_data->total_sales3 = sales;
    }



    unlock_sem(total_sales_mutex);

    close_semaphore(total_sales_mutex);

    //end mutex code

    if (shmdt(shared_data) == -1) {

        perror("shmdt");
        exit(EXIT_FAILURE);

    }

}

void write_happiest_cashier(int num_of_cashier_queue , int happiest_cashier){

    int shmid = shmget(happiest_cashier_key, sizeof(cashier_happiness_Shared_Memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    cashier_happiness_Shared_Memory *shared_data = (cashier_happiness_Shared_Memory *)shmat(shmid, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // mutex code

    sem_t* happiest_cashier_mutex = get_semaphore(happiest_cashier_key_sem);

    lock_sem(happiest_cashier_mutex);

    // 1 for people 2 for items

    if(num_of_cashier_queue == 1){
        shared_data->happy_cashier_1 = happiest_cashier;

    }else if(num_of_cashier_queue == 2){

        shared_data->happy_cashier_2 = happiest_cashier;

    }
    else if(num_of_cashier_queue == 3){

        shared_data->happy_cashier_3 = happiest_cashier;
    }

    unlock_sem(happiest_cashier_mutex);

    close_semaphore(happiest_cashier_mutex);

    //end mutex code

    if (shmdt(shared_data) == -1) {

        perror("shmdt");
        exit(EXIT_FAILURE);

    }

}






#endif
