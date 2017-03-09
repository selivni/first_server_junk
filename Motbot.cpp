#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
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
			{i++;}
		while (k != 0 && (*this)[i] != '\n')
		{
			if ((*this)[i] == ' ')
				k--;
			if (k != 0)
				result[j++] = (*this)[i++];
		}
		result[j] = '\0';
		if (i <= -1)
			exit(-2);
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

SuperString botname = "Botbot_v0.04_Spammer_Edition";

int atoi_ss(SuperString num)
{
//fkthmsklgmdgklmdfklmgsldfkmglsdfmgklsmfd
	return atoi(num.conv_char());
}


class Player
{
	friend class Bot;
	bool init;
	SuperString nickname_p;
	SuperString seek_p;
	int money;
	int raw;
	int product;
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
		init = 0;
		nickname_p = nick;
		next = NULL;
		dead_p = 0;
		sprintf(string,"%s has quit the game!\n",nickname_p.conv_char());
		seek_p = string;
		seek_p.print_s();
		delete[] string;
	}

	SuperString& seek()
	{
		return seek_p;
	}

	void fill(SuperString info)
	{
		SuperString num;
		init=1;
		money = atoi_ss(info.cutword());
		raw = atoi_ss(info.cutword());
		product = atoi_ss(info.cutword());
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

	Player* moar(SuperString nick="\0")
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
	int market[4];
	int count;
	int fd;
	fd_set readset,readset_c;
	bool startflag;
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
	while ('\n' != (c = getchar()))
		string[i++] = c;
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
		printf("%s:\n%d %d %d %d %d %d\n",(plr->nickname()).conv_char(),plr->money,
				 plr->raw,plr->product,plr->build,plr->ready,plr->dead());
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
SuperString c_quit = "quit";
SuperString c_exit = "exit";
SuperString c_info = "info";
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
			sprintf(cmd,"status %s\r\n",(plr->nickname()).conv_char());
			write(fd,cmd,size_of_str(cmd));
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
	for (int k=0; k<4; k++)
	{
		SuperString word;
		getstr(inc);
		word = inc.cutword();
//		Not a mistake, just for future if I'm gonna somehow outline myself from
//		other players
		if (inc[0] != '\n');
		if (newb == NULL)
		{
			plrs = new Player(word);
			newb = plrs;
		}
		else
			newb = newb->moar(word);
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
	struct sockaddr_in me, serv;
	char *string,*tmp;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	me.sin_family = AF_INET;
	me.sin_port = htons(atoi(argv[2])-1);
	me.sin_addr.s_addr = INADDR_ANY;
	while (me.sin_port < 10000 && -1 == bind(fd,(struct sockaddr *) &me, sizeof(me)))
		me.sin_port++;
	if (me.sin_port == 10000)
		{exit(2);}
	serv.sin_family = AF_INET;
	serv.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1],(in_addr *) &serv.sin_addr.s_addr);
	if (-1 == connect(fd,(struct sockaddr *) &serv,sizeof(serv)))
		{}
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
	sprintf(cmd,"buy %d %d\r\nsell %d %d \r\n",2,market[3],2,market[1]);
	write(fd,cmd,size_of_str(cmd));
	sprintf(cmd,"prod 2\r\n");
	write(fd,cmd,8);
	sprintf(cmd,"end\r\n");
	write(fd,cmd,5);
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


int main(int argc, char** argv)
{
	const int bot_did_not_win_yet = 1;
//	char str[]="THIS IS A TEST STRING!";
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
		bot.seek_for(seek);
		bot.anal_dead();
		sleep(1);
		bot.getinfo();
	}
// if this works, im gonna eat my studaque
/*	s=str;
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
	s.print_s();*/
	return 0;
}
