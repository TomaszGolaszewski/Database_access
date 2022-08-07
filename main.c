/*
	Test program to check several solutions for Linux, Makefile, GCC, access to database and multithreading...
	
	by Tomasz Golaszewski
	2022.08.07
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for time
#include <unistd.h> // for pid
#include <sys/stat.h> // for files' stats
#include "module.h"

/*
// structure for storing record
struct Record
{
	int number;
};
*/

// main function
int main(void)
{	
	int lenght_of_local_db = 16;
	int size_of_record = sizeof(struct Record);
	struct Record *local_db = malloc(lenght_of_local_db * size_of_record);
	char name_of_database_file[] = "database.bin";	
	char name_of_lockfile[] = "lockfile.bin";
	char name_of_logfile[] = "logfile.txt";
	
	struct timespec time_of_last_update; // time of last data update
	/*	time_t  tv_sec;	// Seconds
        	long    tv_nsec;	// Nanoseconds
	*/
	struct stat file_stats; // struct with file data - among other things, time of last modification of the database file
	/*	struct timespec st_atim;	// Time of last access
		struct timespec st_mtim;	// Time of last modification
		struct timespec st_ctim;	// Time of last status change 
	*/
	
	// load database first time
	lock_file(name_of_lockfile);	
	load_data(name_of_database_file, size_of_record, lenght_of_local_db, local_db);
	clock_gettime(CLOCK_REALTIME, &time_of_last_update); // update time of last data update
	unlock_file(name_of_lockfile);
	
	// print database
	for(int i=0; i<lenght_of_local_db; i++) printf("%d ", local_db[i].number);
	printf("\n");
	
	char may_I;
	printf("Do you want to change some records? (Y/n): ");
	scanf("%c", &may_I);
		
    	stat(name_of_database_file, &file_stats); // get stats of the database file	
   	if(file_stats.st_mtime > time_of_last_update.tv_sec) // if the database file was modified, update local_db
   	{
   		printf("In the meantime, the database has been changed - updating...\n");
		lock_file(name_of_lockfile);
		update_data(name_of_database_file, name_of_logfile, size_of_record, lenght_of_local_db, local_db, time_of_last_update);
		clock_gettime(CLOCK_REALTIME, &time_of_last_update); // update time of last data update
		unlock_file(name_of_lockfile);
		
		// print database
		for(int i=0; i<lenght_of_local_db; i++) printf("%d ", local_db[i].number);
		printf("\n");
	}
	
	int index_to_change, new_value;
	if(may_I == (int)'Y' || may_I == (int)'y') // 'Y' == 89, 'y' == 121
	{	
		printf("Index to change: ");
		scanf("%d", &index_to_change);
		printf("New value: ");
		scanf("%d", &new_value);
		change_data(index_to_change, new_value, local_db);
		
		// print database
		for(int i=0; i<lenght_of_local_db; i++) printf("%d ", local_db[i].number);
		printf("\n");
	
		// save database
		lock_file(name_of_lockfile);	
		save_data(name_of_database_file, size_of_record, lenght_of_local_db, local_db);
		save_log(name_of_logfile, index_to_change);
		unlock_file(name_of_lockfile);
	}
  
  	free(local_db);
  	printf("Program completed and closed correctly\n");
	return 0;
}
