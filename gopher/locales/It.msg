$
$ Translation by Francesco Brunetta <Brunetta@CC.Uniud.It>
$             23-SEP-1994 (Gopher Client for VMS, 2.0-16)
$             Last upd.  04-MAY-1995 (Gopher Client for VMS 2.1.2)
$			 09-MAY-1995 (Gopher Client for VMS 2.1.3)
$ 
$   -------------------------------------------------------------------
$   Note:  is  my humble opinion that literal translation of some few words
$   (like "bookmark", "file" and "server") is not always  fair.  So  I
$   choose  to let them in English language. If you need, here's some
$   literal translation:
$
$        Bookmark ............... Segnalibro
$        File    ................ Archivio
$        Server  ................ Servitore, Servente
$        Directory .............. Elenco, Direttorio
$
$   Some  other  word  has been translated not litterally, in order to make
$   messages more  readable  (for  examples:  'client'  was  translated  to
$   "Applicazione", although a literal translation would be: "Cliente").
$   -------------------------------------------------------------------
$
$
$ Welcome message...
$

$quote "

1 "Benvenuto nel meraviglioso mondo di Gopher!\n\n"
2 "Sono previste limitazioni nell'uso di GOPHER, che\n"
3 "viene distribuito privo di garanzie. Pregasi consultare il\n"
4 "file 'Copyright' incluso nella distribuzione.\n\n"

5 "Copyright 1991-2000 by the Regents of the University of Minnesota\n\n"
6 "Applicazione Informativa per Internet %s.%s revisione %d (%s)\n"
7 "Premere RETURN per continuare\n"

$ 
$ Errors in .gopherrc
$
8 "Non posso accedere al file di configurazione di Gopher!!"

$
$ Help for command line 
$

9 "Utilizzo: %s [-sSbDr] [-T type] [-p path] [-t title] [hostname port]+\n"
10 "     -s      modalita` sicurezza, utenti senza proprio codice d'accesso\n"
11 "     -S      modalita` sicurezza, utenti con   proprio codice d'accesso\n"
12 "     -p path Percorso d'accesso all'elemento iniziale\n"
13 "     -T type Tipo dell'elemento iniziale\n"
14 "     -i      Parametro da ricercare (per -T 7)\n"
15 "     -b      Prima i <Bookmarks> \n"
16 "     -r      Utente remoto\n"
17 "     -D      Modalita` ricerca errori\n"

$
$ Pager help..
$

18 "Aiuto per la paginazione"
19 "u, ^G, SINISTRA : Ritorno al menu"
20 "spazio, GIU     : Passaggio a pagina successiva"
21 "b, SU           : Passaggio a pagina precedente"
22 "/               : Ricerca un testo"
23 "m               : Invia il corrente documento per posta elettronica"
24 "s               : Salva il corrente documento"
25 "p               : Stampa il corrente documento"
26 "D               : Scarica il corrente documento"

$ 
$ New Application Menu
$ 
27 "Nome del tipo di elemento"
28 "Comando per visualizzazione"
29 "Comando per stampa"

$ 
$ Top Level Options
$ 
30 "Opzioni generali"
31 "Configurazione Comandi per Visualizzazione"
32 "Configurazione Comandi per Stampa"
33 "Definizione nuovi tipi di elemento"

$
$ Telnet connection
$
34 "Attenzione!!!!!, stai per abbandonare il programma"
35 "Internet Gopher e per connetterti ad un altro host."
36 "Se hai problemi premi insieme i tasti Ctrl e"
37 "] e poi scrivi quit"
38 "^ e poi scrivi quit"
39 "Connessione a %.40s, porta %d usando %s."
40 "Connessione a %.40s usando %s."
41 "Usa il codice d'accesso \"%.40s\" per accedere al sistema"
$
$ HTML menu
$
42 "Attenzione!!!!!, stai per abbandonare l'applicazione GOPHER"
43 "e connetterti a un lettore HTML. Usa l'Aiuto del lettore HTML"
44 "se non hai familiarita` con i suoi comandi."
45 "All'uscita dal lettore, tornerai automaticamente"
46 "all'applicazione Gopher."
$
$ Other html msgs
$
47 "Spiacente, questa applicazione non supporta HTML"

$
$ gopher.c
$
48 "Spiacente, non riesco a trovare un amministratore per questo elemento"

$ 
$ Gripe form msgs
$ 
49 "Premi il tasto Enter per inviare il tuo messaggio."
50 "Premi il tasto Tab   al termine di ogni linea che inserisci."
51 "Soggetto"
52 "Problema"
53 "Non posso inviare il messaggio..."
54 "Spedizione messaggio a %s..."
55 "Parole da ricercare"
56 "  Pagina: %d/%d"
57 " per Aiuto, "
58 " per Chiudere"
59 " per menu precedente"
60 "%s: Non ho ricevuto niente per il menu principale, non posso proseguire\n"
61 "%sm%s per spedire per Posta, %sD%s per Scaricare, %ss%s per Salvare, %sp%s per Stampare:"
62 ", %sm%s per spedire per Posta, %sD%s per Scaricare:"
63 ", <m> per spedire per Posta, <D> per Scaricare:"
64 ",\n  <m> per spedire Posta,<D> per Scaricare, <s> per Salvare, <p> per Stampare:"
65 "Sintassi dell'indirizzo rifiutata..."
66 "Parole ricercate nel visualizzatore, in neretto:"
67 "<Bookmarks>"
68 "Non posso aprire il vecchio file..."
69 "Non posso scrivere in SYS$SCRATCH!!!"
70 "Non posso scrivere nella Directory /tmp !!!!"
71 "Non posso aprire il file richiesto..."
72 "Non posso connettermi al sistema %.40s, porta %d."
73 "Scegli un tipo di documento"
74 "Scegli un metodo per scaricare"
75 "Connessione ad un altro Gopher Server"
76 "Connessione a un Server FTP Anonimo tramite un sistema Gopher intermedio"
77 "     Connessione in corso..."
78 "Connessione fallita: %s."
79 "Connessione a sistema remoto fallita."
80 "Non posso creare '%s'"
81 "Non posso eseguire %s"
82 "Non riesco a trovare il testo"
83 "Non riesco a creare un file temporaneo!\n"
84 "C'e` un problema di stampa, spiacente..."
85 "Errore durante invocazione di TPU come visualizzatore!!!"
86 "Uscita: u"
87 "Estrazione campi..."
88 "File non trasferito correttamente"
89 "Informazioni sulla Directory Gopher"
90 "Errore di Gopher"
91 "File di Gopher"
92 "File di Aiuto di Gopher"
93 "Informazioni su elemento di Gopher"
94 "Opzioni di Gopher"
95 "Errore di Trasmissione di Gopher"
96 "Aiuto: ?"
97 "Gopher Server di partenza: %.59s"
98 "Sistema da interrogare:"
99 "Nome del sistema"
100 "Nome del sistema sconosciuto."
101 "Non posso stabilire il tipo del tuo terminale\n"
102 "Applicazione GOPHER per Informazioni in Internet v%s.%s.%d"
103 "Informazioni sul documento"
104 "Data e Orario locali"
105 "Spedizione per Posta del corrente documento a:"
106 "Spedizione per Posta del file..."
107 "Spedizione per Posta del file a %s..."
108 "Visualizzazione animata non ancora implementata"
109 "Nome per questo <bookmark>:"
110 "Errore in rete"
111 "No"
112 "Nessun <bookmark> definito"
113 "Nessun comando di visualizzazione assegnato a questa opzione!"
114 "Utente '%s' inesistente"
115 "Pagina Seguente:   <SPAZIO>"
116 "Pagina Seguente:   Spazio"
117 "Pagina precedente: b"
118 "Porta"
119 "Premere "
120 "Premere <RETURN> per continuare"
121 "Premere <RETURN> quando pronti\n"
122 "Stampa del corrente documento in corso..."
123 "Problemi di scrittura"
124 "Vuoi davvero uscire (y=SI/n=NO) ?"
125 "     Ricezione Directory in corso..."
126 "     File..."
127 "     Ricezione pagina HTML in corso..."
128 "     Ricezione Informazioni in corso..."
129 "     Ricezione file MIME in corso..."
130 "     Ricezione File d'Animazione in corso ..."
131 "     Ricezione Suono in corso..."
132 "     Ricezione Immagine in corso..."
133 "     Estrazione Directory in corso..."
134 "Ritorno al Menu: u"
135 "Menu principale: "
136 "Salva nel file:"
137 "Salva la lista di elementi nel file:"
138 "     Salvataggio File in corso..."
139 "Ricerca tra i titoli della Directory: "
140 "Ricerca fallita..."
141 "Ricerca nel testo:"
142 "     Ricerca nel testo in corso..."
143 "     Ricerca in corso..."
144 "Selettore (Opzionale)"
145 "Mostra Data e Orario locali del sistema remoto"
146 "Dimensione Linguaggio    Tipo documento\n"
147 "---------- ------------- ----------------------------\n"
148 "Spiacente, non posso visualizzare questo file:"
149 "Spiacente, non posso cancellare il menu di primo livello."
150 "Spiacente, non posso scaricare!"
151 "Spiacente, non posso fare un <bookmark> con dei <bookmark>!"
152 "Spiacente, non posso salvare files con questo codice d'accesso"
153 "Spiacente, non posso aprire sottoprocessi con questo codice d'accesso"
154 "Spiacente, non posso salvare questo elemento in un file!"
155 "Spiacente, non posso stampare questo documento"
156 "Spiacente, non conosco meccanismi per stampare questo documento"
157 "Spiacente, telnet non e` disponibile."
158 "Spiacente, questa macchina non supporta immagini"
159 "Spiacente, questa macchina non supporta suoni"
160 "Spiacente, tn3270 non e` disponibile"
161 "Spiacente, non ti e` concesso stampare files"
162 "Spiacente, non ti e` concesso salvare files"
163 "Spiacente, non ti e` concesso modificare opzioni in modalita` sicurezza"
164 "Spiacente, non ti e` concesso fare cio`"
165 "Spiacente, questa macchina non sopporta animazioni"
166 "Spiacente, niente visualizzatore per il file..."
167 "Fallita la creazione del sottoprocesso DCL!"
168 "Fallito l'accesso alla shell di default!"
169 "Attivazione sottoprocesso DCL. Usa Logout per tornare a Gopher.\n"
170 "Passaggio alla shell di default. Usa 'exit' per tornare a Gopher.\n\n"
171 "Comincia pure a recuperare il testo...\n\n"
172 "Comincia pure a scaricare...\n"
173 "Il sistema ha problemi, non posso inviare messaggio via Posta Elettronica"
174 "Il nome del file e`:"
175 "Allocazione <socket> impossibile"
176 "Necessario un Piping di Unix. Controlla la tua configurazione!"
177 "Errore sconosciuto."
178 "Usa prima '/' per dirmi cosa cercare..."
179 "Visita l'elemento numero:"
180 "Si`"
181 "La tua scelta"
182 "\nScaricamento Completo. %d bytes in totale , %d bytes/sec\n"
183 "\nNon posso completare lo scaricamento, spiacente...\n"
184 "\nPremere %sRETURN%s per continuare"
185 "\nPremerem <RETURN>  per continuare"
186 "\n\nInformazioni sul Server\n"
187 "anonymous FTP Host"
188 "gopher: non ho ricevuto niente per il menu principale, non posso proseguire\n"
189 "Il comando 'd'elete (cancellazione) e` solo per i <bookmark>."
190 "^G      : Annulla"
191 "OK: Enter"
192 "Tab, ^N : Campo successivo"
193 "^P      : Campo precedente"
194 "^F      : Visualizza la prossima pagina"
195 "^B      : Visualizza la pagina precedente"
196 "^-      : Aiuto (forse funzionano anche ^/ o ^7)"
197 "Enter   : Accetta"
198 "[Aiuto: ^-]  [Annulla: ^G]"
199 "[Cicla i valori: Space]  [Mostra i Valori: l]"
200 "Scegli un elemento"
$
$ Choose menu..
$
201 "Come usare questo modulo"
202 "^G        : Annulla"
203 "Down, ^N  : Vai al prossimo elemento"
204 "Up, ^P    : Vai all'elemento precedente"
205 "Space, ^F : Mostra la prossima pagina"
206 "b, ^B     : Mostra la pagina precedente"
207 "^         : Mostra la prima pagina"
208 "$         : Mostra l'ultima pagina"
209 "0-9       : Seleziona una scelta specifica"
210 "Enter     : Seleziona l'elemento corrente"
211 "Annulla: ^G"
212 " (per default)"
213 "Aiuto per videata di scelte multiple:"
214 "Nessuna informazione disponibile"
215 "Attenzione, linea errata in gopherrc: %s\n"
216 "Interruzione del salvataggio della configurazione di Gopher!"
217 "Testo"
$ 
$ Default language/region gopher+ view to choose
$
218 "Una nuova configurazione e` disponibile. La carico?"
219 "Stampante di sistema di default"
220 "Stampante ANSI"
221 "Stampa su"

$ Default language/region gopher+ view to choose

222 "It_IT"
$
$  Multilingual HELP files are now possible
$			If CONF.H defines GOPHERHELP_SECURE, make sure you
$			have a separate file for secure users' help that 
$			doesn't document features they're not allowed to use.
$  For VMS:
$
223 "GopherP_Dir:gopher_hlp.It"
224 "GopherP_Dir:gopher_hlp_secure.It"
$
$  For Unix:
$
225 "/usr/local/lib/gopher.hlp.It"
226 "/usr/local/lib/gopher.hlp_secure.It"
$
$ Gripe Enhancements
$
227 "Spiacente, non posso inviare commenti in modalita` sicurezza"
228 "Non posso inviare messaggi per Posta a un indirizzo nullo..."
229 "Relativamente ai seguenti elementi Gopher:\n"
230 "A"
231 "<Ultima Linea>"

$
$ A message that asks the User if they speak this language..
$
232 "Parliamo italiano?"

233 " Sto scaricando  %d bytes"
234 "       1200bps: %d minuti\n"
235 "       2400bps: %d minuti\n"
236 "      14400bps: %d minuti\n\n"

$
$ Add to post-pager option list in SecureMode and NoShellMode
$
237 ", %sD%s per Scaricare:"
238 ", <D> per Scaricare:"

$
$ A message for when there is no language choice..
$
239 "Spiacente, nessun linguaggio da scegliere"


