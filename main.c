#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <string.h>
#define LONGUEUR 75
#define HAUTEUR 25

//type
    typedef struct Instruction{
        char dir;
        int nombre;
    }Instruction;

//prototypes
    void initTerrain(char[][LONGUEUR],int*,int*);
    void initList(Instruction[]);
    void affTerrain(char[][LONGUEUR],int);
    void posSouri(char [][LONGUEUR]);
    void deplacement(char [][LONGUEUR],char[],int*,int*,int*,Instruction[]);
    void affectFinSerpent(Instruction[],int*,int*);

int main()
{

    //VAR
        Instruction ListMouv[LONGUEUR*HAUTEUR];
        char Terrain[HAUTEUR][LONGUEUR];
        char Direction[7]="Droite";
        int TailleSerpent=2;
        int FinSerpentX=0,FinSerpentY=0;

    //Initialisation
        initList(ListMouv);
        initTerrain(Terrain,&FinSerpentX,&FinSerpentY);

    //Lancement de la partie
        affTerrain(Terrain,TailleSerpent);
        deplacement(Terrain,Direction,&TailleSerpent,&FinSerpentX,&FinSerpentY,ListMouv);

    system("pause");

    return 0;
}

void initList(Instruction ListMouv[]){
//BUT initialiser la liste de mouvement
//ENTRER ListMouv
//SORTIE ListMouv initialisé

    int i=0;
    for (i=0;i<LONGUEUR*HAUTEUR;i++){
        ListMouv[i].dir='0';
        ListMouv[i].nombre=0;
    }
    ListMouv[0].dir='D';
    ListMouv[0].nombre=1;
}

void posSouri(char Terrain[][LONGUEUR]){
//BUT positionner une souri au hazard sur le terrain
//ENTREE appel de la procédure
//SORTIE une souri sur le terrain

    srand(time(NULL));
    int x=0,y=0;
    do{
        x=(rand()%HAUTEUR)+1;
        if (x==HAUTEUR){
            x=x-1;
        }
        y=(rand()%LONGUEUR)+1;
        if (y==LONGUEUR){
            y=y-1;
        }
    }while (Terrain[x][y]!=' ');
    Terrain[x][y]='-';
}

void initTerrain(char Terrain[][LONGUEUR],int*pFinSerpentX,int*pFinSerpentY){
//BUT initializer le terrain
//ENTREE le terrain
//SORTIE terrain initialiser

    int i=0, j=0;

    //init grille
        for(i=0;i<HAUTEUR;i++){
            for(j=0;j<LONGUEUR;j++){

                if(i==0 || i==(HAUTEUR-1) || j==0 || (j==LONGUEUR-1)){
                    Terrain[i][j]='/';
                }else{
                    Terrain[i][j]=' ';
                }

            }
        }

    //init serpent
        i=HAUTEUR/2;
        j=LONGUEUR/2;
        Terrain[i][j]='X';
        Terrain[i][j-1]='0';
        *pFinSerpentX=j-1;
        *pFinSerpentY=i;

    //init souri
        posSouri(Terrain);

}

void affTerrain(char Terrain[][LONGUEUR],int tailleSerpent){
//BUT afficher l terrain de jeu
//ENTREE le terrain
//SORTIE le terrain sur l'écran

    system("@cls||clear");
    int i=0,j=0;
    for(i=0;i<HAUTEUR;i++){
        for(j=0;j<LONGUEUR;j++){
            printf("%c",Terrain[i][j]);
        }
        if (i==0){
            printf("Score :");
        }else if (i==1){
            printf("%d",tailleSerpent-2);
        }
        printf("\n");
    }

}

void affectFinSerpent(Instruction ListMouv[],int*pFinSerpentX,int*pFinSerpentY){
//BUT modifier les valeurs de la fin du serpent
//ENTRER les anciennes valeurs
//SORTIE valeurs modifiés

    if (ListMouv[0].dir=='G'){
        *pFinSerpentX=*pFinSerpentX-1;
    }else if(ListMouv[0].dir=='D'){
        *pFinSerpentX=*pFinSerpentX+1;
    }else if(ListMouv[0].dir=='B'){
        *pFinSerpentY=*pFinSerpentY+1;
    }else if(ListMouv[0].dir=='H'){
        *pFinSerpentY=*pFinSerpentY-1;
    }

}

void deplacement(char Terrain[][LONGUEUR], char Direction[],int*pTailleSerpent,int*pFinSerpentX,int*pFinSerpentY,Instruction ListMouv[]){
//BUT deplacer le serpent
//ENTREE appel de la procedure(lancement de la partie)
//SORTIE deplacement du serpent

    int i=0,j=0,test=0,testOver=0,testSouri=0;
    int posXTete=0,posYTete=0;

    do{

        //Localisation tête du serpent
            for (i=0;i<HAUTEUR;i++){
                for(j=0;j<LONGUEUR;j++){
                    if (Terrain[i][j]=='X'){
                        posXTete=j;
                        posYTete=i;
                        Terrain[i][j]=' ';
                    }
                }
            }


        if (strcmp(Direction,"Gauche")==0){
            //mise à jour du tableau
                i=-1;
                do{
                    i++;
                }while (ListMouv[i].dir!='0');
                if (ListMouv[i-1].dir=='G' && i>0){
                    i--;
                }
                ListMouv[i].dir='G';
                ListMouv[i].nombre=ListMouv[i].nombre+1;

            //mise à jour position tête du serpent
                posXTete--;

            //si le serpent ne touche pas le bord
                if (posXTete>=(1)){

                    //cas spéciaux
                        if (Terrain[posYTete][posXTete]=='0'){
                            testOver=1;
                        }else if (Terrain[posYTete][posXTete]=='-'){
                            *pTailleSerpent=*pTailleSerpent+1;
                            Terrain[posYTete][posXTete+1]='0';
                            testSouri=1;
                            posSouri(Terrain);
                        }

                    //si pas cas spéciaux alors mouvement du serpent
                        if (testOver!=1 && testSouri==0){
                            Terrain[posYTete][posXTete]='X';
                            Terrain[*pFinSerpentY][*pFinSerpentX]=' ';
                            affectFinSerpent(ListMouv,pFinSerpentX,pFinSerpentY);
                            ListMouv[0].nombre=ListMouv[0].nombre-1;
                            if (ListMouv[0].nombre==0){
                                for(i=0;i<LONGUEUR*HAUTEUR;i++){
                                    if (ListMouv[i+1].dir!='0'){
                                        ListMouv[i].dir=ListMouv[i+1].dir;
                                        ListMouv[i].nombre=ListMouv[i+1].nombre;
                                    }else{
                                        ListMouv[i].dir='0';
                                        ListMouv[i].nombre=0;
                                    }
                                }
                            }
                            Terrain[posYTete][posXTete+1]='0';
                            affTerrain(Terrain,*pTailleSerpent);
                            Sleep(60);
                            if (GetAsyncKeyState(VK_UP)){
                                Direction="Haut";
                                test=1;
                            }else if(GetAsyncKeyState(VK_DOWN)){
                                Direction="Bas";
                                test=1;
                            }
                        }

                    //réinitialisation testSouri
                        testSouri=0;
                }
        }else if (strcmp(Direction,"Droite")==0){
            //mise à jour du tableau
                i=-1;
                do{
                    i++;
                }while (ListMouv[i].dir!='0');
                if (ListMouv[i-1].dir=='D' && i>0){
                    i--;
                }
                ListMouv[i].dir='D';
                ListMouv[i].nombre=ListMouv[i].nombre+1;

            //mise à jour position tête du serpent
                posXTete++;

            //si le serpent ne touche pas le bord
                if (posXTete<(LONGUEUR-1)){

                    //cas spéciaux
                        if (Terrain[posYTete][posXTete]=='0'){
                            testOver=1;
                        }else if (Terrain[posYTete][posXTete]=='-'){
                            *pTailleSerpent=*pTailleSerpent+1;
                            Terrain[posYTete][posXTete-1]='0';
                            testSouri=1;
                            posSouri(Terrain);
                        }

                    //si pas cas spéciaux alors mouvement du serpent
                        if (testOver!=1 && testSouri==0){
                            Terrain[posYTete][posXTete]='X';
                            Terrain[*pFinSerpentY][*pFinSerpentX]=' ';
                            affectFinSerpent(ListMouv,pFinSerpentX,pFinSerpentY);
                            ListMouv[0].nombre=ListMouv[0].nombre-1;
                            if (ListMouv[0].nombre==0){
                                for(i=0;i<LONGUEUR*HAUTEUR;i++){
                                    if (ListMouv[i+1].dir!='0'){
                                        ListMouv[i].dir=ListMouv[i+1].dir;
                                        ListMouv[i].nombre=ListMouv[i+1].nombre;
                                    }else{
                                        ListMouv[i].dir='0';
                                        ListMouv[i].nombre=0;
                                    }
                                }
                            }
                            Terrain[posYTete][posXTete-1]='0';
                            affTerrain(Terrain,*pTailleSerpent);
                            Sleep(60);
                            if (GetAsyncKeyState(VK_UP)){
                                Direction="Haut";
                                test=1;
                            }else if(GetAsyncKeyState(VK_DOWN)){
                                Direction="Bas";
                                test=1;
                            }
                        }

                    //réinitialisation testSouri
                        testSouri=0;
                }
        }else if (strcmp(Direction,"Haut")==0){
            //mise à jour du tableau
                i=-1;
                do{
                    i++;
                }while (ListMouv[i].dir!='0');
                if (ListMouv[i-1].dir=='H' && i>0){
                    i--;
                }
                ListMouv[i].dir='H';
                ListMouv[i].nombre=ListMouv[i].nombre+1;

            //mise à jour position tête du serpent
                posYTete--;

            //si le serpent ne touche pas le bord
                if (posYTete>=(1)){

                    //cas spéciaux
                        if (Terrain[posYTete][posXTete]=='0'){
                            testOver=1;
                        }else if (Terrain[posYTete][posXTete]=='-'){
                            *pTailleSerpent=*pTailleSerpent+1;
                            Terrain[posYTete+1][posXTete]='0';
                            testSouri=1;
                            posSouri(Terrain);
                        }

                    //si pas cas spéciaux alors mouvement du serpent
                        if (testOver!=1 && testSouri==0){
                            Terrain[posYTete][posXTete]='X';
                            Terrain[*pFinSerpentY][*pFinSerpentX]=' ';
                            affectFinSerpent(ListMouv,pFinSerpentX,pFinSerpentY);
                            ListMouv[0].nombre=ListMouv[0].nombre-1;
                            if (ListMouv[0].nombre==0){
                                for(i=0;i<LONGUEUR*HAUTEUR;i++){
                                    if (ListMouv[i+1].dir!='0'){
                                        ListMouv[i].dir=ListMouv[i+1].dir;
                                        ListMouv[i].nombre=ListMouv[i+1].nombre;
                                    }else{
                                        ListMouv[i].dir='0';
                                        ListMouv[i].nombre=0;
                                    }
                                }
                            }
                            Terrain[posYTete+1][posXTete]='0';
                            affTerrain(Terrain,*pTailleSerpent);
                            Sleep(60);
                            if (GetAsyncKeyState(VK_RIGHT)){
                                Direction="Droite";
                                test=1;
                            }else if(GetAsyncKeyState(VK_LEFT)){
                                Direction="Gauche";
                                test=1;
                            }
                        }

                    //réinitialisation testSouri
                        testSouri=0;
                }
        }else if (strcmp(Direction,"Bas")==0){
            //mise à jour du tableau
                i=-1;
                do{
                    i++;
                }while (ListMouv[i].dir!='0');
                if (ListMouv[i-1].dir=='B' && i>0){
                    i--;
                }
                ListMouv[i].dir='B';
                ListMouv[i].nombre=ListMouv[i].nombre+1;

            //mise à jour position tête du serpent
                posYTete++;

            //si le serpent ne touche pas le bord
                if (posYTete<(HAUTEUR-1)){

                    //cas spéciaux
                        if (Terrain[posYTete][posXTete]=='0'){
                            testOver=1;
                        }else if (Terrain[posYTete][posXTete]=='-'){
                            *pTailleSerpent=*pTailleSerpent+1;
                            Terrain[posYTete-1][posXTete]='0';
                            testSouri=1;
                            posSouri(Terrain);
                        }

                    //si pas cas spéciaux alors mouvement du serpent
                        if (testOver!=1 && testSouri==0){
                            Terrain[posYTete][posXTete]='X';
                            Terrain[*pFinSerpentY][*pFinSerpentX]=' ';
                            affectFinSerpent(ListMouv,pFinSerpentX,pFinSerpentY);
                            ListMouv[0].nombre=ListMouv[0].nombre-1;
                            if (ListMouv[0].nombre==0){
                                for(i=0;i<LONGUEUR*HAUTEUR;i++){
                                    if (ListMouv[i+1].dir!='0'){
                                        ListMouv[i].dir=ListMouv[i+1].dir;
                                        ListMouv[i].nombre=ListMouv[i+1].nombre;
                                    }else{
                                        ListMouv[i].dir='0';
                                        ListMouv[i].nombre=0;
                                    }
                                }
                            }
                            Terrain[posYTete-1][posXTete]='0';
                            affTerrain(Terrain,*pTailleSerpent);
                            Sleep(60);
                            if (GetAsyncKeyState(VK_RIGHT)){
                                Direction="Droite";
                                test=1;
                            }else if(GetAsyncKeyState(VK_LEFT)){
                                Direction="Gauche";
                                test=1;
                            }
                        }

                    //réinitialisation testSouri
                        testSouri=0;
                }
        }

    }while((test!=1) && (testOver!=1) && (posXTete<LONGUEUR) && (posYTete<HAUTEUR) && (posXTete>0) && (posYTete>0));

    //Si serpent sur un bord ou lui même
        if((posXTete==LONGUEUR)||(posYTete==HAUTEUR) || (posXTete==0)||(posYTete==0)){
            printf("Game Over\n\n");
    //Sinon si une autre direction est entré alors relancement du déplacement
        }else{
            deplacement(Terrain,Direction,pTailleSerpent,pFinSerpentX,pFinSerpentY,ListMouv);
        }

}
