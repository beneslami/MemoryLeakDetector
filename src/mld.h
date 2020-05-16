#ifndef MLD_H
#define MLD_H

#include <assert.h>

#define MAX_STRUCTURE_NAME_SIZE 128
#define MAX_FIELD_NAME_SIZE 128

typedef enum{
  UINT8,
  UINT32,
  INT32,
  CHAR,
  OBJ_PTR,
  FLOAT,
  DOUBLE,
  OBJ_STRUCT,
  VOID_PTR
}data_type_t;

typedef enum{
  MLD_FALSE,
  MLD_TRUE
}mld_boolean_t;

typedef struct _struct_db_rec_t struct_db_rec_t;

/* structure database */
typedef struct _field_info_{
  char fname[MAX_FIELD_NAME_SIZE];
  data_type_t dtype;
  unsigned int size;
  unsigned int offset;
  char nested_str_name[MAX_STRUCTURE_NAME_SIZE]; // This field is meaningful only if dtype = OBJ_PTR, or OBJ_STRUCT
} field_info_t;

/* Structure to store the information of one C Structure
  which could have 'n fields' fields */
struct _struct_db_rec_t{
  struct_db_rec_t *next;
  char struct_name[MAX_STRUCTURE_NAME_SIZE];
  unsigned int ds_size;
  unsigned int n_fields;
  field_info_t *fields;
};

/* The head of the linked list representing the Structure database */
typedef struct _struct_db_{
  struct_db_rec_t *head;
  unsigned int count;
}struct_db_t;

/* APIs declaration */
void
print_structure_rec(struct_db_rec_t*);
void
print_structure_db(struct_db_t*);
int
add_structure_to_struct_db(struct_db_t*, struct_db_rec_t*);

/* General Macros */
#define OFFSETOF(struct_name, fld_name) \
        (unsigned int)&(((struct_name*)0)->fld_name)   /* NULL pointer of type struct_name */
#define FIELD_SIZE(struct_name, fld_name) \
        sizeof(((struct_name*)0)->fld_name)            /* NULL pointer of type struct_name */
/* Structure registration helping APIs */
#define FIELD_INFO(struct_name, fld_name, dtype, nested_str_name)     \
  {#fld_name, dtype, FIELD_SIZE(struct_name, fld_name),               \
    OFFSETOF(struct_name, fld_name), #nested_str_name}

#define REG_STRUCT(struct_db, st_name, fields_arr)                    \
  do {                                                                \
    struct_db_rec_t *rec = calloc(1, sizeof(struct_db_rec_t));        \
    strncpy(rec->struct_name, #st_name, MAX_STRUCTURE_NAME_SIZE);     \
    rec->ds_size = sizeof(st_name);                                   \
    rec->n_fields = sizeof(fields_arr)/sizeof(field_info_t);          \
    rec->fields = fields_arr;                                         \
    if(add_structure_to_struct_db(struct_db, rec)){                   \
      assert(0);                                                      \
    }                                                                 \
  } while(0);

/* Object database */
typedef struct _object_db_rec_ object_db_rec_t;
struct _object_db_rec_{
  object_db_rec_t *next;
  void *ptr; /* key */
  unsigned int units;
  struct_db_rec_t *struct_rec;
  mld_boolean_t is_root; /* Is this object root or not */
  mld_boolean_t is_visited; /* used for Graph traversal */
};
typedef struct _object_db_{
  struct_db_t *struct_db;
  object_db_rec_t *head;
  unsigned int count;
}object_db_t;

/* APIs declaration */
void*
xcalloc(object_db_t *, char*, int);
void
print_object_rec(object_db_rec_t*, int);
void
print_object_db(object_db_t*);
void
mld_register_global_object_as_root(object_db_t*, void*, char*, unsigned int);
void
mld_set_dynamic_object_as_root(object_db_t*, void*);
void
run_mld_algorithm(object_db_t*);
void
report_leaked_objects(object_db_t*);
void
xfree(object_db_t*, void*);

#endif
