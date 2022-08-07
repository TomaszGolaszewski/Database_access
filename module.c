#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "module.h"


FILE *open_file_and_check(char *name_of_file, char *type_of_operation)
{
	FILE *fp; // pointer to file
	
	fp = fopen (name_of_file, type_of_operation); // open file
	if( fp == NULL ) // check if file exists
	{
		perror("File cannot be opened!"); // if doesn't - print error message
		exit(-10);
	}
	
	return fp;
}

void load_data(char *name_of_database, int size_of_record, int lenght_of_table, struct Record *local_db)
{
	FILE *fp = open_file_and_check(name_of_database, "rb"); // open file
	
	fread(local_db, size_of_record, lenght_of_table, fp); // read the database
	
	fclose (fp); // close the file
	printf("Data loaded\n");
}

void load_one_record(char *name_of_database, int size_of_record, int number_of_record, struct Record *local_db)
{
	FILE *fp = open_file_and_check(name_of_database, "rb"); // open file
	
 	if(!fseek(fp, size_of_record * number_of_record, SEEK_SET)) // set cursor and check if cursor is on the good position
 	{
		// char * fgets( char * str, int num, FILE * stream );
		// fgets work only with text files, so we will need additional type conversion
		fread(local_db + number_of_record, size_of_record, 1, fp);
	}
	else
	{
		perror("fseek error! - cursor in the wrong position!"); // if doesn't - print error message
	}

	fclose (fp); // close the file
	printf("Record no. %d loaded\n", number_of_record);
}

void update_data(char *name_of_database_file, char *name_of_logfile, int size_of_record, int lenght_of_table, struct Record *local_db, struct timespec *time_of_last_update)
{
    	long s  = time_of_last_update->tv_sec; // time in seconds
   	long ns = time_of_last_update->tv_nsec; // time in nanosecond
   	
   	int pid = getpid();
   	
   	long log_s, log_ns;
   	int log_pid, index_of_changes;

	FILE *fp; // pointer to file
	fp = fopen (name_of_logfile, "r"); // open file
	if( fp == NULL ) // check if file exists
	{
		// if doesn't...
		printf("Logfile doesn't exist - database is updated!"); 
	}
	else
	{
		// if it exists scan logfile until End Of logFile
		while(fscanf(fp, "%ld\t%ld\t%d\t%d\n", &log_s, &log_ns, &log_pid, &index_of_changes) != EOF) 
		{
			//printf("%ld\t%ld\t%d\t%d\n", log_s, log_ns, log_pid, index);
			if((log_s > s || (log_s == s && log_ns > ns)) && log_pid != pid) 
			{	
				// if new record was found (younger entry made by another prosess)		
				load_one_record(name_of_database_file, size_of_record, index_of_changes, local_db); // load modified record
				// update time of last data update
				time_of_last_update->tv_sec = log_s;
				time_of_last_update->tv_nsec = log_ns;
			}
		} 
		printf("Database is updated!\n");
	}
}

void save_data(char *name_of_database, int size_of_record, int lenght_of_table, struct Record *local_db)
{
	FILE *fp = open_file_and_check(name_of_database, "wb"); // open file
	
	fwrite(local_db, size_of_record, lenght_of_table, fp); // write the database into the file
	fclose (fp); // close the file
	printf("Data saved\n");
}

void change_data(int index_to_change, int new_value, struct Record *local_db)
{
	local_db[index_to_change].number = new_value;
}

void save_log(char *name_of_logfile, int index_of_changes)
{
	struct timespec spec;
    	clock_gettime(CLOCK_REALTIME, &spec);
    	long s  = spec.tv_sec; // time in seconds
   	long ns = spec.tv_nsec; // time in nanosecond
   	
   	int pid = getpid();
   	
   	FILE *fp; // pointer to file
	fp = fopen (name_of_logfile, "ab"); // open file

	// when [s+ns]	who [pid] what [index]
   	fprintf(fp, "%ld\t%ld\t%d\t%d\n", s, ns, pid, index_of_changes);
  
	fclose (fp); // close the file
	printf("Log saved\n");
}

void lock_file(char *name_of_lockfile)
{
	// type of lock: spinlock with mutex in the another binary file
	// in the future we can try fcntl() or lockf() - but online reviews about these functions are bad.
	
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
			fscanf(fp,"%d", &flag); // start the race !!!
			rewind(fp); // move cursor at the begining of the lockfile
		}
		while(flag != -1);
	}
	fprintf (fp, "%d", pid); 
	fclose (fp); // close the file 
	// end of the race ! - the smallest time gap 
	// without queue to file, it is possible to lock files by two processes
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


