#include <stdio.h>
#include <stdlib.h>
#include "module.h"
  
// main function
int main(void)
{	
	int lenght_of_local_db= 10;
	int *local_db = malloc(lenght_of_local_db*sizeof(int));
	char name_of_database_file[] = "database.txt";
	
	char name_of_lockfile[] = "lockfile.bin";

	lock_file(name_of_lockfile);	
	load_data(name_of_database_file, lenght_of_local_db, local_db);
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
	save_data(name_of_database_file, lenght_of_local_db, local_db);
	unlock_file(name_of_lockfile);
  
	return 0;
}
