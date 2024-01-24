#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"

void addTask(Task **tasks, int *taskCount, const char *description) {
    *tasks = realloc(*tasks, (*taskCount + 1) * sizeof(Task));
    if (*tasks == NULL) {
        printf("Memory reallocation failed.\n");
        exit(EXIT_FAILURE);
    }

    Task newTask;
    strcpy(newTask.description, description);
    newTask.completed = 0;
    (*tasks)[*taskCount] = newTask;
    (*taskCount)++;
    printf("Task added successfully.\n");
}

void listTasks(const Task tasks[], int taskCount) {
    for (int i = 0; i < taskCount; ++i) {
        printf("%d. %s %s\n", i + 1, tasks[i].completed ? "[X]" : "[ ]", tasks[i].description);
    }
}

void markCompleted(Task tasks[], int taskCount, int taskIndex) {
    if (taskIndex >= 1 && taskIndex <= taskCount) {
        tasks[taskIndex - 1].completed = 1;
        printf("Task marked as completed.\n");
    } else {
        printf("Invalid task index.\n");
    }
}

void saveTasksToFile(const Task tasks[], int taskCount, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < taskCount; ++i) {
        fprintf(file, "%d;%s;%d\n", i + 1, tasks[i].description, tasks[i].completed);
    }

    fclose(file);
}

void loadTasksFromFile(Task **tasks, int *taskCount, const char *filename) {
    FILE *file = fopen(filename, "a+");
    if (file == NULL) {
        printf("No existing tasks file found.\n");
        return;
    }

    *tasks = malloc(1 * sizeof(Task));
    if (*tasks == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%*d;%99[^;];%d%*c", (*tasks)[*taskCount].description, &(*tasks)[*taskCount].completed) == 2) {
        (*taskCount)++;

        *tasks = realloc(*tasks, (*taskCount + 1) * sizeof(Task));
        if (*tasks == NULL) {
            printf("Memory reallocation failed.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    Task *tasks;
    int taskCount = 0;

    const char *homeDir = getenv("HOME");

    if (homeDir == NULL) {
        fprintf(stderr, "Unable to get home directory\n");
        return 1;
    }

    char storePath[256];
    snprintf(storePath, sizeof(storePath), "%s/.config/todo.list", homeDir);

    if (argc < 2) {
        printf("Usage: %s [OPTION]...\n", argv[0]);
        printf("    add\t \"todo desc\"\tAdd a new task with the provided description.\n");
        printf("    done [TASK INDEX]\tMark the task at the specified index as completed.\n");
        printf("    list\t \tDisplay the current to-do list.\n");
        return 1;
    }

    loadTasksFromFile(&tasks, &taskCount, storePath);

    if (strcmp(argv[1], "add") == 0) {
        addTask(&tasks, &taskCount, argv[2]);
    } else if (strcmp(argv[1], "list") == 0) {
        listTasks(tasks, taskCount);
    } else if (strcmp(argv[1], "done") == 0) {
        int taskIndex = atoi(argv[2]);
        markCompleted(tasks, taskCount, taskIndex);
    } else {
        printf("Unknown operation: %s\n", argv[1]);
        return 1;
    }

    saveTasksToFile(tasks, taskCount, storePath);

    return 0;
}
