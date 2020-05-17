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

  /* Create new structure database*/
  struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));
  mld_init_primitive_data_types_support(struct_db);

  /* Create structure record for structure emp_t */
  static field_info_t emp_fields[] = {
    FIELD_INFO(emp_t, emp_name, CHAR, 0),
    FIELD_INFO(emp_t, emp_id, UINT32, 0),
    FIELD_INFO(emp_t, age, UINT32, 0),
    FIELD_INFO(emp_t, mgr, OBJ_PTR, emp_t),
    FIELD_INFO(emp_t, salary, FLOAT, 0)
  };
  REG_STRUCT(struct_db, emp_t, emp_fields);

  static field_info_t std_fields[] = {
    FIELD_INFO(student_t, std_name, CHAR, 0),
    FIELD_INFO(student_t, rollno, UINT32, 0),
    FIELD_INFO(student_t, age, UINT32, 0),
    FIELD_INFO(student_t, aggregate, FLOAT, 0),
    FIELD_INFO(student_t, best_colleague, OBJ_PTR, student_t)
  };
  REG_STRUCT(struct_db, student_t, std_fields);

  //print_structure_db(struct_db);

  /* initialize a new object database */
  object_db_t *object_db = calloc(1, sizeof(object_db_t));
  object_db->struct_db = struct_db;

  student_t *ben = xcalloc(object_db, "student_t", 1); //root object
  strncpy(ben->std_name, "benben", strlen("benben"));
  ben->rollno = 12345;
  ben->age = 27;
  ben->aggregate = 12.24;
  ben->best_colleague = xcalloc(object_db, "student_t", 1); //Forth Object
  strncpy(ben->best_colleague->std_name, "Michio Honda", strlen("Michio Honda"));
  ben->best_colleague->rollno = 54321;
  ben->best_colleague->age = 32;
  ben->best_colleague->aggregate = 30.12;
  ben->p = xcalloc(object_db, "int", 1); // testing primitive data type
  *(ben->p) = 10;
  mld_set_dynamic_object_as_root(object_db, ben);

  student_t *eslami = xcalloc(object_db, "student_t", 1);  // Third object
  strncpy(eslami->std_name, "Benyamin", strlen("Benyamin"));
  eslami->rollno = 9876;
  eslami->age = 72;
  eslami->aggregate = 99.9;

  emp_t *pramod = xcalloc(object_db, "emp_t", 2);  // second object
  strncpy(pramod->emp_name, "Pramod", strlen("Pramod"));
  pramod->emp_id = 123;
  pramod->age = 40;
  pramod->salary = 1000.1;
  pramod->mgr = xcalloc(object_db, "emp_t", 1);  // first object
  strncpy(pramod->mgr->emp_name, "Benji", strlen("Benji"));

  xfree(object_db, pramod->mgr);
  xfree(object_db, pramod);
  xfree(object_db, eslami);
  xfree(object_db, ben->best_colleague);
  xfree(object_db, ben->p);
  //print_object_db(object_db);
  run_mld_algorithm(object_db);
  report_leaked_objects(object_db);
  return 0;
}
