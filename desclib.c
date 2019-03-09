#include <stdio.h>
#include <string.h>
#include "desclib.h"

int field_count;

void field_names(FILE *f_ptr,struct field *dets_ptr){
  FILE *set_ptr=set_to_data_types(f_ptr);

  while(fgetc(f_ptr)!=0xff){
    continue;
  }

  int ch,i=0,field_no=0;

  while(field_no<field_count){
    ch=fgetc(f_ptr);

    if(ch==0xff){
      dets_ptr->name[i]='\0';
      field_no++;
      dets_ptr++;
      i=0;
    }

    else{
      dets_ptr->name[i]=ch;
      i++;
    }
  }
}

FILE *set_to_data_types(FILE *f_ptr){
  fseek(f_ptr,0x2150,SEEK_SET);
  int zero_count_pre=0,zero_count_post=0,marker_flag=0,ch;

  while(1){
    ch=fgetc(f_ptr);
    if(!marker_flag){
      if(ch==0x00){
        zero_count_pre++;
      }
      else if(ch==0x02){
        marker_flag=1;
      }
      else{
        zero_count_pre=0;
      }
    }

    else{
      if(ch==0x00){
        zero_count_post++;
      }
      else{
        zero_count_post=0;
      }
    }

    if(zero_count_pre==1 && zero_count_post==2 && marker_flag==1){
      break;
    }
  }
  fseek(f_ptr,5,SEEK_CUR);
  return f_ptr;
}

void field_data_types(FILE *f_ptr,struct field *dets_ptr){
  FILE *set_ptr=set_to_data_types(f_ptr);
  int ch,field_no=0;

  while(field_no<field_count){
    ch=fgetc(f_ptr);

    switch(ch){
      case 3:
        strncpy(dets_ptr->type,"int",20);
        break;
      case 4:
        strncpy(dets_ptr->type,"float",20);
        break;
      case 5:
        strncpy(dets_ptr->type,"double",20);
        break;
      case 6:
        strncpy(dets_ptr->type,"null",20);
        break;
      case 0x0b:
        strncpy(dets_ptr->type,"time",20);
        break;
      case 0x0d:
        strncpy(dets_ptr->type,"year",20);
        break;
      case 0x0e:
        strncpy(dets_ptr->type,"date",20);
        break;
      case 0x0f:
        strncpy(dets_ptr->type,"varchar",20);
        break;
      default:
        strncpy(dets_ptr->type,"unreg",20);
        break;
    }

    fseek(set_ptr,16,SEEK_CUR);
    dets_ptr++;
    field_no++;
  }
}

void describe_table(struct field *dets_ptr){
  int i;
  printf("+-------------------------------------------+\n");
  printf("| %-20s| %-20s|\n","field","data type");
  printf("+-------------------------------------------+\n");
  for(i=0;i<field_count;i++){
    printf("| %-20s| %-20s|\n",dets_ptr->name,dets_ptr->type);
    dets_ptr++;
  }
  printf("+-------------------------------------------+\n");
}

void set_field_count(FILE *f_ptr){
  fseek(f_ptr,0x2102,SEEK_SET);
  field_count=fgetc(f_ptr);
}
