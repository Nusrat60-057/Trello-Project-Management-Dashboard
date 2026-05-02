#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "task.h"
#include "ui.h"
#include "utils.h"
#include "priority.h"



Task* create_task(const char* title, const char* description, const char* deadline, Priority priority, TaskStatus status)
{
    Task* t = (Task*)malloc(sizeof(Task));
    if (!t) return NULL;

    t->title = strdup(title);
    t->description = strdup(description);
    t->deadline = strdup(deadline);
    t->priority_level = priority;
    t->status = status;
    t->next = NULL;

    return t;
}

void push_task(TaskList* list, Task* new_task)
{
    if (!list->head)
    {
        list->head = new_task;
        return;
    }

    Task* current = list->head;
    while (current->next)
        current = current->next;

    current->next = new_task;
}

void add_task(TaskList* list,const char* project_title)
{
    while (1)
    {
        char* title = (char*)malloc(100);
        char* description = (char*)malloc(256);
        char* deadline = (char*)malloc(20);
        unsigned priority_input, status_input;

        if (!title || !description || !deadline)
        {
            printf("Memory allocation failed.\n");
            return;
        }

        printf("Enter task title: ");
        scanf(" %[^\n]", title);
        while (getchar() != '\n');

        printf("Enter task description: ");
        scanf(" %[^\n]", description);
        while (getchar() != '\n');

        printf("Enter task deadline (YYYY-MM-DD): ");
        scanf(" %[^\n]", deadline);
        while (getchar() != '\n');

        printf("Enter priority (1 = Low, 2 = Medium, 3 = High): ");
        scanf("%u", &priority_input);
        while (getchar() != '\n');

        printf("Enter status (0 = TODO, 1 = IN_PROGRESS, 2 = DONE): ");
        scanf("%u", &status_input);
        while (getchar() != '\n');

        Task* new_task = create_task(title, description, deadline, (Priority)priority_input, (TaskStatus)status_input);
        push_task(list, new_task);

        char filename[100];
        create_task_filename(project_title, filename, sizeof(filename));
        save_tasks_to_file(list, filename);

        printf("Task added successfully.\n");

        free(title);
        free(description);
        free(deadline);

        char choice;
        printf("\nWould you like to add another task? (y/n): ");
        scanf(" %c", &choice);
        while (getchar() != '\n');

        if (choice != 'y' && choice != 'Y') break;
    }
}

void view_tasks(TaskList* list)
{
    if (!list->head)
    {
        printf("No tasks available.\n");
        return;
    }

    Task* current = list->head;
    unsigned index = 1;
    while (current)
    {
        printf("\nTask %u:\n", index++);
        printf("Title      : %s\n", current->title);
        printf("Description: %s\n", current->description);
        printf("Deadline   : %s\n", current->deadline);
        printf("Priority   : %u\n", current->priority_level);
        printf("Status     : %u\n", current->status);
        current = current->next;
    }
}

void save_tasks_to_file(TaskList* task_list, const char* filename)
{
    FILE* file = fopen(filename, "a"); // append mode
    if (!file)
    {
        printf("Error opening task file.\n");
        return;
    }

    Task* current = task_list->head;
    while (current)
    {
        fprintf(file, "%s\n", current->title);
        fprintf(file, "%s\n", current->description);
        fprintf(file, "%s\n", current->deadline);
        fprintf(file, "%u\n", (unsigned)current->priority_level);
        fprintf(file, "%u\n", (unsigned)current->status);
        fprintf(file, "---\n");

        current = current->next;
    }

    fclose(file);
}

TaskList* load_tasks_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    TaskList* list = (TaskList*)malloc(sizeof(TaskList));
    if (!list) return NULL;
    list->head = NULL;

    if (!file)
    {
        // If the file doesn't exist, return an empty task list
        return list;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;
        char* title = strdup(line);

        if (!fgets(line, sizeof(line), file)) break;
        line[strcspn(line, "\n")] = 0;
        char* description = strdup(line);

        if (!fgets(line, sizeof(line), file)) break;
        line[strcspn(line, "\n")] = 0;
        char* deadline = strdup(line);

        if (!fgets(line, sizeof(line), file)) break;
        unsigned int priority = (unsigned int)atoi(line);

        if (!fgets(line, sizeof(line), file)) break;
        unsigned int status = (unsigned int)atoi(line);

        fgets(line, sizeof(line), file); // Skip the "---" separator

        Task* task = create_task(title, description, deadline, priority, status);
        push_task(list, task);

        free(title);
        free(description);
        free(deadline);
    }

    fclose(file);
    return list;
}
//Edit task
void edit_task(TaskList* list, const char* project_title)
{
    if (!list || !list->head)
    {
        printf("No tasks to edit.\n");
        return;
    }

    view_tasks(list);

    printf("Enter the task number to edit: ");
    int task_num;
    scanf("%d", &task_num);
    while (getchar() != '\n');

    Task* current = list->head;
    int i = 1;
    while (current && i < task_num)
    {
        current = current->next;
        i++;
    }

    if (!current)
    {
        printf("Invalid task number.\n");
        return;
    }

    char buffer[100];

    printf("Enter new title (or press Enter to keep '%s'): ", current->title);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0)
    {
        free(current->title);
        current->title = strdup(buffer);
    }

    printf("Enter new description (or press Enter to keep current): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0)
    {
        free(current->description);
        current->description = strdup(buffer);
    }

    printf("Enter new deadline (YYYY-MM-DD) (or press Enter to keep current): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0)
    {
        free(current->deadline);
        current->deadline = strdup(buffer);
    }

    unsigned int new_priority;
    printf("Enter new priority (1=Low, 2=Medium, 3=High), or 0 to keep current: ");
    scanf("%u", &new_priority);
    while (getchar() != '\n');
    if (new_priority >= 1 && new_priority <= 3)
        current->priority_level = (Priority)new_priority;

    unsigned int new_status;
    printf("Enter new status (0=TODO, 1=IN_PROGRESS, 2=DONE), or 9 to skip: ");
    scanf("%u", &new_status);
    while (getchar() != '\n');
    if (new_status <= 2)
        current->status = (TaskStatus)new_status;

    char filename[100];
    create_task_filename(project_title, filename, sizeof(filename));
    FILE* file = fopen(filename, "w");
    Task* temp = list->head;
    while (temp)
    {
        fprintf(file, "%s\n%s\n%s\n%u\n%u\n---\n",
                temp->title, temp->description, temp->deadline,
                (unsigned)temp->priority_level, (unsigned)temp->status);
        temp = temp->next;
    }
    fclose(file);

    printf("Task updated successfully.\n");
}

//Delete task
void delete_task(TaskList* list, const char* project_title)
{
    if (!list || !list->head)
    {
        printf("No tasks to delete.\n");
        return;
    }

    view_tasks(list);

    printf("Enter the task number to delete: ");
    int task_num;
    scanf("%d", &task_num);
    while (getchar() != '\n');

    Task* current = list->head;
    Task* prev = NULL;
    int i = 1;
    while (current && i < task_num)
    {
        prev = current;
        current = current->next;
        i++;
    }

    if (!current)
    {
        printf("Invalid task number.\n");
        return;
    }

    printf("Are you sure you want to delete '%s'? (y/n): ", current->title);
    char confirm;
    scanf(" %c", &confirm);
    while (getchar() != '\n');
    if (confirm != 'y' && confirm != 'Y') return;

    if (prev)
        prev->next = current->next;
    else
        list->head = current->next;

    free(current->title);
    free(current->description);
    free(current->deadline);
    free(current);

    // Save updated task list
    char filename[100];
    create_task_filename(project_title, filename, sizeof(filename));
    FILE* file = fopen(filename, "w");
    Task* temp = list->head;
    while (temp)
    {
        fprintf(file, "%s\n%s\n%s\n%u\n%u\n---\n",
                temp->title, temp->description, temp->deadline,
                (unsigned)temp->priority_level, (unsigned)temp->status);
        temp = temp->next;
    }
    fclose(file);

    printf("Task deleted successfully.\n");
}

void free_task_list(TaskList* task_list)
{
    Task* current = task_list->head;
    while (current)
    {
        Task* next = current->next;
        free(current->title);
        free(current->description);
        free(current->deadline);
        free(current);
        current = next;
    }
    task_list->head = NULL;
    free(task_list);
}

void create_task_filename(const char* project_title, char* buffer, size_t buffer_size)
{
    snprintf(buffer, buffer_size, "tasks_%s.txt", project_title);
}

