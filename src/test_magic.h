#ifndef _TEST_MAGIC_H
#define _TEST_MAGIC_H

struct testmagic
{
	magic_t magic_res;
};

extern struct testmagic* testmagic_init (void);

extern int testmagic_is_datafile (struct testmagic *testmagic, const char *file);

extern void testmagic_free (struct testmagic *testmagic);

extern const char *testmagic_error (struct testmagic *testmagic);

#endif

