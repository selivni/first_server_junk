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
#include "module.hpp"
#include "lex.hpp"
#include "syn_kur.hpp"

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
