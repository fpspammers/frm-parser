#ifndef DESCLIB_H_INCLUDED
#define DESCLIB_H_INCLUDED
#include <stdio.h>
#include <string.h>
#include <dirent.h>

typedef unsigned short int us_int;

struct field{
  char name[20];
  char type[20];
  us_int size;
};

us_int field_count;

us_int field_names(FILE *fptr,struct field *dets_ptr);
us_int field_data_types(FILE *fptr,struct field *dets_ptr);
us_int field_sizes(FILE *f_ptr, struct field *dets_ptr);
FILE *set_to_data_types(FILE *fptr);
us_int describe_table(struct field *dets_ptr,FILE *fptr);
void set_field_count(FILE *f_ptr);
void show_db();
void show_tables();
#endif
