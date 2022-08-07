#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "data_struct.h"


FILE *open_file_and_check(char *name_of_file, char *type_of_operation);

void load_data(char *name_of_database, int size_of_record, int lenght_of_table, struct Record *local_db);

void load_one_record(char *name_of_database, int size_of_record, int number_of_record, struct Record *local_db);

void save_data(char *name_of_database, int size_of_record, int lenght_of_table, struct Record *local_db);

void update_data(char *name_of_database_file, char *name_of_logfile, int size_of_record, int lenght_of_table, struct Record *local_db, struct timespec *time_of_last_update);

void change_data(int index_to_change, int new_value, struct Record *local_db);

void save_log(char *name_of_logfile, int index_of_changes);

void lock_file(char *name_of_lockfile);

void unlock_file(char *name_of_lockfile);

