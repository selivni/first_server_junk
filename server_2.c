#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <signal.h>
#include <time.h>

int l_socket;
/*pls no kill me for this*/

struct list
{
	int fd;
	char *nickname;
	char *in_buffer;
	int bufsize;
	int buf_pointer;
	int raw;
	int prod;
	int fab;
	int cash;
	int end_turn;
	int death;
	int chat_count;
	int chat_ban;
	struct req *requests;
	struct list *next;
};

struct auc
{
	int num;
	int price;
};

struct lv
{
	struct auc buy;
	struct auc sell;
};

struct lv lvl[5];

void init_lvl()
{
	lvl[0].sell.num = 10;
	lvl[0].sell.price = 800;
	lvl[0].buy.num = 30;
	lvl[0].buy.price = 6500;
	lvl[1].sell.num = 15;
	lvl[1].sell.price = 650;
	lvl[1].buy.num = 25;
	lvl[1].buy.price = 6000;
	lvl[2].sell.num = 20;
  	lvl[2].sell.price = 500;
  	lvl[2].buy.num = 20;
  	lvl[2].buy.price = 5500;
  	lvl[3].sell.num = 25;
  	lvl[3].sell.price = 400;
  	lvl[3].buy.num = 15;
  	lvl[3].buy.price = 5000;
  	lvl[4].sell.num = 30;
  	lvl[4].sell.price = 300;
  	lvl[4].buy.num = 10;
  	lvl[4].buy.price = 4500;

}

struct req
{
	struct list *player;
	int prod;
	struct auc buy;
	struct auc sell;
	int build[5];
};

struct adm
{
	int port_number;
	int player_count;
	int nickname_count;
	int player_number;
	int game_status;
	int wake_pipe[2];
	int endturn_count;
	int lvl;
	int player_alive;
};

struct adm admin;

const char s_err_bind[]="Binding fault. Aborting.\n";
const char s_game_full []="The game is full or already started.\n";
const char s_game_not_started []="Please wait until the game starts.\n";
const char s_player_count_1 []="There is ";
const char s_player_count_2 []=" players on the server, and ";
const char s_player_count_3 []=
	" of them have picked their nicknames. Need ";
const char s_player_count_4 []=" to start the game.\n";
const char s_no_player []="There is no player with such nickname.\n";
const char s_got_junk []="Invalid input.\n";
const char s_no_money []="U hef no such mones for that!\n";
const char s_wrong_number []=
	"You cannot produse that quantity of items.\n"
	"Check your free fabriques count or something else (like yur iq lol)\n";
const char s_wrong_buy []=
	"Your cash is too low for that or the bank doesn't want\n"
	"to buy for that price. Sorry (not)\n";
const char s_wrong_sell []=
	"You don't have so much productions or the bank doesn't want\n"
	"to sell for that price.\n";
const char s_over_build []=
	"You can only afford 5 fabriques to get build in one time.\n";
const char s_end_turn []=
	"You successfully ended your turn! Now wait for the others to do it.\n";
const char s_you_dead []=
	"You tried to interact with the world of the living beings.\n"
	"But nothing happened.\n";
const char s_turn_over []=
	"You cant change anything anymore. At least until the turn ends.\n";
const char s_u_slow []=
	"You were too slow to think of a proper nickname. But no worry!\n"
	"We got this for you! Your brand new nickname is ";
const char s_game_starts []=
	"The game starts! Hope you know the rules already, because\n"
	"there is no help for you provided. Except the 'help' command.\n";
const char s_req_name []="Welcome, new player!\n"
	"Please enter your nickname: ";
const char s_limit_reached []=
	"Player capacity reached. Starting 30s countdown...\n";
const char s_plr_lose []="has just got bankrupt!\n";
const char s_new_player []="has joined the game! ";
const char s_plr_quit []="has quit the game!\n";
const char s_ctdn_abort []="Aborting countdown.\n";
const char s_final_countdown []="The final 10s countdown begins!\n";
const char s_auc_begins []=
	"Time passes, month ends.\n"
	"\n"
	"**************************THE AUCTION BEGINS!!!*********************\n";
const char s_auc_buy []=
	"*********************************BUY PART***************************\n"
	"NICKNAME                           NUM   PRICE\n";
const char s_auc_sell []=
	"******************************END OF BUY PART***********************\n"
	"*******************************SELLOUT PART*************************\n"
	"NICKNAME                           NUM   PRICE\n";
const char s_game_over []="has won the game! Congratulations!\n";
const char s_you_loosers []="lol you all lose you loosers\n";
const char s_got_nname []="!\nNow wait for other players to come.\n";
const char s_gotcha []="Your request has been accepted.\n";
const char s_wrong_unbuild []="That build session does not exist.\n";
const char s_ban_spam []=
	"You got parlamentarily banned for spamming too much.\n";
const char s_chat_banned []="You are currently banned from talking.\n";
const char s_out_list []="Here is the list of the players:\n";

const int size_s_player_count = 9+28+43+20;/*WHENSIZEOFISTOOLONGTOTYPE*/

enum
{
	e_limit_reached=1,
	e_plr_lose=2,
	e_new_player=3,
	e_plr_quit=4,
	e_ctdn_abort=5,
	e_init_countdown=6,
	e_final_countdown=7,
	e_auc_begins=8,
	e_buy=9,
	e_sell=10,
	e_game_over=11,
	e_you_loosers=12,

};

int size_of_str(char *string);
void outmarket(struct list *p);

void string_to_everyone(struct list *first, const char* string)
{
	while (first != NULL)
	{
		if (first->nickname == NULL)
		{
			write(first->fd,"\n",1);
		}
		write(first->fd,string,size_of_str((char*)string));
		first = first->next;
	}
}

void write_to_everyone(struct list *first, struct list *p, int status)
{
	char* string;
	switch(status)
	{
	case e_limit_reached:
		string_to_everyone(first,s_limit_reached);
		break;
	case e_plr_lose:
		string = malloc((size_of_str(p->nickname)+10+sizeof(s_plr_lose)));
		sprintf(string,"%s %s",p->nickname,s_plr_lose);
		string_to_everyone(first,string);
		free(string);
		break;
	case e_new_player:
		string = malloc((size_of_str(p->nickname)+20+sizeof(s_new_player)));
		sprintf(string,"%s %s%d/%d\n",p->nickname,s_new_player,
			admin.nickname_count,admin.player_number);
		string_to_everyone(first,string);
		free(string);
		break;
	case e_plr_quit:
		string = malloc((size_of_str(p->nickname)+10+sizeof(s_plr_quit)));
		sprintf(string,"%s %s",p->nickname,s_plr_quit);
		string_to_everyone(first,string);
		free(string);
		break;
	case e_ctdn_abort:
		string_to_everyone(first,s_ctdn_abort);
		break;
	case e_init_countdown:
		string = malloc(10);
		if (((int)p) > 3)
			sprintf(string,"%d\n",(int)p);
		else
			sprintf(string,"%d...\n",(int)p);
		string_to_everyone(first,string);
		free(string);
		break;
	case e_final_countdown:
		string_to_everyone(first,s_final_countdown);
		break;
	case e_auc_begins:
		string_to_everyone(first,s_auc_begins);
		break;
	case e_buy:
		string_to_everyone(first,s_auc_buy);
		break;
	case e_sell:
		string_to_everyone(first,s_auc_sell);
		break;
	case e_game_over:
		string = malloc(70);
		sprintf(string,"%s %s",p->nickname,s_game_over);
		string_to_everyone(first,string);
		free(string);
		break;
	case e_you_loosers:
		string_to_everyone(first,s_you_loosers);
		break;
	default:
		break;
	}
}

void cr_l_sock(int *l_socket)
{
	struct sockaddr_in address;
	*l_socket = socket(AF_INET,SOCK_STREAM,0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(admin.port_number);
	if (-1 == bind(*l_socket,(struct sockaddr*) &address,sizeof(address)))
	{
		printf("%s",s_err_bind);
		exit(-1);
	}
	listen(*l_socket,5);
	printf("Starting to listen on port %d\n",admin.port_number);
}

int face_control(int l_socket, int player_counter)
{
	int newsoc;
	newsoc = accept(l_socket,NULL,NULL);
	if (admin.player_count == admin.player_number || admin.game_status >= 1)
	{
		write(newsoc,&s_game_full,sizeof(s_game_full));
		close(newsoc);
		return -1;
	}
	return newsoc;
}

void new_player(struct list **first, struct list **last, int newsoc)
{
	struct list *new;
	new = malloc(sizeof(struct list));
	new->fd = newsoc;
	new->nickname = NULL;
	new->bufsize = 512;
	new->buf_pointer = 0;
	new->in_buffer = malloc(new->bufsize);
	new->next = NULL;
	new->raw = 4;
	new->prod = 2;
	new->fab = 2;
	new->cash = 10000;
	new->end_turn = 0;
	new->death = 0;
	new->chat_count = 0;
	new->chat_ban = 0;
	new->requests = NULL;
	if (*first == NULL)
	{
		*first = *last = new;
	} else
	{
		(*last)->next = new;
		(*last) = new;
	}
	if (admin.player_count == admin.player_number)
	{
		write_to_everyone(*first,NULL,e_limit_reached);
		alarm(30);
	}
}

struct list* search_fd(struct list* first, int i)
{
	while (first != NULL)
	{
		if (first->fd == i)
			return first;
		else
			first = first->next;
	}
	return NULL;
}

void addletter (char **word, char c, int j, int *wordsize)
{
	int k=8,i;
	if ((j+1) >= (*wordsize))
	{
		char* new_word;
		(*wordsize) += k;
		new_word = malloc((*wordsize));
		for (i = 0; i < j; i++)
			new_word[i] = (*word)[i];
		if ((*word) != NULL)
			free(*word);
		(*word) = new_word;
	}
	(*word)[j] = c;
	(*word)[j+1] = '\0';
}

char* buf_scan(struct list *p)
{
	char* word = NULL;
	int j=0,wordsize=0;
	while (j < (p->buf_pointer) && (p->in_buffer[j]) != '\r' )
	{
		addletter(&word,p->in_buffer[j],j,&wordsize);
		j++;
	}

	if (j < p->buf_pointer && p->in_buffer[j] == '\r')
	{
		int i;
		j += 2;
		for (i=j; i < p->buf_pointer; i++)
			p->in_buffer[i-j] = p->in_buffer[i];
		p->buf_pointer -= j;
	} else
	{
		free(word);
		word = NULL;
	}

	return word;
}

int size_of_str (char *string)
{
	int count=0;
	while (string[count] != '\0' && (count++) <= 512)
		{}
	if (string[count] == '\0')
		return count;
	return 0;
}

char* make_a_gay()
{
	char *gay;
	gay = malloc(9);
	gay[0] = 'I';
	gay[1] = 'M';
	gay[2] = '_';
	gay[3] = 'A';
	gay[4] = '_';
	gay[5] = 'G';
	gay[6] = 'A';
	gay[7] = 'Y';
	gay[8] = '\0';
	return gay;
}

int numcnt(int n)
{
	int cnt=0;
	while (n != 0)
	{
		n = n/10;
		cnt++;
	}
	return cnt;
}

int cmp_str(char* arg, const char *command);

void ch_nick(char **nick,int cmp_cnt)
{
	char *newnick;
	int strsize;

	strsize = size_of_str(*nick);
	if (cmp_cnt == 1)
	{
	newnick = malloc(strsize+2+10);
	sprintf(newnick,"%s_%d",*nick,cmp_cnt);
	} else
	{
		int n;
		n = numcnt(cmp_cnt-1);
		newnick = malloc((strsize+10));
		(*nick)[strsize-n] = '\0';
		sprintf(newnick,"%s%d",*nick,cmp_cnt);
	}
	free(*nick);
	*nick = newnick;
}

char* check_nickname(char *nick, struct list* first)
{
	struct list *p;
	char* newnick=NULL;
	int i=0,j=0,wordsize=0,cmp_cnt=0;
	while (nick[i] != '\0' && i <= 30)
	{
		if (nick[i] != ' ' && nick[i] != (char) 255
			 && nick[i] != '(' && nick[i] != ')')
		{
			addletter(&newnick,nick[i],j,&wordsize);
			j++;
		}
		i++;
	}
	if (newnick == NULL)
	{
		newnick = make_a_gay();
	}
	p = first;
	while (p != NULL)
		if (p->nickname != NULL && cmp_str(p->nickname,newnick))
		{
			cmp_cnt++;
			ch_nick(&newnick,cmp_cnt);
			p = first;
		} else
			p = p->next;
	free(nick);
	return newnick;
}

char* getarg(char *act, int *i)
{
	char* arg;
	int j=0;
	arg = malloc((size_of_str(act)+1));
	while (act[*i] == ' ')
		(*i)++;
	while (act[*i] != ' ' && act[*i] != '\0')
		arg[j++] = act[(*i)++];
	arg[j] = '\0';
	return arg;
}

enum {
	a_market=0,
	a_status=1,
	a_prod=2,
	a_buy=3,
	a_sell=4,
	a_build=5,
	a_end=6,
	a_help=7,
	a_req=8,
	a_junk=9,
	a_regret=10,
	a_chat=11,
	a_unbuild=12,
	a_list=13
};

/*COMMANDS*/

const char c_market []="market";
const char c_status []="status";
const char c_prod []="prod";
const char c_buy []="buy";
const char c_sell []="sell";
const char c_build []="build";
const char c_end []="end";
const char c_help []="help";
const char c_me []="me";
const char c_req []="req";
const char c_unbuild []="unbuild";
const char c_regret []="regret";
const char c_chat []="chat";
const char c_list []="list";
const char r_out []=
	"Here is the ways to undo some requests you made:\n"
	"To cancel a buy/sell order, you should type:\n"
	"[buy][sell] 0 0\n"
	"To cancel a fabric build №n, you should type:\n"
	"unbuild n\n"
	"To cancel a production order, you should type:\n"
	"prod -i\n"
	"where i is the number you want to substitute.\n";
const char h_out []=
	"Here is the list of possible commands for the server:\n"
	"'market' - print market status;\n"
	"'status [*nickname*]' - print player status;\n"
	"'req' - print your requests;\n"
	"'prod *count*' - make an order for production of goods;\n"
	"'buy *count* *price*' - make a buy order;\n"
	"'sell *count* *price*' - same as 'buy';\n"
	"'build' - make an order to build the fabrique;\n"
	"'unbuild *num*' - cancell the build order №num\n"
	"'end' - end your turn\n"
	"'help' - help;\n"
	"'regret' - that would help if you regret some orders you made that turn\n"
	"(if you did not end your turn, obviously)\n\n";

void outhelp(int fd)
{
	write(fd,&h_out,sizeof(h_out));
}

int cmp_str(char* arg, const char *command)
{
	int flag=1,i=0;
	while (arg[i] == command[i] && arg[i] != '\0' && command[i] != '\0')
		i++;
	if (arg[i] != command[i])
		flag = 0;
	return flag;
}

int getargn(char *arg)
{
	if (cmp_str(arg,c_market))
		return a_market;
	if (cmp_str(arg,c_status))
		return a_status;
	if (cmp_str(arg,c_prod))
		return a_prod;
	if (cmp_str(arg,c_buy))
		return a_buy;
	if (cmp_str(arg,c_sell))
		return a_sell;
	if (cmp_str(arg,c_build))
		return a_build;
	if (cmp_str(arg,c_end))
		return a_end;
	if (cmp_str(arg,c_help))
		return a_help;
	if (cmp_str(arg,c_req))
		return a_req;
	if (cmp_str(arg,c_unbuild))
		return a_unbuild;
	if (cmp_str(arg,c_regret))
		return a_regret;
	if (cmp_str(arg,c_chat))
		return a_chat;
	if (cmp_str(arg,c_list))
		return a_list;
	return a_junk;
}

int hm_inbuild(struct list *p)
{
	int i,j=0;
	for (i=0; i<5; i++)
		if (p->requests->build[i] != 0)
			j++;
	return j;
}

void outstatus(struct list *first, struct list *p, const char *arg)
{
	char* string;
	struct list *q=first;
	if (cmp_str((char*)arg,c_me) || arg[0] == '\0')
		q = p;
	else
	{

		while (q != NULL && !(cmp_str(q->nickname,arg)))
			q = q->next;
		if (q == NULL)
		{
			write(p->fd,&s_no_player,sizeof(s_no_player));
			return;
		}
	}
	string = malloc((size_of_str(q->nickname)+94));
	sprintf(string,"%s:\nMoney   Raw     Product Build   Ready   \n"
						"%-7d %-7d %-7d %-7d %-7d\n",
						q->nickname,q->cash,q->raw,q->prod,hm_inbuild(q),q->fab);
	write(p->fd,string,size_of_str(string));
	free(string);
}

int find_empty(struct req *r)
{
	int i;
	for (i=0; i<5; i++)
		if (r->build[i] == 0)
			return i;
	return -1;
}
int get_random_player(int *players, int count)
{
	int r,i;
	r = 1+rand()%count;
/*	r = 1+(int)(count*rand()/(RAND_MAX+1.0));*/
	
	for (i=0; i<admin.player_count; i++)
	{
		if (players[i] == 1)
			r--;
		if (r == 0)
			return i;
	}
	printf("THIS MESSAGE MEANS THAT EVERYTHING GOT\n"
			 "COMPLETELY WRONG IN THE AUCTION ARGUE\n");
	return -1;
}

/*
(25)                     (6)   (5)
nickname                 num   price
*/

void deb_outplayers(int *players)
{
	int i;
	for (i=0; i<admin.player_number;i++)
	{
		printf("%d",players[i]);
	}
	putchar('\n');
}

void auction(struct list *first, struct req *req)
{
	char *string;
	int i, b_num, s_num, min, max, sum;
	int players[admin.player_number];
	write_to_everyone(first,NULL,e_auc_begins);
	b_num = (admin.player_alive*lvl[admin.lvl].buy.num)/10;
	s_num = (admin.player_alive*lvl[admin.lvl].sell.num)/10;
	printf("b_num %d\ns_num %d\n",b_num,s_num);
	/*BUY PART*/
	write_to_everyone(first,NULL,e_buy);
	while (b_num > 0)
	{
		min = lvl[admin.lvl].buy.price;
		sum = 0;
		for (i=0; i<admin.player_number; i++)
		{
			if (req[i].sell.num > 0 && req[i].sell.price < min)
			{
				min = req[i].sell.price;
				sum = req[i].sell.num;
			} else
				if (req[i].sell.num > 0 && req[i].sell.price == min)
					sum += req[i].sell.num;
		}
		if (sum == 0)
			break;
		if (sum <= b_num)
		{
			for (i=0; i<admin.player_number; i++)
				if (req[i].sell.num > 0 && req[i].sell.price == min)
				{
					string = malloc(70);
					sprintf(string,"%-35s%-6d%-5d\n",
										req[i].player->nickname,
										req[i].sell.num,req[i].sell.price);
					string_to_everyone(first,string);
					free(string);
					req[i].player->prod -= req[i].sell.num;
					req[i].player->cash += req[i].sell.num * min;
					b_num -= req[i].sell.num;
					req[i].sell.num = 0;
				}
		} else
		{
			int count=0;
			for (i=0; i<admin.player_number; i++)
				if (req[i].sell.num > 0 && req[i].sell.price == min &&
					 req[i].player->death == 0)
				{
					players[i] = 1;
					count++;
				}	else
					players[i] = 0;
			deb_outplayers(players);
			while (b_num > 0)
			{
				i = get_random_player(players,count);
				printf("\n\n------%d------\n\n",i);
				printf("players[%d] == %d\n",i,players[i]);
				players[i] = 0;
				count--;
				if (b_num >= req[i].sell.num)
				{
					string = malloc(70);
					sprintf(string,"%-35s%-6d%-5d\n",
										req[i].player->nickname,
										req[i].sell.num,req[i].sell.price);
					string_to_everyone(first,string);
					free(string);
					b_num -= req[i].sell.num;
					req[i].player->prod -= req[i].sell.num;
					req[i].player->cash += req[i].sell.num * min;
					req[i].sell.num = 0;
				} else
				{
					string = malloc(70);
					sprintf(string,"%-35s%-6d%-5d\n",
										req[i].player->nickname,
										b_num,req[i].sell.price);
					string_to_everyone(first,string);
					free(string);
					req[i].player->prod -= b_num;
					req[i].player->cash += b_num * min;
					b_num = req[i].sell.num = 0;/*Not that it really is needed*/
				}
			}
		}
	}
	/*END BUY*/
	/*SELLOUT PART*/
	write_to_everyone(first,NULL,e_sell);
	while (s_num > 0)
	{
		max = lvl[admin.lvl].sell.price;
		sum = 0;
		for (i=0; i<admin.player_number; i++)
		{
			if (req[i].buy.num > 0 && req[i].buy.price > max)
			{
				max = req[i].buy.price;
				sum = req[i].buy.num;
			} else
				if (req[i].buy.num > 0 && req[i].buy.price == max)
					sum += req[i].buy.num;
		}
		if (sum == 0)
			break;
		if (sum <= s_num)
		{
			for (i=0; i<admin.player_number; i++)
				if (req[i].buy.num > 0 && req[i].buy.price == max)
				{
					string = malloc(70);
					sprintf(string,"%-35s%-6d%-5d\n",
										req[i].player->nickname,
										req[i].buy.num,req[i].buy.price);
					string_to_everyone(first,string);
					free(string);
					s_num -= req[i].buy.num;
					req[i].player->raw += req[i].buy.num;
					req[i].player->cash -= req[i].buy.num*max;
					req[i].buy.num = 0;
				}
		} else
		{
			int count=0;
			for (i=0; i<admin.player_number; i++)
				if (req[i].buy.num > 0 && req[i].buy.price == max
					 && req[i].player->death == 0)
				{
					players[i] = 1;
					count++;
				}
				else
					players[i] = 0;
			deb_outplayers(players);
			while (s_num > 0)
			{
				i = get_random_player(players,count);
				printf("%d",i);
				players[i] = 0;
				count--;
				if (s_num >= req[i].buy.num)
				{
					string = malloc(70);
					sprintf(string,"%-35s%-6d%-5d\n",
										req[i].player->nickname,
										req[i].buy.num,req[i].buy.price);
					string_to_everyone(first,string);
					free(string);
					s_num -= req[i].buy.num;
					req[i].player->raw += req[i].buy.num;
					req[i].player->cash -= req[i].buy.num * max;
					req[i].buy.num = 0;
				} else
				{
					string = malloc(70);
					sprintf(string,"%-35s%-6d%-5d\n",
										req[i].player->nickname,
										s_num,req[i].buy.price);
					string_to_everyone(first,string);
					free(string);
					req[i].player->raw += s_num;
					req[i].player->cash -= s_num * max;
					s_num = req[i].buy.num = 0;/*Not that it really is needed*/
				}
			}
		}
	}
	/*END SELLOUT*/
	string = malloc(100);
	sprintf(string,
		"\n"
		"*****************************END OF AUCTION*************************\n"
		"\n");
	string_to_everyone(first,string);
	free(string);
}

struct list *find_survivor(struct list *first)
{
	while (first->death == 1 && first != NULL)
		first = first->next;
	return first;
}

void end_game(struct list *first)
{
	admin.game_status = 0;
	while (first != NULL)
	{
		write(first->fd,"boi!\n\n\n",7);
		close(first->fd);
		first = first->next;
	}
	close(l_socket);
	exit(0);
}

void clr_req(struct list *p)
{
	int i;
	struct req *r;
	r = p->requests;
	r->player = NULL;
	r->prod = 0;
	r->buy.num = 0;
	r->buy.price  = 0;
	r->sell.num = 0;
	r->sell.price = 0;
	for (i=0; i<5; i++)
		r->build[i] = 0;
}

void ipoteka(struct list *first, struct req *r)
{
	char* string;
	struct list *p=first;
	int i;
	string = malloc(1024);
	while (p != NULL)
	{
		if (p->death != 1)
		{
			p->raw -= p->requests->prod;
			sprintf(string,"Time for monthly payments!\n"
								"300 for each raw = %d!\n"
								"500 for each prod = %d!\n"
								"1000 for each fab = %d!\n\n",
								p->raw*300,p->prod*500,p->fab*1000);
			write(p->fd,string,size_of_str(string));
			p->cash -= 300 * p->raw + 500 * p->prod + 1000 * p->fab;
			p->prod += p->requests->prod;
			for (i=0; i<5; i++)
			{
				if (p->requests->build[i] == 2)
				{
					sprintf(string,"2500 for the last month of building №%d!\n",
										i);
					p->cash -= 2500;
				}
				if (p->requests->build[i] == 1)
					p->fab++;
				if (p->requests->build[i] > 0)
					p->requests->build[i]--;
			}
			if (p->cash < 0)
			{
				clr_req(p);
				admin.player_alive--;
				p->death = 1;
				write_to_everyone(first,p,e_plr_lose);
			}
		}
		p = p->next;
	}
	free(string);
	if (admin.player_alive == 1)
	{
		p = find_survivor(first);
		write_to_everyone(first,p,e_game_over);
	}
	if (admin.player_alive == 0)
	{
		write_to_everyone(first,NULL,e_you_loosers);
	}
	if (admin.player_alive <= 1)
		end_game(first);
}

const int level_change[5][5] = 
{
	{4,4,2,1,1},
	{3,4,3,1,1},
	{1,3,4,3,1},
	{1,1,3,4,3},
	{1,1,2,4,4}
};

void newlevel()
{
	int r,i,sum;
	sum = 0;
	i = 0;
	r = 1 + (int)(12.0*rand()/(RAND_MAX + 1.0));
	while (sum < r)
		sum += level_change[admin.lvl][i++];
	admin.lvl = i-1;
}

void newturn(struct list *first, struct req *r)
{
	newlevel();
	while (first != NULL)
	{
		if (first->death == 0)
		{
			first->requests->prod = 0;
			first->end_turn = 0;
			first->requests->buy.num = 0;
			first->requests->buy.price = 0;
			first->requests->sell.num = 0;
			first->requests->sell.price = 0;
		}
		outmarket(first);
		outstatus(NULL,first,"");
		first = first->next;
	}
	admin.endturn_count = 0;
}

const char s_market []="MARKET STATUS      NUM   PRICE\n";


void outreq(struct list *p)
{
	char *string;
	struct req *r=p->requests;
	string = malloc(200);
	sprintf(string,"REQUESTS of %s:\n"
						"prod: %d\nAUC  NUM PRICE\n"
						"Buy: %4d%5d\nSell: %3d%5d\n"
						"Build: %d%d%d%d%d\n\n",
						p->nickname,r->prod,
						r->buy.num,r->buy.price,r->sell.num,r->sell.price,
						r->build[0],r->build[1],r->build[2],r->build[3],
						r->build[4]);
	write(p->fd,string,size_of_str(string));
	free(string);
}

void outmarket(struct list *p)
{
	char* arg;
	arg = malloc(160);
	sprintf(arg,"%sBUYING PROD:       %-6d %-5d"
					"\nSELLING RAW:       %-6d %-5d\n\n",
					s_market,(lvl[admin.lvl].buy.num*admin.player_alive)/10,
					lvl[admin.lvl].buy.price,
					(lvl[admin.lvl].sell.num*admin.player_alive)/10,
					lvl[admin.lvl].sell.price);
	write(p->fd,arg,size_of_str(arg));
	free(arg);
}

/* 
MARKET STATUS(19)   NUM(6)PRICE(5)
 */

void chat_proc(struct list *first, struct list *p, char *act, int i)
{
	char* mes;
	int j=0,k=0,n=0;
	if (p->chat_ban == 1)
	{
		write(p->fd,&s_chat_banned,sizeof(s_chat_banned));
		return;
	}
	mes = malloc(size_of_str(act)+size_of_str(p->nickname)+80);
	if (p->death == 1)
	{
		mes[0] = '*';
		mes[1] = 'D';
		mes[2] = 'E';
		mes[3] = 'A';
		mes[4] = 'D';
		mes[5] = '*';
		j = 6;
	}
	mes[j++] = '[';
	while ((p->nickname[k]) != '\0')
		mes[j++] = (p->nickname)[k++];
	mes[j++] = ']';
	mes[j++] = ':';
	mes[j++] = ' ';
	while (act[i] == ' ')
		i++;
	k = i;
	while (act[i] != '\0')
	{
		mes[j++] = act[i++];
		if (j%80 == 0)
		{
			n++;
			mes[j++] = '\n';
		}
	}
	mes[j] = '\n';
	mes[j+1] = '\0';
	if (act[k] != '\0')
	{
		string_to_everyone(first,mes);
		p->chat_count += n*60+40;
		if (p->chat_count >=160 &&
			 !cmp_str(p->nickname,"Botbot_v0.01_Spammer_Edition"))
		{
			p->chat_ban = 1;
			write(p->fd,&s_ban_spam,sizeof(s_ban_spam));
		}
	}
	free(mes);
}

void outlist(struct list *first, struct list *p)
{
	write(p->fd,&s_out_list,sizeof(s_out_list));
	while (first != NULL)
	{
		write(p->fd,first->nickname,size_of_str(first->nickname));
		if (p == first)
			write(p->fd," (you)\n",7);
		else
			write(p->fd,"\n",1);
		first = first->next;
	}
	write(p->fd,"End of list.\n",13);
}

void
ingame_act(struct list *first, struct list *p, char *act, struct req *r)
{
	char* arg=NULL;
	int i=0,argn,j,k;
	arg = getarg(act,&i);

	argn = getargn(arg);
	free(arg);
	switch(argn)
	{
	case a_market:
		outmarket(p);
		break;
	case a_status:
		arg = getarg(act,&i);
		outstatus(first,p,arg);
		free(arg);
		break;
	case a_help:
		outhelp(p->fd);
		break;
	case a_regret:
		write(p->fd,&r_out,sizeof(r_out));
		break;
	case a_req:
		outreq(p);
		break;
	case a_chat:
		chat_proc(first,p,act,i);
		break;
	case a_list:
		outlist(first,p);
		break;
	case a_junk:
		write(p->fd,&s_got_junk,sizeof(s_got_junk));
	default:
		if (p->end_turn == 0 && p->death == 0)
		{
			switch (argn)
			{
			case a_prod:
				arg = getarg(act,&i);
				i = atoi(arg);
				free(arg);
				/*argn i */
				if (p->fab - p->requests->prod >= i &&
					p->requests->prod + i >= 0 &&
					p->raw - p->requests->prod >= i)
				{
					if (p->cash >= 2000 * i)
					{
						p->requests->prod += i;
						p->cash -= 2000 * i;
						write(p->fd,&s_gotcha,sizeof(s_gotcha));
					} else
						write(p->fd,&s_no_money,sizeof(s_no_money));
				} else
					write(p->fd,&s_wrong_number,sizeof(s_wrong_number));
				break;
			case a_buy:
				arg = getarg(act,&i);
				j = atoi(arg);
				free(arg);
				arg = getarg(act,&i);
				k = atoi(arg);
				free(arg);
				/*argn j k*/
				if (j >= 0 && k >= 0 && j*k <= p->cash &&
					(lvl[admin.lvl].sell.price <= k || j == 0))
				{
					p->requests->buy.num = j;
					p->requests->buy.price = k;
					write(p->fd,&s_gotcha,sizeof(s_gotcha));
			} else
					write(p->fd,&s_wrong_buy,sizeof(s_wrong_buy));
				break;
			case a_sell:
				arg = getarg(act,&i);
				j = atoi(arg);
				free(arg);
				arg = getarg(act,&i);
				k = atoi(arg);
				free(arg);
				/*argn j k*/
				if (j >= 0 && k >= 0 && j <= p->prod &&
					lvl[admin.lvl].buy.price >= k)
				{
					p->requests->sell.num = j;
					p->requests->sell.price = k;
					write(p->fd,&s_gotcha,sizeof(s_gotcha));
			} else
					write(p->fd,&s_wrong_sell,sizeof(s_wrong_sell));
				break;
			case a_build:
				if (p->cash >= 2500)
				{
					j = find_empty(p->requests);
					if (j != -1)
					{
						p->requests->build[j] = 5;
						p->cash -= 2500;
						write(p->fd,&s_gotcha,sizeof(s_gotcha));
					} else
						write(p->fd,&s_over_build,sizeof(s_over_build));
				}
				break;
			case a_end:
				write(p->fd,&s_end_turn,sizeof(s_end_turn));
				p->end_turn = 1;
				admin.endturn_count++;
				if (admin.endturn_count == admin.player_alive)
				{
					auction(first,r);
					ipoteka(first,r);
					newturn(first,r);
				}
				break;
			case a_unbuild:
				arg = getarg(act,&i);
				j = atoi(arg) - 1;
				if (j >= 0 && j <= 4 && p->requests->build[j] != 0)
				{
					write(p->fd,&s_gotcha,sizeof(s_gotcha));
					if (p->requests->build[j] == 5 || p->requests->build[j] == 1)
						p->cash += 2500;
					p->requests->build[j] = 0;
				} else
					write(p->fd,&s_wrong_unbuild,sizeof(s_wrong_unbuild));
			default:
				break;
			}
		} else
		{
			if (p->death == 1)
				write(p->fd, &s_you_dead, sizeof(s_you_dead));
			else
				write(p->fd, &s_turn_over, sizeof(s_turn_over));
		}
	}
}

int player_act(struct list *first, int i, struct req *requests)
{
	char *act, *string;
	int buf_add;
	struct list *p;
	p = search_fd(first,i);
	if (p == NULL)
	{
		printf("Spooky act\n");
		exit(-1);
	}

	buf_add =
	read(i,(p->in_buffer)+(p->buf_pointer),(p->bufsize)-(p->buf_pointer));
	if (buf_add == 0)
		return EOF;
	p->buf_pointer += buf_add;
	while ((act = buf_scan(p)) != NULL)
	{
		if (p->nickname == NULL)
		{
			act = check_nickname(act,first);
			p->nickname = act;
			string = malloc(9+size_of_str(p->nickname)+sizeof(s_got_nname)+1);
			sprintf(string,"Welcome, %s%s",p->nickname,s_got_nname);
			write(p->fd,string,size_of_str(string));
			free(string);
			admin.nickname_count++;
			write_to_everyone(first,p,e_new_player);
			if (admin.nickname_count == admin.player_number)
				return 1;
		} else
		{
			if (admin.game_status == 0)
			{
				string = malloc(40+size_s_player_count+1);
				sprintf(string,"%s%s%d%s%d%s%d%s",
									s_game_not_started,s_player_count_1,
									admin.player_count,s_player_count_2,
									admin.nickname_count,s_player_count_3,
									admin.player_number,s_player_count_4);
				write(p->fd,string,size_of_str(string));
				free(string);
			} else 
			{
				ingame_act(first,p,act,requests);
			}
			free(act);
		}
	}
	return 0;
}

struct list* get_end(struct list *first)
{
	if (first != NULL)
		while (first->next != NULL)
			first = first->next;
	return first;
}

void quit_player(struct list **first, struct list **last, int fd, int pid)
{
	struct list **p;
	struct list *q;
	p = first;
	while ((**p).fd != fd)
		p = &((**p).next);
	write_to_everyone(*first,*p,e_plr_quit);
	if (admin.player_count == admin.player_number)
	{
		if (admin.player_alive == 0)
		{
			admin.game_status = 0;
			write_to_everyone(*first,NULL,e_ctdn_abort);
			kill(pid,SIGKILL);
		}
	}
	if (admin.player_alive != 0 && (*p)->death == 0)
		admin.player_alive--;
	free((*p)->in_buffer);
	if ((*p)->nickname != NULL)
	{
		free((*p)->nickname);
		admin.nickname_count--;
	}
	admin.player_count--;
	if ((*p)->requests != NULL)
		clr_req(*p);
	q = (**p).next;
	free(*p);
	if (*p == *last)
	{
		*p = NULL;
		*last = get_end(*first);
	}
	*p = q;
}


int initcountdown(struct list *first)
{
	int p;
	alarm(0);
	alarm(10);
	if ((p = fork()) == 0)
	{
		int i;
		for (i = 10; i>0; i--)
		{
			write_to_everyone(first,(struct list *) i, e_init_countdown);
			sleep(1);
		}
		exit(0);
	} else
	return p;
}

char* make_slowpoke()
{
	char *new;
	new = malloc(9);
	new[0] = 'S';
	new[1] = 'l';
	new[2] = 'o';
	new[3] = 'w';
	new[4] = 'p';
	new[5] = 'o';
	new[6] = 'k';
	new[7] = 'e';
	new[8] = '\0';
	return new;
}

void no_more_slows(struct list *first)
{
	char* newnick;
	struct list *p=first;
	
	while (p != NULL)
	{
		if (p->nickname == NULL)
		{
			newnick = make_slowpoke();
			newnick = check_nickname(newnick,first);
			p->nickname = newnick;
	 		write(p->fd,&s_u_slow,sizeof(s_u_slow));
			write(p->fd,p->nickname,size_of_str(p->nickname));
			write(p->fd,"! Congratulations!\n",20);
		}
		p = p->next;	
	}
}

void alrm_disp(int n)
{
	int k=1;
	write(admin.wake_pipe[1],&k,sizeof(int));
}

void start_game(struct list *first, struct req **requests)
{
	struct req *r;
	struct list *p=first;
	int i=0;
	*requests = malloc(admin.player_count*sizeof(struct req));
	while (p != NULL)
	{
		r = &((*requests)[i]);
		r->player = p;
		r->prod = 0;
		r->buy.num = r->sell.num = r->buy.price = r->sell.price = 0;
		r->build[0]=r->build[1]=r->build[2] = r->build[3] = r->build[4] = 0;
		p->requests = r;
		write(p->fd,&s_game_starts,sizeof(s_game_starts));
		outstatus(first,p,c_me);
		i++;
		p = p->next;
	}
	admin.player_alive = admin.player_count;
	admin.lvl = 2;
	admin.endturn_count = 0;
	admin.game_status = 2;
	alarm(3);
}

void null_chat(struct list *first)
{
	while (first != NULL)
	{
		first->chat_count = 0;
		first = first->next;
	}
	alarm(10);
}

int main(int argc, char **argv)
{
	struct req *requests;
	struct list *plrs_first, *plrs_last;
	fd_set readset, readset_c;
	int c, max_d,ctdn_pid, pipe_st,err;
	srand(time(NULL));
	signal(SIGALRM, &alrm_disp);
	pipe(admin.wake_pipe);
	admin.player_alive = 0;
	admin.endturn_count = 0;
	init_lvl();
	admin.game_status = 0;
	admin.nickname_count = 0;
	admin.player_count = 0;
	admin.player_number = atoi(argv[2]);
	admin.port_number = atoi(argv[1]);
	FD_ZERO(&readset_c);
	plrs_first = plrs_last = NULL;
	cr_l_sock(&l_socket);
	max_d = l_socket;
	FD_SET(admin.wake_pipe[0],&readset_c);
	FD_SET(l_socket,&readset_c);
	FD_SET(0,&readset_c);
	while(1)
	{
		int i;
		readset = readset_c;
		select(max_d+1,&readset,NULL,NULL,NULL);
		if (FD_ISSET(admin.wake_pipe[0],&readset))
		{
			read(admin.wake_pipe[0],&pipe_st,sizeof(int));
			FD_ZERO(&readset);
			if (admin.game_status == 0)
			{
				no_more_slows(plrs_first);
				admin.nickname_count = admin.player_number;
				admin.game_status = 1;
				write_to_everyone(plrs_first,NULL,e_final_countdown);
				ctdn_pid = initcountdown(plrs_first);
			} else
			{
				if (admin.game_status == 1)	
					start_game(plrs_first,&requests);
				if (admin.game_status == 2)
					null_chat(plrs_first);
			}
		}
		if (FD_ISSET(0,&readset))
		{
			if ((c = getchar()) == 'Q')
				return 0;
			putchar(c);
		}
		if (FD_ISSET(l_socket,&readset))
		{
			int newsoc;
			if ((newsoc = face_control(l_socket,admin.player_count)) != -1)
			{
				admin.player_count++;
				if (newsoc > max_d)
					max_d = newsoc;
				new_player(&plrs_first,&plrs_last,newsoc);
				FD_SET(newsoc,&readset_c);
				write(newsoc,&s_req_name,sizeof(s_req_name));
			}
		}
		for (i = 3; i <= max_d; i++)
		{
			if (FD_ISSET(i,&readset) && i != l_socket)
			{
				err = player_act(plrs_first,i,requests);
				FD_CLR(i,&readset);
				if (err == 1)
				{
					admin.game_status = 1;
					write_to_everyone(plrs_first,NULL,e_final_countdown);
					ctdn_pid = initcountdown(plrs_first);
				}
				if (err == EOF)
				{
					close(i);
					alarm(0);
					if (admin.nickname_count == admin.player_count
						 && admin.game_status == 1)
						kill(ctdn_pid,SIGKILL);
					quit_player(&plrs_first,&plrs_last,i,ctdn_pid);
					FD_CLR(i,&readset_c);
				}
			}
		}
	}
}
