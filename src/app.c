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
  struct student_ *best_college;
}student_t;

int
main(int argc, char **argv){

  /* Create new structure database*/
  struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));

  /* Create structure record for structure emp_t */
  static field_info_t emp_fields[] = {
    FIELD_INFO(emp_t, emp_name, CHAR, 0),
    FIELD_INFO(emp_t, emp_id, UINT32, 0),
    FIELD_INFO(emp_t, age, UINT32, 0),
    FIELD_INFO(emp_t, mgr, OBJ_PTR, emp_t),
    FIELD_INFO(emp_t, salary, FLOAT, 0),
  };
  REG_STRUCT(struct_db, emp_t, emp_fields);

  static field_info_t std_fields[] = {
    FIELD_INFO(student_t, std_name, CHAR, 0),
    FIELD_INFO(student_t, rollno, UINT32, 0),
    FIELD_INFO(student_t, age, UINT32, 0),
    FIELD_INFO(student_t, aggregate, FLOAT, 0),
    FIELD_INFO(student_t, best_college, OBJ_PTR, student_t)
  };
  REG_STRUCT(struct_db, student_t, std_fields);

  print_structure_db(struct_db);

  /* initialize a new object database */
  object_db_t *object_db = calloc(1, sizeof(object_db_t));
  object_db->struct_db = struct_db;

  student_t *ben = xcalloc(object_db, "student_t", 1);
  student_t *eslami = xcalloc(object_db, "student_t", 1);
  student_t *pramod = xcalloc(object_db, "emp_t", 2);

  print_structure_db(struct_db);
  return 0;
}
