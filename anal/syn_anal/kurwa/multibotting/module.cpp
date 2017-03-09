#include "module.hpp"
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
int size_of_str (const char *string)
{
	int count=0;
	while (string[count] != '\0' && (count++) <= 512)
		{}
	if (string[count] == '\0')
		return count;
	return 0;
}

SuperString::SuperString (int i)
{
	pointer = 0;
	string = new char[i];
	size = i;
	string[0] = '\0';
}

SuperString::SuperString (const char* a)
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

SuperString::SuperString(const SuperString& old)
{
	size = old.size;
	pointer = old.pointer;
	string = new char[size];
	for (int i=0; i<=pointer; i++)
		string[i] = old.string[i];
}

SuperString::~SuperString () {delete[] (string);}

char* SuperString::conv_char()
{
	int i=0; 
	char* out = new char[size];
	for (i=0; (*this)[i] != '\0'; i++)
		out[i] = (*this)[i];
	out[i] = '\0';
	return out;
}

SuperString SuperString::cutword (int k)
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


SuperString& SuperString::operator=(SuperString old)
{
	size = old.size;
	pointer = old.pointer;
	delete[] string;
	string = new char[size];
	for (int i=0; i<=pointer; i++)
		string[i] = old.string[i];
	return (*this);
}

char& SuperString::operator[] (int i)
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

int SuperString::print_s(int i)
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

//SuperString botname = "Botbot_v0.04_Spammer_Edition";

int atoi_ss(SuperString num)
{
	int i;
	char *s;
	i = atoi(s = num.conv_char());
	delete[] s;
	return i;
}

Player::Player()
{
	init = 0;
	next = NULL;
}

Player::Player(SuperString nick)
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

SuperString& Player::seek()
{
	return seek_p;
}

int Player::product()
{
	return product_p;
}

void Player::fill(SuperString info)
{
	SuperString num;
	init=1;

	money = atoi_ss(info.cutword());
	raw = atoi_ss(info.cutword());
	product_p = atoi_ss(info.cutword());
	build = atoi_ss(info.cutword());
	ready = atoi_ss(info.cutword());
}

SuperString& Player::nickname() 
{
	return nickname_p;
}

int& Player::dead()
{
	return dead_p;
}

Player* Player::moar(SuperString nick)
{
	if (nick[0] != '\0')
		next = new Player(nick);
	else
		next = new Player;
	return next;
}

Bot::Bot()
{
	FD_ZERO(&readset_c);
	plrs = NULL;
	count = 0;
	fd = 0;
//	FD_SET(0,&readset_c);
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
//		select(fd+1,&readset,NULL,NULL,NULL);
//		if (FD_ISSET(0,&readset))
//		{
//			cmd_proc();
//			c = '\0';
//		} else
//		{
			if (0 == read(fd,&c,1))
			{
				printf("Connection closed by foreign host.\n");
				exit(1);
			}
			if (c != '\0')
				gachi[i++] = c;
//		}
	} while (c != '\n');
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
void Bot::getinfo()
{
	char *cmd = new char[64];
	char *s;
	SuperString inc;
	SuperString seek = "Money   Raw     Product Build   Ready   \n";
	Player* plr=plrs;
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

int Bot::player_count()
	{return count-1;}

int* Bot::player_info(int num)
{
	int* info = new int[6];
	Player* plr = plrs;
	while (plr != 0)
	{
		if (plr == me)
		{
			plr = plr->next;
			continue;
		}
		if (--num == 0)
		{
			info[0] = plr->money;
			info[1] = plr->raw;
			info[2] = plr->product_p;
			info[3] = plr->build;
			info[4] = plr->ready;
			info[5] = plr->dead_p;
			return info;
		}
		plr = plr->next;
	}
	return 0;
}

int* Bot::my_info()
{
	int* info = new int[6];
	info[0] = me->money;
	info[1] = me->raw;
	info[2] = me->product_p;
	info[3] = me->build;
	info[4] = me->ready;
	info[5] = me->dead_p;
	return info;
}

int* Bot::getauc(int num)
{
	int* info = new int[6];
	Player* plr = plrs;
	while (plr != 0)
	{
		if (plr == me)
		{
			plr = plr->next;
			continue;
		}
		if (--num == 0)
		{
			for (int i=0; i<6; i++)
				info[i] = plr->auction[i];
			return info;
		}
		plr = plr->next;
	}
	return 0;
}

int* Bot::market_info()
{
	int* val = new int[4];
	for (int i=0; i<4; i++)
		val[i] = market[i];
	return val;
}

void Bot::analauc()
{
	SuperString inc,word;
	Player* plr;
	char* mlkd;
	SuperString seek1="NICKNAME                           NUM   PRICE\n";
	max_buy = 0;
	min_sell = 10000;
	plr = plrs;
	while (plr)
	{
		plr->auction[0] = 0;
		plr->auction[5] = 0;
		plr = plr->next;
	}
	seek_for(seek1);
	getstr(inc);
	while (inc[0] != '*' && 
		   inc[0] != ' ' && 
		   inc[0] != '\n' && 
		   inc[0] != '\r' && 
		   inc[0] != '\0')
	{
		plr = plrs;
		word = inc.cutword();
		while (!cmp_str(word,plr->nickname()) && plr != 0)
			plr = plr->next;
		if (!plr)
		{
			printf("Something strange happened during the auction...\n");
			exit(1);
		}
		plr->auction[0] = 1;
		word = inc.cutword();
		plr->auction[1] = atoi(mlkd = word.conv_char());
		word = inc.cutword();
		delete[] mlkd;
		plr->auction[2] = atoi(mlkd = word.conv_char());
		delete[] mlkd;
		if (plr->auction[2] < min_sell)
			min_sell = plr->auction[2];
		getstr(inc);
	}
	seek_for(seek1);
	getstr(inc);
	while (inc[0] != '*' && 
		   inc[0] != ' ' && 
		   inc[0] != '\n' && 
		   inc[0] != '\r' && 
		   inc[0] != '\0')
	{
		plr = plrs;
		word = inc.cutword();
		while (plr != 0 && !cmp_str(word,plr->nickname()))
			plr = plr->next;
		if (!plr)
		{
			printf("Something strange happened during the auction...\n");
			exit(1);
		}
		plr->auction[5] = 1;
		word = inc.cutword();
		plr->auction[3] = atoi(mlkd = word.conv_char());
		word = inc.cutword();
		delete[] mlkd;
		plr->auction[4] = atoi(mlkd = word.conv_char());
		delete[] mlkd;
		if (plr->auction[4] > max_buy)
			max_buy = plr->auction[4];
		getstr(inc);
	}
}

int* Bot::recommend(int* last_market)
{
	int* result = new int[2];
	printf("Got %d %d %d %d\n",last_market[0],last_market[1]
		   ,last_market[2],last_market[3]);
	printf("My info: market %d %d, %d %d",
		   market[1],market[3],min_sell,max_buy);
	result[0] = trunc(market[1]*(float(min_sell)/last_market[1]))-1;
	result[1] = trunc(market[3]*(float(max_buy)/last_market[3]))+1;
	printf("Returning %d %d\n",result[0], result[1]);
	return result;
}

void Bot::build()
{
	char command[8]="build\r\n";
	write(fd,command,size_of_str(command));
}
