#include <stdio.h>
#include <stdlib.h>
#include <cstring>

// Enums
typedef enum
{
    NEW,
    IN_PROGRESS,
    REVIEW,
    DONE
} status_t;

typedef enum
{
    SUCCESS,
    NOT_FOUND,
    INVALID_STATUS
} update_status_t;

typedef enum
{
    LOGO,
    BROCHURE,
    WEBSITE,
    PACKAGING
} project_type_t;

// Structs
typedef struct
{
    char name[50];
    char email[50];
} client_t;

typedef struct
{
    char name[50];
    int duration;
    project_type_t type;
    status_t status;
    client_t client;
} project_t;

typedef struct
{
    int hours;
    float rate;
} cost_t;

// fucntions
int add_project(project_t projects[], int *num_projects, char name[], int duration, project_type_t type, status_t status, char client_name[], char client_email[]);
int find_project_by_name(project_t projects[], int num_projects, char name[]);
update_status_t update_project_status(project_t projects[], int num_projects, char name[], status_t new_status);
int update_project_duration(project_t projects[], int num_projects, char name[], int new_duration);
cost_t calculate_total_project_cost(project_t projects[], int num_projects);
int delete_project_by_name(project_t projects[], int *num_projects, char name[]);
