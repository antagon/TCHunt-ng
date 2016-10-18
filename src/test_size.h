#ifndef _TEST_SIZE_H
#define _TEST_SIZE_H

struct testsize
{
	unsigned char *file;
	unsigned long int file_len;
};

extern void testsize_init (struct testsize *testsize);

extern int testsize_setfile (struct testsize *res, const char *file);

extern int testsize_check_minsize (struct testsize *res);

extern int testsize_check_modsize (struct testsize *res);

extern void testsize_free (struct testsize *res);

#endif

