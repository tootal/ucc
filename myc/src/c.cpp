#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"

#define ZCW_DEBUG

//////////////////////////////////////////////////////////////////////
int errorCount = 0;
int ds[DsSize];					//���ݶ�	0�ŵ�Ԫ����		
Instruction cs[CsSize];			//�����
int csIndex = 2;		//������±�  1��Ԥ����(���ջᣩ��ת��main��ָ��
						//0�ŵ�Ԫ���ڴ�ų�ʼ"��ַ�Ĵ���"��ָ��
//int dsIndex = 0;		//���ݶ��±�
FILE * infile;		//�����ļ�
FILE * outfile;		//����ļ�


char * instrs[] = {
	"Jtrue",		//Ϊ������ת	(InsJtrue, arg1, , dest)
	"Jfalse",	//Ϊ������ת	(InsJfalse,arg1, , dest)
	"Jmp",		//��������ת	(InsJmp,  ,   , dest)	
	"Mov",		//���ݸ���      (InsMov, arg1,	  ,dest)
	"Init",		//��ʼ��ĳ��Ԫ	(InsInit,arg1,num,	   )
	"Add",		//�ӷ�			(InsAdd, arg1,arg2,dest)
	"Sub",		//����			(InsSub, arg1,arg2,dest)
	"Mul",		//�˷�			(InsMul, arg1,arg2,dest)
	"Div",		//����			(InsDiv, arg1,arg2,dest)
	"Mod",		//ȡ��
	"Nop",		//�ղ���		(InsNop,	,	, 	)
	"Jlt",		//�ж��Ƿ�<		(InsLt,arg1,arg2,result)
	"Jle",		//�ж��Ƿ�<=	(InsLe,arg1,arg2,result)
	"Jgt",		//�ж��Ƿ�>		(InsGt,arg1,arg2,result)
	"Jge",		//�ж��Ƿ�>=	
	"Jeq",		//�ж��Ƿ�==
	"Jne",		//�ж��Ƿ�!=
	"Or",		//�߼�������
	"And",		//�߼�������
	"Not",		//�߼�������
	"In",		//����
	"Out",		//���
	"Uminus",	//���෴��
	"Call",		//���̵���		(InsCall,des, , ,);		
	"Ret",		//���̷���		(InsRet,expr, , );
	"SetBx",	//����bxָ�룬ָ����¼�׵�ַ(InsSetBx,addr, , )
	"AddBx",	//����bx��ֵ
};
//////////////////////////////////////////////////////////////////////////

void printCsInfo(){	
	for(int i = 0; i < csIndex; i++){
		/*
		printf("%s %d %d %d\n",
			instrs[cs[i].optr],cs[i].arg1,cs[i].arg2,cs[i].result);
			*/
		fprintf(outfile,"%d:\t %s %d %d %d\n",
			i,instrs[cs[i].optr],cs[i].arg1,cs[i].arg2,cs[i].result);
	}
}



/**
* ����һ����Ԫʽָ��
*/
void gen(int instrType, int arg1,int arg2,int result){
	if(csIndex >= CsSize){
		printf("Code Segment overflows.");
		exit(1);
	}
	//
	cs[csIndex].optr = instrType;
	cs[csIndex].arg1 = arg1;
	cs[csIndex].arg2 = arg2;
	cs[csIndex].result = result;
	csIndex++;
}
/**
* �﷨������
*/
void syntaxError(char * info){
	errorCount++;
	printf("line %d : %s\n",lineNumber,info);
	//naive error recovery strategy,but simple :)
	//added by zcw, 2010.2.25
	exit(1);
}

/**
* main����
*/
int main(int argc,char * args[]){
	
	if(argc >= 2){	
		if((infile = fopen(args[1],"r")) == NULL){
			printf("can't open file %s .\n",args[1]);
			exit(1);
		}
		else{
			char name[80];
			strcpy(name,args[1]);
			char * dot = strchr(name,'.');
			if(dot == NULL){
				strcat(name,".s");
			}
			else{
				strcpy(dot,".s");
			}
			if( (outfile = fopen(name,"w")) == NULL){
				fclose(infile);
				printf("can't not create file %s.\n",name);
				exit(1);
			}			
		}
	}
	else{
		printf("parameter specifying input file needed.\n");
		exit(1);
	}	
	getToken();
	programBody();	
	if(token != EOF){		
		syntaxError("EOF needed.");	
	}
	printCsInfo();
	if(errorCount == 0)
		interpret();
	else
		printf("there are %d errors in program.",errorCount);
	fclose(infile);
	fclose(outfile);
	printf("\n");	
	return 0;
}
