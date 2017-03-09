#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	k_to=19
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
	lexem(int t=0): value(0),size(0),pointer(0),type(0),strnum(0),rdy(0)
		{}
	
	~lexem()
	{
		if (value != 0)
			delete[] value;
	}

	bool cmp_str(const char* str) const;

	char* string() {return value;}
};

bool lexem::cmp_str(const char* str) const
{
	int i=0;
	while (value[i] == str[i] && str[i] != '\0' && value[i] != '\0')
		{i++;}
	if (value[i] == str[i])
		return true;
	return false;
}
void lexem::addc(char c)
{
	if (size - 1 <= pointer)
	{
		char* newval;
		newval = new char [size+stdsize];
		for (int i=0; i<pointer; i++)
			newval[i] = value[i];
		delete[] value;
		size += stdsize;
		value = newval;
	}
	value[pointer++] = c;
	value[pointer] = '\0';
}

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

bool Matreshqua::is_okay()
{
	return (state == 0);
}

lexem* Matreshqua::esh_char(char c)
{
	lexem* retvalue = 0;
	saved_char = cur;
	cur = c;
	if (c == '\n')
		strnum++;
//	printf("My state is %d\n",state);
	cycle();
	if (lex_snd != 0)
	{
		if (lex_snd->rdy == true)
		{
			lex_snd->strnum = strnum;
			lex_snd->rdy = false;
			retvalue = lex_snd;
		}
	}
	return retvalue;
}

void Matreshqua::cycle()
{
	switch (state)
	{
		case base_s:
			st_base();
			break;
		case num_s:
			st_num();
			break;
		case fnum_s:
			st_fnum();
			break;
		case var_s:
			st_var();
			break;
		case key_s:
			st_key();
			break;
		case delim_s:
			st_delim();
			break;
		case str_s:
			st_str();
			break;
		default:
			exit(2);
	}
}

Matreshqua::Matreshqua()
{
	lex = new lexem;
	lex_snd = 0;
	state = base_s;
	strnum = 1;
	cur = ' ';
}

bool Matreshqua::in_set(const char c,const char* set)
{
	int i=0;
	while (set[i] != '\0')
		if (set[i++] == c)
		{
			lex->delim_type = i-1;
			return true;
		}
	return false;
}

void Matreshqua::base_errproc()
{
	printf("%d: %s%c\"\n",strnum,base_errmes,cur);
	exit(0);
}

void Matreshqua::st_base()
{
	if (cur == '\n')
		return;
	if (cur == ' '|| cur == '\t')
		return;
	lex->addc(cur);
	if (in_set(cur,delim))
	{
		saved_char = cur;
		state = delim_s;
		return;
	}
	if (cur >= '0' && cur <= '9')
	{
		state = num_s;
		return;
	}
	if (cur == '@')
	{
		state = var_s;
		return;
	}
	if ((cur >= 'a' && cur <= 'z') || (cur >= 'A' && cur <= 'Z'))
	{
		state = key_s;
		return;
	}
	if (cur == '"')
	{
		state = str_s;
		return;
	}
	base_errproc();
}

void Matreshqua::uni_errproc()
{
	printf("%d: %s%c\"\n",strnum,num_errmes,cur);
	exit(0);
}

void Matreshqua::st_num()
{
	if (lex->type != num_s)
		lex->type = num_s;
	if (cur >= '0' && cur <= '9')
	{
		lex->addc(cur);
		return;
	}
	if (cur == '.')
	{
		state = fnum_s;
		lex->addc(cur);
		lex->type = fnum_s;
		return;
	}
	if (cur == ' ' ||
		 cur == '\n'||
		 cur == '\t')
	{
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
		return;
	}
	if (in_set(cur,delim))
	{
		saved_char = cur;
		state = delim_s;
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		lex->addc(cur);
		lex->delim_type = lex_snd->delim_type;
	}
	else
	uni_errproc();
}

void Matreshqua::st_fnum()
{
	if (cur >= '0' && cur <= '9')
	{
		lex->addc(cur);
		return;
	}
	if (cur == ' ' ||
		 cur == '\n'||
		 cur == '\t')
	{
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
		return;
	}
	if (in_set(cur,delim))
	{
		saved_char = cur;
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		lex->addc(cur);
		lex->delim_type = lex_snd->delim_type;
		state = delim_s;
	}
	else
		uni_errproc();
}

void Matreshqua::st_var()
{
	if (cur == '_' || 
		 (cur >= 'a' && cur <= 'z') || 
		 (cur >= 'A' && cur <= 'Z') ||
		 (cur >= '0' && cur <= '9'))
	{
		lex->addc(cur);
		return;
	}
	if (cur == ' ' ||
		 cur == '\n'||
		 cur == '\t')
	{
		lex->type = var_s;
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
		return;
	}
	if (in_set(cur,delim))
	{
		saved_char = cur;
		lex->type = var_s;
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		lex->addc(cur);
		lex->delim_type = lex_snd->delim_type;
		state = delim_s;
	}
	else
		uni_errproc();
}

//LIST OF THE KEYWORDS

const int kwordssize=20;



bool Matreshqua::in_set(lexem* l)
{
	const char k_words[kwordssize][32]=
	{
		"programma",
		"stroqua",
		"tsel",
		"netsel",
		"connect",
		"getplrcnt",
		"initplrs",
		"poka",
		"delat",
		"getmarket",
		"buysellend",
		"analdead",
		"getinfo",
		"koniets",
		"prod",
		"buy",
		"sell",
		"turn",
		"yeslee",
		"to"
	};
	int i=0;
	while (i<kwordssize)
	{
		if (l->cmp_str(k_words[i++]))
		{
			lex->kwd_type = i-1;
			return true;
		}
	}
	return false;
}

void Matreshqua::key_errproc()
{
	printf("%d: %s%s\"\n",strnum,key_errmes,lex->string());
	exit(0);
}

void Matreshqua::st_key()
{
	if ((cur >= 'a' && cur <= 'z') || 
		 (cur >= 'A' && cur <= 'Z'))
	{
		lex->addc(cur);
		return;
	}
	if (cur == ' ' ||
		 cur == '\n'||
		 cur == '\t')
	{
		lex->type = key_s;
		lex->rdy = true;
		if (in_set(lex))
			state = delim_s;
		else
			key_errproc();
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
		return;
	}
	if (in_set(cur,delim))
	{
		saved_char = cur;
		lex->type = key_s;
		lex->rdy = true;
		if (in_set(lex))
			state = delim_s;
		else
			key_errproc();
		lex_snd = lex;
		lex = new lexem;
		lex->addc(cur);
		lex->delim_type = lex_snd->delim_type;
	}
	else
		uni_errproc();
}

void Matreshqua::str_errproc()
{
	printf("%d: %s",strnum,str_errmes);
	exit(0);
}

void Matreshqua::st_str()
{
	lex->addc(cur);
	if (cur == '"')
	{
		lex->type = str_s;
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
	}
	if (cur == '\n')
		str_errproc();
}

void Matreshqua::delim_errproc()
{
	printf("%d: %s\n",strnum,delim_errmes);
	exit(1);
}

//	static char delim[]="\n\t +-*/%<>=&|!;,";
void Matreshqua::st_delim()
{
	if (lex->type != delim_s)
		lex->type = delim_s;
	if (cur == saved_char)
	{
		switch (cur)
		{
			case '=':
				lex->delim_type = d_eq;
				break;
			case '|':
				lex->delim_type = d_dstick;
				break;
			case '&':
				lex->delim_type = d_damp;
				default:
				break;
		}
		if (cur == '='||
			 cur == '|'||
			 cur == '&')
		{
			lex->addc(cur);
			lex->rdy = true;
			lex_snd = lex;
			lex = new lexem;
			state = base_s;
			return;
		}
	}
	saved_char = cur;
	lex->rdy = true;
	lex_snd = lex;
	lex = new lexem;
	if (cur == ' ' || 
		 cur == '\n' ||
		 cur == '\t')
	{
		state = base_s;
		return;
	}
	if (!(in_set(cur,delim)))
	{
		state = base_s;
		cycle();
		return;
	}
	lex->addc(cur);

/*	if (lex->type != delim_s)
		lex->type = delim_s;
	if (cur == ' ' || 
		 cur == '\n' ||
		 cur == '\t')
	{
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
		return;
	}
	if (cur != saved_char)
	{
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
		cycle();
		return;
	}
	if (cur == '=')
	{
		lex->addc(cur);
		lex->delim_type = d_eq;
	}
	else
	if (cur == '|')
	{
		lex->addc(cur);
		lex->delim_type = d_dstick;
	}
	else
	if (cur == '&')
	{
		lex->addc(cur);
		lex->delim_type = d_damp;
	}
	if (saved_char == '\r')
		lex->addc(cur);
	lex->rdy = true;
	lex_snd = lex;
	lex = new lexem;
	state = base_s;
	saved_char = '\r';*/
}

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

int& list::type()
{
	return val->type;
}

int& list::kwd_type()
{
	return val->kwd_type;
}

int& list::delim_type()
{
	return val->delim_type;
}

list::list(lexem* nlex=0)
{
	val = nlex;
	next = 0;
}

void list::newitem(lexem* nlex)
{
	struct list* cur=this;
	while (cur->next != 0)
	  cur = cur->next;
	cur->next = new list(nlex);
}

bool list::is_in(lexem* lex)
{
	struct list* cur = this;
	while (cur != 0)
	{
		if (cur->val->cmp_str(lex->value))
			return true;
		cur = cur->next;
	}
	return false;
}

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
	e_exp_complex=7
};

class Errmes
{
	int type;
	int enume;

public:
	Errmes(int num1, int num2=0):type(num1), enume(num2)
		{}
	void errproc(lexem* lex)
	{
		const char s_errproc[]="error: expected";
		const char s_dbl_name[]="error: double declaration of variable";
		const char s_no_name_1[]="error: variable";
		const char s_no_name_2[]="is undeclared";
		const char s_exp_name[]="error: name expected";
		const char s_exp_boper[]="error: expected \"&&\" or \"||\"";
		const char s_exp_numovar[]="error: expected value or variable";
		const char s_exp_complex[]="error: expected comparsion";
		const char s_default[]="error: unknown error";
		const char k_words[kwordssize][32]=
		{
			"programma",
			"stroqua",
			"tsel",
			"netsel",
			"connect",
			"getplrcnt",
			"initplrs",
			"poka",
			"delat",
			"getmarket",
			"buysellend",
			"analdead",
			"getinfo",
			"koniets",
			"prod",
			"buy",
			"sell",
			"turn",
			"yeslee",
			"to"
		};
		switch (type)
		{
		case e_exp_kwd:
			printf("%d: %s \"%s\", got \"%s\" instead\n",lex->strnum,s_errproc,k_words[enume],lex->string());
			break;
		case e_exp_del:
			printf("%d: %s \"%c\", got \"%s\" instead\n",lex->strnum,s_errproc,delim[enume],lex->string());
			break;
		case e_dbl_name:
			printf("%d: %s \"%s\"\n",lex->strnum,s_dbl_name,lex->string());
			break;
		case e_no_name:
			printf("%d: %s \"%s\" %s\n",lex->strnum,s_no_name_1,lex->string(),s_no_name_2);
			break;
		case e_exp_name:
			printf("%d: %s\n",lex->strnum,s_exp_name);
			break;
		case e_exp_boper:
			printf("%d: %s\n",lex->strnum,s_exp_boper);
			break;
		case e_exp_numovar:
			printf("%d: %s\n",lex->strnum,s_exp_numovar);
			break;
		case e_exp_complex:
			printf("%d: %s\n",lex->strnum,s_exp_complex);
			break;
		default:
			printf("%d: %s\n",lex->strnum,s_default);
		}
	}
};


class Syn_anal
{
	list* nametable;
	list* cur;
	void next();
	void tselle();
	void name_add();
	void name_check();
	void block();
	bool is_a_type(lexem* lex);
	void vdecl();
	bool server_command();
	void mulact();
	void yeslee();
	void bexpr();
	void expr();
	bool is_math();
	void comparsion();
	bool is_comp_lex();
	void ass();
	void poka();
	void delat();
	public:
	bool isalright(struct list *lul)
	{
		cur = lul;
		nametable = 0;
		try 
		{
			tselle();
		}
		catch(Errmes &err)
		{
			err.errproc(cur->val);
			return false;
		}
		return true;
	}
};

void Syn_anal::next()
{
	printf("%s\n",cur->val->string());
	if (cur->next == 0)
		cur->type() = endofprogram_s;
	else
		cur = cur->next;
}

void Syn_anal::tselle()
{
	if (!(cur->type() == key_s && cur->kwd_type() == k_prog))
		throw Errmes(e_exp_kwd,k_prog);
	next();

	name_add();

	next();

	if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
		throw Errmes(e_exp_del,d_oper);
	next();

	block();
}

void Syn_anal::name_add()
{
	if (cur->type() == var_s)
	{
		if (nametable == 0)
			nametable = new list(cur->val);
		else
		{
			if (nametable->is_in(cur->val))
				throw Errmes(e_dbl_name);
			nametable->newitem(cur->val);
		}
	} else
		throw Errmes(e_exp_name);
}

void Syn_anal::name_check()
{
	if (cur->type() == var_s)
	{
		if (nametable == 0 || !(nametable->is_in(cur->val)))
			throw Errmes(e_no_name);
	}
}

void Syn_anal::block()
{
	if (!(cur->type() == delim_s && cur->delim_type() == d_fsop))
		throw Errmes(e_exp_del,d_fsop);
	next();

	vdecl();

	mulact();

	if (!(cur->type() == delim_s && cur->delim_type() == d_fscl))
		throw Errmes(e_exp_del,d_fscl);
	next();
}

bool Syn_anal::is_a_type(lexem* lex)
{
	if (cur->type() == key_s && cur->kwd_type() == k_tsel)
		return true;
	if (cur->type() == key_s && cur->kwd_type() == k_nets)
		return true;
	if (cur->type() == key_s && cur->kwd_type() == k_str)
		return true;
	return false;
}

void Syn_anal::vdecl()
{
	while (is_a_type(cur->val))
	{
		do
		{
			next();
			name_add();
			next();
			if (cur->type() == delim_s && cur->delim_type() == d_ksop)
			{
				next();
				if (cur->type() == var_s)
				{
					name_check();
					next();
				} else
				if (cur->type() == num_s)
					next();
				else
					throw Errmes(e_exp_numovar);
				if (cur->type() == delim_s && cur->delim_type() == d_kscl)
					next();
				else throw Errmes(e_exp_del,d_kscl);
			}
		}	while (cur->type() == delim_s && cur->delim_type() == d_zpt);
		if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
			throw Errmes(e_exp_del,d_oper);
		next();
	}
}

bool Syn_anal::server_command()
{
//IMPLYING CUR->TYPE() == KEY_S
	if (cur->kwd_type() == k_con)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_gpc)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_init)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_gmt)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_bse)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_anal)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_info)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_prod)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_buy)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_sell)
	{
		next();
		return true;
	}
	if (cur->kwd_type() == k_turn)
	{
		next();
		return true;
	}
	return false;
}

void Syn_anal::mulact()
{
	while (1)
	{
		if (cur->type() == var_s)
		{
			name_check();

			next();

			ass();
			continue;
		}
		else
		if (cur->type() == key_s)
		{
			if (cur->kwd_type() == k_yeslee)
			{
				next();

				yeslee();
				continue;
			}
			else
			if (cur->kwd_type() == k_poka)
			{
				next();

				poka();
				continue;
			}
			else
			if (cur->kwd_type() == k_dlt)
			{
				next();

				delat();
				continue;
			}
			if (cur->kwd_type() == k_knts)
			{
				next();
				if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
					throw Errmes(e_exp_del,d_oper);
				next();
				continue;
			}
			if (server_command())
			{
				if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
					throw Errmes(e_exp_del,d_oper);
				next();
				continue;
			}
		}
		return;
	}
}

void Syn_anal::yeslee()
{
	if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
		throw Errmes(e_exp_del,d_scop);
	next();

	bexpr();
	
	if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
		throw Errmes(e_exp_del,d_sccl);
	next();
	if (!(cur->type() == key_s && cur->kwd_type() == k_to))
		throw Errmes(e_exp_kwd,k_to);
	next();

	block();
}

void Syn_anal::bexpr()
{
	if (cur->type() == delim_s && cur->delim_type() == d_neg)
	{
		next();
		if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
			throw Errmes(e_exp_del,d_scop);
		next();

		bexpr();
		if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
			throw Errmes(e_exp_del,d_sccl);
		next();
		return;
	}
	if (cur->type() == delim_s && cur->delim_type() == d_scop)
	{
		next();
		bexpr();
		if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
			throw Errmes(e_exp_del,d_sccl);
		next();
		if (cur->type() == delim_s && cur->delim_type() == d_damp)
		{
			next();
		} else
			if (cur->type() == delim_s && cur->delim_type() == d_dstick)
			{
				next();
			} else
				throw Errmes(e_exp_boper);
		if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
			throw Errmes(e_exp_del,d_scop);
		next();

		bexpr();

		if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
			throw Errmes(e_exp_del,d_sccl);
		next();
		return;
	}
	expr();
	comparsion();
}

void Syn_anal::expr()
{
	if (cur->type() == delim_s && cur->delim_type() == d_scop)
	{
		next();
		expr();
		if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
			throw Errmes(e_exp_del,d_sccl);
		next();
	} else
	if (cur->type() == num_s || cur->type() == fnum_s)
	{
		next();
	} else
	if (cur->type() == var_s)
	{
		name_check();
		next();
	} else
		throw Errmes(e_exp_numovar);
	if (is_math())
	{
		next();

		expr();
	}
}

bool Syn_anal::is_math()
{
	if (cur->type() == delim_s)
	{
		if (cur->delim_type() == d_plus)
		{
			return true;
		}
		if (cur->delim_type() == d_minus)
		{
			return true;
		}
		if (cur->delim_type() == d_mult)
		{
			return true;
		}
		if (cur->delim_type() == d_div)
		{
			return true;
		}
		if (cur->delim_type() == d_mod)
		{
			return true;
		}
	}
	return false;
}

bool Syn_anal::is_comp_lex()
{
	if (cur->type() == delim_s)
	{
		if (cur->delim_type() == d_eq)
		{
			next();
			return true;
		}
		if (cur->delim_type() == d_less)
		{
			next();
			return true;
		}
		if (cur->delim_type() == d_more)
		{
			next();
			return true;
		}
	}
	return false;
}

void Syn_anal::comparsion()
{
	if (is_comp_lex())
		expr();
	else
		throw Errmes(e_exp_complex);
}

void Syn_anal::ass()
{
	if (!(cur->type() == delim_s && cur->delim_type() == d_ass))
		throw Errmes(e_exp_del,d_ass);
	next();
	expr();
	if (!cur->type() == delim_s && cur->delim_type() == d_oper)
		throw Errmes(e_exp_del,d_oper);
	next();
}

void Syn_anal::poka()
{
	if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
		throw Errmes(e_exp_del,d_scop);
	next();
	bexpr();
	if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
		throw Errmes(e_exp_del,d_sccl);
	next();
	if (!(cur->type() == key_s && cur->kwd_type() == k_dlt))
		throw Errmes(e_exp_kwd,k_dlt);
	next();
	block();
}

void Syn_anal::delat()
{
	block();
	if (!(cur->type() == key_s && cur->kwd_type() == k_poka))
		throw Errmes(e_exp_kwd,k_poka);
	next();
	if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
		throw Errmes(e_exp_del,d_scop);
	next();
	bexpr();
	if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
		throw Errmes(e_exp_del,d_sccl);
	next();
	if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
		throw Errmes(e_exp_del,d_oper);
	next();
}

static const char s_success[]=
	"LEXICAL ANALYSIS SUCCESSFUL, STARTING DA SYNTAX THINGS\n";
static const char s_unsuccess[]="NOT TODAY,SON\n";
int main(int argc, char** argv)
{
	FILE* fd;
	Matreshqua aut;
	char food;
	Syn_anal compiler;
	struct list* first=0;
	fd = stdin;
	if (argv[1] != 0)
	{
		if (0 == (fd = fopen(argv[1],"r")))
		{
			printf("%s %s; %s\n",s_rerout_fail_1,argv[1],s_rerout_fail_2);
		}
		else
		{
			printf("%s %s:\n",s_rerouting,argv[1]);
		}
	}
	while (EOF != (food = fgetc(fd)))
	{
		lexem* res;
		if ((res = aut.esh_char(food)) != 0)
		{
			if (first != 0)
				first->newitem(res);
			else
				first = new list(res);
			printf("Got a lexem of type \"%s\"! \"%s\"\n",
					 lextype[res->type],res->string());
			res = 0;
		}
	}
	if (aut.is_okay())
		printf("%s",s_success);
	else
	{
		printf("%s",s_unsuccess);
		return 0;
	}

	if (compiler.isalright(first))
		printf("Syntaxis analysis successful!\n");
	return 0;
}
