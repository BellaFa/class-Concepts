/*Given a list of the number of pages in all the books in the school library, and a maximum limit of 
the number of pages  you are willing to read, determine the maximum  number of books you can 
fully read! */


#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000


//function prototypes
long long* fillPrices(long long* bookPrices, int numBooks);
void MergeSort(long long* values, int start, int end);
void Merge(long long* values, int start, int middle, int end);
long long minBooks(long long* list, int length, int pages);




// function to fill the array of prices
long long* fillPrices(long long* bookPrices, int numBooks){
    // variables
    int price=0;

    // allocate memory for array
    bookPrices = malloc(numBooks * sizeof(long long*));

    // Fill in book price values
    for(int i=0;i<numBooks;i++){
        bookPrices[i] = (long long)malloc(sizeof(long long)*SIZE);
        scanf("%lld", &bookPrices[i]);
    }

    return bookPrices;
}

// function to sort the book prices in order
void MergeSort(long long* values, int start, int end) {

    int mid;

    // Check if sorting range is more than one element.
    if (start < end) {

        mid = (start+end)/2;

        // Sort the first half
        MergeSort(values, start, mid);

        // Sort the second half
        MergeSort(values, mid+1, end);

        // combine
        Merge(values, start, mid+1, end);
    }
}

void Merge(long long* values, int start, int middle, int end) {

    //printf("merge %d, %d, %d\n", start, middle, end);

    int *temp, i, length, count1, count2, mc;

    // Allocate the proper amount of space for our auxiliary array.
    length = end - start + 1;
    temp = (int*)calloc(length, sizeof(int));

    // These will be our indexes into our two sorted lists.
    count1 = start;
    count2 = middle;

    // Keeps track of our index into our auxiliary array.
    mc = 0;

    while ((count1 < middle) || (count2 <= end)) {

        if (count2 > end || (count1 < middle && values[count1] < values[count2])) {
            temp[mc] = values[count1];
            count1++;
            mc++;
        }

        // We copy the next value from list two.
        else {
            temp[mc] = values[count2];
            count2++;
            mc++;
        }
    }

    // Copy back all of our values into the original array.
    for (i=start; i<=end; i++)
        values[i] = temp[i - start];

    free(temp);
}

// function to calculate minimum price
long long minBooks(long long* list, int length, int pages){
long long count=0;
int total=0;
    for(int i=0; i<length; i++){
        if(total<=pages &&total+list[i]<=pages){
        count++;
        total = total + list[i];
        }
    }
    return count;
}

int main()
{
    // variables
    long long *bookPrices;
    bookPrices = (long long*)(malloc(sizeof(long long)*SIZE)); //long long array //maximum pages has to be a long long
    int numBooks;
    long long pages;
    int numCases;
    int numRead=0;
    int count = 0;

    // Get amount of cases
    scanf("%d", &numCases);

    while(count<numCases){
    //Get number of books in library, and total pages that can be read
      scanf("%d", &numBooks);
      scanf("%lld", &pages);

    // call function to fill the array
        bookPrices = fillPrices(bookPrices, numBooks);

    // call function to sort the array
         MergeSort(bookPrices,0,numBooks-1);

    // call function to calculate maximum books, print final answer
        numRead = minBooks(bookPrices, numBooks, pages);
        printf("%d\n", numRead);

        free(bookPrices);
        count++;
    }
    return 0;
}
