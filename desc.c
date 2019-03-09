#include <stdio.h>
#include <string.h>
#include "desclib.h"

int field_count;

int main(){
  FILE *fptr;
  int i;

  fptr=fopen("/var/lib/mysql/Student/stud_details.frm","rb");

  if(fptr==NULL){
    printf("Error opening file");
  }

  else{
    set_field_count(fptr);
    struct field field_info[field_count];
    struct field *dets_ptr=field_info;
    field_names(fptr,dets_ptr);
    field_data_types(fptr,dets_ptr);
    describe_table(dets_ptr);
    }
  }
