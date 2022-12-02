/*Isabella Faile
  COP3502 Professor Guha
  P2 assignment
  9/25/21*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 16
#define MAXLETTERS 2

float perm(int array[], int length, int k, int used[]);
float getCost(int *array, int length);
void init(int indance[][MAXLETTERS], int size, int value);
double sqrtNum(double x);

// store coordinates
int tree[N][MAXLETTERS];

int main(void) {
    float answer;
    //get numcases
    int numCases;
  
    scanf("%d", &numCases);
    int count =0;
    while (count < numCases){
        int numTrees;
       
        scanf("%d", &numTrees);

        // Set everything to 0.
        init(tree, N, 0);

        //fill the coordinates
        for(int i=0;i<=(2*numTrees)-1;i++){
               
                scanf("%d",&tree[i][0]);
              
                scanf("%d",&tree[i][1]);
                }

        // brute force
        int used[N];
        for (int i=0; i<N; i++){
            used[i] = 0;
        }
        int order[N];
        answer = perm(order, 2*numTrees, 0, used);
        printf("\n %0.3f", answer);

        count++;
    }

    return 0;
}

// Initializes everything to 0
void init(int indance[][MAXLETTERS], int size, int value) {
    for (int i=0; i<size; i++)
        for (int j=0; j<MAXLETTERS; j++)
            indance[i][j] = value;
}


// Gets lowest distance out of all possible pairs
float perm(int order[], int numTrees, int k, int used[]) {

    // Permutation is done
    if (k == numTrees){
        return getCost(order, numTrees);
    }

    //
    float res = 1000;

    // Try each in slot k
    for (int i=0; i<numTrees; i++) {
        if (used[i]) continue;
        order[k] = i;
        used[i] = 1;
        float thisRes = perm(order, numTrees, k+1, used);
        if (thisRes < res) res = thisRes;
        used[i] = 0;
    }

    return res;
}

// Returns the distance
float getCost(int *order, int numTrees) {
    float distance = 0;
    float total = 0;
      float f,e;

    for(int i=0; i<numTrees/2;i++){

     

      f = (tree[order[i]][0]-tree[order[i+1]][0])*(tree[order[i]][0]-tree[order[i+1]][0]);
      e = (tree[order[i]][1]-tree[order[i+1]][1])*(tree[order[i]][1]-tree[order[i+1]][1]);
      distance = f+e;
      distance = sqrtNum(distance);
      total = distance + total;
    }
    

    return total;
}

//square root function
double sqrtNum(double x){
    double low =1;
    double high =x;
    if(x<1){
        low =x;
        high=1;
    }
    for(int i=0;i<60; i++){
        double mid=(low+high)/2;
        if(mid*mid<x){
            low=mid;
        }
        else{
            high=mid;
        }
    }

    return low;
}

