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
  OBJ_STRUCT
}data_type_t;

#define OFFSETOF(struct_name, fld_name) \
        (unsigned int)&(((struct_name*)0)->fld_name)

#define FIELD_SIZE(struct_name, fld_name) \
        sizeof(((struct_name*)0)->fld_name)

typedef struct _struct_db_rec_t struct_db_rec_t;

typedef struct _field_info_{
  char fname[MAX_FIELD_NAME_SIZE];
  data_type_t dtype;
  unsigned int size;
  unsigned int offset;
  char nested_str_name[MAX_STRUCTURE_NAME_SIZE]; // Below field is meaningful only if dtype = OBJ_PTR, or OBJ_STRUCT
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

void
print_structure_rec(struct_db_rec_t*);
void
print_structure_db(struct_db_t*);
int
add_structure_to_struct_db(struct_db_t*, struct_db_rec_t*);

/*Structure registration helping APIs */
#define FIELD_INFO(struct_name, fld_name, dtype, nested_str_name)  \
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
#endif
