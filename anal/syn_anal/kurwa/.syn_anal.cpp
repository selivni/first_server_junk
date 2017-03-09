//TODO-LIST:
//Change the nametable in the Syn_anal cuz its fucking not good for it, and
//the Entire "list" struct is a piece of shit, you have to admit that.
//Insert the list creation into the Syn_anal, for this you need to understand
//what will go on in the Kurwa-type classes.
//Make a stack for operations (one more class???) and make operations
//processor and stuff and shit
#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <stdio.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <time.h>
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int size_of_str (const char *string)
{
	int count=0;
	while (string[count] != '\0' && (count++) <= 512)
		{}
	if (string[count] == '\0')
		return count;
	return 0;
}

// There is no reasons to use this class here, but I wrote it because I can
// and because I thought there would be some. sry

class SuperString
{
	static const int stdsize=16;
	char* string;
	int size;
	int pointer;
public:
	SuperString (int i=16)
	{
		pointer = 0;
		string = new char[i];
		size = i;
		string[0] = '\0';
	}
/*
	SuperString () 
	{
		pointer = 0;
		size = stdsize;
		string = new char[size];
		string[0] = '\0';
	}

	SuperString (int i) 
	{
		pointer = 0;
		string = new char[i];
		size = i;
		string[0] = '\0';
	}
*/	
	SuperString (const char* a)
	{
		int i=0;
		pointer = 0;
		size = stdsize;
		string = new char[size];
		do
		{
			(*this)[i] = a[i];
		} while (a[i++] != '\0');
		pointer = i-1;
	}

	SuperString(const SuperString& old)
	{
		size = old.size;
		pointer = old.pointer;
		string = new char[size];
		for (int i=0; i<=pointer; i++)
			string[i] = old.string[i];
	}

	~SuperString () {delete[] (string);}

/*	SuperString (SuperString& old)
	{
		size = old.size;
		pointer = old.pointer;
		string = new char[size];
		for (int i=0; i<=pointer; i++)
			string[i] = old.string[i];
	}
*/

	char* conv_char()
	{
		int i=0; 
		char* out = new char[size];
		for (i=0; (*this)[i] != '\0'; i++)
			out[i] = (*this)[i];
		out[i] = '\0';
		return out;
	}

	SuperString cutword (int k=1)
	{
		int i=0,j=0,flg=0;
		SuperString result;
		if (k == -1)
		{
			flg = 1;
			k = 1;
		}
		while ((*this)[i] == ' ')
			i++;
		while (k != 0 && (*this)[i] != '\n')
		{
			if ((*this)[i] == ' ')
				k--;
			if (k != 0)
				result[j++] = (*this)[i++];
		}
		result[j] = '\0';
		if (i <= -1)
			exit(1);
		if (flg == 0)
		{
			int t=0;
			do
			{
				(*this)[t] = (*this)[t+i];
			} while ((*this)[(t++)+i] != '\0');
			pointer = t-1;
		}
		return result;
	}

	
	SuperString& operator=(SuperString old)
	{
		size = old.size;
		pointer = old.pointer;
		delete[] string;
		string = new char[size];
		for (int i=0; i<=pointer; i++)
			string[i] = old.string[i];
		return (*this);
	}

	char& operator[] (int i)
	{
		if (i > pointer)
			pointer = i;
		if (i >= size)
		{
			char* newstr;
			int olds=size;
			while (i >= size)
				size += stdsize;
			newstr = new char[size];
			for (int i=0; i<olds; i++)
				newstr[i] = string[i];
			delete[] (string);
			string = newstr;
		}	
		return string[i];
	}

	int print_s(int i=-1)
	{
		int count=-1;
		if (i == -1)
		{
			count = printf("%s",string);
		} else
		{
			count++;
			for (int k=0; k<i; k++)
			{
				putchar(string[i]);
				count++;
			}
		}
		return count;
	}
};

//SuperString botname = "Botbot_v0.04_Spammer_Edition";

int atoi_ss(SuperString num)
{
//fkthmsklgmdgklmdfklmgsldfkmglsdfmgklsmfd
	int i;
	char *s;
	i = atoi(s = num.conv_char());
	delete[] s;
	return i;
}


class Player
{
	friend class Bot;
	bool init;
	SuperString nickname_p;
	SuperString seek_p;
	int money;
	int raw;
	int product_p;
	int build;
	int ready;
	int dead_p;
public:
	Player* next;
	Player()
	{
		init = 0;
		next = NULL;
	}

	Player(SuperString nick)
	{
		char* string = new char[128];
		char* s;
		init = 0;
		nickname_p = nick;
		next = NULL;
		dead_p = 0;
		sprintf(string,"%s has quit the game!\n",
				  s = (nickname_p.conv_char()));
		seek_p = string;
		seek_p.print_s();
		delete[] string;
	}

	SuperString& seek()
	{
		return seek_p;
	}

	int product()
	{
		return product_p;
	}

	void fill(SuperString info)
	{
		SuperString num;
		init=1;

		money = atoi_ss(info.cutword());
		raw = atoi_ss(info.cutword());
		product_p = atoi_ss(info.cutword());
		build = atoi_ss(info.cutword());
		ready = atoi_ss(info.cutword());
	}
	
//bacuz i can
	SuperString& nickname() 
	{
		return nickname_p;
	}
//bacuz u cant do it anotha way actually ((((((((((((
	int& dead()
	{
		return dead_p;
	}

	Player* moar(SuperString nick="")
	{
		if (nick[0] != '\0')
			next = new Player(nick);
		else
			next = new Player;
		return next;
	}
};


class Bot
{
	Player *plrs;
	Player* me;
	int market[4];
	int count;
	int fd;
	fd_set readset,readset_c;
	bool startflag;
	SuperString botname;
public:
	Bot();
	void connect_to(char **argv);
	void readln(SuperString &string) const;
	void outinfo() const;
	void cmd_proc();
	void getstr(SuperString &string);
	void get_plr_cnt();
	void initplayers();
	void seek_for(SuperString &seek);
	void getinfo();
	void getmarket();
	void doyourthing() const;
	void chat_spam(SuperString gachi);
	void anal_dead();
	void prod(int i);
	void buy(int n, int price);
	void sell(int n, int price);
	void turn();
	int cmp_str(SuperString &A,SuperString &B) const;
};


Bot::Bot()
{
	FD_ZERO(&readset_c);
	plrs = NULL;
	count = fd = 0;
	FD_SET(0,&readset_c);
	startflag = false;
}


void Bot::readln(SuperString& string) const
{
	char c;
	int i=0;
	do
	{
		string[i++] = c = getchar();
		if (c == EOF)
		{
			printf("To quit the program, type \"exit\" or \"quit\"\n");
			string[--i] = '$';
		}
	}
	while ('\n' != c && EOF != c);
	string[i] = '\0';
}


void Bot::outinfo() const
{
	Player* plr = plrs;

	printf("I do not give a fork about ophormlieiniye, because\n"
			 "its just an otladochnaya information. So here you go:\n");
	printf("MARKET: %d %d %d %d\n",market[0],market[1],market[2],market[3]);
	printf("PLAYERS:\n");
	while (plr != NULL)
	{
		printf("%s:\n%d %d %d %d %d %d\n",
				 (plr->nickname()).conv_char(),plr->money,
				 plr->raw,plr->product(),plr->build,plr->ready,plr->dead());
		plr = plr->next;
	}
}


void Bot::chat_spam(SuperString gachi)
{
	int i=1;
	char* string = new char [2048];
	sprintf(string,"chat %s",gachi.conv_char());
	while (string[i++] != '\0')
		{}
	string[(--i)++] = '\r';
	string[i++] = '\n';
	string[i++] = '\0';
	for (i=1; i<50; i++)
	{
		write(fd,string,size_of_str(string));
		usleep(10000);
	}
	delete[] string;
}


//COMMANDS
SuperString c_quit = "quit\n";
SuperString c_exit = "exit\n";
SuperString c_info = "info\n";
SuperString c_spam = "chat";

void Bot::cmd_proc() 
{
	SuperString inc,word;
	readln(inc);
	if (cmp_str(inc,c_exit) || cmp_str(inc,c_quit))
		exit(0);
	if (cmp_str(inc,c_info))
		outinfo();
	if (cmp_str(word = inc.cutword(),c_spam))
		chat_spam(inc);
}


void Bot::getstr(SuperString &gachi) 
{
	Player* plr = plrs;
	int i=0;
	char c;
	do
	{
		readset = readset_c;
		select(fd+1,&readset,NULL,NULL,NULL);
		if (FD_ISSET(0,&readset))
		{
			cmd_proc();
			c = '\0';
		} else
		{
			if (0 == read(fd,&c,1))
			{
				printf("Connection closed by foreign host.\n");
				exit(1);
			}
			if (c != '\0')
				gachi[i++] = c;
		}
	} while (c != '\n');
/*
	kandelyabre incoming
	while ((2+read(fd,&c,1)) && c != '\n') 
		string[i++] = c;
*/
	gachi[i] = '\0';
	if (startflag)
	{
		while (plr != NULL)
		{
			if (cmp_str(plr->seek(),gachi))
			{
				printf("Got one!\n");
				plr->dead() = 1;
				break;
			}
			plr = plr->next;
		}
	}
	gachi.print_s();
}

/*
void Bot::get_str(SuperString& gachi)
{
	char c;
	int counter=0;
	do
	{
		int err;
		err = read(fd,&c,1);
		if (err == 0)
		{
			printf("Connection with server was suddenly lost.\n"
					 "That doesn't mean that it has some bugs\n");
			exit(-3);
		}
		gachi[counter] = c;
	} while (c != '\n');
}
*/

void Bot::seek_for(SuperString &seek) 
{
	SuperString inc,word;
	do
	{
		getstr(inc);
	} while (!cmp_str(inc,seek));
}


void Bot::get_plr_cnt()
{
	SuperString inc,word;
	do 
	{
		getstr(inc);
		word = inc.cutword();
	} while (!cmp_str(word,botname));
	word = inc.cutword(4);
// Just in case I would change sth
	do
		word = inc.cutword();
	while (inc[0] != '\n');
//	Just wanna check how the OS will react about this shield
	char* num = new char[64];
	int i=0,k=0;

	while (word[i++] != '/')
		{}
	while (word[i] != '\0' && word[i] != ' ')
		num[k++] = word[i++];
	num[k] = '\0';
	count = atoi(num);
	delete[] num;
}
//conv_char
void Bot::getinfo()
{
	char *cmd = new char[64];
	char *s;
	SuperString inc;
	SuperString seek = "Money   Raw     Product Build   Ready   \n";
	Player* plr=plrs;
// Why did I ever need that player count?
// ...
	if (startflag)
		seek_for(seek);
	while (plr != NULL)
	{
		if (plr->dead() != 1)
		{
			sprintf(cmd,"status %s\r\n",s = (plr->nickname()).conv_char());
			write(fd,cmd,size_of_str(cmd));
			delete[] s;
			seek_for(seek);
			getstr(inc);
			plr->fill(inc);
		}
		plr = plr->next;
	}
	startflag = true;
	delete[] cmd;
}


void Bot::initplayers()
{
	Player *newb=NULL;
	SuperString inc;
	SuperString seek = 
		"there is no help for you provided. Except the 'help' command.\n";
	seek_for(seek);
	write(fd,"list\r\n",6);
	seek = "Here is the list of the players:\n";
	seek_for(seek);
	for (int k=0; k<count; k++)
	{
		SuperString word;
		getstr(inc);
		word = inc.cutword();
//		Not a mistake, just for future if 
//		I'm gonna somehow outline myself from
//		other players
		if (newb == NULL)
		{
			plrs = new Player(word);
			newb = plrs;
		}
		else
			newb = newb->moar(word);
		if (inc[0] != '\n')
			me = newb;
	}
	getinfo();
}


int Bot::cmp_str(SuperString &A,SuperString &B) const
{
	int flag=1,i=0;
	while (A[i] == B[i] && A[i] != '\0' && B[i] != '\0')
		i++;
	if (A[i] != B[i])
		flag = 0;
	return flag;
}


void Bot::connect_to(char** argv)
{
	struct sockaddr_in /*me,*/ serv;
	char *string,*tmp;
	botname = argv[3];
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
/*	me.sin_family = AF_INET;
	me.sin_port = htons(atoi(argv[2])-1);
	me.sin_addr.s_addr = INADDR_ANY;
	while (me.sin_port < 10000 && -1 == 
			 bind(fd,(struct sockaddr *) &me, sizeof(me)))
		me.sin_port++;
	if (me.sin_port == 10000)
		exit(1);*/
	serv.sin_family = AF_INET;
	serv.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1],(in_addr *) &serv.sin_addr.s_addr);
	if (-1 == connect(fd,(struct sockaddr *) &serv,sizeof(serv)))
	{
		printf("Failed to connect to server by port %s.\n",argv[2]);
		exit(1);
	}
	string = new char[64];
	FD_SET(fd,&readset_c);
	sleep(5);
	sprintf(string,"%s\r\n",tmp = botname.conv_char());
	delete[] tmp;
	write(fd,string,size_of_str(string));
	delete[] string;
}


void Bot::getmarket() 
{
	SuperString seek = "MARKET STATUS      NUM   PRICE\n";
	SuperString inc,word;
	int j=0;
	write(fd,"market\r\n",8);
	seek_for(seek);
	for (int i=0; i<2; i++)
	{
		getstr(inc);
		inc.cutword(2);
		word = inc.cutword();
		market[j++] = atoi_ss(word);
		word = inc.cutword();
		market[j++] = atoi_ss(word);
	}
}


void Bot::doyourthing() const
{
	
	char* cmd = new char[128];
	int raw = 4-(me->raw);
	if (me->dead() != 1)
	{
		if (raw < 0)
			raw = 0;
		sprintf(cmd,"buy %d %d\r\nsell %d %d \r\n",raw,
				market[3],me->product(),market[1]);
		write(fd,cmd,size_of_str(cmd));
		sprintf(cmd,"prod 2\r\n");
		write(fd,cmd,8);
		sprintf(cmd,"end\r\n");
		write(fd,cmd,5);
	}
	delete[] cmd;
}


void Bot::anal_dead()
{
	Player* plr;
	SuperString word,inc,seek="MARKET";
	do
	{
		plr = plrs;
		getstr(inc);
		word = inc.cutword();
		while (plr != NULL)
		{
			if (cmp_str(word,plr->nickname()))
				plr->dead() = 1;
			plr = plr->next;
		}
// IF THE PLAYER IS NAMED "MARKET", THIS ALL WILL BE INCORRECT
// I will probably fix this bug later
	} while (!cmp_str(word,seek));
}

void Bot::prod(int i)
{
	char* cmd = new char[128];
	sprintf(cmd,"prod %d\r\n",i);
	write(fd,cmd,size_of_str(cmd));
	delete[] cmd;
}
void Bot::buy(int n, int price)
{
	char* cmd = new char[128];
	sprintf(cmd,"buy %d %d\r\n",n,price);
	write(fd,cmd,size_of_str(cmd));
	delete[] cmd;
}
void Bot::sell(int n, int price)
{
	char* cmd = new char[128];
	sprintf(cmd,"sell %d %d\r\n",n,price);
	write(fd,cmd,size_of_str(cmd));
	delete[] cmd;
}
void Bot::turn()
{
	char* cmd = new char[128];
	sprintf(cmd,"end\r\n");
	write(fd,cmd,size_of_str(cmd));
	delete[] cmd;
}

/*int main(int argc, char** argv)
{
	const int bot_did_not_win_yet = 1;
//	char str[]="THIS IS A TEST STRING!";
	if (argc < 4)
	{
		printf("nu tam ./SUPER 0 4567 Botbot v0.0x Spammer Edition\n");
		exit(1);
	}
	SuperString s,word;
	Bot bot;
	bot.connect_to(argv);
	bot.get_plr_cnt();
	bot.initplayers();
	while (bot_did_not_win_yet)
	{
		SuperString seek = 
	"*****************************END OF AUCTION*************************\n";
		bot.getmarket();
		bot.doyourthing();
		bot.outinfo();
		bot.seek_for(seek);
		bot.anal_dead();
		bot.getinfo();
	}
// if this works, im gonna eat my studaque
	s=str;
	s[21] = '!';
	s[22] = '\n';
	s[23] = '\0';
	while (s[0] != '\n')
//	for (int k=1; k<=10; k++)
	{
		s.print_s();
		word = s.cutword();
		word.print_s();
		putchar('\n');
	}
	*/
   /*	for (int i=0; i<80; i++)
	{
		s[i] = '0'+i%10;
	}
	s[80] = '\0';
	s.print_s();
	return 0;
}*/
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
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
	k_print=20
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

const int kwordssize=21;



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
		"print"
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
			"to"
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
	int type() {return p_type;}
	MemTable(int a, int b=1);
	void addelem(int a,int b=1);
	MemTable* operator[](int i);
	int size_of();
	int& get_int(int i);
	float& get_float(int i);
	char* get_string(int i);
};

MemTable::MemTable(int a, int b): p_type(a),p_next(0)
{
	p_size = b;
	switch(a)
	{
	case t_tselle:
		p_val = new int[p_size];
		break;
	case t_netselle:
		p_val = new float[p_size];
		break;
	case t_string:
		p_val = new char[p_size*256];
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
	if (i < p_size)
		return *((int*)(p_val+i*sizeof(int)));
	else
		throw IntErr(i_arr_break);
}

float& MemTable::get_float(int i)
{
	if (i < p_size)
		return *((float*)(p_val+i*sizeof(float)));
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

class KurwaConst;
const int stack_size=1048576;
class Stack
{
	KurwaConst* memory[stack_size];
	int i;
public:
	Stack(): i(0)
		{}
	void push(KurwaConst* val)
	{
		memory[i++] = val;
	}
	KurwaConst* pop()
	{
		return memory[--i];
	}
};



IntErr::IntErr(int t): type(t)
	{}

class KurwaElem
{
public:
	static Stack* stack;
	KurwaElem(KurwaElem& a)
		{}
	KurwaElem()
	{
		if (stack == 0)
			stack = new Stack;
	}
	virtual void eval()
		{}
};
Stack* KurwaElem::stack=0;

class KurwaNULL:public KurwaElem
{
	void eval()
		{}
};

//***************************************************************

class KurwaConst: public KurwaElem
{

public:
	void eval()
	{
		stack->push(this);
	}
};

class KurwaNum: public KurwaConst
{
	int value;
public:
	KurwaNum(int i): value(i)
		{}
	int& val() {return value;}
};

class KurwaFNum: public KurwaConst
{
	float value;
public:
	KurwaFNum(float j): value(j)
		{}
	float val() {return value;}
};

class KurwaVarAddr: public KurwaConst
{
public:
	MemTable* val;
	int quant;
	KurwaVarAddr(MemTable* value, int q):val(value),quant(q)
		{}
};

class KurwaString: public KurwaConst
{
	char* value;
public:
	KurwaString(char* a):value(a)
		{}
	char* val() {return value;}
};

//***************************************************************

class KurwaFunc: public KurwaElem
{
	
};

class KurwaPrint: public KurwaFunc
{
public:
	void eval()
	{
		KurwaVarAddr* addr;
		int type;
		addr = dynamic_cast <KurwaVarAddr*> (stack->pop());
		if (addr == 0)
			throw IntErr(i_cast_err);
		type = addr->val->type();
//		printf("PROGRAM OUTPUT: ");
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
};

class KurwaGetVal: public KurwaFunc
{
public:
	void eval()
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
};

class KurwaAss: public KurwaFunc
{
	void eval()
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
			break;
		default:
			throw IntErr(i_wrong_type);
		}
	}
};

class KurwaNeg: public KurwaFunc
{
	void eval()
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
};

class KurwaConj: public KurwaFunc
{
	void eval()
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
};

class KurwaDisj: public KurwaFunc
{
	void eval()
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
};

class KurwaComp: public KurwaFunc
{
	int comp_type;
public:
	KurwaComp(int i):comp_type(i)
		{}
	void eval()
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
};

class KurwaMath: public KurwaFunc
{
	int math_type;
public:
	KurwaMath(int i): math_type(i)
		{}
	float float_oper(float a, float b);
	int int_oper(int a, int b);
	void eval()
	{
		int i1,i2;
		float d1,d2;
		bool flag=false;
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
			flag = true;
			if ((f1 = dynamic_cast <KurwaFNum*> (a)) == 0)
				throw IntErr(i_cast_err);
			d1 = f1->val();
			i1 = d1;
		} else
		{
			i1 = n1->val();
			if ((n2 = dynamic_cast <KurwaNum*> (b)) == 0)
			{
				flag = true;
				if ((f2 = dynamic_cast <KurwaFNum*> (b)) == 0)
					throw IntErr(i_cast_err);
				d2 = f2->val();
				i2 = d2;
			} else
				i2 = n2->val();
		}
		if (flag)
		{
			d1 = i1;
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
};

float KurwaMath::float_oper(float a, float b)
{
	if (math_type == d_plus)
		return a+b;
	if (math_type == d_minus)
		return a-b;
	if (math_type == d_div)
		return a/b;
	if (math_type == d_mult)
		return a*b;
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

class KurwaServ: public KurwaFunc
{
	Bot bot;
	int cmd;
public:
	KurwaServ(int a): cmd(a)
		{}
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
	void eval()
	{
		char* p1;
		char* p2;
		int i1,i2;
		KurwaString* s1;
		KurwaString* s2;
		KurwaNum* n1;
		KurwaNum* n2;
		switch(cmd)
		{
		case k_con:
			s1 = dynamic_cast <KurwaString*> (stack->pop());
			s2 = dynamic_cast <KurwaString*> (stack->pop());
			if (s1 == 0 || s2 == 0)
				throw IntErr(i_cast_err);
			p1 = s1->val();
			p2 = s2->val();
			f_con(p1,p2);
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
			f_buy(i1,i2);
			break;
		case k_sell:
			n1 = dynamic_cast <KurwaNum*> (stack->pop());
			n2 = dynamic_cast <KurwaNum*> (stack->pop());
			if (n1 == 0 || n2 == 0)
				throw IntErr(i_cast_err);
			i1 = n1->val();
			i2 = n2->val();
			f_sell(i1,i2);
			break;
		case k_turn:
			f_turn();
			break;
		default:
			throw IntErr(i_wrong_command);
		}
	}
};

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
	bot.connect_to(fakeargv);
}

void KurwaServ::f_gpc()
{
	bot.get_plr_cnt();
}
void KurwaServ::f_init()
{
	bot.initplayers();
}
void KurwaServ::f_gmt()
{
	bot.getmarket();
}
void KurwaServ::f_bse()
{
	bot.doyourthing();
}
void KurwaServ::f_anal()
{
	SuperString seek = 
"*****************************END OF AUCTION*************************\n";
	bot.seek_for(seek);
	bot.anal_dead();
}
void KurwaServ::f_info()
{
	bot.getinfo();
}
void KurwaServ::f_prod(int i)
{
	bot.prod(i);
}
void KurwaServ::f_buy(int n, int price)
{
	bot.buy(n,price);
}
void KurwaServ::f_sell(int n, int price)
{
	bot.sell(n,price);
}
void KurwaServ::f_turn()
{
	bot.turn();
}

//***************************************************************

class KurwaJumps: public KurwaElem
{
public:
	static int k_current;
};

int KurwaJumps::k_current = 0;

class KurwaJump: public KurwaJumps
{
	int p_dest;
public:
	int& dest() {return p_dest;}
	KurwaJump(int dest):p_dest(dest)
		{}
	void eval()
	{
		k_current = p_dest-1;
	}
};

class KurwaJumpFalse: public KurwaJumps
{
	int p_dest;
public:
	int& dest() {return p_dest;}
	KurwaJumpFalse(int dest):p_dest(dest)
		{}
	void eval()
	{
		KurwaNum* n;
		n = dynamic_cast <KurwaNum*> (stack->pop());
		if (n == 0)
			throw IntErr(i_cast_err);
		if (n->val() == 0)
			k_current = p_dest-1;
	}
};

class KurwaEND: public KurwaJumps
{
	void eval()
	{
		throw IntErr(i_koniets);
	}
};
//***************************************************************
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
class KurwaList
{
	KurwaElem* a;
	KurwaList* p_next;
	static int p_cur;
	int p_num;
	static KurwaList* p_last;
	void last_addelem(KurwaElem* newval);
public:
	KurwaElem* val() {return a;}
	KurwaList(KurwaElem* newval);
	int addelem(KurwaElem* newval);
	KurwaList* next() {return p_next;}
	KurwaList* operator[] (int i);
	int num() {return p_num;}
	const char* type() {return typeid(*a).name();}
};

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

class TRNameTable
{
	char* p_name;
	bool right_name(char* val);
	TRNameTable* p_next;
	static TRNameTable* p_last;
	void last_addelem(char* newval);
public:
	TRNameTable(char* value);
	char* name() {return p_name;}
	void addelem(lexem* lex);
	TRNameTable* next() {return p_next;}
	int is_in(lexem* lex);
};

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
	bool isalright(struct list *lul)
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
	void printlist();
	void interprer();
};

void Syn_anal::interprer()
{
	KurwaList* cur=kurwalist.next();
	int my_current = 1;
	KurwaJumps::k_current = 1;
	try
	{
		while (cur != 0)
		{
//			printf("%d\n",cur->num());
			if (KurwaJumps::k_current == my_current)
			{
				cur->val()->eval();
			}
			else
			{

				cur = kurwalist[KurwaJumps::k_current];
				my_current = KurwaJumps::k_current;
				cur->val()->eval();
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
	int num,quant=0;
	if (cur->type() == var_s)
	{

		if (-1 == (num = nametable.is_in(cur->val)))
			throw Errmes(e_no_name);

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
// E_EXP_NUM!!!!!!!!!!!!!!!!!!!!!!1
				throw Errmes(e_exp_numovar);
			if (cur->type() == delim_s && cur->delim_type() == d_kscl)
				next();
			else throw Errmes(e_exp_del,d_oper);
		}
		newval = new KurwaVarAddr(memtable[num],quant);
		kurwalist.addelem(newval);
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
	next();
}
void Syn_anal::f_buy()
{
	KurwaServ* newval;
	next();
	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);

	next();

	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);

	next();
	newval = new KurwaServ(k_buy);
	kurwalist.addelem(newval);
	next();
}
void Syn_anal::f_sell()
{
	KurwaServ* newval;
	next();
	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
	} else
	if (cur->type() == var_s)
	{
		KurwaGetVal* val = new KurwaGetVal;
		name_check();
		kurwalist.addelem(val);
	} else
		throw Errmes(e_exp_numovar);

	next();

	if (cur->type() == num_s)
	{
		KurwaNum* val1=new KurwaNum(atoi(cur->val->string()));
		kurwalist.addelem(val1);
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
	next();
}
void Syn_anal::f_turn()
{
	KurwaServ* newval = new KurwaServ(k_turn);
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
	compiler.printlist();
	compiler.interprer();

	return 0;
}
