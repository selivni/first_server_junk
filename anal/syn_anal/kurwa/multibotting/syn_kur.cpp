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
#include "syn_kur.hpp"


Errmes::Errmes(int num1, int num2):type(num1), enume(num2)
		{}

void Errmes::errproc(lexem* lex)
{
	const char s_errproc[]="error: expected";
	const char s_dbl_name[]="error: double declaration of variable";
	const char s_no_name_1[]="error: variable";
	const char s_no_name_2[]="is undeclared";
	const char s_exp_name[]="error: name expected";
	const char s_exp_boper[]="error: expected \"&&\" or \"||\"";
	const char s_exp_numovar[]="error: expected value or variable";
	const char s_exp_complex[]="error: expected comparsion";
	const char s_exp_num[]="error: expected numerical";
	const char s_exp_var[]="error: expected variable";
	const char s_exp_str[]="error: expected string constant";
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
		"to",
		"print",
		"plr_count",
		"plr_info",
		"mkt_info",
		"analauc",
		"recommend",
		"float",
		"int"
	};
	switch (type)
	{
	case e_exp_kwd:
		printf("%d: %s \"%s\", got \"%s\" instead\n",
			   lex->strnum,s_errproc,k_words[enume],lex->string());
		break;
	case e_exp_del:
		printf("%d: %s \"%c\", got \"%s\" instead\n",
			   lex->strnum,s_errproc,delim[enume],lex->string());
		break;
	case e_dbl_name:
		printf("%d: %s \"%s\"\n",lex->strnum,s_dbl_name,lex->string());
		break;
	case e_no_name:
		printf("%d: %s \"%s\" %s\n",lex->strnum,
			   s_no_name_1,lex->string(),s_no_name_2);
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
	case e_exp_num:
		printf("%d: %s\n",lex->strnum,s_exp_num);
		break;
	case e_exp_var:
		printf("%d: %s\n",lex->strnum,s_exp_var);
		break;
	case e_exp_str:
		printf("%d: %s\n",lex->strnum,s_exp_str);
		break;
	default:
		printf("%d: %s\n",lex->strnum,s_default);
	}
}


int MemTable::type()
{
	return p_type;
}
	
MemTable::MemTable(int a, int b): p_type(a),p_next(0)
{
	p_size = b;
	switch(a)
	{
	case t_tselle:
		p_val = new int[p_size+1];
		break;
	case t_netselle:
		p_val = new float[p_size+1];
		break;
	case t_string:
		p_val = new char[p_size*256+1];
		break;
	}
}

void MemTable::addelem(int a,int b)
{
	if (p_next == 0)
		p_next = new MemTable(a,b);
	else 
		p_next->addelem(a,b);
}

MemTable* MemTable::operator[](int i)
{
	if (i == 0)
		return this;
	if (p_next == 0)
		return 0;
	return (*p_next)[i-1];
}

int MemTable::size_of()
{
	switch(p_type)
	{
	case t_tselle:
		return sizeof(int);
	case t_netselle:
		return sizeof(float);
	case t_string:
		return 256*sizeof(char);
	default:
		return 0;
	}
}

int& MemTable::get_int(int i)
{
//	printf("Object MemTable:\np_type:%d\n"
//		   "p_size:%d\nGetting int #%d\n"
//		   "i*sizeof(int)==%ld\n",p_type,p_size,i,i*sizeof(int));
	if (i < p_size)
		return ((int*)p_val)[i];
//		return *((int*)((int*)p_val+i*sizeof(int)));
	else
		throw IntErr(i_arr_break);
}

float& MemTable::get_float(int i)
{
	if (i < p_size)
		return ((float*)p_val)[i];
//		return *((float*)((float*)p_val+i*sizeof(float)));
	else
		throw IntErr(i_arr_break);
}

char* MemTable::get_string(int i)
{
	if (i < p_size)
		return (char*) p_val+i*256*sizeof(char);
	else
		throw IntErr(i_arr_break);
}

//****************************************************************************

Stack::Stack(): i(0)
	{}

void Stack::push(KurwaConst* val)
{
	memory[i++] = val;
}

KurwaConst* Stack::pop()
{
	return memory[--i];
}

//****************************************************************************


KurwaElem::KurwaElem(KurwaElem& a)
	{}

KurwaElem::KurwaElem()
{
}

void KurwaElem::eval(Stack* stack)
	{}

void KurwaNULL::eval(Stack* stack)
		{}
//***************************************************************


void KurwaConst::eval(Stack* stack)
{
	stack->push(this);
}


KurwaNum::KurwaNum(int i): value(i)
	{}

int& KurwaNum::val() 
{
	return value;
}

KurwaFNum::KurwaFNum(float j): value(j)
	{}

float KurwaFNum::val() 
{
	return value;
}

KurwaVarAddr::KurwaVarAddr(MemTable* value, int q):val(value),quant(q)
		{}

KurwaString::KurwaString(char* a):value(a)
	{}

char* KurwaString::val() 
{
	return value;
}

//***************************************************************


void KurwaPrint::eval(Stack* stack)
{
	KurwaVarAddr* addr;
	int type;
	addr = dynamic_cast <KurwaVarAddr*> (stack->pop());
	if (addr == 0)
		throw IntErr(i_cast_err);
	type = addr->val->type();
	printf("PROGRAM OUTPUT: ");
	switch(type)
	{
	case t_tselle:
		printf("%d\n",addr->val->get_int(addr->quant));
		break;
	case t_netselle:
		printf("%f\n",addr->val->get_float(addr->quant));
		break;
	case t_string:
		printf("%s\n",addr->val->get_string(addr->quant));
		break;
	default:
		throw IntErr(i_wrong_type);
	}
}

void KurwaGetVal::eval(Stack* stack)
{
	KurwaVarAddr* addr;
	int type;
	addr = dynamic_cast <KurwaVarAddr*> (stack->pop());
	if (addr == 0)
		throw IntErr(i_cast_err);
	type = addr->val->type();
	switch(type)
	{
	case t_tselle:
	{
		KurwaNum* newval1=new KurwaNum(addr->val->get_int(addr->quant));
		stack->push(newval1);
		break;
	}
	case t_netselle:
	{
		KurwaFNum* newval2=
			new KurwaFNum(addr->val->get_float(addr->quant));
		stack->push(newval2);
		break;
	}
	case t_string:
	{
		KurwaString* newval3=
			new KurwaString(addr->val->get_string(addr->quant));
		stack->push(newval3);
		break;
	}
	default:
		throw IntErr(i_wrong_type);
	}
}

void KurwaAss::eval(Stack* stack)
{
	int i=0;
	char* string1;
	int type;
	char* string2;
	KurwaConst* c;
	KurwaVarAddr* addr;
	KurwaNum* newval1;
	KurwaFNum* newval2;
	KurwaString* newval3;
	c = stack->pop();
	addr = dynamic_cast <KurwaVarAddr*> (stack->pop());
	if (addr == 0)
		throw IntErr(i_cast_err);
	type = addr->val->type();
	switch(type)
	{
	case t_tselle:
		if ((newval1 = dynamic_cast <KurwaNum*> (c)) == 0)
			throw IntErr(i_type_mm);
		addr->val->get_int(addr->quant) = newval1->val();
		break;
	case t_netselle:
		if ((newval2 = dynamic_cast <KurwaFNum*> (c)) == 0)
			throw IntErr(i_type_mm);
		addr->val->get_float(addr->quant) = newval2->val();
		break;
	case t_string:
		if ((newval3 = dynamic_cast <KurwaString*> (c)) == 0)
			throw IntErr(i_type_mm);
		string1 = addr->val->get_string(addr->quant);
		string2 = newval3->val();
		do
		{
			string1[i] = string2[i];
			i++;
		} while (i < 256 && string2[i] != '\0');
		string1[i] = '\0';
		break;
	default:
		throw IntErr(i_wrong_type);
	}
}

void KurwaNeg::eval(Stack* stack)
{
	KurwaNum* newval;
	newval = dynamic_cast <KurwaNum*> (stack->pop());
	if (newval == 0)
		throw IntErr(i_cast_err);
	if (newval->val() == 0)
		newval->val() = 1;
	else
		newval->val() = 0;
	stack->push(newval);
}

void KurwaConj::eval(Stack* stack)
{
	KurwaNum* newval1;
	KurwaNum* newval2;
	newval1 = dynamic_cast <KurwaNum*> (stack->pop());
	newval2 = dynamic_cast <KurwaNum*> (stack->pop());
	if (newval1 == 0 || newval2 == 0)
		throw IntErr(i_cast_err);
	newval2->val() = newval1->val() * newval2->val();
	stack->push(newval2);
}

void KurwaDisj::eval(Stack* stack)
{
	KurwaNum* newval1;
	KurwaNum* newval2;
	newval1 = dynamic_cast <KurwaNum*> (stack->pop());
	newval2 = dynamic_cast <KurwaNum*> (stack->pop());
	if (newval1 == 0 || newval2 == 0)
		throw IntErr(i_cast_err);
	newval2->val() = newval1->val() + newval2->val();
	stack->push(newval2);
}

KurwaComp::KurwaComp(int i):comp_type(i)
	{}

void KurwaComp::eval(Stack* stack)
{
	bool b;
	float c,d;
	KurwaConst* a;
	KurwaNum* newval1;
	KurwaFNum* newval2;
	a = stack->pop();
	if ((newval1 = dynamic_cast <KurwaNum*> (a)) == 0)
	{
		if ((newval2 = dynamic_cast <KurwaFNum*> (a)) == 0)
			throw IntErr(i_cast_err);
		c = newval2->val();
	} else
		c = newval1->val();
	a = stack->pop();
	if ((newval1 = dynamic_cast <KurwaNum*> (a)) == 0)
	{
		if ((newval2 = dynamic_cast <KurwaFNum*> (a)) == 0)
			throw IntErr(i_cast_err);
		d = newval2->val();
	} else
		d = newval1->val();
	switch(comp_type)
	{
	case d_less:
		b = d < c;
		break;
	case d_more:
		b = d > c;
		break;
	case d_eq:
		b = d == c;
		break;
	}
	if (b)
		newval1 = new KurwaNum(1);
	else
		newval1 = new KurwaNum(0);
	stack->push(newval1);
}


KurwaMath::KurwaMath(int i): math_type(i)
	{}

void KurwaMath::eval(Stack* stack)
{
	int i1=-1,i2=-1;
	float d1=-1,d2=-1;
	bool flag1=false;
	bool flag2 = false;
	KurwaConst* a;
	KurwaConst* b;
	KurwaNum* n1;
	KurwaNum* n2;
	KurwaFNum* f1;
	KurwaFNum* f2;
	b = stack->pop();
	a = stack->pop();
	if ((n1 = dynamic_cast <KurwaNum*> (a)) == 0)
	{
		flag1 = true;
		if ((f1 = dynamic_cast <KurwaFNum*> (a)) == 0)
			throw IntErr(i_cast_err);
		d1 = f1->val();
		i1 = d1;
	} else
	{
		i1 = n1->val();
	}
	if ((n2 = dynamic_cast <KurwaNum*> (b)) == 0)
	{
		flag2 = true;
		if ((f2 = dynamic_cast <KurwaFNum*> (b)) == 0)
			throw IntErr(i_cast_err);
		d2 = f2->val();
		i2 = d2;
	} else
		i2 = n2->val();
	if (math_type == d_mult)
		printf("MULT GOT %d %d %f %f\n",i1,i2,d1,d2);
	if (flag1 || flag2)
	{
		if (!flag1)
			d1 = i1;
		if (!flag2)
			d2 = i2;
		d1 = float_oper(d1,d2);
		f1 = new KurwaFNum(d1);
		stack->push(f1);
	} else
	{
		i1 = int_oper(i1,i2);
		n1 = new KurwaNum(i1);
		stack->push(n1);
	}
		
}

float KurwaMath::float_oper(float a, float b)
{
	if (math_type == d_plus)
		return a+b;
	if (math_type == d_minus)
		return a-b;
	if (math_type == d_div)
		return a/b;
	if (math_type == d_mult)
	{
		printf("MULT %f*%f\n",a,b);
		return a*b;
	}
	throw IntErr(i_inappr_oper);
}

int KurwaMath::int_oper(int a, int b)
{
	if (math_type == d_plus)
		return a+b;
	if (math_type == d_minus)
		return a-b;
	if (math_type == d_mult)
		return a*b;
	if (math_type == d_div)
		return a/b;
	if (math_type == d_mod)
		return a%b;
	throw IntErr(i_inappr_oper);
}

Bot* KurwaServ::bot = new Bot();

KurwaServ::KurwaServ(int a): cmd(a)
	{}

void KurwaServ::f_con(char* addr, char* porn)
{
	char** fakeargv;
	char* botname;
	botname = new char[32];
	botname[0] = 'B';
	botname[1] = 'o';
	botname[2] = 't';
	botname[3] = 'b';
	botname[4] = 'o';
	botname[5] = 't';
	botname[6] = '_';
	botname[7] = 'v';
	botname[8] = '0';
	botname[9] = '.';
	botname[10] = '0';
	botname[11] = '1';
	botname[12] = '_';
	botname[13] = 'S';
	botname[14] = 'p';
	botname[15] = 'a';
	botname[16] = 'm';
	botname[17] = 'm';
	botname[18] = 'e';
	botname[19] = 'r';
	botname[20] = '_';
	botname[21] = 'E';
	botname[22] = 'd';
	botname[23] = 'i';
	botname[24] = 't';
	botname[25] = 'i';
	botname[26] = 'o';
	botname[27] = 'n';
	botname[28] = '\0';
	fakeargv = new char*[4];
	fakeargv[0] = 0;
	fakeargv[1] = addr;
	fakeargv[2] = porn;
	fakeargv[3] = botname;
	bot->connect_to(fakeargv);
}

void KurwaServ::f_gpc()
{
	bot->get_plr_cnt();
}
void KurwaServ::f_init()
{
	bot->initplayers();
}
void KurwaServ::f_gmt()
{
	bot->getmarket();
}
void KurwaServ::f_bse()
{
	bot->doyourthing();
}
void KurwaServ::f_anal()
{
	SuperString seek = 
"*****************************END OF AUCTION*************************\n";
	bot->seek_for(seek);
	bot->anal_dead();
}
void KurwaServ::f_info()
{
	bot->getinfo();
}
void KurwaServ::f_prod(int i)
{
	bot->prod(i);
}
void KurwaServ::f_buy(int n, int price)
{
	bot->buy(n,price);
}
void KurwaServ::f_sell(int n, int price)
{
	bot->sell(n,price);
}
void KurwaServ::f_turn()
{
	bot->turn();
}

int KurwaServ::f_player_count()
{
	return bot->player_count();
}

void KurwaServ::eval(Stack* stack)
{
	int* pi1;
	int* pi2;
	char* p1;
	char* p2;
	int i1,i2;
	KurwaString* s1;
	KurwaString* s2;
	KurwaNum* n1;
	KurwaNum* n2;
	KurwaVarAddr* va1;
	KurwaVarAddr* va2;
	KurwaFNum* fn1;
	switch(cmd)
	{
	case k_con:
		s1 = dynamic_cast <KurwaString*> (stack->pop());
		s2 = dynamic_cast <KurwaString*> (stack->pop());
		if (s1 == 0 || s2 == 0)
			throw IntErr(i_cast_err);
		p1 = s1->val();
		p2 = s2->val();
		f_con(p2,p1);
		break;
	case k_gpc:
		f_gpc();
		break;
	case k_init:
		f_init();
		break;
	case k_gmt:
		f_gmt();
		break;
	case k_bse:
		f_bse();
		break;
	case k_anal:
		f_anal();
		break;
	case k_info:
		f_info();
		break;
	case k_prod:
		n1 = dynamic_cast <KurwaNum*> (stack->pop());
		if (n1 == 0)
			throw IntErr(i_cast_err);
		i1 = n1->val();
		f_prod(i1);
		break;
	case k_buy:
		n1 = dynamic_cast <KurwaNum*> (stack->pop());
		n2 = dynamic_cast <KurwaNum*> (stack->pop());
		if (n1 == 0 || n2 == 0)
			throw IntErr(i_cast_err);
		i1 = n1->val();
		i2 = n2->val();
		f_buy(i2,i1);
		break;
	case k_sell:
		n1 = dynamic_cast <KurwaNum*> (stack->pop());
		n2 = dynamic_cast <KurwaNum*> (stack->pop());
		if (n1 == 0 || n2 == 0)
			throw IntErr(i_cast_err);
		i1 = n1->val();
		i2 = n2->val();
		f_sell(i2,i1);
		break;
	case k_turn:
		f_turn();
		break;
	case k_pc:
		i1 = f_player_count();
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		va1->val->get_int(va1->quant) = i1;
		break;
	case k_pinfo:
		n1 = dynamic_cast <KurwaNum*> (stack->pop());
		if (n1 == 0)
			throw IntErr(i_cast_err);
		i1 = n1->val();
		pi1 = bot->player_info(i1);
		if (pi1 == 0)
			throw IntErr(i_arr_break);
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		for (i2 = 0; i2 < 6; i2++)
			va1->val->get_int(i2+va1->quant) = pi1[i2];
		delete[] pi1;
		break;
	case k_minfo:
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		pi1 = bot->market_info();
		for (i2 = 0; i2 < 4; i2++)
			va1->val->get_int(i2+va1->quant) = pi1[i2];
		delete[] pi1;
		break;
	case k_aauc:
		bot->analauc();
		break;
	case k_recom:
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		va2 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va2 == 0)
			throw IntErr(i_cast_err);
		pi2 = new int[4];
		for (i2 = 0; i2 < 4; i2++)
			pi2[i2] = va2->val->get_int(i2+va2->quant);
		pi1 = bot->recommend(pi2);
		delete[] pi2;
		for (i2 = 0; i2 < 2; i2++)
			va1->val->get_int(i2+va1->quant) = pi1[i2];
		delete[] pi1;
		break;
	case k_float:
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		n1 = dynamic_cast <KurwaNum*> (stack->pop());
		if (n1 == 0)
			throw IntErr(i_cast_err);
		va1->val->get_float(va1->quant) = n1->val();
		break;
	case k_int:
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		fn1 = dynamic_cast <KurwaFNum*> (stack->pop());
		if (fn1 == 0)
			throw IntErr(i_cast_err);
		va1->val->get_int(va1->quant) = trunc(fn1->val());
		break;
	case k_getauc:
		n1 = dynamic_cast <KurwaNum*> (stack->pop());
		if (n1 == 0)
			throw IntErr(i_cast_err);
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		pi1 = bot->getauc(n1->val());
		for (i2 = 0; i2 < 6; i2++)
			va1->val->get_int(va1->quant+i2) = pi1[i2];
		delete[] pi1;
		break;
	case k_myinfo:
		va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (va1 == 0)
			throw IntErr(i_cast_err);
		pi1 = bot->my_info();
		for (i2 = 0; i2 < 6; i2++)
			va1->val->get_int(i2+va1->quant) = pi1[i2];
		delete[] pi1;
		break;
	case k_build:
		bot->build();
		break;
	default:
		throw IntErr(i_wrong_command);
	}
}

void KurwaIndex::eval(Stack* stack)
{
	KurwaNum* n;
	KurwaVarAddr* va1;
	n = dynamic_cast <KurwaNum*> (stack->pop());
	if (n == 0)
		throw IntErr(i_cast_err);
	va1 = dynamic_cast <KurwaVarAddr*> (stack->pop());
	if (va1 == 0)
		throw IntErr(i_cast_err);
	va1->quant = n->val();
	stack->push(va1);
}

//***************************************************************

int KurwaJumps::k_current = 0;


int& KurwaJump::dest() 
{
	return p_dest;
}

KurwaJump::KurwaJump(int dest):p_dest(dest)
	{}

void KurwaJump::eval(Stack* stack)
{
	k_current = p_dest-1;
}


int& KurwaJumpFalse::dest() 
{
	return p_dest;
}
KurwaJumpFalse::KurwaJumpFalse(int dest):p_dest(dest)
	{}

void KurwaJumpFalse::eval(Stack* stack)
{
	KurwaNum* n;
	n = dynamic_cast <KurwaNum*> (stack->pop());
	if (n == 0)
		throw IntErr(i_cast_err);
	if (n->val() == 0)
		k_current = p_dest-1;
}

void KurwaEND::eval(Stack* stack)
{
	throw IntErr(i_koniets);
}
//***************************************************************

IntErr::IntErr(int t): type(t)
	{}

void IntErr::errproc()
{
	const char s_cast_err[]="dynamic cast error";
	const char s_wrong_type[]="expected num or fnum";
	const char s_type_mm[]="assignment type mismatch";
	const char s_inappr_oper[]="inappropriate operator for this type";
	const char s_wrong_command[]="sth's wrong with your server command";
	const char s_koniets[]="program sucessfuly stopped itself(thank God)";
	const char s_arr_break[]="breaking array borders";
	switch(type)
	{
	case i_cast_err:
		printf("%d: %s\n",KurwaJumps::k_current,s_cast_err);
		break;
	case i_wrong_type:
		printf("%d: %s\n",KurwaJumps::k_current,s_wrong_type);
		break;
	case i_type_mm:
		printf("%d: %s\n",KurwaJumps::k_current,s_type_mm);
		break;
	case i_inappr_oper:
		printf("%d: %s\n",KurwaJumps::k_current,s_inappr_oper);
		break;
	case i_wrong_command:
		printf("%d: %s\n",KurwaJumps::k_current,s_wrong_command);
		break;
	case i_koniets:
		printf("%d: %s\n",KurwaJumps::k_current,s_koniets);
		break;
	case i_arr_break:
		printf("%d: %s\n",KurwaJumps::k_current,s_arr_break);
		break;
	default:
		printf("REALLY BAD SHIT NIGGA\n");
	}	
}


//________KURWALIST_________

KurwaElem* KurwaList::val() 
{
	return a;
}

KurwaList* KurwaList::next() 
{
	return p_next;
}

int KurwaList::num()
{
	return p_num;
}

const char* KurwaList::type() 
{
	return typeid(*a).name();
}

int KurwaList::p_cur=0;

KurwaList* KurwaList::p_last=0;

void KurwaList::last_addelem(KurwaElem* newval)
{
	p_next = new KurwaList(newval);
}

KurwaList::KurwaList(KurwaElem* newval)
{
	a = newval;
	p_next = 0;
	p_last = this;
	p_num = p_cur++;
}

int KurwaList::addelem(KurwaElem* newval)
{
	p_last->last_addelem(newval);
	return p_last->num();
}

KurwaList* KurwaList::operator[](int i)
{
	if (i == 0)
		return this;
	else
	{
		if (p_next == 0)
			return 0;
		return (*p_next)[i-1];
	}
}

//________KURWALIST_________


char* TRNameTable::name()
{
	return p_name;
}

TRNameTable* TRNameTable::next()
{
	return p_next;
}

TRNameTable* TRNameTable::p_last=0;

TRNameTable::TRNameTable(char* value)/*: p_name(value),p_next(0)*/
	{
		p_last = this;
		p_name = value;
		p_next = 0;
	}

void TRNameTable::last_addelem(char* newval)
	{p_next = new TRNameTable(newval);}

void TRNameTable::addelem(lexem* lex)
	{p_last->last_addelem(lex->value);}

bool TRNameTable::right_name(char* val)
{
	int i=0;
	while (p_name[i] == val[i] && p_name[i] != '\0' && val[i] != '\0')
		i++;
	return (p_name[i] == val[i]);
}

int TRNameTable::is_in(lexem* lex)
{
	int count;
	if (p_name != 0)
		if (right_name(lex->value))
			return 0;
	if (p_next == 0)
		return -1;
	if (-1 == (count = p_next->is_in(lex)))
		return -1;
	else
		return count+1;
}



//****************************************************************************

bool Syn_anal::isalright(struct list *lul)
{
	cur = lul;
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

void Syn_anal::interprer()
{
	KurwaList* cur=kurwalist.next();
	int my_current = 1;
	Stack* stack = new Stack();
	KurwaJumps::k_current = 1;
	try
	{
		while (cur != 0)
		{
			printf("%d\n",cur->num());
			if (KurwaJumps::k_current == my_current)
			{
				cur->val()->eval(stack);
			}
			else
			{

				cur = kurwalist[KurwaJumps::k_current];
				my_current = KurwaJumps::k_current;
				cur->val()->eval(stack);
			}
			KurwaJumps::k_current++;
			my_current++;
			if (cur != 0)
				cur = cur->next();

		}
		printf("SUCESS!\n");
	}
	catch (IntErr a)
	{
		a.errproc();
	}
}

void Syn_anal::printlist()
{
	int i=1;
	while (kurwalist[i] != 0)
	{
		printf("Element %d,type %s;\n",i,kurwalist[i]->type());
		i++;
	}
}

Syn_anal::Syn_anal(): kurwalist(0),nametable((char*)"you_will_never_get_me"),
					  memtable(t_tselle,1)
	{}

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

	if (!(cur->type() == var_s))
/////CHANGE THIS TO E_EXP_VAR
		throw Errmes(e_exp_numovar);

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
		if (-1 != nametable.is_in(cur->val))
			throw Errmes(e_dbl_name);
		nametable.addelem(cur->val);
	} else
		throw Errmes(e_exp_name);
}

void Syn_anal::name_check()
{
	KurwaVarAddr* newval;
	KurwaIndex* val1;
	int num,quant=0;
	if (cur->type() == var_s)
	{

		if (-1 == (num = nametable.is_in(cur->val)))
			throw Errmes(e_no_name);

		next();
		
		newval = new KurwaVarAddr(memtable[num],quant);
		kurwalist.addelem(newval);
		if (cur->type() == delim_s && cur->delim_type() == d_ksop)
		{
			next();
			expr();
			val1 = new KurwaIndex;
			kurwalist.addelem(val1);
/*			next();
			if (cur->type() == num_s)
			{
				quant = atoi(cur->val->string());
				next();
			}
			else
// E_EXP_NUM!!!!!!!!!!!!!!!!!!!!!!1
				throw Errmes(e_exp_numovar);*/
			if (cur->type() == delim_s && cur->delim_type() == d_kscl)
				next();
			else throw Errmes(e_exp_del,d_oper);
		}
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

int Syn_anal::is_a_type(lexem* lex)
{
	if (cur->type() == key_s && cur->kwd_type() == k_tsel)
		return t_tselle;
	if (cur->type() == key_s && cur->kwd_type() == k_nets)
		return t_netselle;
	if (cur->type() == key_s && cur->kwd_type() == k_str)
		return t_string;
	return 0;
}

void Syn_anal::vdecl()
{
	int type,quant;
	while ((type = is_a_type(cur->val)))
	{
		do
		{
			quant = 1;
			next();
			name_add();
			next();
			if (cur->type() == delim_s && cur->delim_type() == d_ksop)
			{
				next();
				if (cur->type() == num_s)
				{
					quant = atoi(cur->val->string());
					next();
				}
				else
//E_EXP_NUM
					throw Errmes(e_exp_numovar);
				if (cur->type() == delim_s && cur->delim_type() == d_kscl)
					next();
				else throw Errmes(e_exp_del,d_kscl);
			}
			memtable.addelem(type,quant);
		}	while (cur->type() == delim_s && cur->delim_type() == d_zpt);
		if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
			throw Errmes(e_exp_del,d_oper);
		next();
	}
}

void Syn_anal::f_con()
{
	KurwaString* val1;
	KurwaString* val2;
	KurwaServ* newval;
	next();
	if (cur->type() != str_s)
		throw Errmes(e_exp_str);
	val1 = new KurwaString(cur->val->string());

	next();

	if (cur->type() != str_s)
		throw Errmes(e_exp_str);
	val2 = new KurwaString(cur->val->string());

	next();
	
	kurwalist.addelem(val1);
	kurwalist.addelem(val2);
	newval = new KurwaServ(k_con);
	kurwalist.addelem(newval);
}
void Syn_anal::f_gpc()
{
	KurwaServ* newval = new KurwaServ(k_gpc);
	kurwalist.addelem(newval);
	next();
}
void Syn_anal::f_init()
{
	KurwaServ* newval = new KurwaServ(k_init);
	kurwalist.addelem(newval);	
	next();
}
void Syn_anal::f_gmt()
{
	KurwaServ* newval = new KurwaServ(k_gmt);
	kurwalist.addelem(newval);
	next();
}
void Syn_anal::f_bse()
{
	KurwaServ* newval = new KurwaServ(k_bse);
	kurwalist.addelem(newval);
	next();
}
void Syn_anal::f_anal()
{
	KurwaServ* newval = new KurwaServ(k_anal);
	kurwalist.addelem(newval);
	next();
}
void Syn_anal::f_info()
{
	KurwaServ* newval = new KurwaServ(k_info);
	kurwalist.addelem(newval);
	next();
}
void Syn_anal::f_prod()
{
	KurwaServ* newval;
	next();
	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);
	newval = new KurwaServ(k_prod);
	kurwalist.addelem(newval);
}
void Syn_anal::f_buy()
{
	KurwaServ* newval;
	next();
	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);


	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);

	newval = new KurwaServ(k_buy);
	kurwalist.addelem(newval);
}
void Syn_anal::f_sell()
{
	KurwaServ* newval;
	next();
	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);


	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);

	newval = new KurwaServ(k_sell);
	kurwalist.addelem(newval);
}
void Syn_anal::f_turn()
{
	KurwaServ* newval = new KurwaServ(k_turn);
	kurwalist.addelem(newval);
	next();
}

void Syn_anal::f_pc()
{
	KurwaServ* newval = new KurwaServ(k_pc);
	next();
	if (cur->type() == var_s)
		name_check();
	else throw Errmes(e_exp_var);
	kurwalist.addelem(newval);
}

void Syn_anal::f_pinfo()
{
	KurwaServ* newval = new KurwaServ(k_pinfo);
	next();
	if (cur->type() == var_s)
		name_check();
	else throw Errmes(e_exp_var);
	if (cur->type() == num_s)
	{
		KurwaNum* val= new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* value = new KurwaGetVal;
		name_check();
		kurwalist.addelem(value);
	} else
		throw Errmes(e_exp_numovar);
	kurwalist.addelem(newval);
}

void Syn_anal::f_minfo()
{
	KurwaServ* newval = new KurwaServ(k_minfo);
	next();
	if (cur->type() == var_s)
		name_check();
	else throw Errmes(e_exp_var);
	kurwalist.addelem(newval);
}

void Syn_anal::f_aauc()
{
	KurwaServ* newval = new KurwaServ(k_aauc);
	kurwalist.addelem(newval);
	next();
}
//4 int 2 int

void Syn_anal::f_recom()
{
	KurwaServ* newval = new KurwaServ(k_recom);
	next();
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();

	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	kurwalist.addelem(newval);
}

void Syn_anal::f_float()
{
	KurwaServ* newval = new KurwaServ(k_float);
	KurwaGetVal* val1 = new KurwaGetVal;
	next();
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	kurwalist.addelem(val1);
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	kurwalist.addelem(newval);
}

void Syn_anal::f_int()
{
	KurwaServ* newval = new KurwaServ(k_int);
	KurwaGetVal* val1 = new KurwaGetVal;
	next();
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	kurwalist.addelem(val1);
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	kurwalist.addelem(newval);
}

void Syn_anal::f_getauc()
{
	KurwaServ* newval = new KurwaServ(k_getauc);
	next();
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	if (cur->type() == num_s)
	{
		KurwaNum* val1 = new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val2 = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val2);
	} else throw Errmes(e_exp_numovar);
	kurwalist.addelem(newval);
}

void Syn_anal::f_myinfo()
{
	KurwaServ* newval = new KurwaServ(k_myinfo);
	next();
	if (cur->type() != var_s)
		throw Errmes(e_exp_var);
	name_check();
	kurwalist.addelem(newval);
}

void Syn_anal::f_build()
{
	KurwaServ* newval = new KurwaServ(k_build);
	kurwalist.addelem(newval);
	next();
}

bool Syn_anal::server_command()
{
//IMPLYING CUR->TYPE() == KEY_S
	if (cur->kwd_type() == k_con)
	{
		f_con();
		return true;
	}
	if (cur->kwd_type() == k_gpc)
	{
		f_gpc();
		return true;
	}
	if (cur->kwd_type() == k_init)
	{
		f_init();
		return true;
	}
	if (cur->kwd_type() == k_gmt)
	{
		f_gmt();
		return true;
	}
	if (cur->kwd_type() == k_bse)
	{
		f_bse();
		return true;
	}
	if (cur->kwd_type() == k_anal)
	{
		f_anal();
		return true;
	}
	if (cur->kwd_type() == k_info)
	{
		f_info();
		return true;
	}
	if (cur->kwd_type() == k_prod)
	{
		f_prod();
		return true;
	}
	if (cur->kwd_type() == k_buy)
	{
		f_buy();
		return true;
	}
	if (cur->kwd_type() == k_sell)
	{
		f_sell();
		return true;
	}
	if (cur->kwd_type() == k_turn)
	{
		f_turn();
		return true;
	}
	if (cur->kwd_type() == k_pc)
	{
		f_pc();
		return true;
	}
	if (cur->kwd_type() == k_pinfo)
	{
		f_pinfo();
		return true;
	}
	if (cur->kwd_type() == k_minfo)
	{
		f_minfo();
		return true;
	}
	if (cur->kwd_type() == k_aauc)
	{
		f_aauc();
		return true;
	}
	if (cur->kwd_type() == k_recom)
	{
		f_recom();
		return true;
	}
	if (cur->kwd_type() == k_float)
	{
		f_float();
		return true;
	}
	if (cur->kwd_type() == k_int)
	{
		f_int();
		return true;
	}
	if (cur->kwd_type() == k_getauc)
	{
		f_getauc();
		return true;
	}
	if (cur->kwd_type() == k_myinfo)
	{
		f_myinfo();
		return true;
	}
	if (cur->kwd_type() == k_build)
	{
		f_build();
		return true;
	}
	return false;
}

void Syn_anal::mulact()
{
	KurwaElem* newval;
	while (1)
	{
		if (cur->type() == var_s)
		{
			name_check();
			ass();
			newval = new KurwaAss();
			kurwalist.addelem(newval);
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
				KurwaEND* newval;
				next();
				if (!(cur->type() == delim_s && cur->delim_type() == d_oper))
					throw Errmes(e_exp_del,d_oper);
				newval = new KurwaEND;
				kurwalist.addelem(newval);
				next();
				continue;
			}
			if (cur->kwd_type() == k_print)
			{
				KurwaPrint* newval = new KurwaPrint;
				next();
				if (!(cur->type() == var_s))
					throw Errmes(e_exp_var);
				name_check();
				kurwalist.addelem(newval);
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
	KurwaJumpFalse* newval;
	KurwaNULL* newval2;
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

	newval = new KurwaJumpFalse(0);
	kurwalist.addelem(newval);

	block();
	
	newval2 = new KurwaNULL;
	newval->dest() = kurwalist.addelem(newval2);
}

void Syn_anal::bexpr()
{
	if (cur->type() == delim_s && cur->delim_type() == d_neg)
	{
		KurwaNeg* newval;
		next();
		if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
			throw Errmes(e_exp_del,d_scop);
		next();

		bexpr();
		if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
			throw Errmes(e_exp_del,d_sccl);
		next();
		newval = new KurwaNeg;
		kurwalist.addelem(newval);
		return;
	}
	if (cur->type() == delim_s && cur->delim_type() == d_scop)
	{
		int bool_type;
		next();
		bexpr();
		if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
			throw Errmes(e_exp_del,d_sccl);
		next();
		bool_type = cur->delim_type();
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

		if (bool_type == d_damp)
		{
			KurwaConj* newval=new KurwaConj;
			kurwalist.addelem(newval);
			return;
		}
		if (bool_type == d_dstick)
		{
			KurwaDisj* newval=new KurwaDisj;
			kurwalist.addelem(newval);
			return;
		}
	}
	expr();
	comparsion();
}

void Syn_anal::expr()
{
	int math;
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
		if (cur->type() == num_s)
		{
			KurwaNum* newval=new KurwaNum(atoi(cur->val->value));
			kurwalist.addelem(newval);
		} else
		if (cur->type() == fnum_s)
		{
			KurwaFNum* newval=new KurwaFNum(atol(cur->val->value));
			kurwalist.addelem(newval);
		}
		next();
	} else
	if (cur->type() == str_s)
	{
		KurwaString* newval = new KurwaString(cur->val->value);
		kurwalist.addelem(newval);
		next();
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* newval;
		name_check();
		newval = new KurwaGetVal;
		kurwalist.addelem(newval);
	} else
		throw Errmes(e_exp_numovar);
	if ((math = is_math()))
	{
		KurwaMath* newval = new KurwaMath(math);
		next();

		expr();
		
		kurwalist.addelem(newval);
	}
}

int Syn_anal::is_math()
{
	if (cur->type() == delim_s)
	{
		if (cur->delim_type() == d_plus)
		{
			return d_plus;
		}
		if (cur->delim_type() == d_minus)
		{
			return d_minus;
		}
		if (cur->delim_type() == d_mult)
		{
			return d_mult;
		}
		if (cur->delim_type() == d_div)
		{
			return d_div;
		}
		if (cur->delim_type() == d_mod)
		{
			return d_mod;
		}
	}
	return 0;
}
/*
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
*/
void Syn_anal::comparsion()
{
	KurwaComp* newval;
	if (!(cur->type() == delim_s && 
	   (cur->delim_type() == d_eq || 
		cur->delim_type() == d_less ||
		cur->delim_type() == d_more)))

		throw Errmes(e_exp_complex);
	newval = new KurwaComp(cur->delim_type());

	next();
	
	expr();
	kurwalist.addelem(newval);
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
	int bool_point;
	KurwaNULL* newval=new KurwaNULL;
	KurwaJumpFalse* newval2=new KurwaJumpFalse(0);
	KurwaJump* newval3;
	if (!(cur->type() == delim_s && cur->delim_type() == d_scop))
		throw Errmes(e_exp_del,d_scop);
	next();

	bool_point = kurwalist.addelem(newval);

	bexpr();

	kurwalist.addelem(newval2);
	
	if (!(cur->type() == delim_s && cur->delim_type() == d_sccl))
		throw Errmes(e_exp_del,d_sccl);
	next();
	if (!(cur->type() == key_s && cur->kwd_type() == k_dlt))
		throw Errmes(e_exp_kwd,k_dlt);
	next();
	block();
	newval3 = new KurwaJump(bool_point);
	kurwalist.addelem(newval3);
	newval2->dest() = kurwalist.addelem(newval);
}

void Syn_anal::delat()
{
	int jump_point;
	KurwaNULL* newval=new KurwaNULL;
	KurwaJumpFalse* newval2;
	KurwaNeg* newval3=new KurwaNeg;

	jump_point = kurwalist.addelem(newval);

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

	kurwalist.addelem(newval3);
	newval2 = new KurwaJumpFalse(jump_point);
	kurwalist.addelem(newval2);
}


