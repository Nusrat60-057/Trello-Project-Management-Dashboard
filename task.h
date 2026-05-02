#ifndef TASK_H
#define TASK_H
#include "utils.h"
#include "priority.h"

typedef enum
{
    TODO,
    IN_PROGRESS,
    DONE
} TaskStatus;

typedef struct Task
{
    char* title;
    char* description;
    char* deadline;
    Priority priority_level;
    TaskStatus status;
    struct Task* next;
} Task;

typedef struct
{
    Task* head;
} TaskList;
// Task management functions
Task* create_task(const char* title, const char* description, const char* deadline, Priority priority, TaskStatus status);
void push_task(TaskList* task_list, Task* new_task);
void add_task(TaskList* task_list,const char* project_title);
void view_tasks_by_status(TaskList* task_list, TaskStatus status);
void save_tasks_to_file(TaskList* task_list, const char* filename);
TaskList* load_tasks_from_file(const char* filename);
void update_task_status(TaskList* task_list, int task_number, TaskStatus new_status);
void free_task_list(TaskList* task_list);
void create_task_filename(const char* project_title, char* buffer, size_t buffer_size);
void edit_task(TaskList* list, const char* project_title);
void delete_task(TaskList* list, const char* project_title);
#endif
