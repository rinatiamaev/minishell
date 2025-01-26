/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlouis <nlouis@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 09:00:55 by nlouis            #+#    #+#             */
/*   Updated: 2024/12/13 10:36:30 by nlouis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Function to swap two integers in the array. */
static void	swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/* Function to rearrange the array based on a pivot value and return
the pivot index:

`pivot` is the element used to divide the array.
`index` tracks the position of the last element smaller than the pivot.
`i` is used to iterate through the array.

`pivot` is assigned to the last element in the array as the pivot.
`index` is assigned to one position before the start of the range.
`i` is assigned to start for iterating from the beginning of the range.

The `while loop` iterates through the array from `start` to `end-1`,
if the current element is less than the pivot:
	- the index is moved to the next position,
	- the element at `i` is swapped with the element at `index`,
`i` is incremented to move to the next element.

The `pivot` is swapped with the element at `index+1`, placing it in its
correct position.

The function returns the index of the pivot, which now divides the array
into two parts:
	- Elements smaller than the pivot on the left.
	- Elements greater than or equal to the pivot on the right.
 */
static int	partition(int *array, int start, int end)
{
	int	pivot;
	int	index;
	int	i;

	pivot = array[end];
	index = start - 1;
	i = start;
	while (i < end)
	{
		if (array[i] < pivot)
		{
			index++;
			swap(&array[index], &array[i]);
		}
		i++;
	}
	swap(&array[index + 1], &array[end]);
	return (index + 1);
}

/*
Recursive function to sort the array using QuickSort.

`pivot` stores the pivot index.

If `start < end` ensures that the recursion stops when the `start` index meets
or surpasses the `end` index, which happens when the subarray being processed
is empty or contains a single element.
	- partitions the array and gets the pivot index
	- recursively sort the left part of the array
	- recursively sort the right part of the array
*/
static void	quicksort(int *array, int start, int end)
{
	int	pivot;

	if (start < end)
	{
		pivot = partition(array, start, end);
		quicksort(array, start, pivot - 1);
		quicksort(array, pivot + 1, end);
	}
}

/*
Public function to sort an array using QuickSort. Acts as an entry point.
- If the array has one or no elements, it doesnt need sorting.
- Call the QuickSort function on the entire array.
*/
void	ft_qsort(int *array, int size)
{
	if (size <= 1)
		return ;
	quicksort(array, 0, size - 1);
}

/*
1. Initial Call:
	`ft_qsort(array, size);`
	size > 1, so the quicksort function is called with start = 0 and end = 6.

2. First Call to quicksort:
	`quicksort(array, 0, 6);`
	The pivot is array[end] = 5.
	The partition function is called to rearrange the array around the pivot.

	First Call to partition:

	Initial Array: {8, 3, 7, 4, 6, 2, 5}
	Pivot = 5, Start = 0, End = 6

	Step-by-Step Partitioning:

	index = -1 (one before start).

	Iterate from start to end - 1:

	start_index	Value	Action					Array 			State	index
	0			8	No action (8 > 5)			{8, 3, 7, 4, 6, 2, 5}	-1
	1			3	Swap with array[index + 1]	{3, 8, 7, 4, 6, 2, 5}	0
	2			7	No action (7 > 5)			{3, 8, 7, 4, 6, 2, 5}	0
	3			4	Swap with array[index + 1]	{3, 4, 7, 8, 6, 2, 5}	1
	4			6	No action (6 > 5)			{3, 4, 7, 8, 6, 2, 5}	1
	5			2	Swap with array[index + 1]	{3, 4, 2, 8, 6, 7, 5}	2
	After the loop, swap the pivot (5) with array[index + 1]:

Final Array: {3, 4, 2, 5, 6, 7, 8}
Pivot index returned: 3.

3. Recursive Calls to quicksort:
	`quicksort(array, 0, 2)` for the left subarray.
	`quicksort(array, 4, 6)` for the right subarray.

#Sorting the Left Subarray ({3, 4, 2}):
	#First Recursive Call:
	`quicksort(array, 0, 2);`
	Pivot = 2.
	partition rearranges:
	Final Array: {2, 4, 3, 5, 6, 7, 8}
	Pivot index returned: 0.
	
	#Recursive calls:
	`quicksort(array, 0, -1)` → No action (base case: start >= end).
	`quicksort(array, 1, 2)` for {4, 3}:
	Pivot = 3.
	Rearranged to {2, 3, 4, 5, 6, 7, 8}.
	Recursive calls:
	`quicksort(array, 1, 0)` → No action.
	`quicksort(array, 2, 2)` → No action.

#Sorting the Right Subarray ({6, 7, 8}):
	#First Recursive Call:
	`quicksort(array, 4, 6);`
	Pivot = 8.
	partition rearranges:
	Final Array: {2, 3, 4, 5, 6, 7, 8} (no swaps needed).
	Pivot index returned: 6.
	
	#Recursive calls:
	`quicksort(array, 4, 5)` for {6, 7}:
	Pivot = 7.
	Rearranged to {2, 3, 4, 5, 6, 7, 8} (no swaps needed).
	Recursive calls:
	`quicksort(array, 4, 4)` → No action.
	`quicksort(array, 5, 5)` → No action.
	`quicksort(array, 7, 6)` → No action.

#Final Sorted Array:
	After all recursive calls finish, the array becomes:
	{2, 3, 4, 5, 6, 7, 8}

#Visualization of Recursive Calls:
	ft_qsort({8, 3, 7, 4, 6, 2, 5}, 7)
	├── quicksort({3, 4, 2, 5, 6, 7, 8}, 0, 2)
	│     ├── quicksort({2, 3, 4, 5, 6, 7, 8}, 0, -1)
	│     ├── quicksort({2, 3, 4, 5, 6, 7, 8}, 1, 2)
	│     │     ├── quicksort({2, 3, 4, 5, 6, 7, 8}, 1, 0)
	│     │     └── quicksort({2, 3, 4, 5, 6, 7, 8}, 2, 2)
	├── quicksort({2, 3, 4, 5, 6, 7, 8}, 4, 6)
			├── quicksort({2, 3, 4, 5, 6, 7, 8}, 4, 5)
			│     ├── quicksort({2, 3, 4, 5, 6, 7, 8}, 4, 4)
			│     └── quicksort({2, 3, 4, 5, 6, 7, 8}, 5, 5)
			└── quicksort({2, 3, 4, 5, 6, 7, 8}, 7, 6)
*/