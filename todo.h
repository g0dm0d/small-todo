#ifndef TODO_H
#define TODO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char description[100];
    int completed;
} Task;

void addTask(Task **tasks, int *taskCount, const char *description);
void listTasks(const Task tasks[], int taskCount);
void markCompleted(Task tasks[], int taskCount, int taskIndex);
void saveTasksToFile(const Task tasks[], int taskCount, const char *filename);
void loadTasksFromFile(Task **tasks, int *taskCount, const char *filename);

#endif