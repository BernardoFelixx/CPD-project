#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

    return 0;

}