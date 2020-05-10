# MemoryLeakDetector

Since the advent of C/C++ programming language, memory management is one of the responsibility which developer has to deal with. C/C++ softwares often suffers from two memory related problems like Memory Corruption and Memory Leak. Unlike shitty language like Java, C/C++ do not have luxury things for automatic garbage collection. Java does not allow programmer to access the physical memory directly, but C/C++ does. Therefore, Java applications do not suffer from Memory corruption either, but C/C++ do.

According to below diagram, what happens when a C application calls malloc/free in normal world.

![picture](data/diagram.png)

But, in our project how the diagram is changed:

![picture](data/diagram2.png)

MLD acts like a middle man between C application and OS/Kernel. All memory access via the application is passed to the OS/Kernel via MLD.

The project development phase consists of several steps:
1. MLD library will maintain the information about all structures which the application is using by creating structure database.
2. MLD library will maintain the information about all objects malloc'd by the application by creating object database.
3. MLD library triggers Memory Leak Detection Algorithm on Object database to find leaked objects.

![picture](data/projectdevelopmentphase.png)

### phase 1
Design and implementation of MLD structure database

MLD library must know the information about all structures being used by the application. It is the responsibility of the application to tell the MLD library during initialization about all structures it is using. This is called "**structure Registration**". MLD library will maintain structure database (preferably a linkedlist) to store application structure information. Key to search in structure database is "name of structure".

 ![picture](data/phase1.png)

 Modeling of structure database is as below:

 ```
 typedef struct _field_info_{
   char fname[MAX_FIELD_NAME_SIZE];
   unsigned int size;
   unsigned int offset;
   data_type_t dtype;
   char nested_str_name[MAX_STRUCTURE_NAME_SIZE];
 }field_infor_t;
 ```

 ```
 #define MAX_STRUCTURE_NAME_SIZE 128
 #define MAX_FIELD_NAME_SIZE 128

 typedef struct _struct_db_rec_ struct_db_rec_t;

 struct _struct_db_rec_struct_db_rec_{
   struct_db_rec_t *next;
   char struct_name[MAX_STRUCTURE_NAME_SIZE]; \\Key
   unsigned int ds_size;
   unsigned int n_fields;
   field_infor_t *fields;
 };
```  

```
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
```
For example, our application uses below data structure:
```
typedef struct emp_{
  char emp_name[30];
  unsigned int emp_id;
  unsigned int age;
  struct emp_ *mgr;
  float salary;
}emp_t;
```
Now, our goal is that our C application should be able to register this structure with the MLD library. We are about to see how this structure information will look like in a structure database of MLD library after registration. So in structure database, the first thing to do is to create a structure database record.

![picture](data/structuredatabase.png)

As you can see, the first item is null because at this point of time there is only one structure record in the structure database. The second item is the name of the data structure. Next field is the size of the structure. After that, the next item is the number of items and last field is actually the pointer to an array which will going to store the information about each fields in the data structure. So, last item is pointer to the array of fields, and each element of the array stores the information of one field of C structure.

![picture](data/structuredatabase2.png)

So, remember the data type of fields array is ```field_info_t```. And each field stores one element and its information in C structure. Note that, for "mgr" pointer, the sizeof value is specified by writing void*. No matter what data type mgr is pointing to. the size of mgr is always 4 bytes. So, you can see that how structure database will look like when the C application does registration of its structure with the MLD library. MLD library will have the information about the C structure store in the format of ```struct_db_rec_t```. Obviously, in our C application, we have to write a good deal of code in order to tell the MLD library about the information of the C structure that our C application is using. That is, first C application should malloc ```struct_db_rec_t``` and then populate the field of this structure database record with the information of C structure whose registration is being done. And then our C application should create an array of fields and finally links "fields" pointer in structure database record to point to that array.
