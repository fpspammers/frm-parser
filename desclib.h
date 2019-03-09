#ifndef DESCLIB_H_INCLUDED
#define DESCLIB_H_INCLUDED

struct field{
  char name[20];
  char type[20];
  int size;
};

void field_names(FILE *fptr,struct field *dets_ptr);
void field_data_types(FILE *fptr,struct field *dets_ptr);
FILE *set_to_data_types(FILE *fptr);
void describe_table(struct field *dets_ptr);
void set_field_count(FILE *f_ptr);
#endif
