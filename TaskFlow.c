#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_TITLE 100

// ---------------- STRUCTURES ----------------
typedef struct {
    int id;
    char title[MAX_TITLE];
    int priority;  // 1 = High, 2 = Medium, 3 = Low
    int completed; // 0 = pending, 1 = done
} Task;

Task heap[MAX_TASKS];
int heapSize = 0;

// ---------------- FUNCTION DECLARATIONS ----------------
void addTask();
void viewTasks();
void markTaskComplete();
void deleteTask();
void saveTasks();
void loadTasks();
void swap(Task *a, Task *b);
void heapifyUp(int index);
void heapifyDown(int index);
Task extractMin();

// ---------------- MAIN MENU ----------------
int main() {
    int choice;
    loadTasks(); // Load saved tasks on start

    do {
        printf("\n====== TaskFlow - To-Do List Manager ======\n");
        printf("1. Add Task\n");
        printf("2. View Tasks (by Priority)\n");
        printf("3. Mark Task as Completed\n");
        printf("4. Delete Top Priority Task\n");
        printf("5. Save & Exit\n");
        printf("===========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        switch (choice) {
            case 1: addTask(); break;
            case 2: viewTasks(); break;
            case 3: markTaskComplete(); break;
            case 4: deleteTask(); break;
            case 5:
                saveTasks();
                printf("Tasks saved successfully. Exiting...\n");
                break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// ---------------- HEAP FUNCTIONS ----------------

// Add new task to heap
void addTask() {
    if (heapSize >= MAX_TASKS) {
        printf("Task limit reached!\n");
        return;
    }

    Task newTask;
    newTask.id = heapSize + 1;
    newTask.completed = 0;

    printf("Enter task title: ");
    fgets(newTask.title, MAX_TITLE, stdin);
    newTask.title[strcspn(newTask.title, "\n")] = '\0'; // remove newline

    printf("Enter priority (1 = High, 2 = Medium, 3 = Low): ");
    scanf("%d", &newTask.priority);

    heap[heapSize] = newTask;
    heapifyUp(heapSize);
    heapSize++;

    printf("Task added successfully!\n");
}

// Heapify upwards
void heapifyUp(int index) {
    if (index == 0) return;
    int parent = (index - 1) / 2;
    if (heap[index].priority < heap[parent].priority) {
        swap(&heap[index], &heap[parent]);
        heapifyUp(parent);
    }
}

// Heapify downwards
void heapifyDown(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heapSize && heap[left].priority < heap[smallest].priority)
        smallest = left;
    if (right < heapSize && heap[right].priority < heap[smallest].priority)
        smallest = right;

    if (smallest != index) {
        swap(&heap[index], &heap[smallest]);
        heapifyDown(smallest);
    }
}

// Swap two tasks
void swap(Task *a, Task *b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}

// Extract top-priority task
Task extractMin() {
    Task minTask = heap[0];
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(0);
    return minTask;
}

// ---------------- CORE FEATURES ----------------

// View all tasks
void viewTasks() {
    if (heapSize == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("\n---- Current Tasks (by priority) ----\n");
    for (int i = 0; i < heapSize; i++) {
        printf("[%d] %s | Priority: %d | Status: %s\n",
               heap[i].id,
               heap[i].title,
               heap[i].priority,
               heap[i].completed ? "Done" : "Pending");
    }
}

// Mark task as completed
void markTaskComplete() {
    if (heapSize == 0) {
        printf("No tasks to mark.\n");
        return;
    }

    char title[MAX_TITLE];
    printf("Enter task title to mark completed: ");
    fgets(title, MAX_TITLE, stdin);
    title[strcspn(title, "\n")] = '\0';

    for (int i = 0; i < heapSize; i++) {
        if (strcmp(heap[i].title, title) == 0) {
            heap[i].completed = 1;
            printf("Task marked as completed!\n");
            return;
        }
    }
    printf("Task not found.\n");
}

// Delete top priority task
void deleteTask() {
    if (heapSize == 0) {
        printf("No tasks to delete.\n");
        return;
    }

    Task top = extractMin();
    printf("Deleted Task: %s (Priority %d)\n", top.title, top.priority);
}

// ---------------- FILE HANDLING ----------------

void saveTasks() {
    FILE *fp = fopen("tasks.txt", "w");
    if (!fp) {
        printf("Error saving file!\n");
        return;
    }
    for (int i = 0; i < heapSize; i++) {
        fprintf(fp, "%d,%s,%d,%d\n",
                heap[i].id,
                heap[i].title,
                heap[i].priority,
                heap[i].completed);
    }
    fclose(fp);
}

void loadTasks() {
    FILE *fp = fopen("tasks.txt", "r");
    if (!fp) return; // no saved file

    while (fscanf(fp, "%d,%[^,],%d,%d\n",
                  &heap[heapSize].id,
                  heap[heapSize].title,
                  &heap[heapSize].priority,
                  &heap[heapSize].completed) != EOF) {
        heapSize++;
    }
    fclose(fp);
}
