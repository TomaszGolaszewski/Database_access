#include <stdio.h>
#include <stdlib.h>
#include "module.h"

void load_data(char *name_of_database, int lenght_of_table, int *table_with_data)
{
	FILE *fp; // pointer to file
	
	fp = fopen (name_of_database, "r"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("File cannot be opened!"); // if doesn't - print error message
		exit(-10);
	}
	
	//fread(table_with_data, sizeof(int), lenght_of_table, fp);
	for(int i=0; i<lenght_of_table; i++) fscanf(fp,"%d", &table_with_data[i]);
	//for(int i=0; i<lenght_of_table; i++) printf("%d ", table_with_data[i]);
	//printf(" funkcja\n");
	
	fclose (fp); // close the file
	printf("Data loaded\n");
}

void save_data(char *name_of_database, int lenght_of_table, int *table_with_data)
{
	FILE *fp; // pointer to file
	
	fp = fopen (name_of_database, "w"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("File cannot be opened!"); // if doesn't - print error message
		exit(-10);
	}
	//fprintf (fp, "%s", table_with_data); // save data
	for(int i=0; i<lenght_of_table; i++) fprintf (fp, "%d ", table_with_data[i]);
	fclose (fp); // close the file
	printf("Data saved\n");
}

void change_data(int index_to_change, int new_value, int *table_with_data)
{
	table_with_data[index_to_change] = new_value;
}

void lock_file(char *name_of_lockfile)
{
	FILE *fp; // pointer to file
	int flag;
	int pid = getpid();
	//printf("pid %d\n", pid);
	
	fp = fopen (name_of_lockfile, "r+b"); // open file
	if( fp == NULL ) // check if file exists
	{
		fp = fopen (name_of_lockfile, "wb"); // if doesn't - make new lockfile
		printf("New lockfile was made\n");
	}
	else
	{
		printf("Waiting for access...\n");
		do 
		{
			fscanf(fp,"%d", &flag);
			rewind(fp);
		}
		while(flag != -1);
	}
	fprintf (fp, "%d", pid);
	fclose (fp); // close the file
	printf("File locked\n");
}

void unlock_file(char *name_of_lockfile)
{
	FILE *fp; // pointer to file
	fp = fopen (name_of_lockfile, "wb"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("Lockfile cannot be opened!"); // if doesn't - print error message
		exit(-10);
	}
	fprintf (fp, "-1");
	fclose (fp); // close the file
	printf("File unlocked\n");
}


