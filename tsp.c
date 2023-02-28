#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nqueue/queue.h"
#include "nqueue/queue.hpp"

struct Element{
    int* tour;
    int cost;
    int bound;
    int length;
    int current_city;
};

int compare(struct Element* elem1, struct Element* elem2){
    if(elem1 -> bound < elem2 -> bound){
        return 0;
    }
    else{
        return 1;
    }
}

int main(int argc, char*argv[]){

    int i,j;

    int n_cities, n_roads;
    int city_1,city_2;
    int distance_value;
    int max_value=atoi(argv[2]);

    FILE* ptr;
    char str[50];
    ptr = fopen(argv[1], "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    fgets(str, 50, ptr);
    n_cities=atoi(strtok(str," "));
    n_roads=atoi(strtok(NULL," "));

    int** distances = (int**)malloc(n_cities * sizeof(int*));
    for (i = 0; i < n_cities; i++){
        distances[i] = (int*)malloc(n_cities * sizeof(int));
    }


    /*paralizable with a for loop i<n_cities*/
    
    while (fgets(str, 50, ptr) != NULL) {
        city_1=atoi(strtok(str," "));
        city_2=atoi(strtok(NULL," "));
        distance_value=atoi(strtok(NULL," "));
        distances[city_1][city_2]=distance_value;
        distances[city_2][city_1]=distance_value;
    }

    for(i=0;i<n_cities;i++){
        for(j=0;j<n_cities;j++){
            printf("%d ",distances[i][j]);
        }
        printf("\n");
    }

 
    fclose(ptr);

    
    int lowerbound;
    for(i=0;i<n_cities;i++){
        int min1 = 0;
        int min2 = 0;
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
    lowerbound = lowerbound/2;

    struct Element* element1;
    element1 -> tour = (int*)malloc(1 * sizeof(int));
    element1 -> bound = lowerbound;
    element1 -> current_city = 0;
    element1 -> length = 1;
    element1 -> cost = 0;

    priority_queue_t * queue = queue_create(compare);
    queue_push(queue, element1);
    int bestTourCost = max_value;
    int bestTourLength;
    int* bestTour;
    while(sizeof(queue) != 0){
        struct Element* element = queue_pop(queue);
        if(element -> bound >= bestTourCost){
            if (bestTourCost >= max_value){
                printf("NO SOLUTION");
                return 1;
            }
            else{
            printf("%d\n", bestTourCost);
            for(int j = 0; j < bestTourLength; j++){
                printf("%d ", bestTour[j]);
            }
            printf("0");
            return 0;
            }

        }
        if(element -> length == n_cities){
            if(distances[0][element ->current_city] == 0){
                printf("NO SOLUTION");
                return 1;
            }
            else if(element -> cost + distances[0][element ->current_city] < bestTourCost){
            
            bestTour = element -> tour;
            bestTourCost = element -> cost + distances[0][element ->current_city];
            bestTourLength = element -> length;

        }
        }
        else{
            for (i = 1; i < n_cities; i++){
                int inTour = 0;   
                for (int j = 1; j < element -> length; j++) {
                    if (element -> tour[j] == i) {
                        inTour = 1;
                        break;
                    }
                }
                if(distances[element -> current_city][i] != 0 && inTour != 1){
                    int cf;
                    int ct;
                    int min1f = 0;
                    int min2f = 0;
                    int min1t = 0;
                    int min2t = 0;
                    int distance = distances[element -> current_city][i];
                    for(j=0;j<n_cities;j++){
                        int distancef = distances[element -> current_city][j];
                        int distancet = distances[i][j];
                        if(distancet != 0){
                            if(min1t == 0 || distancet <= min1t){
                                min2t = min1t;
                                min1t = distancet;

                            }
                            else if(min2t == 0 || distancet <= min2t){
                                min2t = distancet;
                            }
                        }
                        if(distancef != 0){
                            if(min1f == 0 || distancef <= min1f){
                                min2f = min1f;
                                min1f = distancef;

                            }
                            else if(min2f == 0 || distancef <= min2f){
                                min2f = distancef;
                            }
                        }

                    }
                    if(distance >= min2f){
                        cf = min2f;
                    }else{
                        cf = min1f;
                    }
                    if(distance >= min2t){
                        cf = min2t;
                    }else{
                        cf = min1t;
                    }
                
                    int newBound = element -> bound + distance - (cf + ct)/2;
                    int newLength = element -> length + 1;
                    if(newBound <= bestTourCost){
                        int* newTour = (int*)malloc(newLength * sizeof(int));
                        for (int j = 0; j < element -> length; j++) {
                                newTour[j] = element -> tour[j];
                            }
                        newTour[element -> length] = i;
                        
                        int newCost = element -> cost + distances[element ->current_city][i];
                        struct Element* newElement;
                        newElement -> tour = newTour;
                        newElement -> bound = newBound;
                        newElement -> current_city = i;
                        newElement -> length = newLength;
                        newElement -> cost = newCost;
                        queue_push(queue, newElement);
                    }
                    }
                }
            }
        }

    if (bestTourCost >= max_value){
        printf("NO SOLUTION");
        return 1;
    }
    else{
    printf("%d\n", bestTourCost);
    for(int j = 0; j < bestTourLength; j++){
        printf("%d ", bestTour[j]);
    }
    printf("0");
    return 0;
    }
    }
