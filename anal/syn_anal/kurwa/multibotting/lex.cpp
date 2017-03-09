#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <time.h>
#include "lex.hpp"

char* lexem::string()
{
 	return value;
}

lexem::lexem(int t): value(0),size(0),pointer(0),type(0),strnum(0),rdy(0)
	{}

lexem::~lexem()
{
	if (value != 0)
		delete[] value;
}

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
	if (cur == '"')
	{
		state = str_s;
		return;
	}
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



//const int kwordssize=21;

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
		"to",
		"print",
		"plr_count",
		"plr_info",//6 int; num/int;
		"mkt_info",
		"analauc",//
		"recommend",//input: 4 int; output: 2 int;
		"float",
		"int",
		"getauc",//6 int; num/int;
		"my_info",
		"build"
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
		 (cur >= 'A' && cur <= 'Z') ||
		 (cur == '_'))
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
	if (cur == '"')
	{
		lex->type = str_s;
		lex->rdy = true;
		lex_snd = lex;
		lex = new lexem;
		state = base_s;
	} else
	lex->addc(cur);
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

