$
$ This danish translation is made by Arne Vajhøj (ARNE@KO.HHS.DK)
$ 30/12/94 partly from the swedish translation.
$

$quote "

$
$ Welcome message...
$

1 "Velkommen til Gophers vidunderlige verden!\n\n"
2 "Gopher har begrænsninger i sit anvendelsesområde og\n"
3 "har ingen garanti.  Kig venligst i filen 'Copyright'\n"
4 "som følger med distributionen.\n\n"

5 "Copyright 1991-2000 by the Regents of the University of Minnesota\n\n"
6 "Interrnet Gopher Information Client %s.%s patch%d (%s)\n"
7 "Tryk RETURN for at fortsætte\n"

$ 
$ Errors in .gopherrc
$
8 "Kan ikke skrive til gophers konfigurations fil!!"

$
$ Help for command line 
$

9 "Anvendelse: %s [-sSbDr] [-T type] [-p sti] [-t titel] [vært port]+\n"
10 "     -s        Sikker modus, brugere uden egen konto\n"
11 "     -S        Sikker modus, brugere med egen konto\n"
12 "     -p sti    Sti til første objekt\n"
13 "     -T type   Type for første objekt\n"
14 "     -i        Søgekriterium (for -T 7)\n"
15 "     -b        Bogmærke først\n"
16 "     -r        Fjernbrugere\n"
17 "     -D        Fejlfindingsmodus\n"

$
$ Sider help..
$

18 "Bladringshjælp"
19 "u, ^G, venstre  : Gå tilbage til menu"
20 "mellemrum, ned  : Gå til næste side"
21 "b, op           : Gå til foregående side"
22 "/               : Søg text"
23 "m               : send aktuelle dokument med email"
24 "s               : gem aktuelle dokument"
25 "p               : udskriv aktuelle dokument"
26 "D               : downloade aktuelle dokument"

$ 
$ New Application Menu
$ 
27 "Content-Type Navn"
28 "Applikation for visning"
29 "Applikation for udskrift"

$ 
$ Top Level Options
$ 
30 "Generelle indstilninger"
31 "Indstil applikationer for visning"
32 "Indstil applikationer for udskrift"
33 "Definer ny Content-Type"

$
$ Telnet connection
$
34 "Advarsel!!!!!, du forlader nu Internet"
35 "Gopher programmet og forbinder til en anden vært."
36 "Hvis du sidder fast så kan du presse ctrl og"
37 "] tasterne, og siden skrive quit"
38 "^ tasterne, og siden skrive q."
39 "Forbinder op %.40s, port %d med %s."
40 "Forbinder op %.40s med %s."
41 "Anvend kontonavnet \"%.40s\" for at logge ind"
$
$ HTML menu
$
42 "Advarsel!!!!!, du forlader nu Internet Gopher"
43 "klienten og forbinder dig op mod en HTML klient. Anvend"
44 "klientens hjælp, hvis du ikke kender programmets kommandoer."
45 "Når du forlader programmet, kommer du tilbage til"
46 "Internet Gopher klienten."
$
$ Other html msgs
$
47 "Beklager, denne klient understøtter ikke HTML"

$
$ gopher.c
$
48 "Beklager, kan ikke finde en administartor for dette objekt!"

$ 
$ Gripe form msgs
$ 
49 "Tryk på Enter for at sende din meddelelse."
50 "Tryk på Tab tasten ved enden af hver linie du skriver."
51 "Æmne"
52 "Problem"
53 "Kan ikke sende mail..."
54 "Sender mail til %s..."
55 "Ord at søge efter"
56 "  Side: %d/%d"
57 " for Hjælp, "
58 " for at Afslutte"
59 " for at gå en menu op"
60 "%s: Intet modtaget for hovedmenuen, kan ikke fortsætte\n"
61 "%sm%s for mail, %sD%s for at downloade, %ss%s for at gemme, eller %sp%s for udskrift:"
62 ", %sm%s for mail, %sD%s for at downloade:"
63 ", <m> for mail, <D> for at downloade:"
64 ",\n   <m> for mail, <D> for at downloade, <s> for at gemme, eller <p> for udskrift:"
65 "Adresse syntaxen forkert..."
66 "Fede søgord i den indbyggede bladrer"
67 "Bogmærke"
68 "Kan ikke åbne gammel fil.."
69 "Kan ikke skrive til SYS$SCRATCH!"
70 "Kan ikke skrive til /tmp biblioteket!"
71 "Kan ikke åbne den ønskede fil.."
72 "Kan ikke forbinde op til værten %.40s, port %d."
73 "Vælg en dokumenttyp"
74 "Vælg en metod til at downloade"
75 "Forbind til en ny Gopher Server"
76 "Forbind til anonymous FTP Server over en Gopher gateway"
77 "Forbind..."
78 "Forbindelse mislykkedes: %s."
79 "Forbindelse til vært mislykkedes."
80 "Kunde ikke kreere '%s'"
81 "Kunde ikke eksekvere %s"
82 "Kunde ikke finde text"
83 "Kunde ikke kreere en temporær fil!\n"
84 "Det var fejl ved udskrift, beklager..."
85 "Fejl ved anvendelse af TPU som bladrer!!!"
86 "Afslutte: u"
87 "Hent felt..."
88 "Filen overførtes ikke korrekt"
89 "Gopher biblioteks information"
90 "Gopher fejl"
91 "Gopher fil"
92 "Gopher hjælpe fil"
93 "Gopher objekt information"
94 "Gopher indstilninger"
95 "Gopher overførselsfejl"
96 "Hjælp: ?"
97 "Home Gopher server: %.59s"
98 "Vært at spørge:"
99 "Værtnavn"
100 "Værtnavn er ukendt."
101 "Jeg kan ikke forstå din terminal type\n"
102 "Internet Gopher information klient v%s.%s.%d"
103 "Link information"
104 "Lokal dato og tid"
105 "Send aktuelle dokument med mail:"
106 "Sender fil..."
107 "Sender fil til %s..."
108 "At se på viedeo er ikke implementerat"
109 "Navn på dette bogmærke:"
110 "Netværksfejl"
111 "Nej"
112 "Ingen bogmærker definerde"
113 "Ingen kommando for at se på dette er defineret!"
114 "Ingen brugere '%s'"
115 "SideNed: <MELLEMRUM>"
116 "SideNed: Mellemrum"
117 "SideOp: b"
118 "Port"
119 "Tryk "
120 "Tryk <RETURN> for at fortsætte"
121 "Tryk <RETURN> når du er klar\n"
122 "Udskriv aktuelle dokument"
123 "Problem ved skrivning"
124 "Virkelig afslutte (y/n) ?"
125 "Modtager bibliotek..."
126 "Modtager fil..."
127 "Modtager HTML side..."
128 "Modtager information..."
129 "Modtager MIME fil..."
130 "Modtager video..."
131 "Modtager lyd..."
132 "Modtager bildede..."
133 "Henter bibliotek..."
134 "Gå tilbage til Menu: u"
135 "Hovedmenu: "
136 "Gem til fil:"
137 "Gem liste af objekter til fil:"
138 "Gemmer fil..."
139 "Søg i bibliotekstitler:"
140 "Søgningen mislykkedes..."
141 "Søg i text:"
142 "Søger i text..."
143 "Søger..."
144 "Afgrænsning (valgfrit)"
145 "Vis værtens lokale dato og tid"
146 "Størrelse  Sprog         Dokumenttype\n"
147 "---------- ------------- ----------------------------\n"
148 "Beklager, kan ikke vise denne fil"
149 "Beklager, kan ikke slette øverste biblioteksniveau."
150 "Beklager, kan ikke downloade det der!"
151 "Beklager, kan ikke lave et bogmærke af bogmærker"
152 "Beklager, kan ikke gemme filer med dennee konto"
153 "Beklager, kan ikke spawnw med denne konto"
154 "Beklager, kan ikke gemme dette objekt til en fil!"
155 "Beklager, kan ikke udskrive dette dokument"
156 "Beklager, det findes ingen metode til at udskrive dette dokument"
157 "Beklager, telnet er ikke tilgængeligt."
158 "Beklager, denne maskine understøtter ikke billeder"
159 "Beklager, denne maskine understøtter ikke lyd"
160 "Beklager, tn3270 er ikke tilgængligt."
161 "Beklager, du har ikke lov til at udskrive filer"
162 "Beklager, du har ikke lov til at gemme filer"
163 "Beklager, du har ikke lov til at lave indstilninger i sikker modus."
164 "Beklager, du har ikke lov til at lave dette"
165 "Beklager, denne maskine understøtter ikke video"
166 "Beklager, findes intet program for denne type af fil...."
167 "Start af DCL mislykkedes!"
168 "Start af standard kommandofortolker (shell) mislykkedes!"
169 "Starter DCL subprocess.  Log ud for at returnere til Gopher.\n"
170 "Starter din standard kommandofortolker (shell). Skriv 'exit' for at returnere til Gopher.\n\n"
171 "Start din \"fangst\" nu...\n\n"
172 "Start downloadningen nu...\n"
173 "Problem med systemet, kan ikke sende mail"
174 "Filnavnet er:"
175 "Kan ikke kreere en netværkskanal (socket)."
176 "Unix \"rørkommando\" (pipe) er ønsket.  Kontroller dine indstilninger!"
177 "Ukendt fejl."
178 "Anvend '/' for at definere en søgning først..."
179 "Kig på objekt nummer: "
180 "Ja"
181 "Dit valg"
182 "\nDownloadning klar. %d bytes totalt, %d bytes/s\n"
183 "\nDownloadning kunde ikke fuldføres, beklager... \n"
184 "\nTryk %sRETURN%s for at fortsætte"
185 "\nTryk <RETURN> for at fortsætte"
186 "\n\nServer information\n"
187 "anonymous FTP vært"
188 "gopher: Intet modtaget for hovedmenuen, kan ikke fortsætte\n"
189 "'d'elete kommandoen er kun for bogmærker."
190 "^G      : Fortryd"
191 "OK: Enter"
192 "Tab, ^N : Flyt til næste felt"
193 "^P      : Flyt til forrige felt"
194 "^F      : Vis næste side"
195 "^B      : Vis forrige side"
196 "^-      : Hjælp (prøv ^/ eller ^7)"
197 "Enter   : Accepter"
198 "[Hjælp: ^-]  [Fortryd: ^G]"
199 "[Skift værdier: mellemrume]  [Vis værdier: l]"
200 "Vælg et punkt"
$
$ Choose menu..
$
201 "Form Hjælp"
202 "^G        : Fortryd"
203 "Down, ^N  : Flyt til næste valg"
204 "Up, ^P    : Flyt til forrige valg"
205 "Space, ^F : Vis næste side"
206 "b, ^B     : Vis forrige side"
207 "^         : Vis første side"
208 "$         : Vis sidste side"
209 "0-9       : Vælg specifik valg"
210 "Enter     : Vælg nuværende valg"
211 "Fortryd: ^G"
212 " (standard)"
213 "Valg Dialog Hjælp"
214 "Intet tilgængelig."
215 "Advarsel, forkert linie i gopherrc: %s\n"
216 "Aborterer gopher konfiguration gem!!"
217 "Tekst"
218 "En ny konfiguration er tilgængelig.  Brug den?"
219 "Standard system printer"
220 "ANSI tilknyttet printer"
221 "Print til"

$ Default language/region gopher+ view til choose

222 "Dk_DK"
$
$  Multilingual HELP files are now possible
$                      If CONF.H defines GOPHERHELP_SECURE, make sure you
$                      have a separate file for secure users' help that
$                      doesn't document features they're not allowed til use.
$  For VMS:
$
223 "GopherP_Dir:gopher_hlp.Dk"
224 "GopherP_Dir:gopher_hlp_secure.Dk"
$
$  For Unix:
$
225 "/usr/local/lib/gopher.hlp.Dk"
226 "/usr/local/lib/gopher.hlp_secure.Dk"
$
$ Gripe Enhancements
$
227 "Beklager,kan ikke sende mail i sikker modus"
228 "Kan ikke sende mail til nul address..."
229 "Med hensyn tilfølgende Gopher punkt:\n"
230 "Til"
231 "<Sidste linie>"

$
$ A message that asks the User if they speak this language..
$

232 "Taler du dansk?"

233 " Downloading %d bytes"
234 "    1200bps: %d minutter\n"
235 "    2400bps: %d minutter\n"
236 "   14400bps: %d minutter\n\n"

$
$ Add til post-sider option list in SecureMode and NoShellMode
$
237 ", %sD%s for at downloade:"
238 ", <D> for at downloade:"
