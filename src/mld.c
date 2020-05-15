#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "mld.h"

/* CSS macros */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char *DATA_TYPE[] = {
  "UINT8",
  "UINT32",
  "INT32",
  "CHAR",
  "OBJ_PTR",
  "VOID_PTR",
  "FLOAT",
  "DOUBLE",
  "OBJ_STRUCT"
};

void
print_structure_rec(struct_db_rec_t *struct_rec){
    if(!struct_rec) return;
    int j = 0;
    field_info_t *field = NULL;
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
    printf(ANSI_COLOR_CYAN "|------------------------------------------------------|------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    for(j = 0; j < struct_rec->n_fields; j++){
        field = &struct_rec->fields[j];
        printf("  %-20s |", "");
        printf("%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n",
                j, field->fname, DATA_TYPE[field->dtype], field->size, field->offset, field->nested_str_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_CYAN "--------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}

void
print_structure_db(struct_db_t *struct_db){

    if(!struct_db) return;
    printf("printing STRUCURE DATABASE\n");
    int i = 0;
    struct_db_rec_t *struct_rec = NULL;
    struct_rec = struct_db->head;
    printf("No of Structures Registered = %d\n", struct_db->count);
    while(struct_rec){
        printf("structure No : %d (%p)\n", i++, struct_rec);
        print_structure_rec(struct_rec);
        struct_rec = struct_rec->next;
    }
}

int
add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec){

    struct_db_rec_t *head = struct_db->head;

    if(!head){
        struct_db->head = struct_rec;
        struct_rec->next = NULL;
        struct_db->count++;
        return 0;
    }

    struct_rec->next = head;
    struct_db->head = struct_rec;
    struct_db->count++;
    return 0;
}

static struct_db_rec_t *
struct_db_look_up(struct_db_t *struct_db, char *struct_name){

    struct_db_rec_t *head = struct_db->head;
    if(!head) return NULL;

    for(; head; head = head->next){
        if(strncmp(head->struct_name, struct_name, MAX_STRUCTURE_NAME_SIZE) ==0)
            return head;
    }
    return NULL;
}

static object_db_rec_t *
object_db_look_up(object_db_t *object_db, void *ptr){

    object_db_rec_t *head = object_db->head;
    if(!head) return NULL;

    for(; head; head = head->next){
        if(head->ptr == ptr)
            return head;
    }
    return NULL;
}

static void
add_object_to_object_db(object_db_t *object_db, void *ptr, int units, struct_db_rec_t *struct_rec, mld_boolean_t is_root){
  object_db_rec_t *obj_rec = object_db_look_up(object_db, ptr);
  assert(!obj_rec);
  obj_rec = calloc(1, sizeof(object_db_rec_t));
  obj_rec->next = NULL;
  obj_rec->ptr = ptr;
  obj_rec->units = units;
  obj_rec->struct_rec = struct_rec;
  obj_rec->is_visited = MLD_FALSE;
  obj_rec->is_root = is_root;

  object_db_rec_t *head = object_db->head;

  if(!head){
    object_db->head = obj_rec;
    obj_rec->next = NULL;
    object_db->count++;
    return;
  }
  obj_rec->next = head;
  object_db->head = obj_rec;
  object_db->count++;
}

void*
xcalloc(object_db_t *object_db, char *struct_name, int units){
  struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
  assert(struct_rec);
  void *ptr = calloc(units, struct_rec->ds_size);
  add_object_to_object_db(object_db, ptr, units, struct_rec);
  return ptr;
}

void
print_object_rec(object_db_rec_t *obj_rec, int i){
  if(!obj_rec) return;
  printf(ANSI_COLOR_MAGENTA"|-----------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
  printf(ANSI_COLOR_YELLOW "|%-3d ptr = %-10p | next = %-10p | units = %-4d | struct_name = %-10s| is_root = %s \n", i, obj_rec->ptr, obj_rec->next, obj_rec->units, obj_rec->struct_rec->struct_name, obj_rec->is_root ? "TRUE" : "FALSE");
  printf(ANSI_COLOR_MAGENTA"|-----------------------------------------------------------------------------------------------------|\n"ANSI_COLOR_RESET);
}

void
print_object_db(object_db_t *object_db){
    object_db_rec_t *head = object_db->head;
    unsigned int i = 0;
    printf(ANSI_COLOR_CYAN"Printing OBJECT database\n");
    for(; head; head = head->next){
      print_object_rec(head, i++);
    }
}

void
mld_register_global_object_as_root(object_db_t *object_db, void *objptr, char *struct_name, unsigned int units){
  struct_db_rec_t *struct_rec = struct_db_look_up(object_db->struct_db, struct_name);
  add_object_to_object_db(object_db, objptr, units, struct_rec, MLD_TRUE);
}

void
mld_set_dynamic_object_as_root(object_db_t *object_db, void *obj_ptr){
  object_db_rec_t *obj_rec = object_db_look_up(object_db, obj_ptr);
  assert(obj_rec);
  obj_rec->is_root = MLD_TRUE;
}

static   void
init_mld_algorithm(object_db_t *object_db){
  object_db_rec_t *obj_rec = object_db->head;
  while(obj_rec){
    obj_rec->is_visited = MLD_FALSE;
    obj_rec = obj_rec->next;
  }
}

static void
mld_explore_objects_recursively(object_db_t *object_db, object_db_rec_t *object_db_rec){
  unsigned int i, n_fields;
  char *parent_obj_ptr = NULL,
       *child_obj_offset = NULL;
  void *child_object_address = NULL;
  field_info_t *field_info = NULL;

  object_db_rec_t *child_object_rec = NULL;
  struct_db_rec_t *parent_struct_rec = parent_obj_rec->struct_rec;
  assert(parent_obj_rec->is_visited);

  for(i = 0; i< parent_obj_rec->units; i++){
    parent_obj_ptr = (char*)(parent_obj_rec->ptr) + (i*parent_struct_rec->ds_size);
    for(n_fields = 0; n_fields< parent_struct_rec->n_fields; n_fields++){
      field_info = &parent_struct_rec->fields(n_fields);
      switch(field_info->dtype){
        case UINT8:
        case UINT32:
        case INT32:
        case CHAR:
        case OBJ_PTR:
        case FLOAT:
        case DOUBLE:
        case OBJ_STRUCT:
          break;
        default:
          ;
        child_obj_offset = parent_obj_ptr + field_info->offset;
        memcpy(&child_object_address, child_obj_offset, sizeof(void*));
        if(!child_object_address) continue;
        child_object_rec = object_db_look_up(object_db, child_object_address);
        assert(child_object_rec);
        if(!child_object_rec->is_visited){
          child_object_rec->is_visited = MLD_TRUE;
          if(field_info->dtype != VOID_PTR)
            mld_explore_objects_recursively(object_db, child_object_rec);
        }
        else{
          continue;
        }
      }
    }
  }
}

void
run_mld_algorithm(object_db_t *object_db){
  /* step 1: Mark all objects in object database as unvisited */
  init_mld_algorithm(object_db);
  /* step 2: Get the first root object from the object_db. it could be present anywhere in object_db.
     so the computational complexity at worst case will be O(n). If there are multiple roots in object_db, the
     algorithm returns the first one. We can start mld algorithm from any root object */
  object_db_rec_t *root_obj = get_next_root_object(object_db, NULL);
  while(root_obj){
    if(root_obj->is_visited){
      /* It means, all objects reachable from this root_obj has already been explored. No need to do it again. Else you will end
      up ininfinite loop. Remember, application data structures are cyclic graphs. */
      root_obj = get_next_root_object(object_db, root_obj);
      continue;
    }
    /*root objets are always reachable since application holds the global variable to it.*/
    root_obj->is_visited = MLD_TRUE;
    /* explore all reachable objects from this root_obj recursively */
    mld_explore_objects_recursively(object_db, root_obj);
    root_obj = get_next_root_object(object_db, root_obj);
  }
}
