#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv){

	FILE* file;
	char fileName[128];
	char line[256];
	int lineCount = 1;
	int optionVal = 0;
	int repeat = 0;//bool
	int exit = 0;//bool

	int n = 0;
	int b = 0;
	int s = 0;
	int E = 0;

	while((optionVal = getopt(argc, argv, "nbsEf:")) != -1){
		switch(optionVal){
			case 'n':
				n = 1;
				break;
			case 'b':
				b = 1;
				break;
			case 's':
				s = 1;
				break;
			case 'E':
				E = 1;
				break;
			case 'f':
				memcpy(fileName, optarg, strlen(optarg) + 1);
				
		}
	}
	
	if((file = fopen(fileName, "r")) != NULL){
		while(fgets(line, 256, file) != NULL){
			if(s == 1){
				if(line[0] == '\n'){
					if(repeat == 1){
						continue;
					}
					repeat = 1;
				}
				else{
					repeat = 0;
				}
			}
			if(E == 1){
				line[strlen(line) - 1] = '$';
				if((b == 0) && (n == 0)){
					printf("%s\n", line);
				}				
			}
			if(b == 1){
				if((line[0] == '\n') || (line[0] == '$')){
					(E == 1) ? printf("%s\n", line) : printf("%s", line);
				}
				else{
					(E == 1) ? printf("%d%s\n", lineCount, line) : printf("%d%s", lineCount, line);
					lineCount++;
				}								
			}
			else if(n == 1){
				(E == 1) ? printf("%d%s\n", lineCount, line) : printf("%d%s", lineCount, line);
				lineCount++;
			}
			
			if((n == 0) && (b == 0) && (E == 0)){
				printf("%s", line);
			}
			
		}
		fclose(file);			
	}
		
	else{
		printf("cat: %s: No such file or directory", fileName);
	}
			
	
	return 0;
}
