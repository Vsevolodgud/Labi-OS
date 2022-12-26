#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "GrepHeader.h"

int main(int argc, char** argv)
{
	FILE* file;

	char fileName[128] = {' '};
	char line[256] = {' '};
	char fileText[512][256] = {{' '}};
	char findWord[128] = {' '};

	int lineIndex = 0;
	int showNlines = 0;
	int showA = 0;
	int showB = 0;
	int optionVal = 0;

	int c = 0;
	int A = 0;
	int B = 0;
	
	if(argc >= 2)
	{	
		memcpy(findWord, argv[1], strlen(argv[1]) + 1);
		while((optionVal = getopt(argc, argv, "cwA:B:f:")) != -1)
		{
			switch(optionVal)
			{
				case 'c':
					c = 1;
					break;

				case 'A':
					A = 1;
					if(strcmp(optarg, "0") != 0)
					{
						showA = atoi(optarg);
					}
					break;

				case 'B':
					B = 1;
					if(strcmp(optarg, "0") != 0)
					{
						showB = atoi(optarg);
					}
					break;

				case 'f':
					memcpy(fileName, optarg, strlen(optarg) + 1);
					break;

			}
		}
		B = (A == 1) ? 0 : B;
		showNlines = (A == 1) ? showA : showB;

		if((file = fopen(fileName, "r")) != NULL)
		{
			while((fgets(line, 256, file) != NULL) && (lineIndex < 512))
			{
				memcpy(fileText[lineIndex], line, 256);
				lineIndex++;
			}
			fclose(file);
		}
		
		else if(stdin != NULL)
		{
			while((fgets(line, 256, stdin) != NULL) && (lineIndex < 512))
			{
				memcpy(fileText[lineIndex], line, 256);
				lineIndex++;
			}
		}
		else
		{
			printf("File or stdin reading error");
			return 1;
		}
		GetResult(findWord, fileText, lineIndex, c, A, B, showNlines);
		
	}
	else
	{
		printf("To few arguments");
		return 1;
	}


	return 0;
}
