#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define EPSILON 0.001
#include <ctype.h>

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

void prepData();
int isNumber(char number[]);
void setZerosToNewCentroids();


double calcMaxCentroidsDelta();

double d(struct cord v1,struct cord v2);

struct vector *data =NULL;
struct vector *newCentroids;
struct vector *prevCentroids;


int iteration_number = 1;
int k = 15;
int iter = 200;
int rowsCount=0;
int columnCount=0;
double maxCentroidDelta = EPSILON+1;


void copyFirstKToNew();

double calcMaxCentroidsDelta();

void printCentroids(struct vector *pVector);

int main(int argc, char *argv[] )
{
    prepData();


    if(argc ==4){
        if(!isNumber(argv[2])){
            printf("Invalid maximum iteration!");
            return 0;
        }
        iter = atoi(argv[2]);
    }
    else{
        if(argc != 3){
            return 0;
        }
    }
    if(!isNumber(argv[1])){
        printf("Invalid number of clusters!");
        return 0;
    }
    k = atoi(argv[1]);

    if(iter<=1 || iter >= 1000 || iter % 1 != 0){
        printf("Invalid maximum iteration!");
        return 0;
    }
    if(k<=1 || k >= rowsCount || k % 1 != 0){
        printf("Invalid number of clusters!");
        return 0;

    }


    copyFirstKToNew(data);
    double minDist;
    struct vector *currNewCentroid;
    struct vector *currPrevCentroid;

    while(iteration_number < iter && maxCentroidDelta>=EPSILON){
        struct vector *currVector = data;
        int *assignmentsCounter = calloc(k, sizeof (int));
        free(prevCentroids);
        prevCentroids = newCentroids;
        setZerosToNewCentroids();

        for(int i = 0; i< rowsCount; i++){

            currNewCentroid = newCentroids;
            currPrevCentroid = prevCentroids;
            minDist = d(*currPrevCentroid->cords,*currVector->cords); // d between x_1 and the first Centroid
            int minIndex= 0;
            struct vector *minPointer = currNewCentroid;


            for (int j =1; j< k; j++){
                currPrevCentroid = currPrevCentroid->next;
                currNewCentroid = currNewCentroid->next;  //    I change!
                double dis = d(*currPrevCentroid->cords,*currVector->cords);
                if(dis<minDist) {
                    minDist=dis;
                    minPointer = currNewCentroid;
                    minIndex=j;
                }
            }     // find the nearest Centroid for x_i with number 0 <= j < k

            assignmentsCounter[minIndex]++;


            struct cord *minDistCentCord = minPointer->cords;
            struct cord *cordsToAdd = currVector->cords;
            for(int i = 0; i<columnCount; i++){
                minDistCentCord->value += cordsToAdd->value;
                minDistCentCord = minDistCentCord->next;
                cordsToAdd = cordsToAdd->next;
            }

            currVector= currVector->next;
        }


        struct vector *currNewCentroid= newCentroids;
        for (int i = 0;i<k;i++){
            if(assignmentsCounter[i]>0) {
                struct cord *cordsToChange = currNewCentroid->cords;
                for(int j=0;j<columnCount;j++){
                    cordsToChange->value = cordsToChange->value / assignmentsCounter[i];
                    if (j != columnCount) {
                        cordsToChange = cordsToChange->next;
                    }
                }
            }
            currNewCentroid=currNewCentroid->next;
        }


        free(assignmentsCounter);
        iteration_number++;
        maxCentroidDelta = calcMaxCentroidsDelta();
    }
    printCentroids(newCentroids);

}

void printCentroids(struct vector *pVector) {
    struct vector *headVector = pVector;

    for(int i = 1; i<=k; i++){
        struct cord *cords = headVector->cords;
        for(int j=1;j<=columnCount; j++){
            printf("%0.4f",cords->value);
            if(j!=columnCount){
                printf(",");
                cords=cords->next;
            }
        }
        printf("\n");
        headVector=headVector->next;
    }
}

double calcMaxCentroidsDelta() {

    struct vector *currNewCentroid = newCentroids;
    struct vector *currPrevCentroid = prevCentroids;
    maxCentroidDelta = d(*currNewCentroid->cords,*currPrevCentroid->cords);

    for(int i =1;i<k;i++){
        currNewCentroid = currNewCentroid->next;
        currPrevCentroid = currPrevCentroid->next;
        double dis =  d(*currNewCentroid->cords,*currPrevCentroid->cords);
        if(dis > maxCentroidDelta)
            maxCentroidDelta = dis;
    }
    return maxCentroidDelta;

}

int isNumber(char number[])
{
    int i = 0;
    for (; number[i] != 0; i++)
    {
        if (!isdigit(number[i]))
            return 0;
    }
    return 1;
}


void prepData(){
    double n;
    char c;

    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;


    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;
    int columnsCountInitialized=0;


    while (scanf("%lf%c", &n, &c) == 2)
    {
        if(!columnsCountInitialized){
            columnCount++;
        }

        if (c == '\n')
        {
            columnsCountInitialized=1;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            rowsCount++;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;

    }
    data = head_vec;
}

void copyFirstKToNew() {
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;

    struct vector *curr_data_vec = data;
    struct cord *curr_data_cord;
    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;
    for(int i=1;i<=k;i++){
        curr_data_cord = curr_data_vec->cords;
        head_cord = malloc(sizeof(struct cord));
        curr_cord = head_cord;
        for (int j=1;j<=columnCount; j++){
            curr_cord->value = curr_data_cord->value;

            if(j != columnCount) {
                curr_cord->next = malloc(sizeof(struct cord));
                curr_cord = curr_cord->next;
            }
            curr_cord->next = NULL;
            curr_data_cord=curr_data_cord->next;  // not need?
        }

        curr_data_vec = curr_data_vec->next;
        curr_vec->cords=head_cord;
        if(i!=k){
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
        }
        curr_vec->next=NULL;

    }
    newCentroids = head_vec;

}
void setZerosToNewCentroids(){
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    head_vec= malloc(sizeof (struct vector));
    curr_vec = head_vec;
    for(int i=1;i<=k;i++) {
        head_cord = malloc(sizeof (struct cord));
        curr_cord = head_cord;
        curr_vec->cords=head_cord;
        for (int j=1;j<=columnCount; j++) {
            curr_cord->value=0;
            if(j != columnCount){
                curr_cord->next = malloc(sizeof(struct cord));
                curr_cord = curr_cord->next;
            }
            curr_cord->next=NULL;
        }
        if(i!= k){
            curr_vec->next= malloc(sizeof (struct vector));
            curr_vec=curr_vec->next;
        }
        curr_vec->next = NULL;
    }
    newCentroids = head_vec;
}
double d(struct cord v1,struct cord v2){
    double sum = 0;
    for(int i =1;i<=columnCount; i++){
        sum += pow(v1.value-v2.value,2);
        if(i!=columnCount) {
            v1 = *v1.next;
            v2 = *v2.next;
        }
    }
    return sqrt(sum);
}
