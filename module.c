#include <stdio.h>
#include <stdlib.h>
#include "module.h"

void load_data(int lenght_of_table, int *table_with_data)
{
	FILE *fp; // pointer to file
	
	fp = fopen ("database.txt", "r"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("File cannot be opened!"); // if doesn't print error message
		exit(-10);
	}
	
	//fread(table_with_data, sizeof(int), lenght_of_table, fp);
	for(int i=0; i<lenght_of_table; i++) fscanf(fp,"%d", &table_with_data[i]);
	//for(int i=0; i<lenght_of_table; i++) printf("%d ", table_with_data[i]);
	//printf(" funkcja\n");
	
	fclose (fp); // close the file
	printf("Data loaded\n");
}

void save_data(int lenght_of_table, int *table_with_data)
{
	FILE *fp; // pointer to file
	
	fp = fopen ("database.txt", "w"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("File cannot be opened!"); // if doesn't print error message
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

void lock_file(void)
{
	FILE *fp; // pointer to file
	fp = fopen ("lockfile.txt", "r+"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("Lockfile cannot be opened!"); // if doesn't print error message
		exit(-10);
	}
	int flag;
	int pid = getpid();
	printf("Waiting for access...\n");
	do 
	{
		rewind(fp);
		fscanf(fp,"%d", &flag);
	}
	while(flag);
	fprintf (fp, "%d", pid);
	fclose (fp); // close the file
	printf("File locked\n");
}

void unlock_file(void)
{
	FILE *fp; // pointer to file
	fp = fopen ("lockfile.txt", "w"); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("Lockfile cannot be opened!"); // if doesn't print error message
		exit(-10);
	}
	fprintf (fp, "0");
	fclose (fp); // close the file
	printf("File unlocked\n");
}


