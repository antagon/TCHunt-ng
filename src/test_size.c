#include <stdio.h>
#include <errno.h>

#include "test_size.h"

int
file_test_minsize (const char *file, unsigned long min_len)
{
	FILE *f;
	long size;
	int rval;

	rval = 1;

	f = fopen (file, "rb");

	if ( f == NULL )
		return -1;

	if ( fseek (f, 0, SEEK_END) == -1 ){
		rval = -1;
		goto cleanup;
	}

	size = ftell (f);

	if ( size == -1 ){
		rval = -1;
		goto cleanup;
	}

	if ( size < min_len )
		rval = 0;

cleanup:
	fclose (f);

	return rval;
}

void
testsize_init (struct testsize *testsize)
{

}

int
testsize_setfile (struct testsize *res, const char *file)
{

}

extern int testsize_check_minsize (struct testsize *res);

extern int testsize_check_modsize (struct testsize *res);

extern void testsize_free (struct testsize *res);
