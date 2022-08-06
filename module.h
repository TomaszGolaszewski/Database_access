#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void load_data(char *name_of_database, int size_of_record, int lenght_of_table, int *table_with_data);

void save_data(char *name_of_database, int size_of_record, int lenght_of_table, int *table_with_data);

void change_data(int index_to_change, int new_value, int *table_with_data);

void save_log(char *name_of_logfile, int index_of_changes);

void lock_file(char *name_of_lockfile);

void unlock_file(char *name_of_lockfile);

