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

// Binary search for index, will return index even if exact value is not found
// This was made to optimize insertion sort.
static int binary_search_index(int *a, int n, int v)
{
    int first = 0;
    int last = n - 1;
    int middle;
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
    return first;

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

// insert value into array of length n -> length n+1
static inline void insertArraySorted(int * a, int n, int el)
{
    int k;
    k = binary_search_index(a, n+1, el);
    insertArray(a, n, k, el);
}


// O(n)
// Split array into lower and upper, return middle value 
// pre: *a is a list, n is number of elements
// post: put lower elements to the left and higher to the right and return last index in lower half (possibly negative
static int partition(int *a, int n)
{
    int pivot = a[0];
    int lower = 1;
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
                sorted = 0;
            }
        }
        if(sorted) return;
    }
}

// pre: 
// *a = array
// n = number of elements (n>=2)
// post:
// *a is sorted
void insertion_sort(int *a, int n)
{
    //int sorted_elements = 0;
    //for (int i = 0; i<n; i++)
    //{
    //    //ui_DEBUG_print_list(a,n);
    //    insertArraySorted(a, sorted_elements++, a[i]);
    //}
    for (int i = 1; i<n; i++)
    {
        int j = i;
        while (j>0 && a[j-1]>a[j])
        {
            swp(&a[j], &a[j-1]);
            j--;
        }
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
    if (n <= 1) return;
    int b = partition(a, n);
    quick_sort(a, b);         // lower
    quick_sort(a+b+1, n-b-1); // upper
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

    while (first <= last)
		{
        middle = (first + last) / 2;

    	  if(a[middle] < v)
				{
            first = middle + 1;
        }
        else if (a[middle] > v)
				{
            last = middle - 1;
        }
				else
					return true;
    }
    return false;
}
