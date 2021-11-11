#include "algorithm.h"
#include "stdio.h"
//
// Private
//

//Binary search for index
int binary_search_index(const int *a, int n, int v)
{
	int first = 0;
  int last = n - 1;
  int middle;

  while (first <= last){
  middle = (first + last) / 2;

    if (a[middle] == v) {
    return middle;
    }
    else if(a[middle] < v){
    first = middle + 1;
    }
    else {
    last = middle - 1;
    }
  }
	return -1; 
}

//Split array into lower and upper, return middle value 
int partition(int *a, int n)
{
	int pivot, lower, upper;
	pivot = a[0];
	lower = 0;
	upper = n - 1;

	do
	{
		while(a[lower] <= pivot && lower <= upper) lower++;
		while(a[upper] > pivot && lower <= upper) upper--;

		if(lower <= upper)
		{
			int temp = a[upper];
			a[upper] = a[lower];
			a[lower] = temp;
		}
	} while(lower <= upper);

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
       	int temp = a[i];
      	a[i] = a[i + 1];
      	a[i + 1] = temp;
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
	// TODO: insertion sort
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
	return false; // TODO: done
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
