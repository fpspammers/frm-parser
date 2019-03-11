void showDB()
{ 
	struct dirent *de; // Pointer for directory entry 

	// opendir() returns a pointer of DIR type. 
	DIR *dr = opendir("/var/lib/mysql"); 

	if (dr == NULL) // opendir returns NULL if couldn't open directory 
	{ 
		printf("Could not open current directory" ); 
		return 0; 
	} 

	
	while ((de = readdir(dr)) != NULL) {
			if(de->d_type==DT_DIR)//can also compare with the numeric constant 4 
			printf("%s\n", de->d_name); 
		}

	closedir(dr);	 
} 


void showTables()
{ 
	
	char file_name[256],db_name[256], path_var[256], *ptr;
	int i;
	strcpy(path_var,"/var/lib/mysql/");
	printf("\n Enter Database Name\n");
	scanf("%s",db_name);
	strcat(path_var,db_name); // setting the appropriate path according to user's selection
	
	struct dirent *de; // Pointer for directory entry 
	
	// opendir() returns a pointer of DIR type. 
	DIR *dr = opendir(path_var); 
	
	if (dr == NULL) // opendir returns NULL if couldn't open directory 
	{ 
		printf("Could not open current directory\n" ); 
		return 0; 
	} 


	while ((de = readdir(dr)) != NULL) 
		{
			if(de->d_type==DT_REG) //DT_REG is a constant for to represent files.
			{
				strcpy(file_name,de->d_name);
				ptr=strrchr(file_name,'.'); // looking for thr last ocurence of '.'
				ptr++;
				if(*ptr=='f')

				{
					for(i=0;file_name[i]!='.';i++)
						printf("%c",file_name[i]);
		
					printf("\n");
				}
			}


		}

	closedir(dr);	  
} 

