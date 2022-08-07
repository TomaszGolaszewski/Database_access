/*
	Test program to check several solutions for Linux, Makefile, GCC, access to database and multithreading...
	
	by Tomasz Golaszewski
	2022.08.07
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for time
#include <unistd.h> // for pid
#include "module.h"
  
// main function
int main(void)
{	
	int lenght_of_local_db = 16;
	int size_of_record = sizeof(int);
	int *local_db = malloc(lenght_of_local_db * size_of_record);
	char name_of_database_file[] = "database.bin";	
	char name_of_lockfile[] = "lockfile.bin";
	char name_of_logfile[] = "logfile.txt";
	
	struct timespec time_of_last_update;

	lock_file(name_of_lockfile);	
	load_data(name_of_database_file, size_of_record, lenght_of_local_db, local_db);
	clock_gettime(CLOCK_REALTIME, &time_of_last_update);
	unlock_file(name_of_lockfile);
	
	for(int i=0; i<lenght_of_local_db; i++) printf("%d ", local_db[i]);
	printf("\n");
	
	char may_I;
	printf("May I continue (Y/n): ");
	scanf("%c", &may_I);
	lock_file(name_of_lockfile);
	update_data(name_of_database_file, name_of_logfile, size_of_record, lenght_of_local_db, local_db, time_of_last_update);
	clock_gettime(CLOCK_REALTIME, &time_of_last_update);
	unlock_file(name_of_lockfile);
	
	for(int i=0; i<lenght_of_local_db; i++) printf("%d ", local_db[i]);
	printf("\n");
	
	int index_to_change, new_value;
	printf("index to change: ");
	scanf("%d", &index_to_change);
	printf("new value: ");
	scanf("%d", &new_value);
	change_data(index_to_change, new_value, local_db);
	
	for(int i=0; i<lenght_of_local_db; i++) printf("%d ", local_db[i]);
	printf("\n");
	
	lock_file(name_of_lockfile);	
	save_data(name_of_database_file, size_of_record, lenght_of_local_db, local_db);
	save_log(name_of_logfile, index_to_change);
	unlock_file(name_of_lockfile);
  
  	printf("Program completed and closed correctly\n");
	return 0;
}
