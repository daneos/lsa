#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>

#include "fmode.h"

int main(int argc, char **argv)
{
	DIR *d;
	struct dirent *dp;
	struct stat finfo;
	struct passwd *pwd;
	struct group *grp;
	struct tm *tm;
	char datestring[256];
	char *dirname;
	char fname[1024];

	// if no argument given, default to .
	if(argc > 1)
		dirname = argv[1];
	else
		dirname = ".";
	
	d = opendir(dirname);
	if(d)
	{
		while ((dp = readdir(d)) != NULL)
		{
			// prepend file name with directory (needed for dirs other than .)			
			snprintf(fname, sizeof fname, "%s/%s", dirname, dp->d_name);
			
			// get file info
			if(lstat(fname, &finfo) == -1)
			{
				perror("stat()");
				continue;
			}

			// type and permissions
			//char perm[10];
			//fperm(finfo.st_mode, perm);
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
			char buf[9];
			fsize(finfo.st_size, buf);
			printf(" %8s", buf);

			tm = localtime(&finfo.st_mtime);

			// get datestring
			strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

			// if link get its destination
			if((finfo.st_mode & S_IFMT) == S_IFLNK)
			{
				char buf[1024];
				int len = readlink(fname, buf, sizeof buf);
				snprintf(fname, sizeof fname, " -> %s", buf);
				fname[len+4] = '\0';
			}
			else
				fname[0] = '\0';

			// print date, name, and destination (if applicable)
			printf("  %s\t%s%s\n", datestring, dp->d_name, fname);
		}
	}
	return 0;
}