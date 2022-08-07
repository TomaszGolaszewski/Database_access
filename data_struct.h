#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <stdint.h>

#pragma pack(push, 1)

// structure for storing record
struct record
{
	int m_data;
};

struct local_db
{ 
	uint64_t m_size_of_record;
	uint64_t m_no_of_records;
	struct record* m_records;
};

#pragma pack(pop)

// ensure no padding added
_Static_assert (sizeof(struct local_db) == 24);


#endif // DATA_STRUCT_H

