#include <stdio.h>
#include <stdlib.h>
#include "module.h"
  
// main function
int main(void)
{	
	int lenght_of_test_table = 10;
	int *test_table = malloc(lenght_of_test_table*sizeof(int));;

	lock_file();	
	load_data(lenght_of_test_table, test_table);
	
	for(int i=0; i<lenght_of_test_table; i++) printf("%d ", test_table[i]);
	printf("\n");
	
	int index_to_change, new_value;
	printf("index to change: ");
	scanf("%d", &index_to_change);
	printf("new value: ");
	scanf("%d", &new_value);
	change_data(index_to_change, new_value, test_table);
	
	for(int i=0; i<lenght_of_test_table; i++) printf("%d ", test_table[i]);
	printf("\n");
	
	save_data(lenght_of_test_table, test_table);
	unlock_file();
	//printf("Done\n");
  
	return 0;
}
