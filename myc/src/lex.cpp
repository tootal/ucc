#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"
//////////////////////////////////////////////////////////////////

int token;							//��ǰ�Ǻ�(������)
int value;							//��ǰ�Ǻŵ�ֵ
char id[MaxIdLen+1];					//��ǰ���Ŵ�
int lineNumber = 0;						//��ǰ�к�
char * types[] = {
	"OR",				//��
	"AND",				//��
	"RELOP",			//��ϵ�����
	"ADDOP",			//�Ӽ�	
	"MULOP",			//�˳�
	"NOT",				//��
	"LP",				//������
	"RP",				//������
	"ID",				//��־��
	"NUM",				//��
	"ASSIGN",			//��ֵ	
	"LB",				//�������
	"RB",				//�Ҵ�����
	"COMMA",			//����
	"SEMICOLON",		//�ֺ�
	"UNDEFINED",		//δ����
	"INT",				//int
	"IF",				//if
	"ELSE",				//else
	"WHILE",			//while
	"RETURN",			//return
	"PRINTF",			//printf
	"SCANF",			//scanf
};

//�ؼ���
static char * keywords[KeyWordsCount] = {
	"int",
	"if",
	"else",
	"while",
	"return",
	"printf",
	"scanf",
};
//�ؼ��ֶ�Ӧ���
static int keyType[KeyWordsCount] = {
	INT,
	IF,
	ELSE,
	WHILE,
	RETURN,
	PRINTF,
	SCANF,
};

static char lineBuf[1024];					//�л���
static int bufIndex = 0;					//�������±�
static int chCount = 0;						//���������ַ�����
static char ch = ' ';							//��ǰ�ַ�
		
///////////////////////////////////////////////////////////////////

static void lexError(char * info);
static int isDigit(char ch);
static int isLetter(char ch);
static void nextChar();
static int getKeyWord(char * str);
void getToken();


///////////////////////////////////////////////////////////////////////

/**
* �ж��ַ���str�Ƿ�Ϊ�ؼ���
* ����ǹؼ��֣��򷵻عؼ��ֵ����ͱ��
* ���򷵻�-1
* @param char * str  �ַ���
*/
int getKeyWord(char * str){	
	for(int i = 0; i < KeyWordsCount; i++){
		if(strcmp(str,keywords[i]) == 0){		
			return keyType[i];	
		}
	}
	return UNDEFINED;
}
//������
void lexError(char * info){
	errorCount++;
	printf("line %d: %s\n",lineNumber,info);
	exit(1);
}
/**
* �Ƿ�Ϊ����
*/
int isDigit(char ch){
	return ch-'0'>=0 && ch -'9'<=0;
}
/**
*
*/
int isLetter(char ch){
	return (ch-'a'>=0 && ch-'z'<=0) || (ch-'A'>=0 && ch-'Z'<=0);
}
//�����ն������е�ע��'//'
static void skipComment(){
	for(int i = 0; i<=chCount-2; i++){
		if(lineBuf[i] == '/' && lineBuf[i+1] == '/'){
			chCount = i+1;
			lineBuf[i] = ' ';
			return ;
		}
	}
}
/** 
*	����һ���ַ��浽ȫ�ֱ���ch��
*/
void nextChar(){
	char preChar = ' ';
	if(bufIndex == chCount){  //��whileѭ�����������
		if(feof(infile)){
			ch = EOF;		  //��Ϊ�ļ�������־			
			return;		
		}
		bufIndex =  0;
		chCount = 0;
		char temp = fgetc(infile);
		while(temp != EOF ){			
			if(temp != '\n'){
				lineBuf[chCount] = temp;
				chCount++;
				temp = fgetc(infile);				
			}
			else{
				lineNumber++;
				break;
			}
		}
		lineBuf[chCount++] = ' ';   //�ѻس����з���Ϊ�ָ����ո�
		skipComment();
	}
	ch = lineBuf[bufIndex];	
	bufIndex++;
}
/**
*	ȡ��һ���Ǻ�,���������Ϣ�浽token  value  id�� 
* 	
*/
void getToken(){
	
	//�����հ�
	while(ch == ' ' || ch == '\t' || ch=='\r') 
		nextChar();
	
	if( ch == '+'){
		token = ADDOP;
		value = ADD;
		nextChar();
	}
	else if(ch == '-'){
		token = ADDOP;
		value = SUB;
		nextChar();
	}
	else if(ch == '%'){
		token = MULOP;
		value = MOD;
		nextChar();
	}
	else if(ch == '*'){
		token = MULOP;
		value = MUL;
		nextChar();
	}
	else if(ch == '/'){
		token = MULOP;
		value = DIV;
		nextChar();
	}
	else if(ch == '('){
		token = LP;
		nextChar();
	}
	else if(ch == ')'){
		token = RP;
		nextChar();
	}
	else if(ch == '='){
		nextChar();
		if( ch == '='){
			token = RELOP;
			value = EQ;
			nextChar();
		}
		else
			token = ASSIGN;
	}
	else if(ch == '<'){
		token = RELOP;
		nextChar();
		if( ch == '='){				
			value = LE;
			nextChar();
		}
		else
			value = LT;
	}
	else if(ch == '>'){
		token = RELOP;
		nextChar();
		if(ch == '='){
			value = GE;
			nextChar();
		}
		else
			value = GT;
	}
	else if(ch == '&'){
		nextChar();
		if(ch == '&'){
			token = AND;
			nextChar();
		}
		else
			lexError("& is missing.");		
	}
	else if(ch == '|'){
		nextChar();
		if(ch == '|'){
			token = OR;
			nextChar();
		}
		else
			lexError("| is missing.");			
	}
	else if(ch == '!'){
		nextChar();
		if( ch == '='){
			token = RELOP;
			value = NE;
			nextChar();
		}
		else 
			token = NOT;
	}
	else if(isLetter(ch)){		
		int index = 0;
		id[index++] = ch;
		nextChar();
		while(isLetter(ch) || isDigit(ch)){
			if(index < MaxIdLen){
				id[index++] = ch;
				nextChar();
			}
			else{
				lexError("identifier is too long.");
				break;
			}
		}
		id[index] = '\0';
		token = getKeyWord(id);
		if(token == UNDEFINED)
			token = ID;
	}
	else if(isDigit(ch)){
		token = NUM;
		int num = 0;
		num = ch - '0';
		nextChar();
		while( isDigit(ch)){
			num = num * 10 + (ch - '0');
			nextChar();
		}
		value = num;
	}
	else if(ch == '{'){
		token = LB;
		nextChar();
	}
	else if(ch == '}'){
		token = RB;
		nextChar();
	}
	else if(ch == ','){
		token = COMMA;
		nextChar();
	}
	else if(ch == ';'){
		token = SEMICOLON;
		nextChar();
	}	
	else if(ch == EOF){
		token = EOF;
#ifdef ZCW_DEBUG
		printf("end of file.\n");		
#endif
	}
	else{
		lexError("Fatal error: unknown character.");
		exit(0);
	}
	/*
	if(token != EOF)
		printf("%s %s %d \n",types[token],id,value);	
	*/
	
}
