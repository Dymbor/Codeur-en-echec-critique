#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // pour time()

int debug(int depart[2]) {
    printf("y = %d\n", depart[0]);
    printf("x = %d\n", depart[1]);
    return 0;
}

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
    int num_coup = 0;
    char grille[N][N];
    int couleur = C_VIDE;
    char copie[N][N];
    const size_t temps_debut = time(NULL); // temps de départ
    initialiseEchiquier(grille);
    do {
        afficheEchiquier (grille, num_coup, temps_debut);
        saisieCoup(grille);
        num_coup += 1;
    } while (partieTerminee(grille, couleur) == NON_TERMINEE);

    afficheEchiquier(grille, num_coup, temps_debut);
    printf("La partie est terminée.\n");

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

void copieGrille(char copie[N][N], const char grille[N][N]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			copie[i][j] = grille[i][j];
		}
  }
}

bool estDansGrille(int ligne, int colonne){
	return ligne >= 0 && ligne < N && colonne >= 0 && colonne < N;
}

bool estCaseVide(const char grille[N][N], int ligne, int colonne){
	return grille[ligne][colonne] == CASE_VIDE;
}

bool estMajuscule (char c) {
	return 'A' <= c && 'Z' >= c;
}

void videGrille(char grille[N][N]) {
	for (int i=0; i < N; i++) {
		for (int j=0; j < N; j++) {
			grille[i][j] = CASE_VIDE;
		}
	}
}

int couleurAdverse(int couleur){
	if (couleur == C_BLANC) {
		return C_NOIR;
	}
	else if (couleur == C_NOIR) {
		return C_BLANC;
	}
	return C_VIDE;
}

int trouveCouleur(const char grille[N][N], int ligne, int colonne){
	if (estMajuscule(grille[ligne][colonne])){
		return C_BLANC;
	}
	else if (!estMajuscule(grille[ligne][colonne])) {
		return C_NOIR;
	}
    printf("C_VIDE");
	return C_VIDE;
}

void saisieCoup(char grille[N][N]){
    char notation_depart[2], notation_arrivee[2];
    int coordDepart[2], coordArrivee[2];

    do {
        printf("Mettez des coordonnées valides: ");
        scanf("%2s", notation_depart);
        scanf("%2s", notation_arrivee);
        convertitEnCoordonnees(notation_depart, coordDepart);
        convertitEnCoordonnees(notation_arrivee, coordArrivee);

    } while (!estDansGrille(coordDepart[0], coordDepart[1]) || !estDansGrille(coordArrivee[0], coordArrivee[1]));
    realiseCoupSiValide(grille, coordDepart, coordArrivee);

}

int partieTerminee(const char grille[N][N], int couleur){
    if (estEnEchec(grille, couleur) == true){
        if (couleur == C_BLANC){
            printf("Le joueur noir a gagné !\n");
            return DEFAITE_BLANC;
        }
        else if (couleur == C_NOIR){
            printf("Le joueur blanc a gagné !\n");
            return DEFAITE_NOIR;
        }
        else {
            printf("Partie nulle !\n");
            return PARTIE_NULLE;
        }
    }
    else {
        return NON_TERMINEE;
    }
}


void afficheEchiquier(const char grille[N][N], int num_coup, size_t temps_debut){
    // effaceConsole();

    printf("Coup n°%d\n", num_coup); //défini quel joueur doit jouer
    if(num_coup % 2 == 0){
        printf("Tour du joueur: blanc\n");
    }
    else{
        printf("Tour du joueur: noir\n");}

    printf("Temps écoulé: %ld secondes\n", time(NULL) - temps_debut); //affiche le temps depuis le début de la partie
    
    printf("   +---+---+---+---+---+---+---+---+\n");
    for (int i = 0; i < N; i++) {
        printf(" %d | ", 8-i);
        for (int j = 0; j < N; j++) {
            afficheCaseEnCouleur(grille[i][j], C_VIDE);
            printf(" | ");
        }
        printf("\n   +---+---+---+---+---+---+---+---+\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
}


void trouvePositionRoi(const char grille[N][N], int positionRoi[2], int couleur){
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if (couleur == C_BLANC){
				if (grille[i][j]=='R'){
					positionRoi[0]=i;
					positionRoi[1]=j;
				}
			}
			else{
				if (grille[i][j] == 'r'){
					positionRoi[0]=i;
					positionRoi[1]=j;
				}
			}
		}
	}
}


bool convertitEnCoordonnees(const char notation[2], int coordonnees[2]){
    if (notation[0] >= 'A' && notation[0] <= 'H'){
        coordonnees[1] = (int) notation[0] - 65;
    }
    if (notation[1]>= '1' && notation[1] <= '8') {
	    coordonnees[0] = '8' - notation[1];
    }
    printf("%d %d\n", coordonnees[0], coordonnees[1]);
    return estDansGrille(coordonnees[0], coordonnees[1]);
}

void realiseCoup(char grille[N][N], const int depart[2], const int arrivee[2]) {
	if (!estCaseVide(grille, depart[0], depart[1])) {
        grille[arrivee[0]][arrivee[1]] = grille[depart[0]][depart[1]];
        grille[depart[0]][depart[1]] = CASE_VIDE;
    	//faire les bonus dans cette fonction, roque, passage du pion a la reine ou autres, et la prise en passant        
    }
}

bool realiseCoupSiValide(char grille[N][N], const int depart[2], const int arrivee[2]) {
	if (estDansGrille(arrivee[0], arrivee[1]) && estDeplacementValide(grille, depart, arrivee) && estCoupValide(grille, depart, arrivee)) {
        char nom_case;
        int choix;
        if ((grille[depart[0]][depart[1]] == 'P' || grille[depart[0]][depart[1]] == 'p') && (arrivee[0] == 0 || arrivee[0] == 7)) {
            printf("Votre pion a atteint le fond du plateau, vous avez accès à la promotion du pion:\n");
            printf("1: Une Dame\n");
            printf("2: Une Tour\n");
            printf("3: Un Fou\n");
            printf("4: Un Cavalier\n");
            scanf("%d", &choix);
            int couleur = (grille[depart[0]][depart[1]] == 'P') ? C_BLANC : C_NOIR;

            if (couleur == C_BLANC) {
                switch (choix) {
                    case 1:
                        nom_case = 'D';
                        break;
                    case 2:
                        nom_case = 'T';
                        break; 
                    case 3:
                        nom_case = 'F';
                        break;
                    case 4:
                        nom_case = 'C';
                        break;
                }
                grille[arrivee[0]][arrivee[1]] = nom_case;
                grille[depart[0]][depart[1]] = CASE_VIDE;
                realiseCoup(grille, depart, arrivee);
                return true;
            }
            if (couleur == C_NOIR) {
                switch (choix) {
                    case 1:
                        nom_case = 'd';
                        break;
                    case 2:
                        nom_case = 't';
                        break; 
                    case 3:
                        nom_case = 'f';
                        break;
                    case 4:
                        nom_case = 'c';
                        break;
                }
                grille[arrivee[0]][arrivee[1]] = nom_case;
                grille[depart[0]][depart[1]] = CASE_VIDE;
                realiseCoup(grille, depart, arrivee);
                return true;
            }
        }
        else {
            realiseCoup(grille, depart, arrivee);
            return true;
        }
		
	}
	return false;
}

bool estCoupValide(const char grille[N][N], const int depart[2], const int arrivee[2]){
	if(estCaseVide(grille, depart[0], depart[1])){
		return false;
	}
	else{
		char copie[N][N];
		copieGrille(copie, grille);
		realiseCoup(copie, depart, arrivee);
		if (estEnEchec(copie, trouveCouleur(grille, depart[0], depart[1]))){
			return false;
		}
		else{
			return true;
		}
	}
}

void initialiseEchiquier(char grille[N][N]){
	
	videGrille(grille);
	char P_BLANC[] = {'T','C','F','D','R','F','C','T'};
	char P_NOIR[] = {'t','c','f','d','r','f','c','t'};
	for(int i=0;i<N;i++){
		grille[0][i] = P_NOIR[i];
		grille[7][i] = P_BLANC[i];
	}
	for (int i=0;i<N;i++){
		grille[1][i]='p';
        grille[6][i]='P';
	}
}

bool estDeplacementValide(const char grille[N][N], const int depart[2], const int arrivee[2])
{
    int deplacements[NB_MAX_DEPL][2];
    int colonneArriveeDemande = arrivee[0];
    int ligneArriveeDemande = arrivee[1];
    int j = listeDeplacementsValides(grille, depart, deplacements);
    for (int i = 0; i < j; i++){
        if(colonneArriveeDemande == deplacements[i][0] && ligneArriveeDemande == deplacements[i][1]){
            return true;
        }
    }
    return false;
}

int listeDeplacementsValides(const char grille[N][N], const int depart[2], int deplacements[NB_MAX_DEPL][2]){
    char piece = grille[depart[0]][depart[1]];
    int j = 0;
    if (piece == 'T' || piece == 't') //Déplacement Tour
    {
        printf("Départ de la tour:\n");
        debug(depart);
        int directions[4][2] = {
            {1, 0},
            {0, 1}, 
            {0, -1},
            {-1, 0}
        };

        for (int i = 0; i < 4; i++){
            int ligneArrive = depart[0] + directions[i][0];
            int colonneArrive = depart[1] + directions[i][1];
            while (grille[ligneArrive][colonneArrive] == CASE_VIDE && estDansGrille(ligneArrive, colonneArrive)){
                deplacements[j][0] = ligneArrive;
                deplacements[j][1] = colonneArrive;
                printf("- mouvements %d de la tour:\n", j);
                debug(deplacements[j]);
                j++;
                ligneArrive += directions[i][0];
                colonneArrive += directions[i][1];
            }

            int couleur_depart = trouveCouleur(grille, depart[0], depart[1]);
            int couleur_arrivee = trouveCouleur(grille, ligneArrive, colonneArrive);

            if (couleur_arrivee != couleur_depart && estDansGrille(ligneArrive, colonneArrive)){
                deplacements[j][0] = ligneArrive;
                deplacements[j][1] = colonneArrive;
                j++;
                printf("- mouvements %d de la tour:\n", j);
                debug(deplacements[j]);
            }
        }
        return j;
    }
    else if (piece == 'C' || piece == 'c') //Déplacement Cavalier
    {
        int moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
        for (int i = 0; i < 8; i++){
            int ligneArrive = depart[0] + moves[i][0];
            int colonneArrive = depart[1] + moves[i][1];
            int couleur_depart = estMajuscule(piece) ? C_BLANC : C_NOIR;
            int couleur_arrivee = trouveCouleur(grille, ligneArrive, colonneArrive);
            if (estDansGrille(ligneArrive, colonneArrive) && couleur_depart != couleur_arrivee){
                deplacements[j][0] = ligneArrive;
                deplacements[j][1] = colonneArrive;
                j++;
            }
        }
        return j;
    }
    else if (piece == 'F' || piece == 'f') //Déplacement Fou
    {
        int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (int d = 0; d < 4; d++) {
            for (int i = 1; i < N; i++) {
                int ligneArrive = depart[0] + i * directions[d][0];
                int colonneArrive = depart[1] + i * directions[d][1];
                int couleur_depart = estMajuscule(piece) ? C_BLANC : C_NOIR;
                int couleur_arrivee = trouveCouleur(grille, ligneArrive, colonneArrive);
                if (estDansGrille(ligneArrive, colonneArrive) && (couleur_arrivee != couleur_depart)){
                    deplacements[j][0] = ligneArrive;
                    deplacements[j][1] = colonneArrive;
                    j++;
                }
                if (couleur_arrivee == C_BLANC) break;
            }
        }
        return j;
    }
    else if (piece == 'D' || piece == 'd') //Déplacement Dame
    {
        int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (int d = 0; d < 8; d++){
            for (int i = 1; i < N; i++){
                int ligneArrive = depart[0] + i * directions[d][0];
                int colonneArrive = depart[1] + i * directions[d][1];
                int couleur_depart = estMajuscule(piece) ? C_BLANC : C_NOIR;
                int couleur_arrivee = trouveCouleur(grille, ligneArrive, colonneArrive);
                if (estDansGrille(ligneArrive, colonneArrive) && (couleur_arrivee != couleur_depart)){
                    deplacements[j][0] = ligneArrive;
                    deplacements[j][1] = colonneArrive;
                    j++;
                }
                if (couleur_arrivee == C_BLANC) break;
            }
        }
        return j;
    }
    else if (piece == 'R' || piece == 'r') //Déplacement Roi
    {
        int deplacement[8][2] = {
            {1, 1}, {1, 0}, {1, -1},
            {0, 1}, {0, -1},
            {-1, 1}, {-1, 0}, {-1, -1}
        };

        for (int i = 0; i < N; i++){
            int ligneArrive = depart[0] + deplacement[i][0];
            int colonneArrive = depart[1] + deplacement[i][1];
            int couleur_depart = estMajuscule(piece) ? C_BLANC : C_NOIR;
            int couleur_arrivee = trouveCouleur(grille, ligneArrive, colonneArrive);

            if (estDansGrille(ligneArrive, colonneArrive)){
                if (couleur_depart != couleur_arrivee){
                    deplacements[j][0] = deplacement[i][0];
                    deplacements[j][1] = deplacement[i][1];
                    j++;
                }
            }
        }
        return j;
    }
    else if (piece == 'P' || piece == 'p') //Déplacement Pion
    {
        {
            int direction = (piece == 'P') ? -1 : 1;//(condition ? valeur si vrai : valeur si faux)
            int startRow = (piece == 'P') ? 6 : 1;
            int ligneArrive = depart[0] + direction;
            if (estDansGrille(ligneArrive, depart[1]) && estCaseVide(grille, ligneArrive, depart[1])){
                deplacements[j][0] = ligneArrive;
                deplacements[j][1] = depart[1];
                j++;
                if (depart[0] == startRow && estCaseVide(grille, ligneArrive + direction, depart[1])){
                    deplacements[j][0] = ligneArrive + direction;
                    deplacements[j][1] = depart[1];
                    j++;
                }
            }
            int captures[2][2] = {{ligneArrive, depart[1] - 1}, {ligneArrive, depart[1] + 1}};
            // les 2 diagonales où le pion peut manger contrairement aux autres pièces
            for (int i = 0; i < 2; i++){
                if (estDansGrille(captures[i][0], captures[i][1]) &&
                    trouveCouleur(grille, captures[i][0], captures[i][1]) == couleurAdverse(
                        trouveCouleur(grille, depart[0], depart[1]))){
                    deplacements[j][0] = captures[i][0];
                    deplacements[j][1] = captures[i][1];
                    j++;
                }
            }
        }
        return j;
    }
    else if (piece == CASE_VIDE) //Case depart vide
    {
        return 0;
    }
}




bool estEnEchec(const char grille[N][N], int couleur) {
    int positionRoi[2];
    trouvePositionRoi(grille, positionRoi, couleur); // Trouve la position du roi de la couleur donnée
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int depart[2] = {i, j};
            if (trouveCouleur(grille, i, j) == couleurAdverse(couleur)) { // Si c'est une pièce adverse
                int arrivee[2] = {positionRoi[0], positionRoi[1]};
                if (estDeplacementValide(grille, depart, arrivee)) { // Si la pièce peut se déplacer sur la case du roi
                    return true; 
                }
            }
        }
    }
    return false;
}
