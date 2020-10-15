#ifndef C_H
#define C_H
/////////////////////////////////////////////////////////////
#define CtrlInfoSize 3	  //���¼�Ŀ�����Ϣ�Ĵ�С,
						  //����:���ص�ַ���ϲ���¼����Ե�ַ������ֵ
#define MaxGSize 1024	  //ȫ�ַ��ű���󳤶�
#define MaxLSize 1024	  //�ֲ����ű���󳤶�		
#define DsSize 65536		  //��������С
#define CsSize 65536		  //��������С
#define MaxIdLen 20		  //��־������󳤶�
#define KeyWordsCount 7	  //���Թؼ��ָ���
#define HaltingPC  -1	  //�����������ֵΪ-1,��ͣ��
/////////////////////////////////////////////////////////////
//��Ԫʽ
typedef struct {
	int optr;		//�����
	int arg1;		//��������ĵ�ַ
	int arg2;		//�Ҳ������ĵ�ַ
	int result;     //��Ž���ĵ�ַ
}Instruction;
//���ʽ (����C�ķ�񣬷�0Ϊ�棬0Ϊ��)
typedef struct {
	int truelist;		//�������    
	int falselist;		//�ٳ�����
	int address;		//��ű��ʽֵ�ĵ�ַ������ֵ
						//��Ϊ����������ȫ�ֱ����ĵ�ַ
						//��Ϊ��������Ϊ�ֲ���������Ե�ַ
	bool isTemp;		//�Ƿ�Ϊ��ʱ����
}ExprNode;

//���ű����
typedef struct {
	int type;			//���� FUN �� VAR
	int address;		//��ַ ȫ��Ϊ����  �ֲ�Ϊ��
	char lexeme[MaxIdLen+1];	//���أ�������������	
	int paramCount;		//�����Ĳ�������
}SymbolEntry;

//���ű�
struct SymbolTable{
	SymbolEntry * entries;	//������
	int index;				//���ű�ǰ��β���±꣬���е�0���
};

//�����ָ�
enum InstrType{
	InsJtrue,	//Ϊ������ת	(InsJtrue, arg1, , dest)
	InsJfalse,	//Ϊ������ת	(InsJfalse,arg1, , dest)
	InsJmp,		//��������ת	(InsJmp,  ,   , dest)	
	InsMov,		//���ݸ���      (InsMov, arg1,	  ,dest)
	InsInit,	//��ʼ��ĳ��Ԫ	(InsInit,arg1,num,	   )
	InsAdd,		//�ӷ�			(InsAdd, arg1,arg2,dest)
	InsSub,		//����			(InsSub, arg1,arg2,dest)
	InsMul,		//�˷�			(InsMul, arg1,arg2,dest)
	InsDiv,		//����			(InsDiv, arg1,arg2,dest)
	InsMod,		//ȡ��
	InsNop,		//�ղ���		(InsNop,	,	, 	)
	InsJlt,		//�ж��Ƿ�<		(InsLt,arg1,arg2,result)
	InsJle,		//�ж��Ƿ�<=	(InsLe,arg1,arg2,result)
	InsJgt,		//�ж��Ƿ�>		(InsGt,arg1,arg2,result)
	InsJge,		//�ж��Ƿ�>=	
	InsJeq,		//�ж��Ƿ�==
	InsJne,		//�ж��Ƿ�!=
	InsOr,		//�߼�������
	InsAnd,		//�߼�������
	InsNot,		//�߼�������
	InsIn,		//����һ����������Ԫdest (InsIn,dest , ,);
	InsOut,		//���һ������	(InsOut,num, ,);
	InsUminus,	//����			(InsUminus,oprn, ,dest)
	InsCall,	//���̵���		(InsCall,des, , ,);		
	InsRet,		//���̷���		(InsRet,expr, , );
	InsSetBx,	//����bxָ�룬ָ����¼�׵�ַ(InsSetBx,addr, , )
	InsAddBx,	//bxָ������			(InsSetBx,addr);
};

//�Ǻ�����
enum SymType
{
	OR ,				//��
	AND,			//��
	RELOP,			//��ϵ�����
	ADDOP,			//�Ӽ�	
	MULOP,			//�˳�
	NOT,			//��
	LP,				//������
	RP,				//������
	ID,				//��־��
	NUM,			//��
	ASSIGN,			//��ֵ	
	LB,				//�������
	RB,				//�Ҵ�����
	COMMA,			//����
	SEMICOLON,		//�ֺ�	
	UNDEFINED,		//δ����
	INT,			//int
	IF,				//if
	ELSE,			//else
	WHILE,			//while
	RETURN,			//return
	PRINTF,			//printf
	SCANF,			//scanf
};
//
enum AddOp{
	ADD,			//�ӷ�
	SUB				//����
};
//
enum MulOp{
	MUL,			//�˷�
	DIV,			//����
	MOD,			//ȡ��
};
//��ʶ������
enum IdType{
	FUN,			//����
	VAR				//����
};

//��ϵ���������
enum RelType{
	GT,				//����		>
	GE,				//���ڵ���	>=
	EQ,				//����		==
	LT,				//С��		<
	LE,				//С�ڵ���	<=
	NE,				//������	!=
	NR				//û�й�ϵ  �������ȹ�ϵ��
};
//���ҽ��
struct SearchingResult{
	int index;		//�ڷ��ű��еڼ��� ��0��ʾδ�ҵ�
	int tableType;	//���ű������
	int address;	//�ҵ��ĵ�ַ
};
//���ű����� 
enum TableType{
	GLOBAL,
	LOCAL,
};

/////////////////////////////////////////////////////////////
extern int errorCount;
extern int ds[DsSize];					//���ݶ�			
extern Instruction cs[CsSize];			//�����
extern int csIndex;		//������±�  0��Ԥ������ת��main��ָ��
//extern int dsIndex;		//���ݶ��±�
extern FILE * infile;		//�����ļ�
extern FILE * outfile;		//����ļ�
extern SymbolTable globalTable;				//ȫ�ַ��ű�
extern SymbolTable localTable;				//�ֲ����ű�
extern char * instrs[];

extern int token;							//��ǰ�Ǻ�(������)
extern int value;							//��ǰ�Ǻŵ�ֵ
extern char id[MaxIdLen+1];					//��ǰ���Ŵ�
extern int lineNumber;						//��ǰ�к�
extern char * types[];
extern FILE * infile;		//�����ļ�
/////////////////////////////////////////////////////////////////////
bool isExpressionStarting(int tok);		//tok�Ƿ�Ϊ���ʽ��ʼ�Ǻ�
void interpret();			//����ִ���м����
int getGlobalMemSize();
void callExpr(char * func);
void setActiveTop(int top);
bool isStatementStarting(int tok);
SearchingResult mixingSearch(int idType,char * lexeme);
bool isGlobalState();
int getActiveTop();
void getToken();
int newtemp();
void freeTemp();
void syntaxError(char * info);
void gen(int instrType, int arg1,int arg2,int result);  //����һ����Ԫʽ
void programBody();			//������
void varDeclare();				//���������б�
void funcBody();			//������
int paramDeclare();		//������������
int statement();			//���
bool isCondition(ExprNode * node);
void decrementTopIfTemp(ExprNode * node);
void setExprNode(ExprNode * node, int tlist,int flist,int addr, bool isTemp);
int mergeList(int list1, int list2);  //�ϲ�����
void backpatch(int list, int cx);	  //��������
void changeConditionToArith(ExprNode * node);
void changeArithToCondition(ExprNode * node);
ExprNode expression();		//���ʽ
int enter(SymbolTable * table,int idType,char * lexeme);
int lookup(SymbolTable * table,int idType,char * lexeme);

////////////////////////////////////////////////////////////////////
#endif
