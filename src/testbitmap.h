#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testAddProjectMaxNameLength()
    {
        project_t projects[10] = {0};
        int num_projects = 0;
        char name[] = "Project 123456789012345678901234567890123456789012345678901234567890";
        int duration = 5;
        project_type_t type = LOGO;
        status_t status = NEW;
        char client_name[] = "Client 1";
        char client_email[] = "client1@example.com";
        TS_ASSERT_EQUALS(add_project(projects, &num_projects, name, duration, type, status, client_name, client_email), 1);
        TS_ASSERT_EQUALS(num_projects, 1);
    }

    void testAddProjectMaxClientNameLength()
    {
        project_t projects[10] = {0};
        int num_projects = 0;
        char name[] = "Project 1";
        int duration = 5;
        project_type_t type = LOGO;
        status_t status = NEW;
        char client_name[] = "Client 123456789012345678901234567890123456789012345678901234567890";
        char client_email[] = "client1@example.com";
        TS_ASSERT_EQUALS(add_project(projects, &num_projects, name, duration, type, status, client_name, client_email), 1);
        TS_ASSERT_EQUALS(num_projects, 1);
    }
    void testAddProjectNonexistentClient()
    {
        project_t projects[10] = {0};
        int num_projects = 0;
        char name[] = "Project 1";
        char client_name[] = "John Smith";
        char client_email[] = "john.smith@example.com";
        int result = add_project(projects, &num_projects, name, 30, WEBSITE, NEW, client_name, client_email);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(num_projects, 1);
        TS_ASSERT_EQUALS(strcmp(projects[0].name, "Project 1"), 0);
        TS_ASSERT_EQUALS(projects[0].duration, 30);
        TS_ASSERT_EQUALS(projects[0].type, WEBSITE);
        TS_ASSERT_EQUALS(projects[0].status, NEW);
        TS_ASSERT_EQUALS(strcmp(projects[0].client.name, "John Smith"), 0);
        TS_ASSERT_EQUALS(strcmp(projects[0].client.email, "john.smith@example.com"), 0);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    void testFindProjectNullProjects()
    {
        int num_projects = 3;
        char name[] = "Project 1";
        project_t *projects = NULL;
        int result = find_project_by_name(projects, num_projects, name);
        TS_ASSERT_EQUALS(result, -1);
    }
    void testFindProjectNullName()
    {
        int num_projects = 3;
        char *name = NULL;
        project_t projects[num_projects];
        int result = find_project_by_name(projects, num_projects, name);
        TS_ASSERT_EQUALS(result, -1);
    }

    void testFindProjectEmptyName()
    {
        int num_projects = 3;
        char name[] = "";
        project_t projects[num_projects];
        int result = find_project_by_name(projects, num_projects, name);
        TS_ASSERT_EQUALS(result, -1);
    }
    ///////////////////////////////////////////////////////////////////
    void testUpdateProjectStatusNewToInProgress()
    {
        project_t projects[2] = {
            {"Project 1", 10, LOGO, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}},
        };
        int num_projects = 2;
        char name1[] = "Project 1";
        update_status_t result = update_project_status(projects, num_projects, name1, IN_PROGRESS);
        TS_ASSERT_EQUALS(result, SUCCESS);
        TS_ASSERT_EQUALS(projects[0].status, IN_PROGRESS);
    }

    void testUpdateProjectStatusInProgressToReview()
    {
        project_t projects[2] = {
            {"Project 1", 10, LOGO, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}},
        };
        int num_projects = 2;
        char name1[] = "Project 2";
        update_status_t result = update_project_status(projects, num_projects, name1, REVIEW);
        TS_ASSERT_EQUALS(result, SUCCESS);
        TS_ASSERT_EQUALS(projects[1].status, REVIEW);
    }

    void testUpdateProjectStatusReviewToDone()
    {
        project_t projects[2] = {
            {"Project 1", 10, LOGO, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, REVIEW, {"Client 2", "client2@example.com"}},
        };
        int num_projects = 2;
        char name1[] = "Project 2";
        update_status_t result = update_project_status(projects, num_projects, name1, DONE);
        TS_ASSERT_EQUALS(result, SUCCESS);
        TS_ASSERT_EQUALS(projects[1].status, DONE);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void testUpdateProjectDurationInProgressToReview()
    {
        project_t projects[2] = {
            {"Project 1", 10, BROCHURE, IN_PROGRESS, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, REVIEW, {"Client 2", "client2@example.com"}}};
        int num_projects = 2;
        char name[] = " Project 1 ";
        int result = update_project_duration(projects, num_projects, name, 5);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(projects[0].duration, 10);
        TS_ASSERT_EQUALS(projects[0].status, IN_PROGRESS);
    }

    void testUpdateProjectDurationReviewToReview()
    {
        project_t projects[2] = {
            {"Project 1", 10, BROCHURE, REVIEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, REVIEW, {"Client 2", "client2@example.com"}}};
        int num_projects = 2;
        char name[] = " Project 1 ";
        int result = update_project_duration(projects, num_projects, name, 7);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(projects[0].duration, 10);
        TS_ASSERT_EQUALS(projects[0].status, REVIEW);
    }
    void testUpdateProjectDuration_NotFound()
    {
        project_t projects[3] = {
            {"Project 1", 5, WEBSITE, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 3, PACKAGING, IN_PROGRESS, {"Client 2", "client2@example.com"}},
            {"Project 3", 10, BROCHURE, DONE, {"Client 3", "client3@example.com"}}};
        int num_projects = 3;
        char name[] = "Project 4";
        int new_duration = 7;
        int result = update_project_duration(projects, num_projects, name, new_duration);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(projects[0].duration, 5);
        TS_ASSERT_EQUALS(projects[1].duration, 3);
        TS_ASSERT_EQUALS(projects[2].duration, 10);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void testCalculateTotalProjectCostNoProjects()
    {
        project_t projects[0];
        cost_t cost = calculate_total_project_cost(projects, 0);
        TS_ASSERT_EQUALS(cost.hours, 0);
        TS_ASSERT_EQUALS(cost.rate, 0.0);
    }
    void testCalculateTotalProjectCostUnknownProjectType()
    {
        project_t projects[] = {{"Project 8", 3, (project_type_t)10, REVIEW, {"Client H", "clienth@example.com"}}};
        cost_t expected_cost = {0, 0.0};
        cost_t actual_cost = calculate_total_project_cost(projects, 1);
        TS_ASSERT_EQUALS(actual_cost.hours, expected_cost.hours);
        TS_ASSERT_DELTA(actual_cost.rate, expected_cost.rate, 0.001);
    }
    void testCalculateTotalProjectCostSingleProject()
    {
        project_t projects[1];
        projects[0].duration = 5;
        projects[0].type = LOGO;
        cost_t cost = calculate_total_project_cost(projects, 1);
        TS_ASSERT_EQUALS(cost.hours, 100);
        TS_ASSERT_EQUALS(cost.rate, 5000.0);
    }
    void testCalculateTotalProjectCostSingleLogo()
    {
        project_t projects[] = {{"Project 1", 2, LOGO, DONE, {"Client A", "clienta@example.com"}}};
        cost_t expected_cost = {40, 2000.0};
        cost_t actual_cost = calculate_total_project_cost(projects, 1);
        TS_ASSERT_EQUALS(actual_cost.hours, expected_cost.hours);
        TS_ASSERT_DELTA(actual_cost.rate, expected_cost.rate, 0.001);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    void testDeleteProjectByName_NotFound2()
    {
        char name1[] = "Project 1";
        char name2[] = "Project 2";
        char name3[] = "Project 3";
        project_t projects[2] = {
            {"Project 1", 5, BROCHURE, NEW, {"John", "john@example.com"}},
            {"Project 2", 10, LOGO, IN_PROGRESS, {"Jane", "jane@example.com"}}};
        int num_projects = 2;
        int result = delete_project_by_name(projects, &num_projects, name3);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(num_projects, 2);
        TS_ASSERT_EQUALS(strcmp(projects[0].name, name1), 0);
        TS_ASSERT_EQUALS(strcmp(projects[1].name, name2), 0);
    }

    void testDeleteProjectByName_LastProject()
    {
        char name1[] = "Project 1";
        char name2[] = "Project 2";
        project_t projects[2] = {
            {"Project 1", 5, BROCHURE, NEW, {"John", "john@example.com"}},
            {"Project 2", 10, LOGO, IN_PROGRESS, {"Jane", "jane@example.com"}}};
        int num_projects = 2;
        int result = delete_project_by_name(projects, &num_projects, name2);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(num_projects, 1);
        TS_ASSERT_EQUALS(strcmp(projects[0].name, name1), 0);
    }

    void testDeleteProjectByNameEmptyArray()
    {
        project_t projects[10];
        int num_projects = 0;
        char name[] = "Project 1";
        int result = delete_project_by_name(projects, &num_projects, name);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(num_projects, 0);
    }
};