#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "GrepHeader.h"

void GetResult(char findWord[128], char fileText[][256], int fileTextSize, int c, int A, int B, int showNlines)
{
	int appeared = 0;
	int lineAppearedId[fileTextSize];

	for(int i = 0;i < fileTextSize;i++)
	{
		if(strstr(fileText[i], findWord) != NULL)
		{
			lineAppearedId[appeared] = i;
			appeared++;
		}
	}

	if(showNlines != 0)
	{	
		int start = -1;
		int end = -1;		
		if(A == 1)//print showNlines up
		{
			for(int i = 0;i < appeared;i++)
			{	
				int lastId = (i == 0) ? 0 : lineAppearedId[i - 1];
				int appearedId = lineAppearedId[i];
				end = appearedId;
				start = (appearedId - showNlines < 0) ? 0 : appearedId - showNlines;
				for(int j = start;j <= end;j++)
				{
					if(j <= lastId)
					{
						continue;
					}
					printf("%s", fileText[j]);
				}
			}
		}
		else if(B == 1)//print showNlines down
		{
			for(int i = 0;i < appeared;i++)
			{	
				int appearedId = lineAppearedId[i];
				start = appearedId;
				if(lineAppearedId[i + 1] <= end)
				{
					continue;
				}
				int shift = (start <= end) ? end - start + 1 : 0;//to not print the same line twice
				end = (appearedId + showNlines > fileTextSize - 1) ? fileTextSize - 1 : appearedId + showNlines;
				for(int j = start + shift;j <= end;j++)
				{
					printf("%s", fileText[j]);
				}
			}
		}

	}
	else if(showNlines == 0)
	{
		int idx;
		for(int i = 0;i < appeared;i++)
		{
			idx = lineAppearedId[i];
			printf("%s", fileText[idx]);
		}
	}

	if(c != 0)
	{
		printf("%s appeared %d times.", findWord, appeared);
	}

}
