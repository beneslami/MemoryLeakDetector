#include "mld.h"
#include <memory.h>
#include <stdlib.h>

typedef struct emp_ {
  char emp_name[30];
  unsigned int emp_id;
  unsigned int age;
  struct emp_ *mgr;
  float salary;
}emp_t;

typedef struct student_{
  char std_name[30];
  unsigned int rollno;
  unsigned int age;
  float aggregate;
  struct student_ *best_colleague;
  int *p;
}student_t;

int
main(int argc, char **argv){

  student_t *ben = calloc(1, sizeof(student_t)); //root object
  strncpy(ben->std_name, "benben", strlen("benben"));
  ben->rollno = 12345;
  ben->age = 27;
  ben->aggregate = 12.24;
  ben->best_colleague = calloc(1, sizeof(student_t)); //Forth Object
  strncpy(ben->best_colleague->std_name, "Michio Honda", strlen("Michio Honda"));
  ben->best_colleague->rollno = 54321;
  ben->best_colleague->age = 32;
  ben->best_colleague->aggregate = 30.12;
  ben->p = calloc(1, sizeof(int)); // testing primitive data type
  *(ben->p) = 10;

  student_t *eslami = calloc(1, sizeof(student_t));  // Third object
  strncpy(eslami->std_name, "Benyamin", strlen("Benyamin"));
  eslami->rollno = 9876;
  eslami->age = 72;
  eslami->aggregate = 99.9;

  emp_t *pramod = calloc(2, sizeof(emp_t));  // second object
  strncpy(pramod->emp_name, "Pramod", strlen("Pramod"));
  pramod->emp_id = 123;
  pramod->age = 40;
  pramod->salary = 1000.1;
  pramod->mgr = calloc(1, sizeof(emp_t));  // first object
  strncpy(pramod->mgr->emp_name, "Benji", strlen("Benji"));

/*
  free(pramod->mgr);
  free(pramod);
  free(eslami);
  free(ben->p);
  free(ben->best_colleague);
  free(ben);  */

  return 0;
}
