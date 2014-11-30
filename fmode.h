/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__FMODE_H__)
#define __FMODE_H__

#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define GB 1073741824
#define MB 1048576
#define KB 1024
//-----------------------------------------------------------------------------
static char *fperm(mode_t mode);
char ftype(mode_t mode);
static char *fsize(off_t size);

//-----------------------------------------------------------------------------
static char *fperm(mode_t mode)
{
	// code by Jonathan Leffler
	static char *rwx[] = {"---", "--x", "-w-", "-wx",
	"r--", "r-x", "rw-", "rwx"};
	static char bits[10];

	strcpy(&bits[0], rwx[(mode >> 6)& 7]);
	strcpy(&bits[3], rwx[(mode >> 3)& 7]);
	strcpy(&bits[6], rwx[(mode & 7)]);
	if(mode & S_ISUID)
		bits[2] = (mode & 0100) ? 's' : 'S';
	if(mode & S_ISGID)
		bits[5] = (mode & 0010) ? 's' : 'l';
	if(mode & S_ISVTX)
		bits[8] = (mode & 0100) ? 't' : 'T';
	bits[9] = '\0';
	return bits;
}

//-----------------------------------------------------------------------------
char ftype(mode_t mode)
{
	switch(mode & S_IFMT)
	{
		case S_IFDIR:
			return 'd';
		case S_IFCHR:
			return 'c';
		case S_IFBLK:
			return 'b';
		case S_IFLNK:
			return 'l';
		case S_IFIFO:
			return 'f';
		case S_IFSOCK:
			return 's';
		default:
			return '-';
	}
}

//-----------------------------------------------------------------------------
static char *fsize(off_t size)
{
	static char buf[10];
	if(size > GB)
		snprintf(buf, sizeof buf, "%.0fG", round(size/GB));
	else if(size > MB)
		snprintf(buf, sizeof buf, "%.0fM", round(size/MB));
	else if(size > KB)
		snprintf(buf, sizeof buf, "%.0fK", round(size/KB));
	else
		snprintf(buf, sizeof buf, "%d", (int)size);
	return buf;
}

//-----------------------------------------------------------------------------
#endif /* __FMODE_H__ */