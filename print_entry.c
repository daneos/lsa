/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "include/print_entry.h"

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
		printf("%8.8s:", pwd->pw_name);
	else
		printf("%8.8d:", f->st_uid);
	// group
	if((grp = getgrgid(f->st_gid)) != NULL)
		printf("%-8.8s", grp->gr_name);
	else
		printf("%-8.8d", f->st_gid);
	// size in reasonable format
	printf(" %4s", fsize(f->st_size));
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
	// print date, name and destination (if applicable)
	printf("  %s  %s%s\n", datestring, dname, link);
}

//-----------------------------------------------------------------------------
void print_short(const struct stat *f, const char *fname, const char *dname)
{
	struct passwd *pwd;
	char link[PATH_MAX+5];

	// type and permissions
	printf("%1c%9.9s ", ftype(f->st_mode), fperm(f->st_mode));
	// owner
	if((pwd = getpwuid(f->st_uid)) != NULL)
		printf("%8.8s", pwd->pw_name);
	else
		printf("%8.8d", f->st_uid);
	// size in reasonable format
	printf(" %4s", fsize(f->st_size));
	// if link get its destination
	if((f->st_mode & S_IFMT) == S_IFLNK)
	{
		char buf[PATH_MAX+1];
		int len;
		if((len = readlink(fname, buf, sizeof buf)) == -1)
		{
			perror("print_short()/readlink()");
			return;
		}
		snprintf(link, sizeof link, " -> %s", buf);
		link[len+4] = '\0';
	}
	else link[0] = '\0';
	// print name and destination (if applicable)
	printf("  %s%s\n", dname, link);
}