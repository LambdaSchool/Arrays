#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array
{
  int capacity;    // How many elements can this array hold?
  int count;       // How many states does the array currently hold?
  char **elements; // The string elements contained in the array
} Array;

/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array(int capacity)
{
  // Allocate memory for the Array struct
  Array *array = malloc(sizeof(Array));

  // Set initial values for capacity and count
  array->capacity = capacity;
  array->count = 0;

  // Allocate memory for elements
  array->elements = calloc(capacity, sizeof(char *));
  return array;
}

/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr)
{

  // Free all elements
  free(arr->elements);

  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr)
{
  // Create a new element storage with double capacity
  arr->capacity *= 2;
  arr->elements = realloc(arr->elements, arr->capacity * sizeof(char *));
  // printf("resize_array complete\n"); // <--debugging

  // Copy elements into the new storage
  // handled in the realloc call

  // Free the old elements array (but NOT the strings they point to)

  // Update the elements and capacity to new values
}

/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index)
{
  // Throw an error if the index is greater than the current count
  if (arr->count < index)
  {
    printf("Given index is higher than the size of the array\n");
    return NULL;
  }
  // Otherwise, return the element at the given index
  else
  {
    // printf("arr_read returning %s\n", arr->elements[index]); // <-debugging
    return arr->elements[index];
  }
}

/*****
 * Insert an element to the array at the given index
 *****/
void arr_insert(Array *arr, char *element, int index)
{
  // Throw an error if the index is greater than the current count
  if (index > arr->count)
  {
    printf("Given index is higher than the size of the array");
    return;
  }

  // Resize the array if the number of elements is over capacity
  if (arr->capacity <= arr->count)
  {
    // printf("array capacity %d\n", arr->capacity); // // <--debugging
    // printf("resize_array initiated\n");           // <--debugging
    resize_array(arr);
    // printf("array capacity %d\n", arr->capacity); // // <--debugging
  }

  // Move every element after the insert index to the right one position
  for (int i = arr->count; i >= index; i--)
  {
    arr->elements[i + 1] = arr->elements[i];
  }

  // Copy the element and add it to the array
  char *copy = strdup(element);
  // printf("copy %s\n", copy); // <--debugging
  arr->elements[index] = copy;

  // Increment count by 1
  arr->count++;
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element)
{

  // Resize the array if the number of elements is over capacity
  if (arr->count == arr->capacity)
  {
    resize_array(arr);
  }

  // Copy the element and add it to the end of the array
  char *copy = strdup(element);
  arr->elements[arr->count] = copy;

  // Increment count by 1
  arr->count++;
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/

// adding an additional function to search
// the array for the index of an element
// to both use in arr_remove and to have
// accessable in general
int arr_find_index(Array *arr, char *element)
{
  for (int i = 0; i < arr->count; i++)
  {
    if (strcmp(arr->elements[i], element) == 0)
    {
      return i;
    }
  }
  return -1;
}

void arr_remove_item(Array *arr, char *element)
{
  // Search for the first occurence of the element and remove it.
  // Don't forget to free its memory!
  int index = arr_find_index(arr, element);
  if (index == -1)
  {
    printf("The requested element was not found in the array");
    return;
  }
  free(arr->elements[index]);

  // Shift over every element after the removed element to the left one position
  for (int i = index; i < arr->count; i++)
  {
    arr->elements[i] = arr->elements[i + 1];
  }
  arr->elements[arr->count] = NULL;

  // Decrement count by 1
  arr->count--;
}

void arr_remove_index(Array *arr, int index)
{
  // Search for the first occurence of the element and remove it.
  // Don't forget to free its memory!
  if (index <= arr->count)
  {
    printf("The index you requested does not exist");
    return;
  }
  free(arr->elements[index]);

  // Shift over every element after the removed element to the left one position
  for (int i = index; i < arr->count; i++)
  {
    arr->elements[i] = arr->elements[i + 1];
  }
  arr->elements[arr->count] = NULL;

  // Decrement count by 1
  arr->count--;
}

// utility functions to match more intuitive array
// expectations

void arr_push(Array *arr, char *element)
{
  arr_append(arr, element);
}

char *arr_pop(Array *arr)
{
  char *retval = arr->elements[arr->count - 1];
  arr_remove_index(arr, arr->count - 1);
  return retval;
}

void arr_shift(Array *arr, char *element)
{
  arr_insert(arr, element, 0);
}

char *arr_unshift(Array *arr)
{
  char *retval = arr->elements[0];
  arr_remove_index(arr, 0);
}

int arr_length(Array *arr)
{
  return arr->count;
}

void arr_sort(Array *arr)
{
  // timsort?
}

void arr_reverse(Array *arr)
{
  // in place
  char *temp;
  int length = arr->count - 1;
  int mid = (arr->count / 2);
  printf("length: %d midpoint: %d\n", length, mid);
  while (mid >= 0)
  {
    temp = arr->elements[mid];
    arr->elements[mid] = arr->elements[length - mid];
    arr->elements[length - mid] = temp;
    mid--;
  }
}

char **arr_copy(Array *arr)
{
  return realloc(arr->elements, arr->capacity * sizeof(char *));
}

/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr)
{
  printf("[");
  for (int i = 0; i < arr->count; i++)
  {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1)
    {
      printf(",");
    }
  }
  printf("]\n");
}

#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  // arr_insert(arr, "VALUE-1", 0);
  // printf("%s added to arr\n", arr->elements[0]);
  // printf("%s\n", arr_read(arr, 0));
  // arr_insert(arr, "VALUE-2", 0);
  // arr_insert(arr, "STRING1", 0);
  // arr_append(arr, "STRING4");
  // arr_insert(arr, "STRING2", 0);
  // arr_insert(arr, "STRING3", 1);
  // arr_print(arr);
  // arr_remove_item(arr, "STRING3");
  // arr_print(arr);
  arr_append(arr, "zero");
  arr_append(arr, "one");
  arr_append(arr, "two");
  arr_append(arr, "three");
  arr_append(arr, "four");
  arr_append(arr, "five");
  arr_append(arr, "six");
  arr_append(arr, "seven");
  arr_append(arr, "eight");
  arr_append(arr, "nine");
  arr_append(arr, "ten");
  arr_print(arr);
  arr_reverse(arr);
  arr_print(arr);

  // printf("is \'a\' = %d in c \n", 'a');

  destroy_array(arr);

  return 0;
}
#endif
