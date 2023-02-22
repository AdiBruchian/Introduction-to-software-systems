#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked-list.h"
#include "grades.h"

#define FAIL -1
#define SUCCESS 0

//structs
struct grades {
    struct list *student_list;
};

struct student {
    int id;
    char *student_name;
    struct list *course_list;
};

struct course{
    char *course_name;
    int course_grade;
};


//function declarations
struct grades* grades_init();
void grades_destroy(struct grades *grades);
int grades_add_student(struct grades *grades, const char *name, int id);
int grades_add_grade(struct grades *grades, const char *name, int id, int grade);
float grades_calc_avg(struct grades *grades, int id, char **out);
int grades_print_student(struct grades *grades, int id);
int grades_print_all(struct grades *grades);


struct student* student_init(char *name, int id);
void student_destroy(void *element);
int student_clone(void *element, void **out);


struct course* course_init(char *name, int grade);
void course_destroy(void *element);
int course_clone(void *element, void **out);


//function implementations

struct grades* grades_init(){

    struct grades *grades = (struct grades*) malloc(sizeof(struct grades));
    if(grades == NULL){
        return NULL;
    }

    int(*clone_t)(void *element, void **output) = student_clone;
    void(*destroy_t)(void *element) = student_destroy;
    grades->student_list = list_init(clone_t,destroy_t);

    if(grades->student_list == NULL){
        return NULL;
    }

    return grades;
}

struct student* student_init(char *name, int id){

    struct student *student = (struct student*) malloc(sizeof(struct student));
    if (student == NULL){
        return NULL;
    }

    student -> student_name = (char*) malloc((strlen(name)+1)*sizeof(char));
    if (student -> student_name == NULL){
        return NULL;
    }

    strcpy(student -> student_name, name);

    student -> id = id;

    int(*clone_t)(void *element, void **output) = course_clone;
    void(*destroy_t)(void *element) = course_destroy;

    student -> course_list = list_init(clone_t,destroy_t);

    return student;
}

struct course* course_init(char *name, int grade){

    struct course *course = (struct course*)malloc(sizeof(struct course));
    if(course == NULL){
        return NULL;
    }

    course -> course_name = (char*)malloc((strlen(name)+1)*sizeof(char));
    if(course -> course_name == NULL){
        return NULL;
    }

    strcpy(course -> course_name, name);

    course -> course_grade = grade;

    return course;
}

void grades_destroy(struct grades *grades){
    list_destroy(grades->student_list);
    free(grades);
}

void student_destroy(void *element){
    struct student *student = (struct student*)element;
    list_destroy(student-> course_list);
    free(student->student_name);
    free(student);
}

void course_destroy(void *element){
    struct course *course = (struct course*)element;
    free(course->course_name);
    free(course);
}

int grades_add_student(struct grades *grades, const char *name, int id){

    if (grades == NULL){
        return FAIL;
    }

    struct iterator *student_iterator = list_begin(grades->student_list);


    while (student_iterator != NULL){
        if (((struct student*)list_get(student_iterator)) -> id == id){
            return FAIL;
        }
        student_iterator = list_next(student_iterator);
    }

    char *new_name= (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(new_name,name);

    struct student *new_student = student_init(new_name, id);
    free(new_name);

    if (new_student == NULL){
        return FAIL;
    }

    if(list_push_back(grades->student_list,new_student)==SUCCESS){
        free(new_student->student_name);
        list_destroy(new_student-> course_list);
        free(new_student);
        return SUCCESS;
    }

    free(new_student->student_name);
    list_destroy(new_student-> course_list);
    free(new_student);
    return FAIL;
}

int grades_add_grade(struct grades *grades, const char *name, int id, int grade){

    if (grades == NULL) {
        return FAIL;
    }

    struct iterator *student_iterator = list_begin(grades->student_list);


    struct student* curr_student = NULL;

    bool student_exists = false;

    while (student_iterator != NULL){
        if (((struct student*)list_get(student_iterator))-> id == id){
            student_exists = true;
            curr_student = (struct student*)list_get(student_iterator);
        }
        student_iterator = list_next(student_iterator);
    }

    if (!student_exists){
        return FAIL;
    }

    char *new_name = (char*)malloc(sizeof(char)*(strlen(name)+1));

    strcpy(new_name,name);

    struct iterator *course_iterator = list_begin(curr_student-> course_list);

    char *course_name = NULL;

    while (course_iterator != NULL){

        course_name = ((struct course*)list_get(course_iterator))->course_name;

        if (strcmp(name,course_name) == SUCCESS) {
            free(new_name);
            return FAIL;
        }
        course_iterator = list_next(course_iterator);
    }


    if (grade < 0 || grade > 100){
        free(new_name);
        return FAIL;
    }

    struct course* new_course = course_init(new_name, grade);
    free(new_name);

    if (list_push_back(curr_student-> course_list,new_course) == SUCCESS){
        free(new_course->course_name);
        free(new_course);
        return SUCCESS;
    }

    free(new_course->course_name);
    free(new_course);
    return FAIL;
}

int course_clone(void *element, void **out) {
    struct course *source = (struct course*)element;
    struct course *new_course = (struct course*)malloc(sizeof(struct course));
    if(!new_course){
        return FAIL;
    }
    new_course->course_name = (char*)malloc(sizeof(char*)*strlen(source->course_name)+1);
    if(new_course->course_name == NULL){
        return FAIL;
    }
    strcpy(new_course->course_name, source->course_name);
    new_course->course_grade = source->course_grade;
    *out = new_course;
    return SUCCESS;
}

int student_clone(void *element, void **out) {
    struct student *source = (struct student*)element;
    struct student *new_student = (struct student*)malloc(sizeof(struct student));
    if(!new_student){
        return FAIL;
    }

    new_student->id = source->id;
    new_student->student_name = (char*)malloc(sizeof(char)*strlen(source->student_name)+1);
    if(new_student->student_name==NULL){
        return FAIL;
    }
    strcpy(new_student->student_name, source->student_name);

    new_student->course_list = list_init(course_clone,course_destroy);
    struct iterator *course_itr = list_begin(source->course_list);
    while(course_itr){
        struct course *current_course = (struct course*)list_get(course_itr);
        list_push_back(new_student->course_list,(void*)current_course);
        course_itr = list_next(course_itr);
    }

    *out = new_student;
    return SUCCESS;
}

int grades_print_student(struct grades *grades, int id){
    if(!grades)
        return FAIL;
    struct iterator* student_cursor = list_begin(grades->student_list);
    while(student_cursor){
        struct student *current_student = (struct student*)list_get(student_cursor);
        if (current_student->id == id){
            printf("%s %d:",current_student->student_name,current_student->id);
            struct iterator* course_cursor = list_begin(current_student->course_list);
            while(course_cursor){
                struct course *current_course = (struct course*)list_get(course_cursor);
                printf(" %s %d",current_course->course_name,current_course->course_grade);
                course_cursor = list_next(course_cursor);
                if(course_cursor)
                    printf(",");
            }
            printf("\n");
            return SUCCESS;
        }
        student_cursor = list_next(student_cursor);
    }
    return FAIL;
}

int grades_print_all(struct grades *grades){
    if(!grades)
        return FAIL;
    struct iterator* student_cursor = list_begin(grades->student_list);
    while(student_cursor){
        struct student *current_student = (struct student*)list_get(student_cursor);
        printf("%s %d:",current_student->student_name,current_student->id);
        struct iterator* course_cursor = list_begin(current_student->course_list);
        while(course_cursor){
            struct course *current_course = (struct course*)list_get(course_cursor);
            printf(" %s %d",current_course->course_name,current_course->course_grade);
            course_cursor = list_next(course_cursor);
            if(course_cursor)
                printf(",");
        }
        printf("\n");
        student_cursor = list_next(student_cursor);
    }
    return SUCCESS;
}

float grades_calc_avg(struct grades *grades, int id, char **out){
    if(!grades){
        *out = NULL;
        return FAIL;
    }
    float grades_sum = 0;
    float course_number = 0;
    struct iterator* student_cursor = list_begin(grades->student_list);
    while(student_cursor){
        struct student *current_student = (struct student*)list_get(student_cursor);
        if (current_student->id == id){
            *out = (char*)malloc(sizeof(char)*strlen(current_student->student_name)+1);
            if(!out){
                *out = NULL;
                return FAIL;
            }
            strcpy(*out,current_student->student_name);
            struct iterator* course_cursor = list_begin(current_student->course_list);
            while(course_cursor){
                struct course *current_course = (struct course*)list_get(course_cursor);
                grades_sum += current_course->course_grade;
                course_number++;
                course_cursor = list_next(course_cursor);
            }
            if (course_number == 0)
                return 0;
            return grades_sum/course_number;
        }
        student_cursor = list_next(student_cursor);
    }
    *out = NULL;
    return FAIL;
}
