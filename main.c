/*
	Test program to check several solutions for Linux, Makefile, GCC, access to database and multithreading...
	
	by Tomasz Golaszewski
	2022.08.07
*/

// ! Program zakłada, że plik .so znajduje się w katalogu/jednym z katalogów wskazanych w zmiennej LD_LIBRARY_PATH
// ! The program requires so that the .so file is located in directory/one of directories pointed by LD_LIBRARY_PATH
// export LD_LIBRARY_PATH=$(pwd)

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for time
#include <unistd.h> // for pid
#include <sys/stat.h> // for files' stats
#include <dlfcn.h> // for dynamic librarby linking
#include "data_struct.h"



void *open_dll(char *name_of_dll)
{
	// for pointing to the library
	void* handle = NULL;

	// for saving the error messages
	const char* error_message = NULL;

	// on error dlopen returns NULL
	handle = dlopen(name_of_dll, RTLD_LAZY);

	// check for error, if it is NULL
	if( !handle )
	{
		fprintf( stderr, "dlopen() %s\n", dlerror() );
		exit( 1 );
	}
	return handle;
}

const uint64_t NO_OF_RECORDS = 16;

// main function
int main(void)
{		
	struct local_db my_local_db;
	my_local_db.m_size_of_record = sizeof(struct record);
	my_local_db.m_no_of_records = NO_OF_RECORDS;
	my_local_db.m_records = malloc(my_local_db.m_size_of_record * my_local_db.m_no_of_records);
	
	
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
	
	// load shered objects functions (dynamic linking)
	void *handle = open_dll("module.so");
	void *(*lock_file)(char *);
	lock_file = (void *(*)(char *)) dlsym(handle, "lock_file");
	
	void (*load_data)(char *, struct local_db *);
	load_data = (void (*)(char *, struct local_db *)) dlsym(handle, "load_data");
	
	void (*load_one_record)(char *, int, struct local_db *);
	load_one_record= (void (*)(char *, int, struct local_db *)) dlsym(handle, "load_one_record");
	
	void (*save_data)(char *, struct local_db *);
	save_data = (void (*)(char *, struct local_db *)) dlsym(handle, "save_data");
	
	void (*update_data)(char *, char *, struct local_db *, struct timespec* );
	update_data = (void (*)(char *, char *, struct local_db *, struct timespec*)) dlsym(handle, "update_data");
	
	void (*change_data)(int, int, struct local_db * );
	change_data = (void (*)(int, int, struct local_db *)) dlsym(handle, "change_data");
	
	void (*save_log)(char*, int);
	save_log= (void (*)(char*, int)) dlsym(handle, "save_log");
	
	void (*unlock_file)(char*);
	unlock_file= (void (*)(char*)) dlsym(handle, "unlock_file");
	
	// load database first time
	lock_file(name_of_lockfile);	
	load_data(name_of_database_file, &my_local_db);
	clock_gettime(CLOCK_REALTIME, &time_of_last_update); // update time of last data update
	unlock_file(name_of_lockfile);
	
	// print database
	for(int i=0; i<my_local_db.m_no_of_records; i++) printf("%d ", my_local_db.m_records[i].m_data);
	printf("\n");
	
	char may_I;
	printf("Do you want to change some records? (Y/n): ");
	scanf("%c", &may_I);
		
    	stat(name_of_database_file, &file_stats); // get stats of the database file	
   	if(file_stats.st_mtime > time_of_last_update.tv_sec) // if the database file was modified, update local_db
   	{
   		// in the future we can try aio_notify()
   		
   		printf("In the meantime, the database has been changed - updating...\n");
		lock_file(name_of_lockfile);
		update_data(name_of_database_file, name_of_logfile, &my_local_db, &time_of_last_update);
		//clock_gettime(CLOCK_REALTIME, &time_of_last_update); // update time of last data update
		unlock_file(name_of_lockfile);
		
		// print database
		for(int i=0; i<my_local_db.m_no_of_records; i++) printf("%d ", my_local_db.m_records[i].m_data);
		printf("\n");
	}
	
	int index_to_change, new_value;
	if(may_I == (int)'Y' || may_I == (int)'y') // 'Y' == 89, 'y' == 121
	{	
		printf("Index to change: ");
		scanf("%d", &index_to_change);
		printf("New value: ");
		scanf("%d", &new_value);
		change_data(index_to_change, new_value, &my_local_db);
		
		// print database
		for(int i=0; i<my_local_db.m_no_of_records; i++) printf("%d ", my_local_db.m_records[i].m_data);
		printf("\n");
	
		// save database
		lock_file(name_of_lockfile);	
		save_log(name_of_logfile, index_to_change);
		save_data(name_of_database_file, &my_local_db);		
		unlock_file(name_of_lockfile);
	}

  	free(my_local_db.m_records );
  	dlclose( handle );
  	printf("Program completed and closed correctly\n");
	return 0;
}
