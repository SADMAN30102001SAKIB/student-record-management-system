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

// Merge function to help with mergesort
void merge(Student arr[], int left, int mid, int right)
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
void mergesort(Student arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergesort(arr, left, mid);
        mergesort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void display_menu()
{
    printf("\n\n");
    printf("********** MENU **********\n");
    printf("1. Add Student\n");
    printf("2. Delete Student\n");
    printf("3. Display All Students\n");
    printf("4. Display Students By GPA\n");
    printf("5. Quit\n");
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
    scanf("%d", &new_student.id);
    printf("Enter student name: ");
    scanf("%s", new_student.name);
    printf("Enter student GPA: ");
    scanf("%f", &new_student.gpa);

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
    scanf("%d", &id_to_delete);

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
        return 0;
    }

    for (int i = index_to_delete; i < (*num_students) - 1; i++)
    {
        arr[i] = arr[i + 1];
    }

    (*num_students)--;

    printf("\nStudent deleted successfully.");

    return 0;
}

int display_all_students(Student arr[], int num_students)
{
    if (num_students == 0)
    {
        fprintf(stderr, "\nNo students to display.");
        return -1;
    }

    printf("\n\n");
    printf("********** ALL STUDENTS **********\n");
    printf("%-10s%-20s%-10s\n", "ID", "Name", "GPA");

    for (int i = 0; i < num_students; i++)
    {
        printf("%-10d%-20s%-10.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }

    return 0;
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

    mergesort(arr_copy, 0, num_students - 1); // sort

    printf("\n");
    printf("********** STUDENTS SORTED BY GPA **********\n");
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
    scanf("%d", &MAX_NUM_STUDENTS);

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