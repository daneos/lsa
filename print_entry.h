/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__PRINT_ENTRY_H__)
#define __PRINT_ENTRY_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <unistd.h>

//-----------------------------------------------------------------------------
void print_long(const struct stat *f, const char *fname, const char *d_name);

//-----------------------------------------------------------------------------
void print_long(const struct stat *f, const char *fname, const char *dname)
{
	struct passwd *pwd;
	struct group *grp;
	struct tm *tm;
	char datestring[256];
	char link[PATH_MAX+5];

	// type and permissions
	printf("%1c%9.9s ", ftype(f->st_mode), fperm(f->st_mode));
	// owner
	if((pwd = getpwuid(f->st_uid)) != NULL)
		printf("%10.10s:", pwd->pw_name);
	else
		printf("%10.10d:", f->st_uid);
	// group
	if((grp = getgrgid(f->st_gid)) != NULL)
		printf("%-10.10s", grp->gr_name);
	else
		printf("%-10.10d", f->st_gid);
	// size in reasonable format
	printf(" %8s", fsize(f->st_size));
	// get datestring
	tm = localtime(&f->st_mtime);
	strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
	// if link get its destination
	if((f->st_mode & S_IFMT) == S_IFLNK)
	{
		char buf[PATH_MAX+1];
		int len;
		if((len = readlink(fname, buf, sizeof buf)) == -1)
		{
			perror("print_long()/readlink()");
			return;
		}
		snprintf(link, sizeof link, " -> %s", buf);
		link[len+4] = '\0';
	}
	else link[0] = '\0';
	// print date, name, and destination (if applicable)
	printf("  %s\t%s%s\n", datestring, dname, link);
}

//-----------------------------------------------------------------------------
#endif /* __PRINT_ENTRY_H__ */ 