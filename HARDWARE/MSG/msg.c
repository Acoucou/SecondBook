#include "msg.h"
#include <stdio.h>
#include <string.h>

int fingString(char *pcBuf, char *left, char *right, char *pcRes){
	char *pcBegin = NULL;
	char *pcEnd = NULL;

	pcBegin = strstr(pcBuf, left);

	pcEnd = strstr(pcBegin + strlen(left) + 4, right);

	if(pcBegin == NULL || pcEnd == NULL || pcBegin >= pcEnd){
		return 0;
	}else{
		pcBegin += strlen(left);
		memcpy(pcRes, pcBegin + 4, pcEnd - pcBegin - 4);
		return 1;
	}
}




