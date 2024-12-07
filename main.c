#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLUSH_STDIN(x) {if(x[strlen(x)-1]!='\n'){do fgets(Junk,16,stdin);while(Junk[strlen(Junk)-1]!='\n');}}
// un macro pour supprimer les espaces, qui va affecter a x le mot sans l'espace avant de faire la compilation
char Junk[16]; /* tampon pour discorder les entrees excessive de l'utilisateur, utiliser par "FLUSH_STDIN" (macro)*/


typedef struct {
    char tl;      // tuile actuelle a la positon du plateau
    int lettre;   // letter multiplier (2 ou 3), 0 si rien
    int mot;      // word multiplier (2 or 3), 0 si rien
} plateau_s;



//tuiles valeurs sont
//               A B C D E F G H I J K L M N O P Q  R S T U V W X Y Z
int TlScore[26]={1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};

// ajouter une colone pour dessiner la limite
plateau_s plateau[15][16];


// la location du mot
// on demande à l'utilisateur  d'introduire la location (ligne et colonne)
// et on verifie la validation du location si elle est dans le plateau
int get_location(int *ligne, int *col)
{
    char *tomp, input[16];

    printf("entrer ligne/col pour commencer ex:A5 ou 'Q' pour quitter : ");
    fflush(stdout);

    fgets(input,sizeof(input),stdin);
    FLUSH_STDIN(input);

    input[0] &= 0x5F; //conversion du alpha en UPPERCASE (a-->A)

    if (input[0]=='Q')
        return -1;

    *ligne = input[0]-0x41; // rendre dans a case 0 du ligne
    if (*ligne<0 || *ligne>14)
    {
        printf("non valide, essaye encore.\n");
        return 0;
    }
    *col = strtol(&input[1],&tomp,10);
    if (tomp==&input[1] || *col<1 || *col>15)
    {
        printf("non valide, essaye encore.\n");
        return 0;
    }
    *col = *col - 1; // rendre colonne dans le tableau

    return 1;
}

// demander à l'utilisateur d'introduire a location
//  dir = 0 horizontal ou dir = 1 vertical
int get_direction(int *dir)
{
    char input[16];

    printf("entrer direction 'H'(0) ou 'V'(1), ou <ENTRER> pour redemarer : ");
    fflush(stdout);

    fgets(input,sizeof(input),stdin);
    FLUSH_STDIN(input);

    input[0] &= 0x5F; //conversion du alpha en UPPERCASE (a-->A)

    if (input[0]=='H' || input[0] == '0')
    {
        *dir = 0;
        return 1;
    }
    if (input[0]=='V' || input[0] == '1')
    {
        *dir = 1;
        return 1;
    }
    if (input[0]=='\n')
        return -1; // si le joueur veut quitter

    printf("non valide, essaye encore.\n");
    return 0;
}


// demander à l'utilisateur d'introduire le mot apres verifier la valdiation du mot
//   mot validé doit etre dans les limites du plateau avec un longoeur esrte toujours dans le plateau
int get_mot(char *mot, int ligne, int col, int dir)
{
    char *tomp=mot;
    char unchar;
    size_t longoeur;

    printf("entrer mot, ou <ENTRER> pour redemarer: ");
    fflush(stdout);

    fgets(mot,17,stdin);
    FLUSH_STDIN(mot);      // discorder excessives entrée

    if (mot[0] == '\n')  // nouvelle ligne indiquer que l'utilisateur veut redemarer
        return -1;

        // supprimer les signes du nouvelle ligne du mot et les remplacer par le signe du fin du chaine
    mot[strcspn(mot,"\n")]='\0';
        // verifie que suelement les majuscules sont utilisaient
    while(*tomp)
    {
        unchar = *tomp++ &= 0x5F; //on affect à unchar le charactere dan sle *tomp et le converti en majuscule et on increment le tomp pour sortir du bocule
        if (unchar < 'A' || unchar > 'Z')
        {
            printf("non valide, essaye encore.\n");
            return 0;
        }
    }
        // on verifie si la lognoeur du mot ne depasse pas les limites
    longoeur = strlen(mot);
    if ((longoeur | ((dir)?ligne:col)) > 15) // on ajoute les bites du lognoeur à l'un des deux (ligne ou colonne )et on verifie si ils deppassent la limite du plateau // le ternaire conditionel prend pour dir=0 les colones et pour dir=1 les lignes .
    {
        printf("non valide, essaye encore.\n");
        return 0;
    }

    return 1;
}

// le score du mot selon la validation du mot etles valeur du lettres et du mot
// un placement du mot selon les lettres qui les precdedent ,et une verification si le mot est deja ecrit
int score_mot(char *mot, int *score, int ligne, int col, int dir, int accept)
{
    char *tomp = mot;
    int tompscore, valide=0, motmult=1, l=ligne, c=col, indice;

    tompscore = 0;
    *score = 0;

    while(*tomp)
    {
        if (l == 7 && c == 7)  // la premiere etape utlisee le centre du plateau
            valide = 1;

       indice = *tomp - 0x41;  // trouver la valeur du tuile

        if (plateau[l][c].tl != ' ') // si la lettre est deja placer
        {
            if (plateau[l][c].tl != *tomp) // si elle est la meme que dans le *tomp
            {
                printf("non valide,essaye encore.\n");
                return 0;
            }
            valide= 1;
            tompscore += TlScore[indice]; // tulile score a aucun multiple
        }
        else
        {       // placement du nouvelle lettre dans la tuile
            tompscore += (TlScore[indice] * ((plateau[l][c].lettre > 1)?plateau[l][c].lettre:1));
            if (plateau[l][c].mot> 1)
                motmult *= plateau[l][c].mot;
        }

        if (dir) l++;
        else c++;
            // incrementation du pointeur pour sortir du bucle
        tomp++;
    }

    if (valide == 0)
    {
        printf("non valide, essay encore.\n");
        return 0;
    }
        // cle score final
    *score = tompscore * motmult;

    if (accept) // ajoute le mot si l'utlisateur a introduit oui
    {
        l=ligne;
        c=col;
        tomp=mot;
        while (*tomp)
        {
            plateau[l][c].tl=*tomp++;
            if (dir) l++;
            else c++;
        }
    }
    return 1;
}

// initialliser le plateau a ses valeur selon les tuiles
void int_plateau()
{
    int i, j;

    for (i=0; i<15; i++)
        for (j=0; j<16; j++)
        {
            plateau[i][j].lettre = 0;
            plateau[i][j].mot = 0;
            plateau[i][j].tl = ' ';
            if ((i==0 || i==7 || i==14) && (j==0 || j==7 || j==14))
                plateau[i][j].mot = (i==7 && j==7) ? 2 : 3;
            else if ((i==5 || i==9) && ((j-1)%4==0))
                plateau[i][j].lettre = 3;
            else if ((j==5 || j==9) && ((i-1)%4==0))
                plateau[i][j].lettre = 3;
            else if ((i==6 || i==8) && (j==2 || j==6 || j==8 || j==12))
                plateau[i][j].lettre = 2;
            else if ((j==6 || j==8) && (i==2 || i==6 || i==8 || i==12))
               plateau[i][j].lettre = 2;
            else if ((i==3 || i==11) && (j==0 || j==7 || j==14))
                plateau[i][j].lettre = 2;
            else if ((j==3 || j==11) && (i==0 || i==7 || i==14))
                plateau[i][j].lettre = 2;
            else if (i==j || i==(14-j))
               plateau[i][j].mot = 2;
        }
}
// plateau du dimesion 15x15

// dessiner le plateau et multiplier les tuiles selon les cases
//  +---+  +- -+  +...+  +. .+  +   +
//  |3x |  |2x |  .3x .  .2x .   rien
//  |   |  |   |  .let.  .let.   mult
//  |   |  |   |  .   .  .   .
//  +---+  +- -+  +...+  +. .+  +   +
//
void dessiner_plateau()
{
    int i, j, lettre, mot;

    printf("    1   2   3   4   5   6   7   8   9   10  11  12  13  14  15\n\n  ");
    for (i=0; i<15; i++)
    {
        for (j=0; j<15; j++)
        {
            lettre = (plateau[i][j].lettre | ((i)?plateau[i-1][j].lettre:0));
            mot = (plateau[i][j].mot | ((i)?plateau[i-1][j].mot:0));
            if (lettre>1)
                printf("%s",(lettre==2)?"+. .":"+...");
            else if (mot>1)
                printf("%s",(mot==2)?"+- -":"+---");
            else
                printf("+   ");
        }
        printf("+\n  ");
        for (j=0; j<16; j++)
        {
            lettre = (plateau[i][j].lettre | ((j)?plateau[i][j-1].lettre:0));
            mot = (plateau[i][j].mot | ((j)?plateau[i][j-1].mot:0));
            if (lettre>1)
                printf(".   ");
            else if (mot>1)
                printf("|   ");
            else
                printf("    ");
        }
        printf("\n%c ",i+0x61);
        for (j=0; j<16; j++)
        {
            lettre = (plateau[i][j].lettre | ((j)?plateau[i][j-1].lettre:0));
            mot = (plateau[i][j].mot | ((j)?plateau[i][j-1].mot:0));
            if (lettre == 3)
                printf(". %c ",(j<15)?plateau[i][j].tl:i+0x61);
            else if (mot == 3)
                printf("| %c ",(j<15)?plateau[i][j].tl:i+0x61);
            else
                printf("  %c ",(j<15)?plateau[i][j].tl:i+0x61);
        }
        printf("\n  ");
        for (j=0; j<16; j++)
        {
            lettre = (plateau[i][j].lettre | ((j)?plateau[i][j-1].lettre:0));
            mot = (plateau[i][j].mot | ((j)?plateau[i][j-1].mot:0));
            if (lettre>1)
                printf(".   ");
            else if (mot>1)
                printf("|   ");
            else
                printf("    ");
        }
        printf("\n  ");
    }
    printf("+---+   +   +. .+   +   +   +---+   +   +   +. .+   +   +---+\n\n  ");
    printf("  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 \n\n");
 }

int main()
{
    int ligne, col, direction, player=1, score_p1=0, score_p2=0, tompscore, result;
    char mot[17], prompt[8];

    int_plateau();

    while(1)
    {       // dessiner le plateau du jeu dans l'ecran avec les scores de deux joueurs
        dessiner_plateau();
        printf("SCORE  :                 Player 1: %-4d            Player 2: %d\n",score_p1, score_p2);
            // indiquer qui va commencer
        printf("\n -- PLAYER #%d --\n",player);

            // prendre la location ou les letrres doit se placer ou bien quitter
        do result = get_location(&ligne, &col);
        while (result == 0);
        if (result == -1)
            break;

            // la direction de placement (Horiz, Vert) pour placer les mots ou bien reprendre la direction encore une fois
        do result = get_direction(&direction);
        while (result == 0);
        if (result == -1)
            continue;

            // prendre le mot ou bien refaire la location
        do result = get_mot(mot, ligne, col, direction);
        while (result == 0);
        if (result == -1)
            continue;

            // test validation et score du mot sur le plateau
        tompscore=0;
        while (1)
        {
            result = score_mot(mot, &tompscore,ligne, col, direction, 0);
            if (result == 1)
                break;
            do result = get_mot(mot, mot, col, direction);
            while (result == 0);
            if (result == -1)
                break;
        }
            //redemarer une nouvelle location
        if (result == -1)
            continue;

            // display score for valid word allow accept or reject
        printf(" le score est %d. vouz acceptez le mot? (O/N): ",tompscore);
        fflush(stdout);
        fgets(prompt,sizeof(prompt),stdin);
        prompt[0] &= 0x5F;  //convertir alpha to UPPERCASE
        FLUSH_STDIN(prompt);

            // ecrire un mot dans la memoire ou laisser l'espac pour location/mot
        if (prompt[0]=='O')
            result = score_mot(mot, &tompscore, ligne, col, direction, 1);
        else
            continue;


        if (result == 0)
            printf("ERREURE:  mot non entrée!\n");

            // score du joueur
        else if (player == 1)
            score_p1 += tompscore;
        else
            score_p2 += tompscore;

        player ^= 3;  // XOR fonction pour changer les roles Player1 <--> Player2
    }
    if(score_p1<score_p2) printf("le gagnant est player2");
    else printf(" le gagnant est player1");

    return 0;
}

