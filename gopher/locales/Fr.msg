$
$ Translation courtesy of Sylvie St. Georges <St-Georges.Sylvie@uqam.ca>
$ Fixes by Eddy.Beliveau@hec.ca
$


$
$ Welcome message...
$

$quote "

1 "Bienvenue dans l'univers merveilleux de Gopher!\n\n"
2 "Gopher ne comprend aucune garantie et son utilisation n'est pas\n"
3 "sans restriction. S.V.P. se référer au fichier 'Copyright'\n"
4 "inclus dans la distribution.\n\n"

5 "Copyright 1991-2000 by the Regents of the University of Minnesota\n\n"
6 "Client d'information Gopher pour l'Internet %s.%s révision%d (%s)\n"
7 "Appuyez sur <RETOUR> pour continuer"

$ 
$ Errors in .gopherrc
$
8 "Impossible d'écrire dans le fichier de configuration de gopher!!"

$
$ Help for command line 
$

9 "Usage: %s [-sSbDr] [-T type] [-p chemin] [-t titre] [serveur] [port]+\n"
10 "     -s        Mode sécuritaire, utilisateurs sans code d'accès\n"
11 "     -S        Mode sécuritaire, utilisateurs avec un code d'accès\n"
12 "     -p chemin Chemin d'accès pour le premier écran\n"
13 "     -T type   Type du premier élément\n" 
14 "     -i        Paramètre de recherche (pour -T 7)\n"
15 "     -b        Signets comme écran d'entrée\n"
16 "     -r        Utilisateur avec accès à distance\n"
17 "     -D        Mode d'épuration\n"

$
$ Pager help..
$

18 "Aide pour le défileur d'écran"
19 "u, ^G, flèche vers la gauche          : Retourne au menu"
20 "barre d'espacement, flèche vers le bas: Affiche la page suivante"
21 "b, flèche vers le haut                : Affiche la page précédente"
22 "/                                     : Active la recherche"
23 "m                                     : Envoie le document par courrier"
24 "s                                     : Sauvegarde le document"
25 "p                                     : Imprime le document"
26 "D                                     : Active le téléchargement"

$ 
$ New Application Menu
$ 
27 "Type du contenu"
28 "Progamme de visualisation"
29 "Programme d'impression"

$ 
$ Top Level Options
$ 
30 "Options générales"
31 "Configuration pour la visualisation"
32 "Configuration pour l'impression"
33 "Nouveau type de contenu" 

$
$ Telnet connection
$
34 "Attention!!!!!, vous allez quitter le programme"
35 "Gopher pour vous connecter sur un autre ordinateur."
36 "En cas de problème, pressez simultanément les touches"
37 "Ctrl et ] puis tapez quit."
38 "Ctrl et ^ puis tapez q."
39 "Connexion à %.40s, port %d par %s."
40 "Connexion à %.40s par %s."
41 "Au login, utilisez le nom d'usager \"%.40s\""
$
$ HTML menu
$
42 "Attention!!!!!, vous allez quitter le client Gopher"
43 "pour vous connecter à un lecteur de fichiers HTML." 
44 "Consultez l'aide HTML pour une description des commandes."
45 "En quittant le lecteur HTML, vous retournerez au client"
46 "Gopher."
$
$ Other html msgs
$
47 "Désolé, ce client ne supporte pas HTML"

$
$ gopher.c
$
48 "Désolé, aucun administrateur pour cet élément!"

$ 
$ Gripe form msgs
$ 
49 "Appuyez sur <RETOUR> pour envoyer votre message."
50 "Appuyez sur <TAB> à la fin de chaque ligne que vous tapez."
51 "Sujet"
52 "Problème"
53 "Impossible d'envoyer du courrier..."
54 "Commentaires envoyés à %s..."
55 "Rechercher les mots-clés:"
56 "  Page: %d/%d"
57 " pour Aide, "
58 " pour Quitter"
59 " pour menu Précédent"
60 "%s: Aucune information disponible dans le menu principal,\n impossible de continuer\n"
61 "%sm%s courrier, %sD%s télécharger, %ss%s sauvegarder, ou %sp%s"
62  ", %sm%s courrier, %sD%s télécharger:"
63 ", <m> courrier, <D> télécharger:"
64 ",\n   <m> courrier, <D> télécharger, <s> sauvegarder, ou <p> imprimer:"
65 "Erreur de syntaxe dans l'adresse..."
66 "Mots recherchés apparaissant en caractères gras"
67 "Menu des signets"
68 "Impossible d'ouvrir l'ancien fichier.."
69 "Impossible d'écrire dans SYS$SCRATCH!"
70 "Impossible d'écrire dans le répertoire /tmp!"
71 "Impossible d'ouvrir le fichier.."
72 "Connexion à %.40s, port %d impossible."
73 "Choisissez un type de document"
74 "Choisissez le mode de téléchargement:"
75 "Connexion à un autre serveur Gopher"
76 "Connexion à un serveur FTP anonymous par la passerelle Gopher"
77 "Connexion en cours..."
78 "Problème de connexion: %s."
79 "Problème de connexion avec l'autre ordinateur."
80 "Impossible de créer '%s'"
81 "Impossible d'exécuter %s"
82 "Recherche infructueuse"
83 "Impossible de créer le fichier tmp!\n"
84 "Désolé! Problème d'impression..."
85 "Erreur dans l'appel de TPU comme défileur d'écran!!!"
86 "Sortie: u"
87 "Champs disponibles..."
88 "Erreur survenue pendant le transfert"
89 "Information sur un répertoire Gopher"
90 "Erreur Gopher"
91 "Fichier Gopher"
92 "Fichier d'aide Gopher"
93 "Information sur un élément Gopher"
94 "Options pour Gopher"
95 "Erreur de transmission Gopher"
96 "Aide: ?"
97 "Serveur Gopher primaire: %.59s"
98 "Ordinateur à interroger:"
99 "Nom du serveur"
100 "Ce nom d'ordinateur est inconnu."
101 "Je ne peux identifier votre type de terminal\n"
102 "Client d'information Gopher pour l'Internet v%s.%s.%d"
103 "Information sur le lien"
104 "Date et heure locales"
105 "Envoi du document par courrier à:"
106 "Envoi du fichier par courrier..."
107 "Envoi du fichier par courrier à %s..."
108 "Visualisation pour l'animation non disponible"
109 "Nom pour ce signet:"
110 "Erreur de réseau"
111 "Non"
112 "Aucun signet n'est défini"
113 "Aucune commande de visualisation n'a été assignée pour cette option!"
114 "Usager '%s' inconnu"
115 "PageSuivante: <ESPACE>"
116 "PageSuivante: Espace"
117 "PagePrécédente: b"
118 "Port"
119 "Appuyez sur "
120 "Appuyez sur <RETOUR> pour continuer"
121 "Appuyez sur <RETOUR> lorsque vous serez prêt\n"
122 "Appuyez sur <RETOUR> pour continuer\n"
123 "Impression en cours"
124 "Désirez-vous quitter (Y=oui,N=non) ?"
125 "Réception du répertoire..."
126 "Réception du fichier..."
127 "Réception de la page HTML..."
128 "Réception de l'information.."
129 "Réception du fichier MIME..."
130 "Réception du fichier d'animation..."
131 "Réception du fichier de son..."
132 "Réception du fichier graphique..."
133 "Recherche du répertoire..."
134 "Retour au menu: u"
135 "Menu principal: "
136 "Sauvegarder dans le fichier:"
137 "Menu sauvegardé dans le fichier:"
138 "Sauvegarde du fichier..."
139 "Recherche dans le menu pour:"
140 "Recherche infructueuse..."
141 "Recherche dans le texte pour:"
142 "Recherche le texte..."
143 "Recherche..."
144 "Sélecteur (Optionnel)"
145 "Affichage de la date et de l'heure locales d'un site"
146 "Taille     Langage       Type de document\n"
147 "---------- ------------- ----------------------------\n"
148 "Désolé, impossible de visualiser ce fichier"
149 "Désolé, ce dernier élément ne peut être effacé."
150 "Désolé, impossible à télécharger!"
151 "Désolé, le menu des signets ne peut être défini comme un nouveau signet"
152 "Désolé, il ne vous est pas permis de sauvegarder des fichiers"
153 "Désolé, cet élément ne peut être sauvegardé!"
154 "Désolé, cette option ne vous est pas permise"
155 "Désolé, impossible d'imprimer ce document"
156 "Désolé, méthode d'impression non disponible pour ce document"
157 "Désolé, telnet n'est pas disponible."
158 "Désolé, ce client ne supporte pas les fichiers images"
159 "Désolé, cet ordinateur ne supporte pas les fichiers sonores"
160 "Désolé, tn3270 n'est pas disponible."
161 "Désolé, il ne vous est pas permis d'imprimer des fichiers"
162 "Désolé, il ne vous est pas permis de sauvegarder des fichiers"
163 "Désolé, il ne vous est pas permis de modifier les options."
164 "Désolé, cette option ne vous est pas permise"
165 "Désolé, cet ordinateur ne supporte pas l'animation"
166 "Désolé, aucun programme de visualisation pour ce fichier..."
167 "Impossible d'accéder au DCL!"
168 "Impossible d'accéder au shell implicite!"
169 "Passage au DCL. Quittez (logout) pour revenir à Gopher.\n"
170 "Passage au shell implicite.  Tapez 'exit' pour revenir à Gopher.\ n\n"
171 "Préparez la réception pour la saisie d'écran ...\n\n"
172 "Préparez la réception pour le téléchargement...\n"
173 "Problème de système, impossible d'envoyer du courrier"
174 "Le nom du fichier est:"
175 "Problème d'allocation de port."
176 "Redirection Unix demandée. Vérifiez votre configuration!"
177 "Erreur inconnue."
178 "Utilisez '/' pour une première recherche..."
179 "Afficher le numéro: "
180 "Oui"
181 "Votre choix"
182 "\nTéléchargement complété. %d octets au total, %d octets/sec\n"
183 "\nDésolé, téléchargement non complété... \n"
184 "\nAppuyez sur %sRETOUR%s pour continuer"
185 "\nAppuyez sur <RETOUR> pour continuer"
186 "\n\nServeur d'information\n"
187 "Site FTP anonymous"
188 "gopher: Aucune information disponible dans le menu principal,\nimpossible de continuer\n"
189 "La commande 'd'elete n'affecte que les signets"
190 "^G      : Annuler"
191 "OK: Retour"
192 "Tab, ^N : Champ suivant"
193 "^P      : Champ précédent"
194 "^F      : Écran suivant"
195 "^B      : Écran précédent"
196 "^_      : Aide (^/ ou ^7)"
197 "Retour  : Accepter"
198 "[Aide: ^-]  [Annuler: ^G]"
199 "[Valeurs(défilement): Espace]  [Valeurs(liste): l]"
200 "Choisissez"
$
$ Choose menu...
$
201 "Aide pour formulaire"
202 "^G                      : Annuler"
203 "Flèche vers le bas, ^N  : Choix suivant"
204 "Flèche vers le haut, ^N : Choix précédent"
205 "Espace, ^F              : Écran suivant"
206 "b, ^B                   : Écran précédent"
207 "^                       : Écran initial"
208 "$                       : Écran"
209 "0-9                     : Sélection d'un élément"
210 "Retour                  : Sélection de l'élément courant"
211 "Annuler: ^G"
212 " (par défaut)"
213 "Aide pour écran à choix multiples"
214 "Aucune information disponible."
215 "Attention, mauvaise entrée dans gopherrc: %s\n"
216 "Sauvegarde de la configuration interrompue!!"
217 "Texte"
218 "Désirez-vous la nouvelle configuration (Y=oui,N=non)"
219 "Imprimante du système"
220 "Imprimante esclave"
221 "Imprimer vers"

$ Default language/region gopher+ view to choose
222 "Fr_FR"


$  Multilingual HELP files are now possible
$                      If CONF.H defines GOPHERHELP_SECURE, make sure you
$                      have a separate file for secure users' help that
$                      doesn't document features they're not allowed to use.
$  For VMS:
$
223 "GopherP_Dir:gopher_hlp.Fr"
224 "GopherP_Dir:gopher_hlp_secure.Fr"
$
$  For Unix:
$
225 "/usr/local/lib/gopher.hlp.Fr"
226 "/usr/local/lib/gopher.hlp_secure.Fr"
$
$ Gripe Enhancements
$
227 "Désolé, on ne peut envoyer vos commentaires à l'administrateur"
228 "On ne peut envoyer du courrier à une adresse non définie"
229 "En ce qui concerne l'élément Gopher suivant:\n"
230 "A"
231 "<Dernière ligne>"

232 "Parlez-vous Francais?"

233 "Téléchargement %d octets"
234 "    1200 bauds %d minutes\n"
235 "    2400 bauds %d minutes\n"
236 "   14400 bauds %d minutes\n"
