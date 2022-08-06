#include <stdio.h>
#include <stdlib.h>

void load_data(char *name_of_database, int lenght_of_table, int *table_with_data);

void save_data(char *name_of_database, int lenght_of_table, int *table_with_data);

void change_data(int index_to_change, int new_value, int *table_with_data);

void lock_file(char *name_of_lockfile);

void unlock_file(char *name_of_lockfile);

