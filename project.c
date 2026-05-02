#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "project.h"
#include "task.h"
#include "ui.h"

#define MAX_LINE 256


Project* create_project(const char* title, const char* description, const char* deadline)
{
    Project* p = (Project*)malloc(sizeof(Project));
    if (!p) return NULL;

    p->title = strdup(title);
    p->description = strdup(description);
    p->deadline = strdup(deadline);
    p->next = NULL;

    p->task_list = (TaskList*)malloc(sizeof(TaskList));
    p->task_list->head = NULL;

    return p;
}

void push_project(ProjectList* list, Project* new_project)
{
    if (!list->head)
    {
        list->head = new_project;
        return;
    }

    Project* current = list->head;
    while (current->next)
        current = current->next;

    current->next = new_project;
}

ProjectList load_project(const char* filename)
{
    ProjectList list;
    list.head = NULL;

    FILE* file = fopen(filename, "r");
    if (!file)
    {
        return list;  // return an empty list instead of NULL
    }

    char line[MAX_LINE];

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

        fgets(line, sizeof(line), file); // Skip '---'

        Project* p = create_project(title, description, deadline);

        char task_filename[MAX_LINE];
        create_task_filename(title, task_filename, sizeof(task_filename));
        p->task_list = load_tasks_from_file(task_filename);
        push_project(&list, p);

        free(title);
        free(description);
        free(deadline);
    }

    fclose(file);
    return list;
}


void view_project(ProjectList* list)
{

    if (!list->head)
    {
        printf("No saved projects found.\n");
        return;
    }

    printf("\n--- Projects ---\n");
    Project* current = list->head;
    uint16_t index = 1;
    while (current)
    {
        printf("%u. %s\n", index++, current->title);
        current = current->next;
    }

    uint16_t choice;
    printf("\nEnter the project number to see, or 0 to go back: ");
    scanf("%hu", &choice);
    while (getchar() != '\n');

    if (choice == 0)
    {
        return;
    }

    current = list->head;
    uint16_t i = 1;
    while (current && i < choice)
    {
        current = current->next;
        i++;
    }

    if (current)
    {
        printf("\nProject: %s\nDescription: %s\nDeadline: %s\n",
               current->title, current->description, current->deadline);
        // TODO: manage_project(current);
    }
    else
    {
        printf("Invalid choice.\n");
    }

}


void add_project(ProjectList* list)
{
    while(1)
    {
        char* title = (char*)malloc(100);
        char* description = (char*)malloc(256);
        char* deadline = (char*)malloc(20);

        if (!title || !description || !deadline)
        {
            printf("Memory allocation failed.\n");
            return;
        }

        printf("Enter project title: ");
        scanf(" %[^\n]", title);

        printf("Enter project description: ");
        scanf(" %[^\n]", description);

        printf("Enter project deadline (YYYY-MM-DD): ");
        scanf(" %[^\n]", deadline);

        Project* new_proj = create_project(title, description, deadline);
        printf("\nAdding Project.....\n");
        push_project(list, new_proj);

        save_project_to_file(new_proj);  // appends one new project to file

        printf("Project added successfully.\n");

        free(title);
        free(description);
        free(deadline);

        char choice;
        printf("\nWould you like to add another project? (y/n): ");
        scanf(" %c", &choice);
        while (getchar() != '\n'); // clear input buffer

        if (choice != 'y' && choice != 'Y')
        {
            break;
        }
    }
}

void save_project_to_file(Project* project)
{
    FILE* file = fopen("projectlist.txt", "a"); // append mode
    if (!file)
    {
        printf("Error opening file to save project.\n");
        return;
    }

    fprintf(file, "%s\n", project->title);
    fprintf(file, "%s\n", project->description);
    fprintf(file, "%s\n", project->deadline);
    fprintf(file, "---\n");

    fclose(file);
}

void free_project_list(ProjectList *list){
    Project* current = list->head;
    while (current) {
        Project* next = current->next;
        free(current->title);
        free(current->description);
        free(current->deadline);

        if (current->task_list) {
            free_task_list(current->task_list);
        }

        free(current);
        current = next;
    }
    list->head = NULL;
}

#include <stdio.h>
#include "project.h"
#include "task.h"
#include "ui.h"
void edit_project(ProjectList* list)
{
    if (!list || !list->head) {
        printf("No projects to edit.\n");
        return;
    }

    printf("\n--- Select Project to Edit ---\n");
    Project* current = list->head;
    unsigned index = 1;
    while (current) {
        printf("%u. %s\n", index++, current->title);
        current = current->next;
    }

    printf("Enter project number to edit (or 0 to cancel): ");
    unsigned choice;
    scanf("%u", &choice);
    while (getchar() != '\n');

    if (choice == 0) return;

    current = list->head;
    unsigned i = 1;
    while (current && i < choice) {
        current = current->next;
        i++;
    }

    if (!current) {
        printf("Invalid selection.\n");
        return;
    }

    char new_title[100], new_description[256], new_deadline[20];
    printf("Enter new title (or press Enter to keep '%s'): ", current->title);
    fgets(new_title, sizeof(new_title), stdin);
    new_title[strcspn(new_title, "\n")] = 0;

    printf("Enter new description (or press Enter to keep current): ");
    fgets(new_description, sizeof(new_description), stdin);
    new_description[strcspn(new_description, "\n")] = 0;

    printf("Enter new deadline (YYYY-MM-DD) (or press Enter to keep current): ");
    fgets(new_deadline, sizeof(new_deadline), stdin);
    new_deadline[strcspn(new_deadline, "\n")] = 0;

    // Save old task filename if title changed
    char old_task_filename[100];
    create_task_filename(current->title, old_task_filename, sizeof(old_task_filename));

    if (strlen(new_title) > 0) {
        free(current->title);
        current->title = strdup(new_title);
    }
    if (strlen(new_description) > 0) {
        free(current->description);
        current->description = strdup(new_description);
    }
    if (strlen(new_deadline) > 0) {
        free(current->deadline);
        current->deadline = strdup(new_deadline);
    }

    // If title was changed, rename task file
    char new_task_filename[100];
    create_task_filename(current->title, new_task_filename, sizeof(new_task_filename));
    if (strcmp(old_task_filename, new_task_filename) != 0) {
        rename(old_task_filename, new_task_filename);
    }

    // Rewrite entire project list to file
    FILE* file = fopen("projectlist.txt", "w");
    if (!file) {
        printf("Error updating project file.\n");
        return;
    }

    Project* iter = list->head;
    while (iter) {
        fprintf(file, "%s\n%s\n%s\n---\n", iter->title, iter->description, iter->deadline);
        iter = iter->next;
    }
    fclose(file);

    printf("Project updated successfully.\n");
}
void delete_project(ProjectList* list)
{
    if (!list || !list->head) {
        printf("No projects to delete.\n");
        return;
    }

    printf("\n--- Select Project to Delete ---\n");
    Project* current = list->head;
    unsigned index = 1;
    while (current) {
        printf("%u. %s\n", index++, current->title);
        current = current->next;
    }

    printf("Enter project number to delete (or 0 to cancel): ");
    unsigned choice;
    scanf("%u", &choice);
    while (getchar() != '\n');

    if (choice == 0) return;

    Project* prev = NULL;
    current = list->head;
    unsigned i = 1;
    while (current && i < choice) {
        prev = current;
        current = current->next;
        i++;
    }

    if (!current) {
        printf("Invalid selection.\n");
        return;
    }

    // Confirm deletion
    printf("Are you sure you want to delete project '%s'? (y/n): ", current->title);
    char confirm;
    scanf(" %c", &confirm);
    while (getchar() != '\n');

    if (confirm != 'y' && confirm != 'Y') return;

    // Remove from linked list
    if (prev) prev->next = current->next;
    else list->head = current->next;

    // Delete task file
    char task_filename[100];
    create_task_filename(current->title, task_filename, sizeof(task_filename));
    remove(task_filename);

    // Free memory
    free(current->title);
    free(current->description);
    free(current->deadline);
    if (current->task_list) free_task_list(current->task_list);
    free(current);

    // Rewrite projectlist.txt
    FILE* file = fopen("projectlist.txt", "w");
    if (!file) {
        printf("Error updating project file.\n");
        return;
    }
    current = list->head;
    while (current) {
        fprintf(file, "%s\n%s\n%s\n---\n", current->title, current->description, current->deadline);
        current = current->next;
    }
    fclose(file);

    printf("Project deleted successfully.\n");
}
void manage_project(ProjectList* list) {
    if (!list || !list->head) {
        printf("No projects available to manage.\n");
        return;
    }

    // Show list of projects
    printf("\n--- Select Project to Manage ---\n");
    Project* current = list->head;
    unsigned index = 1;
    while (current) {
        printf("%u. %s\n", index++, current->title);
        current = current->next;
    }

    // User input
    printf("Enter project number (or 0 to go back): ");
    unsigned choice;
    scanf("%u", &choice);
    while (getchar() != '\n');

    if (choice == 0) return;

    // Locate selected project
    current = list->head;
    unsigned i = 1;
    while (current && i < choice) {
        current = current->next;
        i++;
    }

    if (!current) {
        printf("Invalid selection.\n");
        return;
    }

    // Load tasks for selected project (if not already loaded)
    if (!current->task_list) {
        current->task_list = (TaskList*)malloc(sizeof(TaskList));
        current->task_list->head = NULL;

        char task_filename[100];
        create_task_filename(current->title, task_filename, sizeof(task_filename));
        TaskList* loaded = load_tasks_from_file(task_filename);
        if (loaded) {
            free(current->task_list);  // Replace empty task_list with loaded
            current->task_list = loaded;
        }
    }


    // Menu loop
    int running = 1;
    while (running) {
        printf("\n--- Managing Project: %s ---\n", current->title);
        display_manage_menu();  // UI function you already created

        printf("Enter choice: ");
        unsigned option;
        scanf("%u", &option);
        while (getchar() != '\n');

        char task_filename[100];
        create_task_filename(current->title, task_filename, sizeof(task_filename));

        switch (option) {
    case 1:
        view_tasks(current->task_list);
        break;
    case 2:
        add_task(current->task_list, current->title);
        break;
    case 3:
        edit_task(current->task_list, current->title);  // ✅ use new function
        break;
    case 4:
        delete_task(current->task_list, current->title);  // ✅ use new function
        break;
    case 5:
        printf("Reorder task not implemented yet.\n");
        break;
    case 6:
        printf("Returning back to main menu....\n");
        running = 0;
        break;
    default:
        printf("Invalid option. Try again.\n");
}

    }
}
