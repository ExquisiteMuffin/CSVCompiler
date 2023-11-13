#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define constant array sizes
#define BUFF 100
#define COLS 6
#define ROWS 580

typedef struct node {
  char *attribute;
  struct node *next;
} node;

// Set hash table
node *table[COLS];

bool load() 
{
  FILE *fp = fopen("ref.csv", "r");
  char buffer[BUFF];
  if (!fp) {
    printf("Cannot open file!\n");
  }

  bool isHeader = true;
  while (fgets(buffer, BUFF, fp)) 
  {
    //Skip the header to avoid prepending the column titles
    if (isHeader) {
      isHeader = false;
      continue;
    }
    //printf("%s", buffer);
    char *value = strtok(buffer, ",");
    int columnIndex = 0; //Index to add each attribute type to a separate linked list
    while (value != NULL && columnIndex <= COLS) 
    {
      node *new = malloc(sizeof(node)); //Temporary node to prepennd to linked list
      new->attribute = strdup(value); //Copies the token value to a new memory address
      new->next = NULL;
      new->next = table[columnIndex];
      table[columnIndex] = new;
      columnIndex++;
      value = strtok(NULL, ",");
    }
  }
  fclose(fp);
  return true;
}

void unload() //Unloads the linked list and frees the memory (so that valgrind doesn't get mad at me)
{
  node *ptr;
  for (int i = 0; i < COLS; i++)
  {
    ptr = table[i];
    while (ptr != NULL)
    {
      node *next = ptr->next;
      free(ptr);
      ptr = next;
    }
  }
}

void printSorted(int nodes, int index)
{
  if (nodes == 1)
  {
    //Free memory and print sorted tokens
    node *ptr = table[index];
    while (ptr != NULL)
    {
      node *next = ptr->next;
      printf("%s\n", ptr->attribute);
      ptr = next;
    }
  }
  else if (nodes == 2)
  {
    //Free memory and print sorted tokens
    node *ptr1 = table[0];
    node *ptr2 = table[index];
    while (ptr2 != NULL)
    {
      node *next1 = ptr1->next;
      node *next2 = ptr2->next;
      printf("%s -> %s\n", ptr1->attribute, ptr2->attribute);
      ptr1 = next1;
      ptr2 = next2;
    }
  }
}

bool sort(int index, char condition[7]) 
{
  if (index == 0 && strcmp(condition, "desc") == 0) 
  {
    int c = 0;
    int swaps = 0;
    while (c != ROWS) 
    {
      node *ptr = table[index];
      while (ptr->next != NULL) 
      {
        node* next = ptr->next;
        if (strcmp(next->attribute, ptr->attribute) > 0) 
        {
          //Swap the two attributes in the linked list without reconstructing the linked list
          char *temp = next->attribute;
          next->attribute = ptr->attribute;
          ptr->attribute = temp;
        }
        ptr = next; //Sets the pointer to the next node, thuse looping through the linked list further
      }
      c++;
    }
    printSorted(1, index);
  }
  else if (index == 0 && strcmp(condition, "asc") == 0)
  {
    printf("hi\n");
    int c = 0;
    while (c != ROWS+1) 
    {
      node *ptr = table[index];
      while (ptr->next != NULL) 
      {
        node* next = ptr->next;
        if (strcmp(next->attribute, ptr->attribute) < 0) 
        {
          //Swap the two attributes in the linked list without reconstructing the linked list
          char *temp = next->attribute;
          next->attribute = ptr->attribute;
          ptr->attribute = temp;
        }
        ptr = next; //Continues the loop
      }
      c++;
    }
    printSorted(1, index);
  }
  else if (index == 2 && strcmp(condition, "n") == 0)
  {
    int c = 0;
    while (c != ROWS+1)
    {
      node *ptr1 = table[0];
      node *ptr2 = table[index];
      while (ptr2->next != NULL) 
      {
        node *next1 = ptr1->next;
        node *next2 = ptr2->next;
        if (strcmp(next2->attribute, ptr2->attribute) < 0)
        {
          char *temp1 = next1->attribute;
          char *temp2 = next2->attribute;

          next1->attribute = ptr1->attribute;
          next2->attribute = ptr2->attribute;

          ptr1->attribute = temp1;
          ptr2->attribute = temp2;
        }
        ptr1 = next1;
        ptr2 = next2;
      }
      c++;
    }
    printSorted(2, index);
  }
  else if (index == 4 && strcmp(condition, "asc") == 0)
  {
    int c = 0;
    while (c != ROWS+1)
    {
      node *ptr1 = table[0];
      node *ptr2 = table[index];
      while (ptr2->next != NULL)
      {
        node *next1 = ptr1->next;
        node *next2 = ptr2->next;
        if (atoi(ptr2->attribute) < atoi(next2->attribute))
        {
          char *temp1 = next1->attribute;
          char *temp2 = next2->attribute;

          next1->attribute = ptr1->attribute;
          next2->attribute = ptr2->attribute;

          ptr1->attribute = temp1;
          ptr2->attribute = temp2;
        }
        ptr1 = next1;
        ptr2 = next2;
      }
      c++;
    }
    printSorted(2, index);
  }
  else if (index == 4 && strcmp(condition, "desc") == 0)
  {
    int c = 0;
    while (c != ROWS+1)
    {
      node *ptr1 = table[0];
      node *ptr2 = table[index];
      while (ptr2->next != NULL)
      {
        node *next1 = ptr1->next;
        node *next2 = ptr2->next;
        if (atoi(ptr2->attribute) > atoi(next2->attribute))
        {
          char *temp1 = next1->attribute;
          char *temp2 = next2->attribute;

          next1->attribute = ptr1->attribute;
          next2->attribute = ptr2->attribute;

          ptr1->attribute = temp1;
          ptr2->attribute = temp2;
        }
        ptr1 = next1;
        ptr2 = next2;
      }
      c++; 
    }
    printSorted(2, index);
  }
  return true;
}

int main(void) 
{
  char query[1];
  char next[12];
  while (true) 
  {
    printf("Continue sorting? ('y' or 'n'): \n");
    scanf("%s", query);
    if (strcmp(query, "y") == 0) 
    {
      if (load()) 
      {
        //move everything in here once back
        printf("Sort by ('genre', 'title', or 'money'): \n");
        scanf("%s", next);

        if (strcmp(next, "genre") == 0) 
        {
          sort(2, "n");
        } 
        else if (strcmp(next, "title") == 0) 
        {
          char condition[7];
          printf("Type 'asc' or 'desc': \n");
          scanf("%s", condition);
          if (strcmp(condition, "desc") != 0 && strcmp(condition, "asc") != 0)
          {
            printf("Please type a valid response!\n");
            continue;
          }
          sort(0, condition);
        } 
        else if (strcmp(next, "money") == 0) 
        {
          char condition[4];
          printf("Type 'asc' or 'desc': \n");
          scanf("%s", condition);
          if (strcmp(condition, "asc") != 0 && strcmp(condition, "desc") != 0)
          {
            printf("Please type a valid response!\n");
            continue;
          }
          sort(4, condition);
        } 
        else 
        {
          printf("Invalid option!\n");
        }
      } 
      else if (strcmp(query, "n") == 0) 
      {
        printf("Alright. Peace out jit.");
        break;
      } 
      else 
      {
        printf("Invalid input. Please enter 'y' or 'n'.\n");
        continue;
      }
    }
  }
  unload();
  return 0;
}
