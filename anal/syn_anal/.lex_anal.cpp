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
	k_endt=13,
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
	d_neg=12,
	d_oper=13,
	d_zpt=14,
	d_fsop=15,
	d_fscl=16,
	d_scop=17,
	d_sccl=18,
	d_damp=19,
	d_dstick=20,
	d_eq=21
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
	bool rdy;
	void addc(char c);
	lexem(int t=0): value(NULL),size(0),pointer(0),type(0),rdy(0)
		{}
	
	~lexem()
	{
		if (value != NULL)
			delete[] value;
	}

	bool cmp_str(const char* str) const;

	char* string()
	{
		return value;
	}
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

enum 
{
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
const char delim[]="\n\t +-*/%<>=&|!;,{}()";

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
	lexem* retvalue = NULL;
	saved_char = cur;
	cur = c;
//	printf("My state is %d\n",state);
	cycle();
	if (lex_snd != NULL)
	{
		if (lex_snd->rdy == true)
		{
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
	lex_snd = NULL;
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
			if (c == '\n')
				strnum++;
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
	{
		strnum++;
		return;
	}
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

static const char s_success[]="SUCCESS\n";
static const char s_unsuccess[]="NOT TODAY,SON\n";
int main(int argc, char** argv)
{
	FILE* fd;
	Matreshqua aut;
	char food;
	fd = stdin;
	if (argv[1] != NULL)
	{
		if (NULL == (fd = fopen(argv[1],"r")))
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
		if ((res = aut.esh_char(food)) != NULL)
		{
			printf("Got a lexem of type \"%s\"! \"%s\"\n",lextype[res->type],res->string());
			delete res;
			res = NULL;
		}
	}
	if (aut.is_okay())
		printf("%s",s_success);
	else
		printf("%s",s_unsuccess);
	return 0;
}


















