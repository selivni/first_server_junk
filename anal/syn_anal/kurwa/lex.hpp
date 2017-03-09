#ifndef LEX_PROTECTOR
#define LEX_PROTECTOR
//FOR KWDTYPE
enum
{
	k_prog=0,
	k_str=1,
	k_tsel=2,
	k_nets=3,
	k_con=4,
	k_gpc=5,
	k_init=6,
	k_poka=7,
	k_dlt=8,
	k_gmt=9,
	k_bse=10,
	k_anal=11,
	k_info=12,
	k_knts=13,
	k_prod=14,
	k_buy=15,
	k_sell=16,
	k_turn=17,
	k_yeslee=18,
	k_to=19,
	k_print=20,
	k_pc=21,
	k_pinfo=22,
	k_minfo=23,
	k_aauc=24,
	k_recom=25,
	k_float=26,
	k_int=27,
	k_getauc=28,
	k_myinfo=29,
	k_build=30
};

//FOR DELIMTYPE
enum
{
	d_enter=0,
	d_tab=1,
	d_space=2,
	d_plus=3,
	d_minus=4,
	d_mult=5,
	d_div=6,
	d_mod=7,
	d_less=8,
	d_more=9,
	d_ass=10,
	d_addr=11,
	d_stick=12,
	d_neg=13,
	d_oper=14,
	d_zpt=15,
	d_fsop=16,
	d_fscl=17,
	d_scop=18,
	d_sccl=19,
	d_ksop=20,
	d_kscl=21,
	d_damp=22,
	d_dstick=23,
	d_eq=24
};

struct lexem
{
//DO NOT TOUCH IT PLEASE
	char* value;
	int size;
	int pointer;
	const static int stdsize=16;
//\DO NOT TOUCH IT PLEASE
	int type;
	int kwd_type;
	int delim_type;
	int strnum;
	bool rdy;
	void addc(char c);
	lexem(int t=0);
	~lexem();
	bool cmp_str(const char* str) const;
	char* string();
};

//STATE OF THE MATRESHQUA AND DA LEXEM TYPE

enum 
{
	endofprogram_s=-1,
	base_s=0,
	num_s=1,
	fnum_s=2,
	var_s=3,
	key_s=4,
	delim_s=5,
	str_s=6
};

const char base_errmes[]=
	"error: unexpected token \"";
const char num_errmes[]=
	"error: expected delimeter, got \"";
const char key_errmes[]=
	"error: unknown keyword \"";
const char str_errmes[]=
	"error: expected \", got \\n\n";
const char delim_errmes[]=
	"error: you typed some strange shit here...\n"
	"dont even know what to say here";
const char delim[]="\n\t +-*/%<>=&|!;,{}()[]";

const int kwordssize=31;
class Matreshqua 
{
	int state;
	int strnum;
	char saved_char;
	char cur;
	lexem* lex;
	lexem* lex_snd;
	void base_errproc();
	void st_base();
	void uni_errproc();
	void st_num();
	void st_fnum();
	void st_var();
	void key_errproc();
	void st_key();
	void str_errproc();
	void st_str();
	void delim_errproc();
	void st_delim();
	bool in_set(const char c,const char* set);
	bool in_set(lexem* l);
	void cycle();
public:
	bool is_okay();
	Matreshqua();
	lexem* esh_char(char c);
};

const char s_rerouting[]="Reading symbols from";
const char s_rerout_fail_1[]="Failed to open ";
const char s_rerout_fail_2[]="now reading from stdin:";

const char lextype[7][30]=
{
	"\n\n\n\n",
	"NUMBER",
	"FNUMBER",
	"VARIABLE",
	"KEYWORD",
	"DELIMETER",
	"STRING"
};

struct list
{
	lexem* val;
	list* next;
	list(lexem* nlex);
	void newitem(lexem* nlex=0);
	bool is_in(lexem* lex);
	int& type();
	int& kwd_type();
	int& delim_type();
};
#endif
