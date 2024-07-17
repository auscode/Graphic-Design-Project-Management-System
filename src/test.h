#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testAddProjectUniqueName()
    {
        project_t projects[10] = {0};
        int num_projects = 0;
        char name[] = "Project 1";
        int duration = 5;
        project_type_t type = LOGO;
        status_t status = NEW;
        char client_name[] = "Client 1";
        char client_email[] = "client1@example.com";
        TS_ASSERT_EQUALS(add_project(projects, &num_projects, name, duration, type, status, client_name, client_email), 1);
        TS_ASSERT_EQUALS(num_projects, 1);
    }
    void testAddProjectDuplicateName()
    {
        project_t projects[10] = {0};
        int num_projects = 0;
        char name[] = "Project 1";
        int duration = 5;
        project_type_t type = LOGO;
        status_t status = NEW;
        char client_name[] = "Client 1";
        char client_email[] = "client1@example.com";
        TS_ASSERT_EQUALS(add_project(projects, &num_projects, name, duration, type, status, client_name, client_email), 1);
        TS_ASSERT_EQUALS(add_project(projects, &num_projects, name, duration, type, status, client_name, client_email), 0);
        TS_ASSERT_EQUALS(num_projects, 1);
    }
    void testAddProjectMaxProjects()
    {
        project_t projects[5] = {0};
        int num_projects = 9;
        char name[] = "Project 6";
        int duration = 5;
        project_type_t type = LOGO;
        status_t status = NEW;
        char client_name[] = "Client 1";
        char client_email[] = "client1@example.com";
        TS_ASSERT_EQUALS(add_project(projects, &num_projects, name, duration, type, status, client_name, client_email), 1);
        TS_ASSERT_EQUALS(num_projects, 10);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    void testFindProjectNotFound()
    {
        int num_projects = 3;
        char name[] = "Project 1";
        project_t projects[num_projects] = {
            {"Project 2", 10, LOGO, IN_PROGRESS, {"Client 1", "client1@example.com"}},
            {"Project 3", 20, BROCHURE, NEW, {"Client 2", "client2@example.com"}},
            {"Project 4", 30, WEBSITE, DONE, {"Client 3", "client3@example.com"}}};
        int result = find_project_by_name(projects, num_projects, name);
        TS_ASSERT_EQUALS(result, -1);
    }
    void testFindProjectFound()
    {
        int num_projects = 3;
        char name[] = "Project 1";
        project_t projects[num_projects] = {
            {"Project 1", 10, LOGO, IN_PROGRESS, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, BROCHURE, NEW, {"Client 2", "client2@example.com"}},
            {"Project 3", 30, WEBSITE, DONE, {"Client 3", "client3@example.com"}}};
        int result = find_project_by_name(projects, num_projects, name);
        TS_ASSERT_EQUALS(result, 0);
    }
    void testFindProjectByName_Found()
    {
        project_t projects[10] = {
            {"Project 1", 30, LOGO, NEW, {"Client 1", "client1@gmail.com"}},
            {"Project 2", 60, BROCHURE, IN_PROGRESS, {"Client 2", "client2@gmail.com"}},
            {"Project 3", 90, WEBSITE, REVIEW, {"Client 3", "client3@gmail.com"}}};
        int num_projects = 3;
        char name[] = "Project 2";
        TS_ASSERT_EQUALS(find_project_by_name(projects, num_projects, name), 1);
    }
    ////////////////////////////////////////////////////////////////
    void testUpdateProjectStatusNotFound()
    {
        project_t projects[2] = {
            {"Project 1", 10, LOGO, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}},
        };
        int num_projects = 2;
        char name[] = "Project 3";
        update_status_t result = update_project_status(projects, num_projects, name, IN_PROGRESS);
        TS_ASSERT_EQUALS(result, NOT_FOUND);
    }
    void testUpdateProjectStatusNoChange()
    {
        project_t projects[2] = {
            {"Project 1", 10, LOGO, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}},
        };
        int num_projects = 2;
        char name1[] = "Project 1";
        update_status_t result = update_project_status(projects, num_projects, name1, NEW);
        TS_ASSERT_EQUALS(result, SUCCESS);
        TS_ASSERT_EQUALS(projects[0].status, NEW);
    }
    void testUpdateProjectStatusInvalidStatus()
    {
        project_t projects[2] = {
            {"Project 1", 10, LOGO, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}},
        };
        int num_projects = 2;
        char name1[] = "Project 1";
        update_status_t result = update_project_status(projects, num_projects, name1, REVIEW);
        TS_ASSERT_EQUALS(result, INVALID_STATUS);
        TS_ASSERT_EQUALS(projects[0].status, NEW);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////
    void testUpdateProjectDurationNotInList()
    {
        project_t projects[2] = {
            {"Project 1", 10, BROCHURE, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}}};
        int num_projects = 2;
        char name[] = "Project 3";
        int result = update_project_duration(projects, num_projects, name, 30);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(projects[0].duration, 10);
        TS_ASSERT_EQUALS(projects[1].duration, 20);
    }
    void testUpdateProjectDurationSuccess()
    {
        project_t projects[1];
        int num_projects = 1;
        char name[] = "Project 1";
        int duration = 10;
        project_type_t type = LOGO;
        status_t status = NEW;
        char client_name[] = "John";
        char client_email[] = "john@example.com";
        int result = update_project_duration(projects, num_projects, name, 20);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(projects[0].duration, 0);
    }
    void testUpdateProjectDurationNegativeDuration()
    {
        project_t projects[2] = {
            {"Project 1", 10, BROCHURE, NEW, {"Client 1", "client1@example.com"}},
            {"Project 2", 20, WEBSITE, IN_PROGRESS, {"Client 2", "client2@example.com"}}};
        int num_projects = 2;
        char name[] = "Project 1";
        int result = update_project_duration(projects, num_projects, name, -5);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(projects[0].duration, -5);
    }
    ////////////////////////////////////////////////////////////////////////////
    void testCalculateTotalProjectCostSingleBrochure()
    {
        project_t projects[] = {{"Project 2", 4, BROCHURE, REVIEW, {"Client B", "clientb@example.com"}}};
        cost_t expected_cost = {120, 6000.0};
        cost_t actual_cost = calculate_total_project_cost(projects, 1);
        TS_ASSERT_EQUALS(actual_cost.hours, expected_cost.hours);
        TS_ASSERT_DELTA(actual_cost.rate, expected_cost.rate, 0.001);
    }
    void testCalculateTotalProjectCostSingleWebsite()
    {
        project_t projects[] = {{"Project 3", 8, WEBSITE, IN_PROGRESS, {"Client C", "clientc@example.com"}}};
        cost_t expected_cost = {320, 16000.0};
        cost_t actual_cost = calculate_total_project_cost(projects, 1);
        TS_ASSERT_EQUALS(actual_cost.hours, expected_cost.hours);
        TS_ASSERT_DELTA(actual_cost.rate, expected_cost.rate, 0.001);
    }
    void testCalculateTotalProjectCostMultipleProjects()
    {
        project_t projects[] = {
            {"Project 4", 2, LOGO, DONE, {"Client D", "clientd@example.com"}},
            {"Project 5", 4, PACKAGING, NEW, {"Client E", "cliente@example.com"}},
            {"Project 6", 6, BROCHURE, IN_PROGRESS, {"Client F", "clientf@example.com"}},
            {"Project 7", 8, WEBSITE, REVIEW, {"Client G", "clientg@example.com"}}};
        cost_t expected_cost = {740, 37000.0};
        cost_t actual_cost = calculate_total_project_cost(projects, 4);
        TS_ASSERT_EQUALS(actual_cost.hours, expected_cost.hours);
        TS_ASSERT_DELTA(actual_cost.rate, expected_cost.rate, 0.001);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////
    void testDeleteProjectByName_Success()
    {
        char name1[] = "Project 1";
        char name2[] = "Project 2";
        char name3[] = "Project 3";
        project_t projects[3] = {
            {"Project 1", 10, BROCHURE, NEW, {"Client 1", "client1@gmail.com"}},
            {"Project 2", 15, WEBSITE, IN_PROGRESS, {"Client 2", "client2@gmail.com"}},
            {"Project 3", 20, LOGO, REVIEW, {"Client 3", "client3@gmail.com"}}};
        int num_projects = 3;
        int result = delete_project_by_name(projects, &num_projects, name2);
        TS_ASSERT_EQUALS(num_projects, 2);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(find_project_by_name(projects, num_projects, name2), -1);
        TS_ASSERT_EQUALS(projects[0].name, name1);
        TS_ASSERT_EQUALS(projects[1].name, name3);
    }
    void testDeleteProjectByName_NotFound()
    {
        char name1[] = "Project 1";
        char name2[] = "Project 2";
        char name3[] = "Project 3";
        char name4[] = "Project 4";
        project_t projects[3] = {
            {"Project 1", 10, BROCHURE, NEW, {"Client 1", "client1@gmail.com"}},
            {"Project 2", 15, WEBSITE, IN_PROGRESS, {"Client 2", "client2@gmail.com"}},
            {"Project 3", 20, LOGO, REVIEW, {"Client 3", "client3@gmail.com"}}};
        int num_projects = 3;
        int result = delete_project_by_name(projects, &num_projects, name4);
        TS_ASSERT_EQUALS(num_projects, 3);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(projects[0].name, name1);
        TS_ASSERT_EQUALS(projects[1].name, name2);
        TS_ASSERT_EQUALS(projects[2].name, name3);
    }
    void testDeleteProjectByName_Successful()
    {
        char name1[] = "Project 1";
        char name2[] = "Project 2";
        char name3[] = "Project 3";
        project_t projects[3] = {
            {"Project 1", 5, BROCHURE, NEW, {"John", "john@example.com"}},
            {"Project 2", 10, LOGO, IN_PROGRESS, {"Jane", "jane@example.com"}},
            {"Project 3", 15, WEBSITE, REVIEW, {"Bob", "bob@example.com"}}};
        int num_projects = 3;
        int result = delete_project_by_name(projects, &num_projects, name2);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(num_projects, 2);
        TS_ASSERT_EQUALS(strcmp(projects[0].name, name1), 0);
        TS_ASSERT_EQUALS(strcmp(projects[1].name, name3), 0);
    }
};
