# Crea-File-C
Questo progetto contiene un file C che se buildato permette di eseguire un comando per la creazione di file C contenenti codice con pipe e fork, con i relativi commenti già inseriti.
## Installazione
1. Scaricare la repository da Github
2. aprire un terminale ed entrare nella cartella appena scaricata
3. buildare il file creanpipe.c con il comando:
```
$ gcc creanpipe.c -o creac
```
4. spostare il file creac appena creato in /usr/local/bin, o in una qualsiasi cartella del PATH:
```
$ mv creac /usr/local/bin
```
A questo punto è possibile usare il comando **creac** per creare un file .c con il nome passato come parametro, e specificando se vogliamo crearlo con una sola pipe e un solo processo figlio oppure n pipe e n processi figli.

## esempi
```
$ creac prova
```
Crea un file prova.c con una pipe e un processo figlio.

Se abbiamo bisogno di n pipe e n processi figli basta aggiungere la flag -n:
```
creac prova -n
```

Si possono anche inserire più parametri:
```
creac -n file1 file2 file3
```
Crea i file1.c file2.c file3.c, tutti con n pipe e n processi figli
