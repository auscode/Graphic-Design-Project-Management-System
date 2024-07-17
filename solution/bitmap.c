#include "bitmap.h"

/*
This function adds a new project to an array of projects, given the project's name, duration, type, status, client name, and client email.
It first checks if a project with the same name already exists in the array, and if so, returns 0 indicating failure to add the project.
If the project doesn't exist in the array, a new client and project are created with the given information, added to the array,
and the number of projects is incremented. The function then returns 1 indicating success in adding the project.
*/
int add_project(project_t projects[], int *num_projects, char name[], int duration, project_type_t type, status_t status, char client_name[], char client_email[])
{
    if (find_project_by_name(projects, *num_projects, name) != -1)
    {
        return 0;
    }
    client_t new_client;
    strncpy(new_client.name, client_name, sizeof(new_client.name));
    strncpy(new_client.email, client_email, sizeof(new_client.email));
    project_t new_project;
    strncpy(new_project.name, name, sizeof(new_project.name));
    new_project.duration = duration;
    new_project.type = type;
    new_project.status = status;
    new_project.client = new_client;
    projects[*num_projects] = new_project;
    (*num_projects)++;
    return 1;
}

/*
This function searches an array of project_t structures for a project with a specific name.
It returns the index of the matching project if found, or -1 if the project is not found or
if the input parameters are invalid (e.g., projects or name is NULL or name has length 0).
*/
int find_project_by_name(project_t projects[], int num_projects, char name[])
{
    if (projects == NULL || name == NULL)
    {
        return -1;
    }
    if (strlen(name) == 0)
    {
        return -1;
    }
    for (int i = 0; i < num_projects; i++)
    {
        if (strcmp(projects[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/*
This function updates the status of a project in an array of projects.
It first finds the project with a given name and returns an error if the project is not found.
Then it checks if the new status is valid based on the current status of the project.
If the new status is not valid, it returns an error.
Finally, if the new status is valid, it updates the project status and returns success.
*/
update_status_t update_project_status(project_t projects[], int num_projects, char name[], status_t new_status)
{
    int project_index = find_project_by_name(projects, num_projects, name);

    if (project_index == -1)
    {
        return NOT_FOUND;
    }

    if (projects[project_index].status == new_status)
    {
        return SUCCESS;
    }

    if ((projects[project_index].status == NEW && new_status != IN_PROGRESS) ||
        (projects[project_index].status == IN_PROGRESS && new_status != REVIEW && new_status != DONE) ||
        (projects[project_index].status == REVIEW && new_status != DONE) ||
        (projects[project_index].status == DONE && new_status != NEW))
    {
        return INVALID_STATUS;
    }

    projects[project_index].status = new_status;

    return SUCCESS;
}

/*
This function updates the duration of a project in an array of projects. It takes in the array of projects,
the number of projects, the name of the project to be updated, and the new duration. It searches for the
project by name, updates its duration, and returns 1 if successful. If the project is not found, it returns 0.
Additionally, it handles edge cases where the new duration is negative or the project is in progress or
review and the new duration is less than the current duration. In such cases, it sets the project status to review.
*/
int update_project_duration(project_t projects[], int num_projects, char name[], int new_duration)
{
    int found = 0;
    int i = 0;
    // Search for the project by name
    for (i = 0; i < num_projects; i++)
    {
        if (strcmp(projects[i].name, name) == 0)
        {
            // Update the project's duration
            projects[i].duration = new_duration;
            found = 1;
            break;
        }
    }
    // Edge case: if project not found, return 0
    if (!found)
    {
        return 0;
    }
    // Edge case: if new duration is negative, set it to zero
    if (new_duration < 0)
    {
        new_duration = 0;
    }
    // Edge case: if the project is in progress or review and the new duration
    // is less than the current duration, set the project status to review
    if ((projects[i].status == IN_PROGRESS || projects[i].status == REVIEW) &&
        new_duration < projects[i].duration)
    {
        projects[i].status = REVIEW;
    }
    // Return success
    return 1;
}

/*
This function calculates the total cost of a list of projects by iterating through the array of project
objects and calculating the total hours worked based on the type and duration of each project.
The hourly rate for each project type is fixed and set as follows: LOGO - 20 hours at $50 per hour,
BROCHURE - 30 hours at $50 per hour, WEBSITE - 40 hours at $50 per hour, and PACKAGING - 50 hours at $50 per hour.
The total cost is returned as a structure that includes the total hours worked and the total cost at a fixed hourly rate of $50 per hour.
*/

cost_t calculate_total_project_cost(project_t projects[], int num_projects)
{
    cost_t total_cost;
    total_cost.hours = 0;
    total_cost.rate = 0;
    for (int i = 0; i < num_projects; i++)
    {
        if (projects[i].type == LOGO)
        {
            total_cost.hours += 20 * projects[i].duration;
        }
        else if (projects[i].type == BROCHURE)
        {
            total_cost.hours += 30 * projects[i].duration;
        }
        else if (projects[i].type == WEBSITE)
        {
            total_cost.hours += 40 * projects[i].duration;
        }
        else if (projects[i].type == PACKAGING)
        {
            total_cost.hours += 50 * projects[i].duration;
        }
    }
    total_cost.rate = total_cost.hours * 50.0;
    return total_cost;
}

/*
This function deletes a project from an array of projects by name. It first searches for the project with the given name in the array.
If found, it shifts all the projects after the one to be deleted down by one position, decrements the number of projects, clears
the memory used by the deleted project, and returns 1 to indicate success. If the project is not found, it returns 0.
*/
int delete_project_by_name(project_t projects[], int *num_projects, char name[])
{
    // Search for the project with the given name
    int index = find_project_by_name(projects, *num_projects, name);
    if (index == -1)
    {
        // The project wasn't found
        return 0;
    }

    // Shift all projects after the one to be deleted down by one position
    for (int i = index; i < *num_projects - 1; i++)
    {
        projects[i] = projects[i + 1];
    }

    // Decrement the number of projects
    (*num_projects)--;

    // Clear the memory used by the deleted project
    memset(&projects[*num_projects], 0, sizeof(project_t));

    // Return 1 to indicate success
    return 1;
}
