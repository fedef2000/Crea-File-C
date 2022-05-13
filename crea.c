#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

int main(int argc, char **argv){

	if(argc < 2){
		printf("inserire il nome del file da creare\n");
		exit(1);
	}
	char files[5][25]; //massimo 5 file con nome di massimo 25 caratteri
	bool f=true;
	bool p=true;
	int nf = 0;

	for(int i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			if(argv[i][1] == 'f' || argv[i][1] == 'F'){
				p = false;
			}else if (argv[i][1] == 'p' || argv[i][1] == 'P'){
				f = false;
			}else if (argv[i][1] == 'a' || argv[i][1] == 'A'){
				f = true;
				p = true;
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

		char inizio[] =
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
		"\n";
		char fine[] =
		"\n"
		"	exit(0);\n"
		"}";
		char pip[] =
		"\n	int piped[2]; //inizializzo un vettore di due elementi da usare nella pipe\n"
		"	//si crea una pipe\n"
		"	if(pipe(piped) < 0){\n"
		"		printf(\"errore creazione pipe\\n\");\n"
		"		exit(2);\n"
		"	}\n\n";
		char frk[] =
		"\n	int pid; //da usare per la fork\n"
		"	int pidFiglio, status, ritorno; //per wait del padre\n"
		"	//creo il processo figlio con la fork e controllo che sia stato creato correttamente\n"

		"	if((pid = fork()) < 0){\n"
		"		printf(\"errore creazione figlio\\n\");\n"
		"		exit(1);\n"
		"	}\n"
		"	if(pid == 0){\n"
		"		/*Processo figlio*/\n"
		"		exit(0);\n"
		"	}\n\n"
		"	/*processo padre*/\n"
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
		"	}\n\n";

		char fpip[] =
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
		"	}\n\n";


		write(fd, inizio, sizeof(inizio) - 1);

		if(p && f)
		{
			write(fd, fpip, sizeof(fpip) -1);
		}else if(p){
			write(fd, pip, sizeof(pip) - 1);
		}else if(f){
			write(fd, frk, sizeof(frk) - 1);
		}

		write(fd, fine, sizeof(fine) - 1);

	}
	exit(0);
}
