#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"
///////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//����һ������
static void processOneDeclare(){
	if(token == INT){
		getToken();
		if(token == ID){
			int index = lookup(&localTable,VAR,id);
			if(index == 0)
				index = enter(&localTable,VAR,id);
			else
				syntaxError("reclared paramname.");			
			getToken();
		}
		else{
			syntaxError("paramname missed.");
		}	
	}
	
}
/**
* ������������,���ز����ĸ���,���ܸ�����û�в���
*/
int paramDeclare(){	
	int count = 0;
	if(token == INT){
		count++;
		processOneDeclare();
		while(token == COMMA){
			getToken();
			processOneDeclare();
			count ++;
		}
	}
	return count;

}

