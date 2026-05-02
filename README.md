# Trello Project Management Dashboard

A command-line project management application written in C, inspired by Trello. This application allows you to create, manage, and organize projects with tasks, priorities, and deadlines.

## Features

- **Project Management**: Create, view, edit, and delete projects
- **Task Management**: Add tasks to projects with:
  - Task titles and descriptions
  - Deadlines
  - Three priority levels (Low, Medium, High)
  - Task status tracking (TODO, In Progress, Done)
- **Data Persistence**: Automatically save and load projects from file
- **User-Friendly Interface**: Interactive command-line menu system
- **Linked List Implementation**: Efficient data structure for managing projects and tasks

## Project Structure

```
├── main.c              # Main entry point
├── ui.c/ui.h           # User interface and menu system
├── project.c/project.h # Project management functions
├── task.c/task.h       # Task management functions
├── priority.c/priority.h # Priority level definitions
├── utils.c/utils.h     # Utility functions
├── Makefile            # Build configuration
├── projectlist.txt     # Data file (auto-generated)
└── Report/             # Documentation directory
```

## Requirements

- **Compiler**: GCC (or any C compiler compatible with C99)
- **OS**: Windows/Linux/macOS
- **Libraries**: Standard C library (stdio.h, stdlib.h, string.h, etc.)

## Building the Project

### Using Make:

```bash
# Build the executable
make

# Run the program
make run

# Clean up object files
make clean
```

### Manual Compilation:

```bash
gcc -Wall -g -o Trello.exe main.c priority.c project.c task.c ui.c utils.c
```

## Usage

1. **Start the Application**:
   ```bash
   Trello.exe
   ```

2. **Main Menu Options**:
   - Create new projects
   - View existing projects
   - Manage projects (edit/delete)
   - Manage tasks within projects
   - Exit the application

3. **Task Management**:
   - Add tasks to any project
   - Set task priority (Low, Medium, High)
   - Update task status (TODO → In Progress → Done)
   - View all tasks for a project

4. **Data Storage**:
   - Projects are automatically saved to `projectlist.txt`
   - Data persists between sessions

## Data Structures

### Project
- Title
- Description
- Deadline
- Linked list of tasks

### Task
- Title
- Description
- Deadline
- Priority level
- Status (TODO, IN_PROGRESS, DONE)

### Priority Levels
- `PRIORITY_LOW` (1)
- `PRIORITY_MEDIUM` (2)
- `PRIORITY_HIGH` (3)

## Key Functions

### Project Management
- `create_project()` - Create a new project
- `add_project()` - Add project with user input
- `edit_project()` - Modify project details
- `delete_project()` - Remove a project
- `manage_project()` - Project management menu
- `load_project()` - Load projects from file
- `save_project_to_file()` - Save project to file

### Task Management
- `create_task()` - Create a new task
- `add_task()` - Add task to a project
- `view_task()` - Display task details
- `edit_task()` - Modify task information
- `delete_task()` - Remove a task

## File Format

Projects are stored in `projectlist.txt` with the following structure:
- Project metadata (title, description, deadline)
- Associated tasks with their details
- Priority and status information

## Notes

- All data is stored locally in the project directory
- The application uses linked lists for dynamic memory management
- User input is validated to ensure data integrity
- The interface provides clear navigation with menu options

## Future Enhancements

- Database integration for larger data sets
- Export to CSV or JSON formats
- Sorting and filtering capabilities
- Multi-user support
- Category/tag system for tasks



## 👤 Author

**Nusrat Rahman Aurna**

Connect with the team:
- 🔗 [GitHub](https://github.com/Nusrat60-057)
- 💼 [LinkedIn](https://www.linkedin.com/in/nusrat-rahman-aurna-292b16331/)

---

For more information or issues, please refer to the Report directory or contact the development team.
