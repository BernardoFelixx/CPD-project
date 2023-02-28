#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nqueue/queue.h"

typedef struct{
    int* tour;
    double cost;
    double bound;
    int length;
    int current_city;
} Element;

char compare(void* node1, void* node2) {
    Element* element1 = (Element*)node1;
    Element* element2 = (Element*)node2;
    if (element1->bound < element2->bound)
        return 0;
    else if (element1->bound == element2->bound && element1->current_city < element2->current_city)
        return 0;
    return 1;
}

void printSolution(int* bestTour, int bestTourLength, double bestTourCost) {
    printf("%0.1f\n", bestTourCost);
    for(int j = 0; j < bestTourLength; j++)
        printf("%d ", bestTour[j]);
    printf("0\n");
}

int isElementInTour (int element, int length, int* tour) {
    for (int j = 1; j < length; j++)
        if (tour[j] == element)
            return 1;
    return 0;
}

double getMinDistance (double distance, double value1, double value2) {
    if(distance >= value2)
        return value2;
    return value1;
}

/*void printElement (Element* element) {
    printf("Tour: ");
    for(int j = 0; j < element->length; j++)
        printf("%d ",element->tour[j]);
    printf("\nCost: %f   Bound: %f    Length: %d    current_city: %d\n\n\n", element->cost, element->bound, element->length, element->current_city);
}*/

int main(int argc, char*argv[]){

    int i, j, n_cities, n_roads, city_1, city_2; 
    double distance_value;
    double max_value=atof(argv[2]);
    
    FILE* ptr;
    char str[50];
    ptr = fopen(argv[1], "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    fgets(str, 50, ptr);
    n_cities=atoi(strtok(str," "));
    n_roads=atoi(strtok(NULL," "));

    double** distances = (double**)malloc(n_cities * sizeof(double*));
    for (i = 0; i < n_cities; i++)
        distances[i] = (double*)malloc(n_cities * sizeof(double));

    /*paralizable with a for loop i<n_cities*/
    
    while (fgets(str, 50, ptr) != NULL) {
        city_1=atoi(strtok(str," "));
        city_2=atoi(strtok(NULL," "));
        distance_value=atof(strtok(NULL," "));
        distances[city_1][city_2]=distance_value;
        distances[city_2][city_1]=distance_value;
    }
    fclose(ptr);
   
    double lowerbound;
    for(i=0;i<n_cities;i++){
        double min1 = 0;
        double min2 = 0;
        for(j=0;j<n_cities;j++){
            if(distances[i][j] != 0){
                if(min1 == 0 || distances[i][j] <= min1){
                    min2 = min1;
                    min1 = distances[i][j];
                }
                else if(min2 == 0 || distances[i][j] <= min2){
                    min2 = distances[i][j];
                }
            }
        }
        lowerbound = lowerbound + min1 + min2;
    }
    lowerbound = lowerbound/(double)2;

    Element* element1;
    element1 = malloc(sizeof(Element));
    element1->tour = (int*)malloc(1 * sizeof(int));
    element1->tour[0] = 0;
    element1->bound = lowerbound;
    element1->current_city = 0;
    element1->length = 1;
    element1->cost = 0;

    priority_queue_t* queue = queue_create(compare);
    queue_push(queue, element1);
    double bestTourCost = max_value;
    int bestTourLength;
    int* bestTour = (int*)malloc(n_cities * sizeof(int));
    while(queue->size != 0){
        Element* element = queue_pop(queue);
        //printElement(element);
        if(element->bound >= bestTourCost){
            if (bestTourCost >= max_value){
                printf("NO SOLUTION\n");
                return 1;
            }
            else{
                printSolution(bestTour, bestTourLength, bestTourCost);
                return 0;
            }
        }
        if(element->length == n_cities){
            if(distances[0][element->current_city] != 0 && element->cost + distances[0][element->current_city] < bestTourCost){
                bestTour = element->tour;
                bestTourCost = element->cost + distances[0][element->current_city];
                bestTourLength = element->length;
            }
        }
        else{
            for (i = 1; i < n_cities; i++){
                int inTour = isElementInTour(i, element->length, element->tour);  
                if(distances[element->current_city][i] != 0 && inTour != 1){
                    double cf, ct, min1f = 0, min2f = 0, min1t = 0, min2t = 0;
                    double distance = distances[element->current_city][i];
                    for(j = 0; j < n_cities; j++){
                        //First city
                        double distancef = distances[element->current_city][j];
                        if(distancef != 0){
                            if(min1f == 0 || distancef <= min1f){
                                min2f = min1f;
                                min1f = distancef;
                            }
                            else if(min2f == 0 || distancef <= min2f){
                                min2f = distancef;
                            }
                        }
                        //Second city
                        double distancet = distances[i][j];
                        if(distancet != 0){
                            if(min1t == 0 || distancet <= min1t){
                                min2t = min1t;
                                min1t = distancet;
                            }
                            else if(min2t == 0 || distancet <= min2t){
                                min2t = distancet;
                            }
                        }
                    }
                    
                    //First city
                    cf = getMinDistance(distance, min1f, min2f);
                    //Second city
                    ct = getMinDistance(distance, min1t, min2t);
                    
                    double newBound = element->bound + distance - ((cf + ct)/(double)2);
                    int newLength = element->length + 1;
                    if(newBound <= bestTourCost){
                        int* newTour = (int*)malloc(newLength * sizeof(int));
                        for (int j = 0; j < element->length; j++)
                            newTour[j] = element->tour[j];
                        newTour[element->length] = i;
                        
                        double newCost = element->cost + distances[element->current_city][i];
                        Element* newElement;
                        newElement = malloc(sizeof(Element));
                        newElement->tour = newTour;
                        newElement->bound = newBound;
                        newElement->current_city = i;
                        newElement->length = newLength;
                        newElement->cost = newCost;
                        printElement(newElement);
                        queue_push(queue, newElement);
                    }
                }
            }
        }
    }

    if (bestTourCost >= max_value){
        printf("NO SOLUTION\n");
        return 1;
    }
    else{
        printSolution(bestTour,bestTourLength, bestTourCost);
        return 0;
    }
}
