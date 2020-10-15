#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"
//////////////////////////////////////////////////////////////////
static int activeTop = CtrlInfoSize;	//���¼��ջ��ָ��
static int globalTop = 1;	//ȫ�����������α�
static SymbolEntry globalEntries[MaxGSize+1];
static SymbolEntry localEntries[MaxLSize+1];
SymbolTable globalTable = {globalEntries,0};	//ȫ�ַ��ű�
SymbolTable localTable = {localEntries,0};	//�ֲ����ű�

//////////////////////////////////////////////////////////////////
void setActiveTop(int top);
int newtemp();
void freeTemp();
int getGlobalMemSize();
int getActiveTop();			//����
void exitIfFull(SymbolTable * table,int idType);
static bool isGlobalTable(SymbolTable * table);
SearchingResult mixingSearch(int idType,char * lexeme);
//////////////////////////////////////////////////////////////////
int getGlobalMemSize(){
	return (globalTop-1);
}
//����activeTopָ��
void setActiveTop(int top){
	activeTop = top;
}
//�ж��Ƿ�Ϊȫ�ַ��ű�
static bool isGlobalTable(SymbolTable * table){
	if( table == &globalTable)
		return true;
	else 
		return false;
}
// ���һ����ʱ����
int newtemp(){
	int temp = activeTop;
	activeTop++;
	return temp;	
	
}
int getActiveTop(){
	return activeTop;
}
//�ͷ�һ����ʱ����
void freeTemp(){
	activeTop--;	
}
//�����ű����������˳�
static void exitIfFull(SymbolTable * table){
	if(isGlobalTable(table)){
		if(table->index >= MaxGSize){
			printf("global table is full.\n");
			exit(1);
		}
	}
	else if(table->index >= MaxLSize){
		printf("local table is full.\n");
		exit(1);
	}	
}
//�ڷ��ű��β׷��һ��,���������жϱ����Ƿ��Ѿ�������Ϊid�ı�־��
int enter(SymbolTable * table,int idType,char * lexeme ){	
	exitIfFull(table);
	table->index++;
	strcpy(table->entries[table->index].lexeme,lexeme);	
	table->entries[table->index].type = idType;
	//�ֲ�������ռ�ռ���δ������ֲ�����ʱ�����ܱ��䵱��ʱ
	//����ʹ�ã���һ������֮�󣬾Ͳ��ٱ���ʱ��������	
	if(!isGlobalTable(table))
		table->entries[table->index].address = newtemp();		
	else{
		//�ø�����ʾȫ�ֱ����ĵ�ַ,�������ĵ�ַҪ�����꺯���������
		if(idType == VAR){
			table->entries[table->index].address = -globalTop;
			globalTop++;
		}
	}
	return table->index;
}
//�ж�lexeme�Ƿ��ڷ��ű��У�������ڣ��򷵻ر���λ��
//��������ڣ��򷵻�0
int lookup(SymbolTable * table,int idType,char * lexeme){
	//��0����Ϊ�ڱ�
	strcpy(table->entries[0].lexeme,lexeme);
	// added by zcw, 2010.2.24
	table->entries[0].type = idType;
	//�����Ƿ���ƥ���
	int i = table->index;
	while(strcmp(table->entries[i].lexeme,lexeme)!=0
		|| table->entries[i].type != idType){
			i--;
	}
	return i;
	// commented by zcw  , 2010.2.24
	//for(int i = table->index; i >= 0; i--){
	//	if(strcmp(table->entries[i].lexeme,lexeme)==0
	//		&& table->entries[i].type == idType){
	//		return i;
	//	}
	//}
	//return 0;
}
//���ʽ���ң����Ҿֲ����ű�����ȫ�ַ��ű�
SearchingResult mixingSearch(int idType,char * lexeme){
	SearchingResult result;
	result.index = lookup(&localTable,idType,lexeme);
	result.tableType = LOCAL;
	if(result.index != 0){
		result.address = localTable.entries[result.index].address;		
	}
	else{
		result.index = lookup(&globalTable,idType,lexeme);
		result.tableType = GLOBAL;
		if(result.index != 0)
			result.address = globalTable.entries[result.index].address;
	}	
	return  result;
}

