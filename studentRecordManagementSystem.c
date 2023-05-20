#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 20

typedef struct
{
    int id;
    char name[MAX_SIZE];
    float gpa;
} Student;

int save_students_to_file(Student arr[], int num_students, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        fprintf(stderr, "\nError saving to file: %s\n", filename);
        perror("fopen");
        return -1;
    }

    for (int i = 0; i < num_students; i++)
    {
        fprintf(file, "%d,%s,%.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }

    if (fclose(file) != 0)
    {
        fprintf(stderr, "\nError closing file: %s\n", filename);
        perror("fclose");
        return -1;
    }

    return 0;
}

int get_students_from_file(Student arr[], int *num_students, const char *filename, int MAX_NUM_STUDENTS)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        fprintf(stderr, "\nError opening file: %s\n", filename);
        perror("fopen");
        return -1;
    }

    printf("\ngetting data from file...\n");

    char line[100];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        int id;
        char name[50];
        float gpa;

        if (sscanf(line, "%d,%[^,],%f", &id, name, &gpa) == 3)
        {
            if (*num_students == MAX_NUM_STUDENTS)
            {
                fprintf(stderr, "Maximum number of students reached.");
                return -1;
            }

            arr[*num_students].id = id;
            strcpy(arr[*num_students].name, name);
            arr[*num_students].gpa = gpa;

            (*num_students)++;
        }
    }

    if (fclose(file) != 0)
    {
        fprintf(stderr, "\nError closing file: %s\n", filename);
        perror("fclose");
        return -1;
    }

    printf("done getting %d student data.", *num_students);

    return 0;
}

void display_menu()
{
    printf("\n\n");
    printf("********** MENU **********\n");
    printf("1. Add Student\n");
    printf("2. Delete Student\n");
    printf("3. Display All Students\n");
    printf("4. Display Students By GPA\n");
    printf("5. Display Students By Name\n");
    printf("6. Search Students\n");
    printf("7. Calculate Average GPA\n");
    printf("8. Update Student\n");
    printf("9. Quit\n");
}

int add_student(Student arr[], int *num_students, int MAX_NUM_STUDENTS)
{
    if (*num_students == MAX_NUM_STUDENTS)
    {
        fprintf(stderr, "\nMaximum number of students reached.");
        return -1;
    }

    Student new_student;

    printf("\nEnter student ID: ");
    if (scanf("%d", &new_student.id) != 1)
    {
        fprintf(stderr, "\nInvalid input for student ID.");
        return -1;
    }

    for (int i = 0; i < *num_students; i++)
    {
        if (arr[i].id == new_student.id)
        {
            fprintf(stderr, "\nStudent with the same ID already exists.\n");
            return -1;
        }
    }

    getchar();
    printf("Enter student name: ");
    fgets(new_student.name, MAX_SIZE, stdin);
    new_student.name[strlen(new_student.name) - 1] = '\0';

    printf("Enter student GPA: ");
    if (scanf("%f", &new_student.gpa) != 1)
    {
        fprintf(stderr, "\nInvalid input for student GPA.");
        return -1;
    }

    if (new_student.gpa < 0 || new_student.gpa > 4)
    {
        fprintf(stderr, "\nInvalid GPA. Please enter a value between 0 and 4.\n");
        return -1;
    }

    arr[*num_students] = new_student;
    (*num_students)++;

    printf("\nStudent added successfully.");

    return 0;
}

int delete_student(Student arr[], int *num_students)
{
    if (*num_students == 0)
    {
        fprintf(stderr, "\nNo students to delete.");
        return -1;
    }

    int id_to_delete;

    printf("\nEnter ID of student to delete: ");
    if (scanf("%d", &id_to_delete) != 1)
    {
        fprintf(stderr, "\nInvalid input for student ID.");
        return -1;
    }

    int index_to_delete = -1;

    for (int i = 0; i < *num_students; i++)
    {
        if (arr[i].id == id_to_delete)
        {
            index_to_delete = i;
            break;
        }
    }

    if (index_to_delete == -1)
    {
        printf("\nStudent with ID %d not found.", id_to_delete);
        return -1;
    }

    for (int i = index_to_delete; i < (*num_students) - 1; i++)
    {
        arr[i] = arr[i + 1];
    }

    (*num_students)--;

    printf("\nStudent deleted successfully.");

    return 0;
}

int update(Student arr[], int num_students)
{
    int id;

    printf("\nEnter student ID: ");
    if (scanf("%d", &id) != 1)
    {
        fprintf(stderr, "\nInvalid input for student ID.");
        return -1;
    }

    int index = -1;

    for (int i = 0; i < num_students; i++)
    {
        if (arr[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("\nStudent with ID %d not found.", id);
        return -1;
    }

    printf("Enter new name: ");
    getchar();
    fgets(arr[index].name, MAX_SIZE, stdin);
    arr[index].name[strlen(arr[index].name) - 1] = '\0';

    printf("Enter new GPA: ");
    if (scanf("%f", &arr[index].gpa) != 1)
    {
        fprintf(stderr, "\nInvalid input for student GPA.");
        return -1;
    }

    printf("\nStudent updated successfully.");

    return 0;
}

int display_all_students(Student arr[], int num_students)
{
    if (num_students == 0)
    {
        fprintf(stderr, "\nNo students to display.");
        return -1;
    }

    printf("\n");
    printf("********** ALL STUDENTS **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        printf("%-10d%-20s%-10.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }

    return 0;
}

int avg_gpa(Student arr[], int num_students)
{
    if (num_students == 0)
    {
        fprintf(stderr, "\n0 students.");
        return -1;
    }

    float sum = 0;

    for (int i = 0; i < num_students; i++)
    {
        sum += arr[i].gpa;
    }

    printf("\nThe average GPA is %f\n", sum / num_students);

    return 0;
}

void display_search_menu()
{
    printf("\n");
    printf("********** SEARCH BY **********\n");
    printf("1. ID\n");
    printf("2. GPA\n");
    printf("3. Name\n");
}

int search_by_ID(Student arr[], int num_students)
{
    int id;

    printf("\nEnter student ID: ");
    if (scanf("%d", &id) != 1)
    {
        fprintf(stderr, "\nInvalid input for student ID.");
        return -1;
    }

    int index = -1;

    for (int i = 0; i < num_students; i++)
    {
        if (arr[i].id == id)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("\nStudent with ID %d not found.", id);
        return -1;
    }

    printf("\n");
    printf("********** SEARCH RESULTS **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        if (arr[i].id == id)
        {
            printf("%-10d%-20s%-10.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
        }
    }

    return 0;
}

int search_by_GPA(Student arr[], int num_students)
{
    float gpa;

    printf("\nEnter student GPA: ");
    if (scanf("%f", &gpa) != 1)
    {
        fprintf(stderr, "\nInvalid input for student GPA.");
        return -1;
    }

    if (gpa < 0.0 || gpa > 4.0)
    {
        fprintf(stderr, "\nInvalid GPA. Please enter a value between 0 and 4.\n");
        return -1;
    }

    int index = -1;

    for (int i = 0; i < num_students; i++)
    {
        if (arr[i].gpa == gpa)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("\nStudent with GPA %f not found.", gpa);
        return -1;
    }

    printf("\n");
    printf("********** SEARCH RESULTS **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        if (arr[i].gpa == gpa)
        {
            printf("%-10d%-20s%-10.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
        }
    }

    return 0;
}

int search_by_Name(Student arr[], int num_students)
{
    char name[MAX_SIZE];

    printf("\nEnter student Name: ");
    getchar();
    fgets(name, MAX_SIZE, stdin);
    name[strlen(name) - 1] = '\0';

    int index = -1;

    for (int i = 0; i < num_students; i++)
    {
        if (strcmp(arr[i].name, name) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("\nStudent with GPA %s not found.", name);
        return -1;
    }

    printf("\n");
    printf("********** SEARCH RESULTS **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        if (strcmp(arr[i].name, name) == 0)
        {
            printf("%-10d%-20s%-10.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
        }
    }

    return 0;
}

int search_students(Student arr[], int num_students)
{
    if (num_students == 0)
    {
        fprintf(stderr, "\nNo students to display.");
        return -1;
    }

    display_search_menu();

    int choice;

    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1)
    {
        fprintf(stderr, "\nInvalid input for choice.\n");
        return -1;
    }

    if (choice < 1 || choice > 3)
    {
        fprintf(stderr, "\nInvalid Choice. Please enter a value between 1 and 3.\n");
        return -1;
    }

    switch (choice)
    {
    case 1:
        search_by_ID(arr, num_students);
        break;
    case 2:
        search_by_GPA(arr, num_students);
        break;
    case 3:
        search_by_Name(arr, num_students);
        break;
    default:
        printf("Invalid choice.\n");
    }

    return 0;
}

// Merge function to help with mergesort
void merge(Student arr[], int left, int mid, int right, int gpa_name)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Student L[n1], R[n2];

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (gpa_name)
        {
            if (L[i].gpa >= R[j].gpa)
            {
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
        }
        else
        {
            char num1 = L[i].name[0], num2 = R[j].name[0];

            if (num1 >= 'a' && num1 <= 'z')
            {
                num1 -= 32;
            }
            if (num2 >= 'a' && num2 <= 'z')
            {
                num2 -= 32;
            }

            if (num1 <= num2)
            {
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
        }

        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Mergesort function
void mergesort(Student arr[], int left, int right, int gpa_name)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesort(arr, left, mid, gpa_name);
        mergesort(arr, mid + 1, right, gpa_name);

        merge(arr, left, mid, right, gpa_name);
    }
}

int display_students_by_gpa(Student arr[], int num_students, int MAX_NUM_STUDENTS)
{
    if (num_students == 0)
    {
        fprintf(stderr, "\nNo students to display.");
        return -1;
    }

    // Create a copy of the array to sort by GPA
    Student arr_copy[MAX_NUM_STUDENTS];

    for (int i = 0; i < num_students; i++)
    {
        arr_copy[i] = arr[i];
    }

    mergesort(arr_copy, 0, num_students - 1, 1); // sort

    printf("\n");
    printf("********** STUDENTS SORTED BY GPA **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        printf("%-10d%-20s%-10.2f\n", arr_copy[i].id, arr_copy[i].name, arr_copy[i].gpa);
    }

    return 0;
}

int display_students_by_name(Student arr[], int num_students, int MAX_NUM_STUDENTS)
{
    if (num_students == 0)
    {
        fprintf(stderr, "\nNo students to display.");
        return -1;
    }

    // Create a copy of the array to sort by GPA
    Student arr_copy[MAX_NUM_STUDENTS];

    for (int i = 0; i < num_students; i++)
    {
        arr_copy[i] = arr[i];
    }

    mergesort(arr_copy, 0, num_students - 1, 0); // sort

    printf("\n");
    printf("********** STUDENTS SORTED BY NAME **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        printf("%-10d%-20s%-10.2f\n", arr_copy[i].id, arr_copy[i].name, arr_copy[i].gpa);
    }

    return 0;
}

int main()
{
    int MAX_NUM_STUDENTS;

    printf("\nHow many student's data you have or want to store?: ");
    if (scanf("%d", &MAX_NUM_STUDENTS) != 1)
    {
        fprintf(stderr, "\nInvalid input for MAX number.\n");
        exit(1);
    }

    Student *arr = malloc(MAX_NUM_STUDENTS * sizeof(Student));

    if (arr == NULL)
    {
        fprintf(stderr, "\nFailed to allocate memory\n");
        exit(1);
    }

    int num_students = 0;

    get_students_from_file(arr, &num_students, "students.txt", MAX_NUM_STUDENTS);

    while (1)
    {
        display_menu();

        int choice;

        printf("Enter your choice: ");
        scanf("%d", &choice);
        while ((choice < 1 || choice > 9))
        {
            fprintf(stderr, "\nInvalid Choice. Please enter a value between 1 and 9.\n");
            printf("Enter your choice again: ");
            scanf("%d", &choice);
        }

        switch (choice)
        {
        case 1:
            add_student(arr, &num_students, MAX_NUM_STUDENTS);
            break;
        case 2:
            delete_student(arr, &num_students);
            break;
        case 3:
            display_all_students(arr, num_students);
            break;
        case 4:
            display_students_by_gpa(arr, num_students, MAX_NUM_STUDENTS);
            break;
        case 5:
            display_students_by_name(arr, num_students, MAX_NUM_STUDENTS);
            break;
        case 6:
            search_students(arr, num_students);
            break;
        case 7:
            avg_gpa(arr, num_students);
            break;
        case 8:
            update(arr, num_students);
            break;
        case 9:
            printf("Goodbye!\n");
            save_students_to_file(arr, num_students, "students.txt");
            free(arr);
            exit(0);
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}