//  Disk Scheduling Algorithms
//
//  Copyright © 2015 Matthew Szczeblewski. All rights reserved.
//
//  Program Description:
//  Create a circular & doubly linked list to emulate a rotating hard disk.
//  Populate the cylinders using a random number generator.
//  Implement three different disk scheduling algorithims: SSTF, SCAN, C-Look.

#include<stdio.h>
#include<stdlib.h>
#include <time.h>

//Cylinder
struct node{
    int cylinder;
    struct node *prev;
    struct node *next;
};


//Global variables
int saveHead, headMovement, numCylinders = 0;
struct node* head = NULL;
struct node* first = NULL;
struct node* last = NULL;


//Function Declarations
struct node* newNode(int data);
void generateCylinder(int data);
int randomNumber();

void displayCylinders(struct node *ptr);
void sortCylinders(struct node *ptr);
void swap(struct node *a, struct node *b);

void sstfDisk(struct node *ptr);
void scanDisk(struct node *ptr);
void cLookDisk(struct node *ptr);




/* -------------------------  Main  ------------------------- */
int main() {
    
    //Create seed for random # generator
    srand(time(NULL));
    
    for (int i =0; i < 5; i++) {
        
        //Populate cylinders to be traveresed
        for (int i= 0; i < 20; i++)
            generateCylinder(randomNumber());
        
        
        sortCylinders(head);
        displayCylinders(head);
        
        //head poisition
        while(head->cylinder != saveHead)
            head = head->next;
        printf("Current Head Position:  %d\n", head->cylinder);
        
        sstfDisk(head);
        scanDisk(head);
        cLookDisk(head);
        printf("------------------------------------------------\n\n");
        
        //Reset for Next Cylinder Test
        head = NULL;
        first = NULL;
        last = NULL;
        numCylinders = 0;
    }
    return 0;
}

/* ------------------------- Function Def ------------------------- */


//Create New Node
struct node* newNode(int data){
    struct node *ptr = (struct node *)malloc(sizeof (struct node));
    ptr->cylinder = data;
    ptr->prev = NULL;
    ptr->next = NULL;
    return (ptr);
}


//Insert cylinder into doubly linked list
void generateCylinder(int data){
    struct node *ptr = newNode(data);
    struct node *temp;
    
    
    if (!head){
        head = ptr;
        first = ptr;
        last = ptr;
        head->next = head;
        head->prev = head;
        numCylinders++;
    }
    else{
        temp = head->prev;
        temp->next = ptr;
        ptr->prev = temp;
        ptr->next = head;
        head->prev = ptr;
        last = ptr;
        numCylinders++;
    }
}


//Generate random number for Cylinder
int randomNumber(){
    
    return (rand()%1000);
}


//Print out all cylinders in list
void displayCylinders(struct node *ptr){
    
    int i = 0;
    
    while (i < numCylinders){
        printf("Cylinder %d:  %d\n",i , ptr->cylinder);
        ptr = ptr->next;
        i++;
    }
    printf("\n");
}

//Sort cylinders to make traversal easier
void sortCylinders(struct node *head){
    
    saveHead = head->cylinder;
    int swapped;
    struct node *ptr;
    
    do{
        swapped = 0;
        ptr = head;
        
        while (ptr->next != head){
            if (ptr->cylinder > ptr->next->cylinder){
                swap(ptr, ptr->next);
                swapped = 1;
            }
            ptr = ptr->next;
        }
    }while (swapped);
}


void swap(struct node *node1, struct node *node2){
    
    int temp = node1->cylinder;
    node1->cylinder = node2->cylinder;
    node2->cylinder = temp;
}




/* ---------------------- Scheduling Algorithm Functions ---------------------- */

//Shortest Seek Time First algorithim
//Head bounces between shortest seek times from its current position until entire disk is serviced.
void sstfDisk(struct node *ptr){
    
    int left, right, i=0;
    struct node *currentHigh = ptr->next;
    struct node *currentLow = ptr->prev;
    
    while (i < 19) {
        
        left = abs(ptr->cylinder - currentLow->cylinder);
        right = abs(ptr->cylinder - currentHigh->cylinder);
        
        if (left < right) {//go left
            headMovement+=left;
            ptr = currentLow;
            currentLow = ptr->prev;
            
        }
        else {//go right
            headMovement+=right;
            ptr = currentHigh;
            currentHigh = ptr->next;
        }
        i++;
    }
    printf("\nTotal Head Distance Moved For  SSTF: %d \n", headMovement);
    headMovement = 0;
}



//SCAN (Elevator Algorithm)
//Disk arm starts at first request, servicing first in closest direction
//When head reaches end of disk, movement is reversed to service to the other end of disk.
void scanDisk(struct node *ptr){
    
    int left, right, flag, i=0;
    int mid = ((last->cylinder-first->cylinder)/2);
    
    struct node *currentHigh = ptr->next;
    struct node *currentLow = ptr->prev;
    
    //Set Flag for end of list to go to opposite side
    if (ptr->cylinder < mid)
        flag = 1;
    else
        flag = 0;
    
    
    while (i < 20) {
        
        left = abs(ptr->cylinder - currentLow->cylinder);
        right = abs(ptr->cylinder - currentHigh->cylinder);
        
        if (flag == 1) {//go left
            if (ptr->prev->cylinder > ptr->cylinder) {
                headMovement+=(first->cylinder + currentHigh->cylinder);
                ptr = currentHigh;
                flag = 0;
            }
            else{
                headMovement+=left;
                ptr = currentLow;
                currentLow = ptr->prev;
            }
        }
        else{//go right
            if (ptr->next->cylinder < ptr->cylinder) {
                headMovement = headMovement+ ((999-last->cylinder) + (999 - currentLow->cylinder));
                ptr = currentLow;
                flag = 1;
            }
            else{
                headMovement+=right;
                ptr = currentHigh;
                currentHigh = ptr->next;
            }
        }
        i++;
    }
    printf("Total Head Distance Moved For  SCAN: %d \n", headMovement);
    headMovement = 0;
}



//C-Look Algorithm
//Disk arm goes as far as last request in one direction.
//Then reverses direction immediately, without going all the way to the end of disk
void cLookDisk(struct node *ptr){
    
    int mid = ((last->cylinder - first->cylinder)/2);
    
    if (ptr->cylinder > mid){
        while (ptr->next != head) {
            if (ptr->cylinder == last->cylinder){
                ptr = ptr->next;
            }
            else{
                headMovement+=abs(ptr->next->cylinder - ptr->cylinder);
                ptr = ptr->next;
            }
        }
    }
    else{
        while (ptr->prev != head) {
            if (ptr->cylinder == first->cylinder){
                ptr = ptr->prev;
            }
            else{
                headMovement+=abs(ptr->cylinder - ptr->prev->cylinder);
                ptr = ptr->prev;
            }
        }
    }
    printf("Total Head Distance Moved For CLooK: %d \n", headMovement);
    headMovement = 0;
}












