#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "desclib.h"

int field_count;
char table_name[256];

//Extracts the field names from the appropriate offsets and stores each one in the array of structures

void field_names(FILE *f_ptr,struct field *dets_ptr){
  FILE *set_ptr=set_to_data_types(f_ptr); //Sets pointer to point to the start of the section of the file that stores the data types.

  while(fgetc(f_ptr)!=0xff){ //Moves the pointer ahead until it doesnt encounter a byte containing the value ff or 255.
    continue;
  }

  int ch,i=0,field_no=0;

  while(field_no<field_count){
    ch=fgetc(f_ptr);

    if(ch==0xff){ //If the byte that was read has the value ff then it indicates the end of a field name, thus the struct pointer and field_no need to be incremented.
      dets_ptr->name[i]='\0';
      field_no++;
      dets_ptr++;
      i=0;
    }

    else{ //Else reads each character and adds it to the field name string
      dets_ptr->name[i]=ch;
      i++;
    }
  }
}

//Dynamically sets the file pointer to point to the offset in the file from where the data types can begin to be read

FILE *set_to_data_types(FILE *f_ptr){
  fseek(f_ptr,0x2150,SEEK_SET); //seeks to a neutral offset close to the target offset.
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
        marker_flag=0;
      }
    }

    if(zero_count_pre==1 && zero_count_post==2 && marker_flag==1){
      break;
    }
  }
  return f_ptr;
}

//Extracts the byte that corresponds to the data-type of the field, compares it to expected values and stores the data type as a string in the array of structures.

void field_data_types(FILE *f_ptr,struct field *dets_ptr){

  FILE *set_ptr=set_to_data_types(f_ptr);
  fseek(set_ptr,5,SEEK_CUR);  //Sets the pointer to the offset where it can start reading the data types

  int ch,field_no=0;

  while(field_no<field_count){
    ch=fgetc(f_ptr);

    switch(ch){ //Switch-Case checks the value of the byte and stores the data type that corresponds to it.
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

    fseek(set_ptr,16,SEEK_CUR); //Moves the pointer ahead 16 bytes to the next field's data type byte.
    dets_ptr++;
    field_no++;
  }
}

//Extracts the size in bytes of each field and stores it in the array of structures.

void field_sizes(FILE *f_ptr, struct field *dets_ptr){
  FILE *set_ptr=set_to_data_types(f_ptr);
  fseek(set_ptr,-6,SEEK_CUR);
  int field_no=0;

  while(field_no<field_count){
    dets_ptr->size=fgetc(f_ptr);
    fseek(set_ptr,16,SEEK_CUR);
    dets_ptr++;
    field_no++;
  }
}

//Prints all of the values from the array of strutures in a formatted manner.

void describe_table(struct field *dets_ptr,FILE *f_ptr){
  int i;
  field_names(f_ptr,dets_ptr);
  field_data_types(f_ptr,dets_ptr);
  field_sizes(f_ptr,dets_ptr);
  printf("+---------------------+---------------------+------+\n");
  printf("| %-20s| %-20s| %-5s|\n","field","data type","size");
  printf("+---------------------+---------------------+------+\n");
  for(i=0;i<field_count;i++){
    printf("| %-20s| %-20s| %-5d|\n",dets_ptr->name,dets_ptr->type,dets_ptr->size);
    dets_ptr++;
  }
  printf("+---------------------+---------------------+------+\n");
}

//Reads the byte containing the number of fields in the table and stores it in the field_count variable.

void set_field_count(FILE *f_ptr){
  fseek(f_ptr,0x2102,SEEK_SET);
  field_count=fgetc(f_ptr);
}

//Displays all the tables in the Database.

void show_tables(){
	char file_name[256], db_name[256], path_var[256], *ptr;
	int i;
	strcpy(path_var,"/var/lib/mysql/");
	printf("\nEnter Database Name\n");
	scanf("%s",db_name);
	strcat(path_var,db_name); // setting the appropriate path according to user's selection

	struct dirent *de; // Pointer for directory entry

	// opendir() returns a pointer of DIR type.
	DIR *dr = opendir(path_var);

	if (dr == NULL) // opendir returns NULL if couldn't open directory
	{
		printf("Could not open current directory\n" );
	}

  else{
    printf("+---------------------+\n| %-20s|\n+---------------------+\n","Tables");
    while ((de = readdir(dr)) != NULL){

      if(de->d_type==DT_REG){ //DT_REG is a constant for to represent files.
  			strcpy(file_name,de->d_name);
  			ptr=strrchr(file_name,'.'); // looking for thr last ocurence of '.'
  			ptr++;

        if(*ptr=='f'){
  				for(i=0;file_name[i]!='.';i++){
            table_name[i]=file_name[i];
          }
          table_name[i]='\0';
          printf("| %-20s|\n",table_name);
  			}
  		}
  	}
    printf("+---------------------+\n");
  	closedir(dr);
  }
}

//Displays all the databases on the system.

void show_db(){
	struct dirent *de; // Pointer for directory entry

	// opendir() returns a pointer of DIR type.
	DIR *dr = opendir("/var/lib/mysql");

	if (dr == NULL){ // opendir returns NULL if couldn't open directory
		printf("Could not open current directory" );
	}

  printf("+---------------------+\n| %-20s|\n+---------------------+\n","Databases");
	while ((de = readdir(dr)) != NULL){
			if(de->d_type==DT_DIR)//can also compare with the numeric constant 4
			   printf("| %-20s|\n",de->d_name);
	}
  printf("+---------------------+\n");
	closedir(dr);
}
