#include <stdio.h>
#include <string.h>
#include "desclib.h"

int field_count;

int main(){
  FILE *fptr;
  int i;

  fptr=fopen("faculty.frm","rb");

  if(fptr==NULL){
    printf("Error opening file");
  }

  else{
    set_field_count(fptr);
    struct field field_info[field_count];
    struct field *dets_ptr=field_info;
    describe_table(dets_ptr,fptr);
    }
  }
