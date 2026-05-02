#ifndef PROJECT_H
#define PROJECT_H

#include <stdint.h>
#include"task.h"

typedef struct Project {
    char* title;
    char* description;
    char* deadline;
    TaskList* task_list;
    struct Project* next;
} Project;

typedef struct {
    Project* head;
} ProjectList;

// Project management functions
Project* create_project(const char* title, const char* description, const char* deadline);  // Takes user input inside
void push_project(ProjectList* list, Project* new_project);
void add_project(ProjectList* list);  // Adds a new project from user input
void view_project();
ProjectList load_project(const char* filename);
void save_project_to_file(Project* project);
void free_project_list(ProjectList *list);
void manage_project(ProjectList* list);
void edit_project(ProjectList* list);
void delete_project(ProjectList* list);


#endif
