$
$ This Swedish Translation is made by Johan Svensson <Johan@ecsdg.lu.se>,
$ EkonomiCentrum Software Development Group, Lund University, Sweden.
$ Please send comments and suggestions to him about this file!
$ 1994-06-08
$ ---
$ This file may be used freely as long as the text above follows the work.
$

$quote "

$
$ Welcome message...
$

1 "Välkommen till  Gophers underbara värld!\n\n"
2 "Gopher har begränsningar i sitt användningsområde och\n"
3 "har ingen garanti.  Titta vänligen i filen 'Copyright'\n"
4 "som följer med distributionen.\n\n"

5 "Copyright 1991-2000 by the Regents of the University of Minnesota\n\n"
6 "Internet Gopher Information Client %s.%s patch%d (%s)\n"
7 "Tryck RETURN för att fortsätta\n"

$ 
$ Errors in .gopherrc
$
8 "Kan inte skriva till gophers inställningsfil!!"

$
$ Help for command line 
$

9 "Användning: %s [-sSbDr] [-T typ] [-p sökväg] [-t titel] [datorvärd port]+\n"
10 "     -s        Säkert läge, användare utan eget konto\n"
11 "     -S        Säkert läge, användare med  eget konto\n"
12 "     -p sökväg Sökväg till första objekt\n"
13 "     -T typ    Första objektets typ\n"
14 "     -i        Sökkriterium (för -T 7)\n"
15 "     -b        Bokmärken först\n"
16 "     -r        Fjärranvändare\n"
17 "     -D        Avlusningsläge\n"

$
$ Pager help..
$

18 "Bläddringshjälp"
19 "u, ^G, vänster  : Återgå till meny"
20 "mellanslag, ner : Gå till nästa sida"
21 "b, upp          : Gå till föregående sida"
22 "/               : Sök text"
23 "m               : skicka aktuellt dokument med email"
24 "s               : spara aktuellt dokument"
25 "p               : skriv ut aktuellt dokument"
26 "D               : ladda ner aktuellt dokument"

$ 
$ New Application Menu
$ 
27 "Content-Type Namn"
28 "Applikation för visning"
29 "Applikation för utskrift"

$ 
$ Top Level Options
$ 
30 "Allmänna inställningar"
31 "Ställ in applikationer för visning"
32 "Ställ in applikationer för utskrift"
33 "Definiera ny Content-Type"

$
$ Telnet connection
$
34 "Varning!!!!!, du håller på att lämna Internet"
35 "Gopher programmet och koppla upp dig mot en annan datorvärd."
36 "Om du kör fast kan du hålla ner ctrl och"
37 "] tangenterna, och sedan skriva quit"
38 "^ tangenterna, och sedan skriva q."
39 "Kopplar upp %.40s, port %d med %s."
40 "Kopplar upp %.40s med %s."
41 "Använd kontonamnet \"%.40s\" för att logga in"
$
$ HTML menu
$
42 "Varning!!!!!, du håller på att lämna Internet Gopher"
43 "Klienten och koppla upp dig mot en HTML Klient. Använd"
44 "klientens Hjälp om du inte kan programmets kommandon."
45 "När du lämnar programmet, kommer du tillbaks till"
46 "Internet Gopher Klienten."
$
$ Other html msgs
$
47 "Tyvärr, denna klient stödjer inte HTML"

$
$ gopher.c
$
48 "Kan tyvärr inte hitta en administratör för detta objekt!"

$ 
$ Gripe form msgs
$ 
49 "Tryck på Enter för att skicka ditt meddelande."
50 "Tryck på Tab tangenten vid slutet av varje rad."
51 "Ämne"
52 "Problem"
53 "Kan inte skicka datorpost..."
54 "Skickar datorpost till %s..."
55 "Ord att leta efter"
56 "  Sida: %d/%d"
57 " för Hjälp, "
58 " för att Avsluta"
59 " för att gå upp en meny"
60 "%s: Erhöll inget för huvudmenyn, kan inte fortsätta\n"
61 "%sm%s för datorpost, %sD%s för att ladda ner, %ss%s för att spara, eller %sp%s för utskrift:"
62 ", %sm%s för datorpost, %sD%s för att ladda ner:"
63 ", <m> för datorpost, <D> för att ladda ner:"
64 ",\n   <m> för datorpost, <D> för att ladda ner, <s> för att spara, eller <p> för utskrift:"
65 "Adress syntaxen felaktig..."
66 "Feta sökord i den inbyggda bläddraren"
67 "Bokmärken"
68 "Kan inte öppna gammal fil.."
69 "Kan inte skriva till SYS$SCRATCH!"
70 "Kan inte skriva till /tmp biblioteket!"
71 "Kan inte öppna begärd fil.."
72 "Kan inte koppla upp datorvärden %.40s, port %d."
73 "Välj en dokumenttyp"
74 "Välj en metod att ladda ner"
75 "Koppla upp en ny Gopher Server"
76 "Koppla upp anonymous FTP Server över en Gopher gateway"
77 "Kopplar upp..."
78 "Uppkoppling misslyckades: %s."
79 "Uppkoppling mot datorvärd misslyckades."
80 "Kunde inte skapa '%s'"
81 "Kunde inte exekvera %s"
82 "Kunde inte hitta text"
83 "Kunde inte skapa en temporär fil!\n"
84 "Det blev fel vid utskrift, ledsen..."
85 "Fel vid anrop av TPU som bläddrare!!!"
86 "Avsluta: u"
87 "Hämtar fält..."
88 "Filen överfördes inte korrekt"
89 "Gopher Biblioteks Information"
90 "Gopher Fel"
91 "Gopher Fil"
92 "Gopher Hjälp Fil"
93 "Gopher Objekt Information"
94 "Gopher Inställningar"
95 "Gopher Överföringsfel"
96 "Hjälp: ?"
97 "Hem Gopher server: %.59s"
98 "Datorvärd att fråga:"
99 "Datornamn"
100 "Datornamn är okänt."
101 "Jag kan inte förstå din terminal typ\n"
102 "Internet Gopher Information Klient v%s.%s.%d"
103 "Länk Information"
104 "Lokalt Datum och Tid"
105 "Skicka aktuellt dokument med datorpost:"
106 "Skickar fil..."
107 "Skickar fil till %s..."
108 "Att titta på video är inte implementerat"
109 "Namn på detta bokmärke:"
110 "Nätverksfel"
111 "Nej"
112 "Inga bokmärken definierade"
113 "Inget kommando för att titta på detta är definierat!"
114 "Användare '%s' finns ej"
115 "SidaNer: <MELLANSLAG>"
116 "SidaNer: Mellanslag"
117 "SidaUpp: b"
118 "Port"
119 "Tryck "
120 "Tryck <RETURN> för att fortsätta"
121 "Tryck <RETURN> när du är klar\n"
122 "Skriv ut aktuellt dokument"
123 "Problem vid Skrivning"
124 "Verkligen Avsluta (y/n) ?"
125 "Tar emot Bibliotek..."
126 "Tar emot Fil..."
127 "Tar emot HTML sida..."
128 "Tar emot Information..."
129 "Tar emot MIME Fil..."
130 "Tar emot Video..."
131 "Tar emot Ljud..."
132 "Tar emot Bild..."
133 "Hämtar Bibliotek..."
134 "Återgå till Meny: u"
135 "Huvudmeny: "
136 "Spara till fil:"
137 "Spara lista av objekt till fil:"
138 "Sparar Fil..."
139 "Sök i bibliotekstitlar:"
140 "Sökningen misslyckades..."
141 "Sök i text:"
142 "Söker i text..."
143 "Söker..."
144 "Avgränsare (valfritt)"
145 "Visa datorvärdens lokala datum och tid"
146 "Storlek    Språk         Dokumenttyp\n"
147 "---------- ------------- ----------------------------\n"
148 "Tyvärr, kan inte visa denna fil"
149 "Tyvärr, kan inte radera översta biblioteksnivån."
150 "Tyvärr, kan inte ladda ner det där!"
151 "Tyvärr, kan inte göra ett bokmärke av bokmärken"
152 "Tyvärr, kan inte spara filer med detta konto"
153 "Tyvärr, kan inte köra med detta konto"
154 "Tyvärr, kan inte spara objektet till en fil!"
155 "Tyvärr, kan inte skriva ut detta dokument"
156 "Tyvärr, det finns ingen metod att skriva ut detta dokument"
157 "Tyvärr, telnet är inte tillgängligt."
158 "Tyvärr, denna maskin stödjer inte bilder"
159 "Tyvärr, denna maskin stödjer inte ljud"
160 "Tyvärr, tn3270 är inte tillgängligt."
161 "Tyvärr, du har inte behörighet att skriva ut filer"
162 "Tyvärr, du har inte behörighet att spara filer"
163 "Tyvärr, du har inte behörighet att göra inställningar i Säkert Läge."
164 "Tyvärr, du har inte behörighet att göra detta"
165 "Tyvärr, denna maskin stödjer inte videos"
166 "Tyvärr, finns inget program för denna typ av fil...."
167 "Start av DCL misslyckades!"
168 "Start av standard kommandotolk (shell) misslyckades!"
169 "Startar DCL delprocess.  Logga ut för att återvända till Gopher.\n"
170 "Startar din standard kommandotolk (shell). Skriv 'exit' för att återvända till Gopher.\n\n"
171 "Starta din \"fångst\" nu...\n\n"
172 "Starta nedladdningen nu...\n"
173 "Problem med systemet, kan inte skicka datorpost"
174 "Filnamnet är:"
175 "Kan inte skapa en nätverkskanal (socket)."
176 "Unix \"rörkommando\" (pipe) är begärt.  Kontrollera dina inställningar!"
177 "Okänt fel."
178 "Använd '/' för att definiera en sökning först..."
179 "Titta på objekt nummer: "
180 "Ja"
181 "Ditt val"
182 "\nNedladdning klar. %d bytes totalt, %d bytes/s\n"
183 "\nNedladdning kunde inte slutföras, ledsen... \n"
184 "\nTryck %sRETURN%s för att fortsätta"
185 "\nTryck <RETURN> för att fortsätta"
186 "\n\nServer Information\n"
187 "anonymous FTP datorvärd"
188 "gopher: Erhöll inget för huvudmenyn, kan inte fortsätta\n"

$ Default language/region gopher+ view to choose

222 "Sv_SV"

232 "Talar du svenska?"

$
$ Add to post-pager option list in SecureMode and NoShellMode
$
237 ", %sD%s för att ladda ner:"
238 ", <D> för att ladda ner:"

