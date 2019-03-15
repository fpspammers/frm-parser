#include "desclib.h"

int main(){
  FILE *fptr;
  us_int option,flag=1;

  fptr=fopen("faculty.frm","rb");

  if(fptr==NULL){
    printf("Error opening file");
  }

  else{
    set_field_count(fptr);
    struct field field_info[field_count];
    struct field *dets_ptr=field_info;

    while(flag){
      printf("\nType a number for the command you want to perform:\n");
      printf("\n-> For describe tables type '1'\n");
      printf("-> For show tables type '2'\n");
      printf("-> For show databses type '3'\n");
      printf("-> To quit type '4'\n\n");
      scanf("%hu",&option);

      switch(option){
        case 1:
          describe_table(dets_ptr,fptr);
          break;
        case 2:
          show_tables();
          break;
        case 3:
          show_db();
          break;
        case 4:
          flag=0;
          break;
        default:
          printf("Type a valid option.\n");
      }
    }
  }
}
