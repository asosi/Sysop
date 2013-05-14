Sysop
=====

Repo privata per il progetto di Sysop

######################################################################

Progetto 1: Modifica utility di base di GNU/Linux


Premessa
Le utility di base di GNU/Linux (ls, cd, cat, ecc.) sono distribuite all’interno di tutte le distribuzione GNU/Linux. Il progetto e’ quello di implementare le utility elencate ai punti seguenti:


Utility:

mkbkp
=====

• crea un file di archivio per il salvataggio di files e directories passati come argomenti
• flags:
• -f <archivio> indica l'archivio da creare od estrarre.
• -c indica la creazione dell'archivio
• -x indica l'estrazione dell'archivio nella directory corrente
• -t indica che deve essere visualizzato un elenco del contenuto dell'archivio
es:
mkbkp -c -f etc.bkp /etc
produce un file “etc.bkp” contenente un salvataggio di tutti i files presenti in /etc


equal
=====

• Prevede due argomenti, che possono essere nomi di files o directories.
• Ritorna 0 se gli argomenti hanno contenuto identico
• Ritorna 1, se gli argomenti differiscono, elencando a video quali elementi differiscono


plive
=====

• Ogni secondo mostra un elenco dei ( -n <num>, default 10) processi che usano più cpu sul sistema, in ordine di occupazione.
• Per ogni processo vengono mostrati:
<process id> <process id del padre> <percentuale di cpu usata> <nome dell eseguibile del processo>
• premendo un numero da 1 a 9 la visualizzazione si aggiorna al numero di secondi corrispondente; premendo “q” il comando termina.

######################################################################

Note:
• Obbligatorio usare la system call getopt.
• Si possono usare solo le system call, all’interno del codice sorgente non e’ ammesso.
l’utilizzo di comandi gia’ disponibili nel sistema GNU/Linux in forma di codice oggetto.
compilato
• Tutti i processi generati non devono produrre processi zombie
• Ogni utility sviluppata deve produrre il corretto comportamento a video e, allo stesso
tempo, registrare tutte le attivita’ di input, output ed error in file di log che devono obbligatoriamente essere memorizzati in /var/log/utility/<nome_utility> con il medesimo nome dell’utility in uso.
• Se da console viene cancellato un file di log, la successiva invocazione del comando dovra’ segnalare il fatto che il file non esiste e ricrearlo.