#ifndef SYN_KUR_PROTECTOR
#define SYN_KUR_PROTECTOR
#include "lex.hpp"
#include "module.hpp"
//TYPES OF ERRORS
enum
{
	e_exp_kwd=0,
	e_exp_del=1,
//semicolon
//	e_exp_sc,
	e_dbl_name=2,
	e_no_name=3,
	e_exp_name=4,
	e_exp_boper=5,
	e_exp_numovar=6,
	e_exp_complex=7,
	e_exp_num=8,
	e_exp_var=9,
	e_exp_str=10
};

class Errmes
{
	int type;
	int enume;
public:
	Errmes(int num1, int num2=0);
	void errproc(lexem* lex);
};

//****************************************************************************
enum
{
	t_tselle=1,
	t_netselle=2,
	t_string=3,
	t_atselle=4,
	t_anetselle=5
};


enum
{
	i_cast_err=1,
	i_wrong_type=2,
	i_type_mm=3,
	i_inappr_oper=4,
	i_wrong_command=5,
	i_koniets=6,
	i_arr_break=7
};

class IntErr
{
	int type;
public:
	IntErr(int t);
	void errproc();
};

//At this point I dont wanna mess up with last_elem kinda stuff, so
//welcome to the recursion!

class MemTable
{
	void* p_val;
	int p_type;
	int p_size;
	MemTable* p_next;
public:
	int type();
	MemTable(int a, int b=1);
	void addelem(int a,int b=1);
	MemTable* operator[](int i);
	int size_of();
	int& get_int(int i=0);
	float& get_float(int i=0);
	char* get_string(int i=0);
};

const int stack_size=1048576;
class KurwaConst;
class Stack
{
	KurwaConst* memory[stack_size];
	int i;
public:
	Stack();
	void push(KurwaConst* val);
	KurwaConst* pop();
};

class KurwaElem
{
public:
	static Stack* stack;
	KurwaElem(KurwaElem& a);
	KurwaElem();
	virtual void eval(Stack* stack);
};


class KurwaNULL:public KurwaElem
{
	void eval(Stack* stack);
};

//****************************************************************************

class KurwaConst: public KurwaElem
{

public:
	void eval(Stack* stack);
};

class KurwaNum: public KurwaConst
{
	int value;
public:
	KurwaNum(int i);
	int& val();
};

class KurwaFNum: public KurwaConst
{
	float value;
public:
	KurwaFNum(float j);
	float val();
};

class KurwaVarAddr: public KurwaConst
{
public:
	MemTable* val;
	int quant;
	KurwaVarAddr(MemTable* value, int q);
};

class KurwaString: public KurwaConst
{
	char* value;
public:
	KurwaString(char* a);
	char* val();
};

//****************************************************************************

class KurwaFunc: public KurwaElem
{
	
};

class KurwaPrint: public KurwaFunc
{
public:
	void eval(Stack* stack);
};

class KurwaGetVal: public KurwaFunc
{
public:
	void eval(Stack* stack);
};

class KurwaAss: public KurwaFunc
{
	void eval(Stack* stack);
};

class KurwaNeg: public KurwaFunc
{
	void eval(Stack* stack);
};

class KurwaConj: public KurwaFunc
{
	void eval(Stack* stack);
};

class KurwaDisj: public KurwaFunc
{
	void eval(Stack* stack);
};

class KurwaComp: public KurwaFunc
{
	int comp_type;
public:
	KurwaComp(int i);
	void eval(Stack* stack);
};

class KurwaMath: public KurwaFunc
{
	int math_type;
public:
	KurwaMath(int i);
	float float_oper(float a, float b);
	int int_oper(int a, int b);
	void eval(Stack* stack);
};


class KurwaServ: public KurwaFunc
{
	static Bot* bot;
	int cmd;
public:
	KurwaServ(int a);
	void f_con(char* addr, char* porn);
	void f_gpc();
	void f_init();
	void f_gmt();
	void f_bse();
	void f_anal();
	void f_info();
	void f_prod(int i);
	void f_buy(int n, int price);
	void f_sell(int n, int price);
	void f_turn();
	int f_player_count();
	int* f_player_info(int i);
	void eval(Stack* stack);
};

class KurwaIndex: public KurwaFunc
{
public:
	void eval(Stack* stack);
};

//****************************************************************************

class KurwaJumps: public KurwaElem
{
public:
	static int k_current;
};

class KurwaJump: public KurwaJumps
{
	int p_dest;
public:
	int& dest();
	KurwaJump(int dest);
	void eval(Stack* stack);
};

class KurwaJumpFalse: public KurwaJumps
{
	int p_dest;
public:
	int& dest();
	KurwaJumpFalse(int dest);
	void eval(Stack* stack);
};

class KurwaEND: public KurwaJumps
{
	void eval(Stack* stack);
};
//****************************************************************************

//________KURWALIST_________
class KurwaList
{
	KurwaElem* a;
	KurwaList* p_next;
	static int p_cur;
	int p_num;
	static KurwaList* p_last;
	void last_addelem(KurwaElem* newval);
public:
	KurwaElem* val();
	KurwaList(KurwaElem* newval);
	int addelem(KurwaElem* newval);
	KurwaList* next();
	KurwaList* operator[] (int i);
	int num();
	const char* type();
};

//________KURWALIST_________

class TRNameTable
{
	char* p_name;
	bool right_name(char* val);
	TRNameTable* p_next;
	static TRNameTable* p_last;
	void last_addelem(char* newval);
public:
	TRNameTable(char* value);
	char* name();
	void addelem(lexem* lex);
	TRNameTable* next();
	int is_in(lexem* lex);
};
//****************************************************************************
class Syn_anal
{
	KurwaList kurwalist;
	TRNameTable nametable;
	MemTable memtable;
	list* cur;
	void next();
	void tselle();
	void name_add();
	void name_check();
	void block();
	int is_a_type(lexem* lex);
	void vdecl();
	void f_con();
	void f_gpc();
	void f_init();
	void f_gmt();
	void f_bse();
	void f_anal();
	void f_info();
	void f_prod();
	void f_buy();
	void f_sell();
	void f_turn();
	void f_pc();
	void f_pinfo();
	void f_minfo();
	void f_aauc();
	void f_recom();
	void f_float();
	void f_int();
	void f_getauc();
	void f_myinfo();
	void f_build();
	bool server_command();
	void mulact();
	void yeslee();
	void bexpr();
	void expr();
	int is_math();
	void comparsion();
//	bool is_comp_lex();
	void ass();
	void poka();
	void delat();
	public:
	Syn_anal();
	bool isalright(struct list *lul);
	void printlist();
	void interprer();
};

#endif
