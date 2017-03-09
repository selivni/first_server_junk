#ifndef MODULE_PROTECTOR
#define MODULE_PROTECTOR
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

int size_of_str (const char *string);

class SuperString
{
	static const int stdsize=16;
	char* string;
	int size;
	int pointer;
public:
	SuperString (int i=16);
	SuperString (const char* a);
	SuperString(const SuperString& old);
	~SuperString ();
	char* conv_char();
	SuperString cutword (int k=1);
	SuperString& operator=(SuperString old);
	char& operator[] (int i);
	int print_s(int i=-1);
};

//SuperString botname = "Botbot_v0.04_Spammer_Edition";

int atoi_ss(SuperString num);

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
	int auction[6];
public:
	Player* next;
	Player();
	Player(SuperString nick);
	SuperString& seek();
	int product();
	void fill(SuperString info);
	SuperString& nickname();
	int& dead();
	Player* moar(SuperString nick="");
};

class Bot
{
	Player* plrs;
	Player* me;
	int market[4];
	int count;
	int fd;
	fd_set readset,readset_c;
	bool startflag;
	SuperString botname;
	int max_buy;
	int min_sell;
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
	void build();
	int player_count();
	int* player_info(int num);
	int* getauc(int num);
	int* market_info();
	int* my_info();
	void analauc();
	int* recommend(int* last_market);
	int cmp_str(SuperString &A,SuperString &B) const;
};

#endif
