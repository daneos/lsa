/*
 * lsa - list only those files you have permission to
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL 2.0 license
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <linux/limits.h>
#include <errno.h>

#include "fmode.h"

int main(int argc, char **argv)
{
	struct stat finfo;
	struct passwd *pwd;
	struct group *grp;
	struct tm *tm;
	char datestring[256];
	char *dirname;
	char fname[PATH_MAX];
	int n,i;
	struct dirent **list;

	// if no argument given, default to .
	if(argc > 1)
		dirname = argv[1];
	else
		dirname = ".";
	
	if((n = scandir(dirname, &list, NULL, alphasort)) < 0)
	{
		perror("scandir()");
		return -1;
	}
	for(i=0; i < n; i++)
	{
		// prepend file name with directory (needed for dirs other than .)			
		snprintf(fname, sizeof fname, "%s/%s", dirname, list[i]->d_name);
		
		// get file info
		if(lstat(fname, &finfo) == -1)
		{
			perror("stat()");
			continue;
		}

		// type and permissions
		printf("%1c%9.9s ", ftype(finfo.st_mode), fperm(finfo.st_mode));

		// owner
		if((pwd = getpwuid(finfo.st_uid)) != NULL)
			printf("%10.10s:", pwd->pw_name);
		else
			printf("%10.10d:", finfo.st_uid);

		// group
		if((grp = getgrgid(finfo.st_gid)) != NULL)
			printf("%-10.10s", grp->gr_name);
		else
			printf("%-10.10d", finfo.st_gid);

		// size in reasonable format
		printf(" %8s", fsize(finfo.st_size));

		// get datestring
		tm = localtime(&finfo.st_mtime);
		strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

		// if link get its destination
		if((finfo.st_mode & S_IFMT) == S_IFLNK)
		{
			char buf[PATH_MAX];
			int len = readlink(fname, buf, sizeof buf);
			snprintf(fname, sizeof fname, " -> %s", buf);
			fname[len+4] = '\0';
		}
		else
			fname[0] = '\0';

		// print date, name, and destination (if applicable)
		printf("  %s\t%s%s\n", datestring, list[i]->d_name, fname);
	}
	return 0;
}