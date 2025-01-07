#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // pour time()

//////////////////////////////////////////
// Constantes:

// Option pour tourner l’échiquier lorsque le joueur noir est en train de jouer:
#define TOURNER_ECHIQUIER false

// Contenu d’une case vide de l’échiquier:
#define CASE_VIDE ' '

// L’échiquier sera modélisé par un tableau 2D de caractères, de taille NxN.
#define N 8

// Nombre maximal de pièces d'une couleur pouvant être capturées.
#define NB_MAX_CAPTURES 15

// Nombre maximal de coup possible pour une pièce. Ne pas modifier:
#define NB_MAX_DEPL (4*(N)-5)

// Encodage des couleurs d'une case de l'échiquier:
#define C_BLANC 0
#define C_NOIR  1
#define C_VIDE  2

// Encodage de l'état en cours d'une partie:
#define NON_TERMINEE  0
#define DEFAITE_BLANC 1
#define DEFAITE_NOIR  2
#define PARTIE_NULLE  3

// Encodage des pièces par un caractère - majuscules pour
// le joueur blanc et minuscules pour le joueur noir:
// Tour:     'T' ou 't'
// Fou:      'F' ou 'f'
// Dame:     'D' ou 'd'
// Roi:      'R' ou 'r'
// Cavalier: 'C' ou 'c'
// Pion:     'P' ou 'p'

//////////////////////////////////////////
// Prototypes des fonctions fournies:

// Ne pas modifier les prototypes ci-dessous.
// Si l'on souhaite implémenter des fonctionnalités non exigées, on pourra
// faire cela en ajoutant des variables globales et de nouvelles fonctions.

void effaceConsole(void);
void afficheCaseEnCouleur(char c, int couleurEnEchec);

//////////////////////////////////////////
// Prototypes des fonctions à implémenter:

// Initialisation et affichage:
bool estDansGrille(int ligne, int colonne);
bool estCaseVide(const char grille[N][N], int ligne, int colonne);
void videGrille(char grille[N][N]);
void copieGrille(char copie[N][N], const char grille[N][N]);
void initialiseEchiquier(char grille[N][N]);
void afficheEchiquier(const char grille[N][N], int num_coup, size_t temps_debut);

// Gestion des couleurs, saisie d'un coup:
int couleurAdverse(int couleur);
int trouveCouleur(const char grille[N][N], int ligne, int colonne);
bool estMajuscule(char c);
bool convertitEnCoordonnees(const char notation[2], int coordonnees[2]);
void saisieCoup(char grille[N][N]);

// Implémentation du jeu d'échec:
void trouvePositionRoi(const char grille[N][N], int positionRoi[2], int couleur);
void realiseCoup(char grille[N][N], const int depart[2], const int arrivee[2]);
bool realiseCoupSiValide(char grille[N][N], const int depart[2], const int arrivee[2]);
bool estEnEchec(const char grille[N][N], int couleur);
bool estCoupValide(const char grille[N][N], const int depart[2], const int arrivee[2]);
bool estDeplacementValide(const char grille[N][N], const int depart[2], const int arrivee[2]);
int partieTerminee(const char grille[N][N], int couleur);
int listeDeplacementsValides(const char grille[N][N], const int depart[2], int deplacements[NB_MAX_DEPL][2]);


int main(void)
{
	int Etatpartie = NON_TERMINEE;
	int num_coup = 0;
	
	char grille[N][N];
	int couleur = C_VIDE;
	char grille[N][N];
	const size_t temps_debut = time(NULL); // temps de départ

	initialiseEchiquier(grille);
	while (Etatpartie == NON_TERMINEE){ //nom de la fonction a modifier au bsn de la fonction partieTerminee
		afficheEchiquier (grille, num_coup, temps_debut);
		saisieCoup(grille);
		partieTerminee(grille, couleur);
	}
	afficheEchiquier(grille, num_coup, temps_debut);

	if (Etatpartie == DEFAITE_BLANC){ 
		printf("Les noirs ont gagné\n"); //nom de la fonction a modifier au bsn de la fonction partieTerminee
	}
	else if (Etatpartie == DEFAITE_NOIR){ //nom de la fonction a modifier au bsn de la fonction partieTerminee
		printf("Les blancs ont gagné\n");
	}
	else if (Etatpartie == PARTIE_NULLE){ //nom de la fonction a modifier au bsn de la fonction partieTerminee
		printf("Partie nulle\n");
	}
	
	
	return 0;
}

//////////////////////////////////////////
// Fonctions fournies:

void effaceConsole(void)
{
	if (system("clear") != 0) {
		printf("La console n'a pas pu etre effacee.\n");
	}
}

// Si personne n'est en échec, passer pour couleur C_VIDE.
void afficheCaseEnCouleur(char c, int couleurEnEchec)
{
	int couleurChar = estMajuscule(c) ? 2 : 4; // jaune ou mauve clair
	if ((c == 'R' && couleurEnEchec == C_BLANC) || (c == 'r' && couleurEnEchec == C_NOIR)) { // roi en échec
		couleurChar = 0; // rouge
	}
	if (c == CASE_VIDE) {
		printf("%c", CASE_VIDE);
	}
	else {
		printf("\e[1;%dm%c\e[0m", couleurChar+91, c);
	}
}
bool estDansGrille(int ligne, int colonne)
{
	scanf("%d%d", &ligne, &colonne);
	if (ligne >= 0 && ligne < N && colonne >= 0 && colonne < N) {
		return true;
	}
	else {
		return false;
	}
}

bool estCaseVide(const char grille[N][N], int ligne, int colonne){
	if (grille[ligne][colonne] == CASE_VIDE) {
		return true;
	}
	else {
		return false;
	}

}

void videGrille(char grille[N][N]) {
	for (int i=0; i < N; i++) {
		for (int j=0; j < N; j++) {
			grille[i][j] = CASE_VIDE;
		}
	}
}


void saisieCoup(char grille[N][N]){
	  char depart[2], arrivee[2];
  int coordDepart[2], coordArrivee[2], notation[2], coordonnees[2];
  do {
	printf("Allez c'est à vous !! Entrez les coordonnées du coup (du type : E5E7): ");
	scanf("%2s", depart);
	scanf("%2s", arrivee);
  } while (!convertitEnCoordonnees(depart, coordDepart) || !convertitEnCoordonnees(arrivee, coordArrivee));
 convertitEnCoordonnees(notation, coordonnees);
 realiseCoupSiValide(grille,depart, arrivee) ;

};
