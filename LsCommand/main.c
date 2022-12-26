#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>


#define NONE "\e[0m"
#define BLUE "\e[0;34m"
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"


int main(int argc, char* argv[])
{
  DIR* thedirectory;

  struct dirent* thefile;
  struct stat thestat;
  struct passwd* tf; 
  struct group* gf;

  char fileName[128];
  char buf[512];
  char date[25];
  char fileType = ' ';

  int optionVal = 0;
  int a = 0;
  int l = 0;

  if(argc > 2)
  {
    while((optionVal = getopt(argc, argv, "la")) != -1)
    {
      switch(optionVal)
      {
        case 'l':
          l = 1;
          break;
        case 'a':
          a = 1;
          break;
      }
    }
  }
  else if(argc == 2)
  {
    memcpy(fileName, argv[1], strlen(argv[1]) + 1);
  }
  else
  {
    printf("To few arguments");
    return 1;
  }

  thedirectory = opendir(fileName);
  if(thedirectory == NULL)
  {
    memcpy(fileName, argv[2], strlen(argv[2]) + 1);
    thedirectory = opendir(fileName);
  }

  if(thedirectory != NULL)
  {
    while((thefile = readdir(thedirectory)) != NULL) 
    {   
        sprintf(buf, "%s/%s", fileName, thefile->d_name);
  
        stat(buf, &thestat);
    
        switch (thestat.st_mode & S_IFMT)
        {
            case S_IFDIR:
                fileType = 'd';
                break;
            case S_IFLNK:
                fileType = 'l';
                break;
            default: 
                fileType = '-';
        }
		
        if((a == 1) && (l == 0))
        {
			if(fileType == 'd')
			{
				printf(BLUE "%s\n" NONE, thefile->d_name);
			}
			else if(fileType == 'l')
			{
				printf(RED "%s\n" NONE, thefile->d_name);
			}
			else
			{
				printf(GREEN "%s\n" NONE, thefile->d_name);
			}
			continue;
        }
        else if((l == 0) && (a == 0))
        {
          if(buf[2] == '.')
          {
            continue;
          }
          else
          {
            if(fileType == 'd')
			{
				printf(BLUE "%s\n" NONE, thefile->d_name);
			}
			else if(fileType == 'l')
			{
				printf(RED "%s\n" NONE, thefile->d_name);
			}
			else
			{
				printf(GREEN "%s\n" NONE, thefile->d_name);
			}
            continue;
          }
        }
        else if((a == 0) && (l == 1) && (buf[2] == '.'))
        {
          continue;
        }

		printf("%c", fileType);

        printf( (thestat.st_mode & S_IRUSR) ? "r" : " -");
        printf( (thestat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (thestat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (thestat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (thestat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (thestat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (thestat.st_mode & S_IROTH) ? "r" : "-");
        printf( (thestat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (thestat.st_mode & S_IXOTH) ? "x" : "-");


        printf(" %8ld ", thestat.st_nlink);

        tf = getpwuid(thestat.st_uid);
        printf("%12s ", tf->pw_name);

        gf = getgrgid(thestat.st_gid);
        printf("%12s ", gf->gr_name);

        memcpy(date, ctime(&thestat.st_mtime), 25);
        date[24] = '\0';

        printf("%8zu",thestat.st_size);
        printf(" %s ", date);
        if(fileType == 'd')
        {
            printf(BLUE "%s\n" NONE, thefile->d_name);
        }
        else if(fileType == 'l')
        {
            printf(RED "%s\n" NONE, thefile->d_name);
        }
        else
        {
            printf(GREEN "%s\n" NONE, thefile->d_name);
        }
    }
  }
  else{
    printf("opendir() error");
    return 1;
  }
  closedir(thedirectory);
  return 0;
}
