#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "project.h"
//#define BLUE    "\x1b[34m"
//#define GREEN   "\x1b[32m"
//#define RESET   "\x1b[0m"
#include<windows.h>
void loading_animation() {
    printf("Loading project");
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        Sleep(1000);  // On Linux/macOS
    }
    printf("\n");
}

void start_interface() {

    ProjectList list = load_project("projectlist.txt");
    while (1) {
        clear_screen();
        display_main_menu();

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear input buffer

        switch (choice) {
    case 1:
        view_project(&list);
        break;
    case 2:
        add_project(&list);
        break;
    case 3:
        manage_project(&list);
        break;
    case 4:
        edit_project(&list);
        break;
    case 5:
        delete_project(&list);
        break;
    case 6:
        free_project_list(&list);
        printf("Exiting. Thank you for using Trello\n");
        return;
    default:
        printf("Invalid choice. Please try again.\n");
}


        pause_and_clear();
    }
}


void display_main_menu() {
    printf("\x1b[97m""\n=====  Trello in C =====\n""\x1b[0m");
    printf("\x1b[96m""1. View All Projects\n");
    printf("2. Create a New Project\n");
    printf("3. Manage Project\n");
    printf("4. Edit Project\n");
    printf("5. Delete Project\n");
    printf("6. Exit\n");
}

void display_manage_menu(){
    printf("\n--- Manage Project: project name ---\n");
    printf("1. View Tasks by Status\n");
    printf("2. Add New Task\n");
    printf("3. Edit Task\n");
    printf("4. Delete Task\n");
    printf("5. Reorder\n");
    printf("6. Go back\n");

}


void display_task_menu() {
    printf("\n--- project name ---\n");
    printf("1. View Lists\n");
    printf("2. Edit Card (Task)\n");
    printf("3. Delete Card (Task)\n");
    printf("4. Reorder Cards (Tasks)\n");
    printf("5. View Cards (Tasks)\n");
    printf("6. Back\n");
}
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_and_clear() {
    printf("Press Enter to continue...");
    getchar();
    clear_screen();
}

