
/*Given a list of the number of groups, the initial sizes of each group, the skip value for each group
and the threshold value for each group, determine the winner of the lottery (group number and
person number). In addition to determining the lottery winner, you must print a log for each group
of which people get eliminated (in the order that they are eliminated from contention of winning
the lottery.)*/

#include <stdio.h>
#include <stdlib.h>

//Function Prototypes
struct node* fillList(struct node *front);
struct node* insert(struct node *front, int num);
struct node* addPerson(struct node *front, int num);
int counter(struct node *front);
struct node* removePerson(struct node* list);
int findValue(struct node* list);
struct node* deletePerson(struct node* list, int skip,int threshhold);

//Structs
struct node{
    int numPerson;
    struct node *next; //pointer to next element
};

//function to fill list
struct node* fillList(struct node *front){
    //variables
    int numPeople=0;


    scanf("%d", &numPeople);

    for(int i=1;i<=numPeople;i++){
        front = addPerson(front, i);
    }
    return front;
};

struct node* insert(struct node *front, int num) {

  struct node *temp, *iter;

  // Create temp node and store the value.
  temp = (struct node*)malloc(sizeof(struct node));
  temp->numPerson = num;

  // Case if empty
  if (front == NULL) {

     // linking to itself
     temp->next = temp;
  }

  // adding to a list with something in it already
  else {

     // Link new node to the first element in the old list.
     temp->next = front;

     // Iterate to the last element in the old list.
     iter = front;
     while (iter->next != front)
       iter = iter->next;

     //  relinks to the new first element, temp.
     iter->next = temp;
  }
}



//function adds element to list
struct node* addPerson(struct node *front, int i) {
  struct node *temp;
  struct node *iter;
  struct node *savelink;

  // insert into the front of the list.
  if ((front == NULL)|| i < front->numPerson) {
    return insert(front, i);
  }

  // Create new node.
  temp = (struct node*)malloc(sizeof(struct node));
  temp->numPerson = i;
  temp->next = NULL;

  // Start iter at the front of the linked list.
  iter = front;

  // cycle through list
  while (iter->next != front && iter->next->numPerson < i)
    iter = iter->next;


  // Save the node 
  savelink = iter->next;

  // Insert temp.
  iter->next = temp;
  temp->next = savelink;

  // Return a pointer to the front of the list.
  return front;
}

//gets total number of elements in list
int counter(struct node *front){
    int total = 0;
    struct node *current = front;

    // Iterate till end of list
    do
    {
        current = current->next;
        total++;
    } while (current != front);

    // Return total elements in list
    return total;
}

//gets skip and threshold values and calls deletePerson
struct node* removePerson(struct node* front){
    //variables
    int skip=0;
    int threshold=0;

    scanf("%d", &skip);

    scanf("%d", &threshold);

        front = deletePerson(front, skip, threshold);


    return front;

}

//function deletes people until threshold is reached
struct node* deletePerson(struct node* front, int skip, int threshold){

    struct node *curr = front;
    struct node *prev;
    int start =0;

	while(counter(front) != threshold){

    //initial case if front is empty
	if (front == NULL)
        return front;

    //account for when at first element for first calculation (skip)
    if(start==0){
        for(int j=0; j<skip;j++){
            prev = curr;
            curr = curr->next;
        }
    }
    //after first element for first calculation (skip+1)
    if(start>0){
        for(int j=0; j<skip+1;j++){
            prev = curr;
            curr = curr->next;
        }
    }

    //if first
    if (curr == front)
    {
        prev = front;
        while (prev->next != front)
            prev = prev->next;
        front = curr->next;
        prev->next = front;

        printf("%d\n", curr->numPerson);
    }

    // if last
    else if (curr->next == front && curr == front)
    {
        prev->next = front;
        printf("%d\n", curr->numPerson);
    }
    //if in middle
    else
    {
        prev->next = curr->next;
        printf("%d\n", curr->numPerson);
    }


    start++;

   }
   //free current
   free(curr);

    return front;

}


//Function finds lowest value in node
int findValue(struct node* front){
    //case for if list is empty
    if (front == NULL) {
        return 0;
    }

    // pointer to go through list
    struct node* current;

    // current pointer equals front
    current = front;

    // first item in list is min
    int min = current->numPerson;

    // While last node is not reached
    do {

        //if next item is less, update min
        if (current->numPerson < min) {
            min = current->numPerson;
        }

        current = current->next;
    }while(current != front);

    return min;
}



int main()
{
    //variables
    int numCases=0;
    int numGroups=0;
    int count=0;
    int compare=0;
    int groupWin=0;
    int personWin=0;
    struct node *list=NULL;

    //get numCases
   
    scanf("%d", &numCases);

    //loop the amount of cases
    while(count<numCases){

    //variables set to zero again for next case
    compare=0;
    groupWin=0;
    personWin=0;
    
    //get number of groups
   
    scanf("%d", &numGroups);

        //loop the amount of groups
        for(int i=1; i<=numGroups; i++){
            //struct node *list=NULL;
            list =NULL;
            printf("\nGroup #%d:\n",i);
            //call function to make linked list
            list = fillList(list);

            //call function to return final list after removal process
            list = removePerson(list);

            //initialize compare to first node lowest value
            if(i==1){
                compare = findValue(list) + i;
                groupWin = i;
                personWin = findValue(list);
            }
            //if next groups lowest value is less than prior group update compare
            int tmp = findValue(list)+1;
            if(tmp < compare && i != 1){
                compare = tmp;
                groupWin = i;
                personWin = tmp-1;
            }

        }
        //print lottery winner
        printf("Lottery winner is person %d from group %d.\n", personWin, groupWin);
        free(list);

    count++;
    }

    return 0;
}
