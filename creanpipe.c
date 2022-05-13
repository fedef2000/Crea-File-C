#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>


char unaPipe[] =
		"#include<stdlib.h>\n"
		"#include<stdio.h>\n"
		"#include<fcntl.h>\n"
		"#include<sys/wait.h>\n"
		"#include<unistd.h>\n\n"
		"int main(int argc, char **argv)\n"
		"{\n"
		"	//controllo numero dei parametri, ce ne deve essere almeno uno\n"
		"	if(argc < 2)\n"
		"	{\n"
		"		printf(\"numero dei parametri errato %d\\n\", argc);\n"
		"		exit(1);\n"
		"	}\n"
		"\n"
		"	int pid; //da usare per la fork\n"
		"	int pidFiglio, status, ritorno; //per wait del padre\n"
		"	int piped[2]; //vettore di due elementi da usare nella pipe\n"
		"	//creo una pipe e controllo sia stata creata correttamente\n"
		"	if(pipe(piped) < 0){\n"
		"		printf(\"errore creazione pipe\\n\");\n"
		"		exit(2);\n"
		"	}\n\n"
		"	//creo il processo figlio con la fork e controllo che sia stato creato correttamente\n"
		"	if((pid = fork()) < 0){\n"
		"		printf(\"errore creazione figlio\\n\");\n"
		"		exit(1);\n"
		"	}\n"
		"	if(pid == 0){\n"
		"		/*Processo figlio*/\n"
		"		close(piped[0]); //chiudo il lato di lettura della pipe\n"
		"		char s[] = \"\";\n"
		"		write(piped[1], s, 1);\n"
		"		exit(0);\n"
		"	}\n\n"
		"	/*processo padre*/\n"
		"	close(piped[1]); //chiudo il lato di scrittura della pipe\n"
		"	char mess[1];\n"
		"	read(piped[0], mess, 1);\n\n"
		"	/*il padre aspetta il figlio*/\n"
		"	pidFiglio = wait(&status);\n"
		"	if(pidFiglio < 0){\n"
		"		printf(\"errore wait\\n\");\n"
		"		exit(4);\n"
		"	}\n"
		"	if((status & 0xFF) != 0){\n"
		"		printf(\"figlio con pid %d terminato in modo anomalo\\n\", pidFiglio);\n"
		"	}else{\n"
		"		ritorno=(int)((status >> 8) & 0xFF);\n"
		"		printf(\"il figlio con pid=%d ha ritornato %d (se 255 problemi!)\\n\", pidFiglio, ritorno);\n"
		"	}\n"
		"\n"
		"	exit(0);\n"
		"}";

char npipe[] =
"#include <stdio.h>\n"
"#include <stdlib.h>\n"
"#include <fcntl.h>\n"
"#include <unistd.h>\n"
"#include <string.h>\n"
"#include <sys/wait.h>\n\n"
"/* tipo che definisce una pipe */\n"
"typedef int pipe_t[2];\n\n"
"int main(int argc, char *argv[])\n"
"{\n"
"	int pid;	/* pid per fork */\n"
"	int N = argc;	/* numero di processi e di pipe, in questo caso settato al numero di parametri*/\n"
"	int fdr;	/* per open */\n"
"	pipe_t *p;	/* array dinamico di pipe */\n"
"	int pidFiglio, status, ritorno;	/* variabili per wait*/\n"
"\n"
"	if (argc < 4)	/* devono essere passati almeno TRE parametri*/\n"
"	{\n"
"		printf(\"Errore nel numero di parametri dato che argc = %d\\n\", argc);\n"
"		exit(1);\n"
"	}\n"
"\n"
"	/* alloco l'array di pipe */\n"
"	p = (pipe_t *)malloc(sizeof(pipe_t) * N);\n"
"	if (p == NULL)\n"
"	{\n"
"		printf(\"Errore nella creazione array dinamico\\n\");\n"
"		exit(3);\n"
"	}\n"
"\n"
"	/* creo N pipe */\n"
"	for (int i=0; i < N; i++)\n"
"	{\n"
"		if (pipe(p[i]) < 0)\n"
"		{\n"
"			printf(\"Errore nella creazione della pipe\\n\");\n"
"			exit(4);\n"
"		}\n"
"	}\n"
"	printf(\"DEBUG-Sono il processo padre con pid %d e sto per generare %d figli\\n\", getpid(), N);\n"
"\n"
"	for (int i=0; i < N; i++)\n"
"	{\n"
"		/* creazione dei figli */\n"
"		if ((pid = fork()) < 0)\n"
"		{\n"
"			printf (\"Errore nella fork\\n\");\n"
"			exit(5);\n"
"		}\n"
"\n"
"		if (pid == 0) /* figlio */\n"
"		{\n"
"			printf(\"DEBUG-Figlio %d con pid %d\\n\", i, getpid());\n"
"			/* chiude tutte le pipe che non usa */\n"
"			for (int k = 0; k < N; k++)\n"
"			{\n"
"				/* ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che e' la i-esima */\n"
"				close(p[k][0]);\n"
"				if (k != i)\n"
"				{\n"
"					close(p[k][1]);\n"
"				}\n"
"			}\n"
"			/* ogni figlio apre lo stesso file */\n"
"			if ((fdr = open(/*inserire nome del file da aprire*/, O_RDONLY)) < 0)\n"
"			{\n"
"				printf(\"Errore nella apertura del file %s\\n\", argv[1]);\n"
"				exit(-1); 	/* torniamo -1 che verrà interpretato come 255 e quindi come errore */\n"
"			}\n"
"			while(read(fdr, /*indirizzo dove salvare dato*/, /*dimensione del dato*/) > 0)\n"
"			{\n"
"				/*qualcosa da fare sul tipo di dato letto dal file*/\n"
"			}\n"
"			/* comunica al padre usando la i-esima pipe */\n"
"			write(p[i][1], /*indirizzo dato da scrivere*/, /*dimensione*/);\n"
"			exit(0);	/* torniamo 0 che rappresenta SUCCESSO */\n"
"		}\n"
"	}\n"
"\n"
"	/* padre */\n"
"	/* chiude tutte le pipe che non usa */\n"
"	for (int k=0; k < N; k++)\n"
"	{\n"
"		close(p[k][1]);\n"
"	}\n"
"\n"
"	/* legge dalle pipe i messaggi */\n"
"	for (int i=0; i < N; i++)\n"
"	{\n"
"		if (read(p[i][0], /*dove salvare il dato*/, /*dimensione*/) > 0)\n"
"		{\n"
"			/*qualcosa da fare sul dato letto dalla pipe*/\n"
"		}\n"
"	}\n"
"\n"
"	/* Attesa della terminazione dei figli */\n"
"	for(int i=0;i < N;i++)\n"
"	{\n"
"		if ((pidFiglio = wait(&status)) < 0)\n"
"		{\n"
"			printf(\"Errore wait\\n\");\n"
"			exit(6);\n"
"		}\n"
"		if ((status & 0xFF) != 0)\n"
"		{\n"
"			printf(\"Figlio con pid %d terminato in modo anomalo\\n\", pidFiglio);\n"
"		}else\n"
"		{\n"
"			ritorno=(int)((status >> 8) & 0xFF);\n"
"			printf(\"Il figlio con pid=%d ha ritornato %d (se 255 problemi!)\\n\", pidFiglio, ritorno);\n"
"		}\n"
"	}\n"
"\n"
"\n"
"	exit(0);	//se tutto ha successo ritorno 0\n"
"}\n";


int main(int argc, char **argv){

	if(argc < 2){
		printf("inserire il nome del file da creare\n");
		exit(1);
	}
	char files[5][25]; //massimo 5 file con nome di massimo 25 caratteri
	bool n=false;
	int nf = 0;

	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(argv[i][1] == 'n' || argv[i][1] == 'N'){
				n = true;
			}else{
				printf("flag %c non valida\n", argv[i][1]);
				exit(2);
			}
		}else
		{
			strncpy(files[nf], argv[i], strlen(argv[i]));
			nf++;
		}
	}

	for(int i = 0; i < nf; i++){
		printf("creazione del file %s.c\n", files[i]);
		char ex[] = ".c";
		strncat(files[i], ex, 3);
		int fd;
		fd = creat(files[i], 0644);
		if (fd <0){
			printf("errore nella creazione\n");
			exit(2);
		}
		printf("file %s creato\n", files[i]);

		if(n == false)
		{
			write(fd, unaPipe, sizeof(unaPipe) -1);
		}else{
			write(fd, npipe, sizeof(npipe) - 1);
		}
	}
	exit(0);
}

