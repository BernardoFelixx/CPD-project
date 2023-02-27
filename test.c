#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main(){
    char str[] = "3 4";
    int n_cities;
    int n_roads;
    n_cities=atoi(strtok(str," "));
    n_roads=atoi(strtok(str," "));
    printf("%d, %d", n_cities, n_roads);
}