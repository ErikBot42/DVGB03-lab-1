#include "algorithm.h"
#include "stdio.h"
#include "ui.h"
//
// Private
//

// post: swap the value the pointers point to.
static inline void swp (int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

//Binary search for index, will return index even if exact value is not found
static int binary_search_index(int *a, int n, int v)
{
    int first = 0;
    int last = n - 1;
    int middle;
    //ui_DEBUG_print_list(a, n);
    while (first < last)
    {
        middle = (first + last)/2;
        if (a[middle] < v)
        {
            first = middle + 1;
        }
        else
        {
            last = middle;
        }
    }
    //printf("debug> insert element %d at a[%d] = %d\n", v, first, a[first]);
    return first;

    //while (first <= last){
    //    middle = (first + last) / 2;

    //    if (a[middle] == v) {
    //        return middle;
    //    }
    //    else if(a[middle] < v){
    //        first = middle + 1;
    //    }
    //    else {
    //        last = middle - 1;
    //    }
    //}
    //return -1; 
}

// insert assuming 1 space was allocated after array
static inline void insertArray(int * a, int n, int index, int el)
{
    int insertedValue = el;
    for (int i = index; i<(n+1); i++)
    {
        swp(&insertedValue, &a[i]); 
    }
}

static inline void insertArraySorted(int * a, int n, int el)
{
    int k;// = n;
    //for (int i = 0; i<n; i++)
    //{
    //    if (a[i]>el)
    //    {
    //        k = i;
    //        break;
    //    }
    //}
    //printf("debug> a[%d] = %d\n", k, a[k]);
    k = binary_search_index(a, n+1, el);
    insertArray(a, n, k, el);
}



//Split array into lower and upper, return middle value 
// pre: *a is a list, n is number of elements
// post: put lower elements to the left and higher to the right and return last index in lower half (possibly negative
static int partition(int *a, int n)
{
    int pivot = a[0];
    int lower = 0;
    int upper = n - 1;

    do
    {
        while(a[lower] <= pivot && lower <= upper) lower++;
        while(a[upper] > pivot && lower <= upper) upper--;

        if(lower <= upper)
        {
            swp(&a[upper], &a[lower]);

            // if elements are swapped, they are sorted
            upper--; lower++;
        }
    } while(lower <= upper);
    swp(&a[upper], &a[0]);

    return upper;
}
//
// Public
//
//
// pre: 
// *a = array
// n = number of elements (n>=2)
// post:
// *a is sorted
void bubble_sort(int *a, int n)
{
    for(int j = 1; j < n; j++){
        int sorted = 1;
        for (int i = 0; i < n - j; i++) {
            if(a[i] > a[i+1]) {
                swp(&a[i], &a[i+1]);
                //int temp = a[i];
                //a[i] = a[i + 1];
                //a[i + 1] = temp;
                sorted = 0;
            }
        }
        if(sorted) return;
    }
}
// TODO: done
//}

// pre: 
// *a = array
// n = number of elements (n>=2)
// post:
// *a is sorted
void insertion_sort(int *a, int n)
{
    int sorted_elements = 0;
    for (int i = 0; i<n; i++)
    {
        //ui_DEBUG_print_list(a,n);
        insertArraySorted(a, sorted_elements++, a[i]);
    }
}

// pre: 
//*a = array
// n = number of elements (n>=2)
// post:
// *a is sorted
void quick_sort(int *a, int n);

void quick_sort(int *a, int n)
{
    //static int iter = 0;
    //printf("%d\n", iter++);
    //fflush(stdout);

    if (n <= 1) return;

    int b = partition(a, n);
    //printf("a[%d] = %d, ", b, a[b]); ui_DEBUG_print_list(a, n);

    quick_sort(a, b); //sorts lower to mid
    quick_sort(a+b+1, n-b-1); 

    //quick_sort(a, tal); //sorts lower to mid
    //quick_sort(a + tal, n - tal); //sorts mid to end

    // TODO: quick sort
}

bool linear_search(const int *a, int n, int v)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] == v)
            return true;
    }
    return false; 
}

bool binary_search(const int *a, int n, int v)
{
    int first = 0;
    int last = n - 1;
    int middle;

    while (first <= last){
        middle = (first + last) / 2;

        if (a[middle] == v) {
            return true;
        }
        else if(a[middle] < v){
            first = middle + 1;
        }
        else {
            last = middle - 1;
        }
    }
    return false; // TODO: optimization
}
