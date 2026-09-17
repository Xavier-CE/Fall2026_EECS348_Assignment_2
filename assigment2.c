/*
 * Program Name: EECS 348 Assignment 2
 *
 * Description:
 * This C program implements a max heap data structure for managing emails
 * based on their sender priority and date.
 *
 * Input:
 * Email details (sender, subject, date) and commands entered through
 * standard input.
 *
 * Output:
 * Messages displaying the next email, email counts, and error messages.
 *
 * Collaborators:
 * None
 *
 * Other Sources:
 * Copilot and Gemini
 *
 * Author:
 * Xavier Eddins
 *
 * Creation Date:
 * September 17, 2026
 *
 * Revision Date:
 * September 17, 2026
 *
 * Revisions:
 * Improved comments, input validation, memory error checking,
 * and program organization.
 */

#include <stdio.h> // Include standard input/output library for printf and scanf functions
#include <stdlib.h> // Include standard library for memory allocation functions
#include <string.h> // Include string library for string manipulation functions

#define MAX_STR 200 // Define a constant for the maximum string length used in email fields
#define INITIAL_CAPACITY 10 // Define a constant for the initial capacity of the max heap

typedef struct { // Define a structure to represent an email with sender, subject, date, and priority
    char sender[MAX_STR];
    char subject[MAX_STR];
    char date[MAX_STR];
    int priority;
} Email; // The name of the structure is Email

typedef struct { // Define a structure to represent a max heap
    Email *data; // Pointer to an array of Email structures that will hold the emails in the heap
    int size; // Integer to keep track of the current number of emails in the heap
    int capacity; // Integer to keep track of the maximum number of emails the heap can hold before needing to resize
} MaxHeap; // The name of the structure is MaxHeap

void trim(char *str) { // Function to remove leading and trailing whitespace from a string
    int start = 0; // Initialize a variable to track the starting index of the non-whitespace characters
    int end; // Declare a variable to track the ending index of the non-whitespace characters
    int length; // Declare a variable to store the length of the string

    if (str == NULL) { // Check if the input string is NULL
        return; // If it is NULL, return immediately as there is nothing to trim
    }

    length = strlen(str); // Calculate the length of the string using the strlen function

    // Find the first non-space character.
    while (start < length && // Loop to find the index of the first non-whitespace character
           (str[start] == ' ' || str[start] == '\t')) { // Check if the current character is a space or a tab
        start++;
    }

    // Find the last non-space character.
    end = length - 1;

    while (end >= start && // Loop to find the index of the last non-whitespace character
           (str[end] == ' ' || str[end] == '\t' || // Check if the current character is a space, tab, newline, or carriage return
            str[end] == '\n' || str[end] == '\r')) { // Check if the current character is a newline or carriage return
        end--; // Decrement the end index to move left in the string
    }

    // Move the trimmed string to the beginning.
    if (end >= start) { // Check if there are non-whitespace characters in the string
        memmove(str, str + start, end - start + 1); // Use memmove to shift the non-whitespace characters to the beginning of the string
        str[end - start + 1] = '\0'; // Null-terminate the string after the last non-whitespace character
    } else { // If the string is all whitespace, set it to an empty string
        str[0] = '\0';
    }
}

int validDate(const char *date) { // Function to validate the format of a date string in the format "MM-DD-YYYY"
    int mm, dd, yyyy; // Declare variables to hold the month, day, and year extracted from the date string
    char extra; // Declare a variable to check for any extra characters after the expected date format

    if (sscanf(date, "%d-%d-%d%c", // Use sscanf to parse the date string into month, day, year, and check for extra characters
               &mm, &dd, &yyyy, &extra) != 3) {
        return 0;
    }

    // Check the month.
    if (mm < 1 || mm > 12) {// Validate that the month is between 1 and 12
        return 0;
    }

    // Check the day.
    if (dd < 1 || dd > 31) { // Validate that the day is between 1 and 31
        return 0;
    }

    // Check the year.
    if (yyyy < 1) {// Validate that the year is a positive integer
        return 0;
    }

    return 1;
}

int senderPriority(const char *sender) { // Function to assign a priority value based on the sender of the email
    if (strcmp(sender, "Boss") == 0) {// Check if the sender is "Boss"
        return 50000;// Assign the highest priority value for "Boss"
    }

    if (strcmp(sender, "Subordinate") == 0) {// Check if the sender is "Subordinate"
        return 40000;// Assign a high priority value for "Subordinate"
    }

    if (strcmp(sender, "Peer") == 0) {// Check if the sender is "Peer"
        return 30000;// Assign a moderate priority value for "Peer"
    }

    if (strcmp(sender, "ImportantPerson") == 0) {// Check if the sender is "ImportantPerson"
        return 20000;// Assign a lower priority value for "ImportantPerson"
    }

    if (strcmp(sender, "OtherPerson") == 0) {// Check if the sender is "OtherPerson"
        return 10000;// Assign the lowest priority value for "OtherPerson"
    }

    return 0;
}

int dateToInt(const char *date) {// Function to convert a date string in the format "MM-DD-YYYY" into an integer for comparison
    int mm, dd, yyyy;

    if (sscanf(date, "%d-%d-%d", &mm, &dd, &yyyy) != 3) {// Use sscanf to parse the date string into month, day, and year
        return 0;
    }

    return yyyy * 10000 + mm * 100 + dd;// Convert the date into an integer in the format YYYYMMDD for easy comparison
}

int computePriority(const char *sender, const char *date) {// Function to compute the overall priority of an email based on the sender and date
    return senderPriority(sender) + dateToInt(date);// Combine the sender priority and the date integer to compute the overall priority of the email
}

MaxHeap* createHeap(int capacity) {// Function to create a new max heap with a specified initial capacity
    MaxHeap *heap; // Allocate memory for the MaxHeap structure

    heap = malloc(sizeof(MaxHeap)); // Allocate memory for the MaxHeap structure using malloc

    // Check if heap memory was allocated.
    if (heap == NULL) { // Check if malloc failed to allocate memory for the heap
        return NULL;
    }

    heap->data = malloc(sizeof(Email) * capacity); // Allocate memory for the array of Email structures based on the specified capacity

    // Check if email array memory was allocated.
    if (heap->data == NULL) { // Check if malloc failed to allocate memory for the email array
        free(heap); // Free the previously allocated memory for the heap structure to avoid memory leaks
        return NULL; 
    }

    heap->size = 0; // Initialize the size of the heap to 0, indicating that it currently contains no emails
    heap->capacity = capacity; // Set the capacity of the heap to the specified value, indicating how many emails it can hold before needing to resize

    return heap; // Return a pointer to the newly created max heap structure
}

void swap(Email *a, Email *b) { // Function to swap two Email structures in memory
    Email temp = *a; // Store the value of the first Email structure in a temporary variable
    *a = *b; // Assign the value of the second Email structure to the first Email structure
    *b = temp; // Assign the value stored in the temporary variable to the second Email structure, completing the swap
}

void heapifyUp(MaxHeap *heap, int index) { // Function to restore the max heap property by moving an email up the heap if its priority is higher than its parent's
    while (index > 0) { // Continue looping as long as the current index is not the root of the heap
        int parent = (index - 1) / 2; // Calculate the index of the parent node in the heap based on the current index

        if (heap->data[index].priority > // Check if the current email's priority is greater than its parent's priority
            heap->data[parent].priority) { // If the current email has a higher priority than its parent, swap them to maintain the max heap property

            swap(&heap->data[index], // Swap the current email with its parent in the heap
                 &heap->data[parent]); // Use the swap function to exchange the positions of the two emails in the heap

            index = parent; // Update the index to the parent's index to continue checking up the heap
        } else {
            break;
        }
    }
}

void heapifyDown(MaxHeap *heap, int index) { // Function to restore the max heap property by moving an email down the heap if its priority is lower than its children's
    int left; // Declare a variable to hold the index of the left child of the current node
    int right; // Declare a variable to hold the index of the right child of the current node
    int largest; // Declare a variable to hold the index of the node with the largest priority among the current node and its children

    while (1) { // Start an infinite loop that will continue until the max heap property is restored
        left = index * 2 + 1; // Calculate the index of the left child of the current node based on the current index
        right = index * 2 + 2; // Calculate the index of the right child of the current node based on the current index
        largest = index; // Initialize the largest index to the current index, assuming the current node has the highest priority

        // Check if the left child has higher priority.
        if (left < heap->size && // Check if the left child index is within the bounds of the heap size
            heap->data[left].priority > // Check if the left child's priority is greater than the current largest priority
            heap->data[largest].priority) { // If the left child has a higher priority than the current largest, update the largest index to the left child's index

            largest = left; // Update the largest index to the left child's index
        }

        // Check if the right child has higher priority.
        if (right < heap->size && // Check if the right child index is within the bounds of the heap size
            heap->data[right].priority > // Check if the right child's priority is greater than the current largest priority
            heap->data[largest].priority) { // If the right child has a higher priority than the current largest, update the largest index to the right child's index

            largest = right; // Update the largest index to the right child's index
        }

        // Stop when the current position
        // is already the largest.
        if (largest == index) { // Check if the current index is still the largest, meaning the max heap property is satisfied
            break;
        }

        swap(&heap->data[index], // Swap the current email with the email at the largest index to restore the max heap property
             &heap->data[largest]); // Use the swap function to exchange the positions of the two emails in the heap

        index = largest; // Update the index to the largest index to continue checking down the heap
    }// End of the while loop
} // End of the heapifyDown function

int insertEmail(MaxHeap *heap, Email email) { // Function to insert a new email into the max heap while maintaining the max heap property

    // If the heap is full, double its capacity.
    if (heap->size == heap->capacity) { // Check if the current size of the heap has reached its capacity, indicating that it is full and needs to be resized
        int newCapacity = heap->capacity * 2; // Calculate the new capacity by doubling the current capacity of the heap

        Email *newData = realloc( // Use realloc to resize the array of Email structures to the new capacity
            heap->data, // Pass the current data pointer to realloc, which will attempt to resize the memory block
            sizeof(Email) * newCapacity // Calculate the new size in bytes for the resized array based on the new capacity and the size of the Email structure
        );

        // Check whether realloc was successful.
        if (newData == NULL) { // Check if realloc failed to allocate memory for the resized array
            return 0;
        }

        heap->data = newData; // Update the data pointer in the heap structure to point to the newly allocated memory block
        heap->capacity = newCapacity; // Update the capacity of the heap to reflect the new size after resizing
    }

    // Add the email to the end of the heap.
    heap->data[heap->size] = email;

    // Move the email upward if necessary.
    heapifyUp(heap, heap->size);

    heap->size++;

    return 1;
}

Email extractMax(MaxHeap *heap) { // Function to remove and return the email with the highest priority from the max heap
    Email empty = {"", "", "", -1}; // Define an empty Email structure to return in case the heap is empty, with default values for sender, subject, date, and priority

    // Return an empty email if the heap is empty.
    if (heap->size == 0) { // Check if the heap is empty by checking if its size is zero
        return empty;
    }

    // Save the highest priority email.
    Email maxEmail = heap->data[0]; // Store the email with the highest priority (the root of the max heap) in a variable to return later

    // Move the last email to the root.
    heap->data[0] = heap->data[heap->size - 1]; // Replace the root of the heap with the last email in the heap to maintain the complete binary tree structure

    heap->size--; // Decrease the size of the heap by one to reflect the removal of the highest priority email

    // Restore the max heap property.
    if (heap->size > 0) {
        heapifyDown(heap, 0); // Call the heapifyDown function to restore the max heap property starting from the root of the heap after replacing it with the last email
    }

    return maxEmail; // Return the email with the highest priority that was removed from the heap
}

int peekMax(MaxHeap *heap, Email *result) { // Function to retrieve the email with the highest priority from the max heap without removing it

    if (heap->size == 0) { // Check if the heap is empty by checking if its size is zero
        return 0;
    }

    *result = heap->data[0]; // Copy the email with the highest priority (the root of the max heap) into the provided result pointer

    return 1;
}

void printEmail(const Email *e) { // Function to print the details of an email to standard output
    printf("Sender: %s\n", e->sender); // Print the sender of the email to standard output
    printf("Subject: %s\n", e->subject); // Print the subject of the email to standard output
    printf("Date: %s\n", e->date); // Print the date of the email to standard output
}

int main() { // Main function to execute the email priority queue program
    MaxHeap *queue; // Declare a pointer to a MaxHeap structure that will be used to manage the email priority queue
    char line[MAX_STR]; // Declare a character array to hold each line of input read from standard input
 
    // Create the email priority queue.
    queue = createHeap(INITIAL_CAPACITY); // Call the createHeap function to initialize a new max heap with the defined initial capacity for managing emails

    // Check whether the heap was created successfully.
    if (queue == NULL) { // Check if the createHeap function returned NULL, indicating that memory allocation for the heap failed
        printf("Memory allocation failed.\n"); // Print an error message to standard output indicating that memory allocation for the heap failed
        return 1;
    }

    // Read each command from standard input.
    while (fgets(line, sizeof(line), stdin)) { // Use fgets to read a line of input from standard input into the line array, continuing until EOF is reached

        // Remove the newline from the input.
        line[strcspn(line, "\r\n")] = '\0'; // Use strcspn to find the index of the first newline character in the line and replace it with a null terminator to remove the newline from the input

        if (strncmp(line, "EMAIL ", 6) == 0) { // Check if the input line starts with the command "EMAIL " to indicate that an email is being added to the queue

            Email newEmail; // Declare a new Email structure to hold the details of the email being added to the queue

            char sender[MAX_STR]; // Declare a character array to hold the sender of the email
            char subject[MAX_STR]; // Declare a character array to hold the subject of the email
            char date[MAX_STR]; // Declare a character array to hold the date of the email

            char *ptr; // Declare a pointer to a character that will be used to traverse the input line and extract the email details
            char *token; // Declare a pointer to a character that will be used to hold each token extracted from the input line during parsing

            // Start reading the information
            // immediately after "EMAIL ".
            ptr = line + 6;

            // Get the sender.
            token = strtok(ptr, ",");

            if (token == NULL) { // Check if the token is NULL, indicating that the sender field is missing from the input line
                printf("Invalid email format.\n");
                continue;
            }

            strncpy(sender, token, MAX_STR - 1); // Copy the sender token into the sender array, ensuring that it does not exceed the maximum string length defined by MAX_STR
            sender[MAX_STR - 1] = '\0';

            // Get the subject.
            token = strtok(NULL, ","); // Use strtok to get the next token from the input line, which should be the subject of the email, using a comma as the delimiter

            if (token == NULL) { // Check if the token is NULL, indicating that the subject field is missing from the input line
                printf("Invalid email format.\n"); // Print an error message to standard output indicating that the email format is invalid due to a missing subject field
                continue;
            }

            strncpy(subject, token, MAX_STR - 1); // Copy the subject token into the subject array, ensuring that it does not exceed the maximum string length defined by MAX_STR
            subject[MAX_STR - 1] = '\0'; // Null-terminate the subject string to ensure it is properly formatted as a C string

            // Get the date.
            token = strtok(NULL, ","); // Use strtok to get the next token from the input line, which should be the date of the email, using a comma as the delimiter

            if (token == NULL) { // Check if the token is NULL, indicating that the date field is missing from the input line
                printf("Invalid email format.\n"); // Print an error message to standard output indicating that the email format is invalid due to a missing date field
                continue;
            }

            strncpy(date, token, MAX_STR - 1); // Copy the date token into the date array, ensuring that it does not exceed the maximum string length defined by MAX_STR
            date[MAX_STR - 1] = '\0'; // Null-terminate the date string to ensure it is properly formatted as a C string

            // Remove extra spaces from each field.
            trim(sender); // Call the trim function to remove leading and trailing whitespace from the sender string to ensure it is properly formatted
            trim(subject); // Call the trim function to remove leading and trailing whitespace from the subject string to ensure it is properly formatted
            trim(date); // Call the trim function to remove leading and trailing whitespace from the date string to ensure it is properly formatted

            // Make sure none of the fields are empty.
            if (strlen(sender) == 0 || // Check if any of the fields (sender, subject, date) are empty after trimming whitespace
                strlen(subject) == 0 || // Check if the subject field is empty
                strlen(date) == 0) { // Check if the date field is empty

                printf("Invalid email format.\n"); // Print an error message to standard output indicating that the email format is invalid due to one or more empty fields
                continue;
            }

            // Make sure the date is valid.
            if (!validDate(date)) { // Call the validDate function to check if the date string is in a valid format (MM-DD-YYYY) and represents a valid date
                printf("Invalid date format.\n"); // Print an error message to standard output indicating that the date format is invalid if the validDate function returns false
                continue;
            }

            // Copy the validated information
            // into the Email structure.
            strcpy(newEmail.sender, sender); // Copy the validated sender string into the sender field of the newEmail structure using strcpy
            strcpy(newEmail.subject, subject); // Copy the validated subject string into the subject field of the newEmail structure using strcpy
            strcpy(newEmail.date, date); // Copy the validated date string into the date field of the newEmail structure using strcpy

            // Calculate the email priority.
            newEmail.priority = // Call the computePriority function to calculate the overall priority of the email based on the sender and date, and store it in the priority field of the newEmail structure
                computePriority(sender, date); // Compute the priority of the email based on the sender and date using the computePriority function

            // Insert the email into the heap.
            if (!insertEmail(queue, newEmail)) { // Call the insertEmail function to add the newEmail structure to the max heap, and check if the insertion was successful
                printf("Memory allocation failed.\n"); // Print an error message to standard output indicating that memory allocation failed if the insertEmail function returns false
                break;
            }
        }

        else if (strcmp(line, "NEXT") == 0) { // Check if the input line is the command "NEXT" to indicate that the user wants to view the next email in the queue

            Email nextEmail; // Declare a new Email structure to hold the details of the next email to be displayed

            if (peekMax(queue, &nextEmail)) { // Call the peekMax function to retrieve the email with the highest priority from the max heap without removing it, and check if it was successful
                printf("Next email:\n");
                printEmail(&nextEmail);
            } else { // If the peekMax function returns false, indicating that there are no emails in the queue, print a message to standard output indicating that there are no emails to read
                printf("No emails to read.\n");
            }
        }

        else if (strcmp(line, "READ") == 0) { // Check if the input line is the command "READ" to indicate that the user wants to read and remove the next email in the queue

            if (queue->size == 0) { // Check if the size of the max heap is zero, indicating that there are no emails in the queue
                printf("No emails to read.\n");
            } else { // If there are emails in the queue, call the extractMax function to remove and return the email with the highest priority from the max heap, and print its details to standard output
                extractMax(queue);
            }
        }

        else if (strcmp(line, "COUNT") == 0) { // Check if the input line is the command "COUNT" to indicate that the user wants to know how many emails are currently in the queue
            printf("There are %d emails to read.\n", // Print the current size of the max heap, which represents the number of emails in the queue, to standard output
                   queue->size);
        }
    }

    // Free all dynamically allocated memory.
    free(queue->data); // Free the memory allocated for the array of Email structures in the max heap to avoid memory leaks
    free(queue); // Free the memory allocated for the MaxHeap structure itself to avoid memory leaks

    return 0; // Return 0 to indicate successful program completion
} // End of the main function
