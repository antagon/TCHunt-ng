#ifndef _STROLLDIR_H
#define _STROLLDIR_H

struct stroller_flist_path
{
	char *path;
	struct stroller_flist_path *next;
};

struct stroller_flist
{
	struct stroller_flist_path *head;
	struct stroller_flist_path *tail;
};

typedef struct
{
	struct stroller_flist dir_file;
	struct stroller_flist dir_subdir;
	int eol;
} stroller_t;

extern int strolldir_open (stroller_t *res, const char *dir);

extern int strolldir_scan (stroller_t *res);

extern const struct stroller_flist* strolldir_getfiles (stroller_t *res);

extern const char* strolldir_getdir (stroller_t *res);

extern const char* strolldir_nextdir (stroller_t *res);

extern int strolldir_eol (stroller_t *res);

extern void strolldir_close (stroller_t *res);

#endif

