#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include <errno.h>
// Structure d’un noeud de la liste chainee
typedef struct Noeud{
    char mot[50];
    struct Noeud *next;
}Noeud;
//Fonction qui met un mot au queue du liste
void PutAtEnd(Noeud **head, char motx[50]) {
    Noeud *newElement = malloc(sizeof(Noeud));
    if (newElement == NULL) return;  
    strcpy(newElement->mot, motx);
    newElement->next = NULL;
    if (*head == NULL) {
        *head = newElement;
        return;
    }
    Noeud *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newElement;
}
//fonction qui vide une liste
void freeList(Noeud **head) {
    Noeud *current = *head;
    Noeud *tmp;
    while (current != NULL) {
        tmp = current;        
        current = current->next; 
        free(tmp);          
    }
    *head = NULL; 
}
//fonction qui verifie si une chaine de caracteres est un entier
int estEntier(char *variable) {
    char *end;
    errno = 0;
    strtol(variable, &end, 10);
    if (errno != 0 || *end != '\0')
        return 0;
    return 1;
}
//fonction qui verifie si une chaine de caracteres est un float
int estFloat(char *s) {
    char *end;
    errno = 0;
    strtof(s, &end);
    if (errno != 0 || *end != '\0')
        return 0;
    return 1;
}
//fonction qui realise l'analyse lexical
void Analyse_lexical(FILE *pd,FILE *pc,FILE *pb){
    printf("\n-------------Analyse lexical-------------\n");
    //1-Verification des caracteres:
    char cc=fgetc(pc);  // Caractere du code source
    char cd=fgetc(pd);  // Caractere du dictionnaire
    int existc=0;
    int existl=1;
    int i=0,k=0,r=1;
    int ec=0,taille=0;
    char tabc[1000]={0};
    tabc[0]=' ';
    // Parcourir tout le fichier code
    while(cc!=EOF){
        ec=0;
        // Chercher le caractere dans le dictionnaire
        while(cd!=EOF){
            if(cc==cd){
                existc=1; // Caractere trouve
                ec=1;
                break;
            }
            else{
                cd=fgetc(pd);
            }
        }
        // Les espaces et retours a la ligne sont toujours valides
        if(cc == ' ' || cc == '\n' || cc == '\r') ec = 1;
        // Si caractere invalide , l’ajouter au buffer d’erreurs
        if(ec==0){
            tabc[r++]=cc;
            tabc[r++]=' ';
            tabc[r]='\0';
            ec=1;
        }
        // Rembobiner le dictionnaire pour la prochaine recherche
        rewind(pd);
        cd=fgetc(pd);
        cc=fgetc(pc);
        if(existc==0){
            existl=0; // La ligne contient des erreurs
        }

        if(cc=='\n' && existl==1){
            k++; //compteur des lignes valides
            i++; //compteur des lignes
            }
        
        if((cc=='\n'|| cc==EOF) && existl==0){
            i++;
            existl=1;
            printf("Erreur lexical : les caracteres {%s} n'existent pas dans le dictionnaire: la ligne %d\n",tabc,i);
            fprintf(pb,"Erreur lexical : les caracteres {%s} n'existent pas dans le dictionnaire : la ligne %d\n",tabc,i);
            tabc[0]='\0';
            tabc[0]=' ';
            r=1;
        }
        existc=0;
    }
    if(i==k){ //si le nombre des lignes valide egale le nombre des lignes
        printf("tous les caracteres existent dans le dictionnaire\n");
        fprintf(pb,"tous les caracteres existent dans le dictionnaire\n");
        fflush(pb);
    }
//2-Verification des mots:
    int existm=0; //supposons que le mot n existent pas dans le dictionnaire
    existl=1; //supposons que tous les mots de la ligne existent dans le dictionnaire
    rewind(pd);
    int testp=1; //supposons que tous les mots de code existent dans le dictionnaire
    rewind(pc);
    cd=fgetc(pd);
    cc=fgetc(pc);
    char motc[50];
    char motd[20];
    i=0;
    int j=0;
    k=0;
    Noeud *head=NULL;
    char nomvar[20];
    char typevar[20];
    char motsintrouvables[100];
    while(cc!=EOF){
        i=0;
        // Extraire un mot du code ( jusqu ’a l ’ espace ou nouvelle ligne )
        while(cc!=' ' && cc!='\n' && cc!=EOF){
            if(i<19){
            motc[i]=cc;
            }
            cc=fgetc(pc);
            i++;
        }
         motc[i]='\0';
        // Ajouter le mot a la liste chainee
        PutAtEnd(&head,motc);
        i=0;
        // Verifier si le mot de langueur suprieur à 1
        if(strlen(motc)>1){
        while(cd!='\n'){
            while (cd!=' '&& cd!='\n'){
                motd[j]=cd;
                cd=fgetc(pd);
                j++;
            }
            motd[j]='\0';
            /* Le mot est valide s ’ il est dans le dictionnaire ,
             ou s ’ il est un nombre , ou une chaine entre guillemets*/
            if(strcmp(motc,motd)==0 || estEntier(motc)==1 || estFloat(motc)==1 || (motc[0]=='"' && motc[strlen(motc)-1]=='"')){
                existm=1;
            }
            if(cd!='\n'){
            cd=fgetc(pd);
            }
            motd[0]='\0';
            j=0;
        }
        //renitialiser le pointeur de dictionnaire
    rewind(pd);
    cd=fgetc(pd);
    /*Ajout des mots qui n existent pas dans le dictionnaire
        au chaine motsintrouvables*/
    if(existm==0){
        existl=0;
        strcat(motsintrouvables,motc);
        strcat(motsintrouvables," ");
    }
    else{
        existm=0;
    }
        }
        motc[0]='\0'; //vidant le motc

//3-Dectection des fautes de frappes
        // A la fin de chaque ligne , verifier les fautes courantes
        if(cc=='\n' || cc== EOF){
            // Detection de fautes sur " Lire "
             if(strcmp(head->mot,"Lir")==0 || strcmp(head->mot,"Lre")==0 ||strcmp(head->mot,"Lie")==0 ||strcmp(head->mot,"ire")==0 ){
                 printf("Erreur lexical :Vouliez‑vous dire 'Lire' ?( ligne %d )\n",k+1);
                fprintf(pb,"Erreur lexical : mal utilisation de 'Lire' ( ligne %d )\n",k+1);
             }
             // Detection de fautes sur " Ecrire "
             if(strcmp(head->mot,"Ecrir")==0 || strcmp(head->mot,"Ecrie")==0 ||strcmp(head->mot,"Ecrre")==0 ||strcmp(head->mot,"Erire")==0 ||strcmp(head->mot,"rire")==0){
                 printf("Erreur lexical :Vouliez‑vous dire 'Ecire' ?( ligne %d )\n",k+1);
                fprintf(pb,"Erreur lexical : mal utilisation de 'Ecrire' ( ligne %d )\n",k+1);
             }
            // Liberer la memoire de la liste
           freeList(&head);
        }
        if((cc=='\n'|| cc==EOF) && existl==1){
            k++;
        }
        //En cas de retour a la ligne et l un des mots de ce ligne n est pas dans le dictionnaire
        if((cc=='\n'|| cc==EOF) && existl==0){
            k++;
            existl=1;
            printf("Erreur lexical : l un des mots-cles n existe pas dans le dictionnaire ( %s ) : la ligne %d\n",motsintrouvables,k);
            fprintf(pb,"Erreur lexical : l un des mots-cles n existe pas dans le dictionnaire ( %s ) : la ligne %d\n",motsintrouvables,k);
            strcpy(motsintrouvables,"");
            testp=0;
        }
        if(cc!=EOF){
            cc=fgetc(pc);
        }
    }
    //En cas tous les mots de code existent dans le dictionnare
       if(testp==1){
        printf("tous les mots-cles existent dans le dictionnaire\n");
        fprintf(pb,"tous les mots-cles existent dans le dictionnaire\n");
        fflush(pb);
} 
}

void Analyse_syntaxique(FILE *pd,FILE *pc,FILE *pb){
    printf("\n-------------Analyse syntaxique-------------\n");
    //Declaration des variables
    char cc; 
    char cd;
    int existl=1;
    int i=0,k=0;
    rewind(pd);
    rewind(pc);
    cd=fgetc(pd);
    cc=fgetc(pc);
    char motc[50];
    i=0;
    int xd; //prend le ligne de mots-cles conditionnels
    k=0;
    int sicounter=0; //compte le nombre des mots-cles conditionnels
    char simot[20]; 
    Noeud *head=NULL;
    char nomvar[20]; 
    char typevar[20];
    while(cc!=EOF){
        i=0;
        while(cc!=' ' && cc!='\n' && cc!=EOF){
            if(i<19){
            motc[i]=cc;
            }
            cc=fgetc(pc);
            i++;
        }
         motc[i]='\0';
        PutAtEnd(&head,motc);
        i=0;
        motc[0]='\0';
        if(cc=='\n' || cc== EOF){
            // Verifier que le code commence par " Debut "
            if(k==0 && strcmp(head->mot,"Debut") != 0 ){
                printf("Erreur syntaxique : le code doit commencer avec 'Debut' \n");
                fprintf(pb,"Erreur syntaxique : le code doit commencer avec 'Debut' \n");
            }
            // Verifier que le code se termine par " Fin "
            if(cc==EOF && strcmp(head->mot,"Fin")!=0){
                printf("Erreur syntaxique : le code doit terminer avec 'Fin'\n");
                fprintf(pb,"Erreur syntaxique : le code doit terminer avec 'Fin' \n");
            }
            //Calculons le nombre des mots-cles conditionnels et verfier leurs ordre
            if(strcmp(head->mot,"Si")==0 || strcmp(head->mot,"Sinon")==0 || strcmp(head->mot,"Finsi")==0){
                if(strcmp(head->mot,"Si")==0){
                    simot[sicounter]='v';
                    xd=k+1;
                }
                if(strcmp(head->mot,"Sinon")==0){
                    simot[sicounter]='l';
                    xd=k+1;
                }
                if(strcmp(head->mot,"Finsi")==0){
                    simot[sicounter]='d';
                    xd=k+1;
                }
                sicounter++;
            }
            //Vérification de l’instruction Ecrire
            if(strcmp(head->mot,"Ecrire")==0){
                // Verifier qu ’ il y a au moins 4 tokens apres " Ecrire "
                if(head->next!=NULL && head->next->next!=NULL && head->next->next->next!=NULL && head->next->next->next->next!=NULL){
                // Verifier la syntaxe : Ecrire ( x ) ;
                if(strcmp(head->next->mot,"(")!=0 ||strlen(head->next->next->mot)!=1 || strcmp(head->next->next->next->mot,")")!=0  || strcmp(head->next->next->next->next->mot,";")!=0){
                    printf("Erreur syntaxique : mal utilisation de 'Ecrire' ( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mal utilisation de 'Ecrire' (ligne %d) \n",k+1);
                }
                }
                else{
                    // Pas assez de tokens
                    printf("Erreur syntaxique : mal utilisation de 'Ecrire'( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mal utilisation de 'Ecrire'( ligne %d )\n",k+1);
                }
            }
            //Vérification de l’instruction Lire
            if(strcmp(head->mot,"Lire")==0){
                // Verifier qu ’ il y a au moins 4 tokens apres " Lire "
                if(head->next!=NULL && head->next->next!=NULL && head->next->next->next!=NULL && head->next->next->next->next!=NULL){
                    // Verifier la syntaxe : Ecrire ( x ) ;
                if(strcmp(head->next->mot,"(")!=0 || strlen(head->next->next->mot)!=1 || isalpha(head->next->next->mot[0])==0 || strcmp(head->next->next->next->mot,")")!=0 || strcmp(head->next->next->next->next->mot,";")!=0 ){
                    printf("Erreur syntaxique : mal utilisation de 'Lire'( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mal utilisation de 'Lire'( ligne %d )\n",k+1);
                }
                }
                else{
                    // Pas assez de tokens
                    printf("Erreur syntaxique : mal utilisation de 'Lire'( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mal utilisation de 'Lire'( ligne %d )\n",k+1);
                }
            }
            //Verifions declaration de variable
            if(strcmp(head->mot,"Variable")==0){
                if(head->next!=NULL && head->next->next!=NULL && head->next->next->next!=NULL){
                    // Verifier : Variable nom : type
                if(isalpha(head->next->mot[0])==0 || strcmp(head->next->next->mot,":")!=0 ||(strcmp(head->next->next->next->mot,"int")!=0 && strcmp(head->next->next->next->mot,"float")!=0 && strcmp(head->next->next->next->mot,"char")!=0)){
                    printf("Erreur syntaxique : mauvaise declaration de variable( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mauvaise declaration de variable( ligne %d )\n",k+1);
                }
                }
                else{
                    // Pas assez de tokens
                    printf("Erreur syntaxique : mauvaise declaration de variable( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mauvaise declaration de variable( ligne %d )\n",k+1);
                }
                 // Sauvegarder le nom et le type pour analyse semantique
                    strcpy(nomvar,head->next->mot);
                    strcpy(typevar,head->next->next->next->mot);
            }
            //Verifions affectation de variable
            if(strcmp(head->mot,nomvar)==0){
                if(head->next!=NULL && head->next->next!=NULL && head->next->next->next!=NULL){
                if(strcmp(head->next->mot,"=")!=0 || strcmp(head->next->next->next->mot,";")!=0){
                    // Verifier : nom = valeur ;
                    printf("Erreur syntaxique : mauvaise affectation de variable( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur syntaxique : mauvaise affectation de variable( ligne %d )\n",k+1);
                }
            }
            else{
                // Pas assez de tokens
                printf("Erreur syntaxique : mauvaise affectation de variable( ligne %d )\n",k+1);
                fprintf(pb,"Erreur syntaxique : mauvaise affectation de variable( ligne %d )\n",k+1);
            }
            }
            //Verifions si-sinon-finsi
            if(strcmp(head->mot,"Si")==0){
                if(head->next!=NULL && head->next->next!=NULL && head->next->next->next!=NULL && head->next->next->next->next!=NULL){
                    //Verifier le syntaxe
                if(strcmp(head->next->mot,"(")!=0 ||strlen(head->next->next->mot)!=1 || isalpha(head->next->next->mot[0])==0 || strlen(head->next->next->next->next->mot)!=1 || strcmp(head->next->next->next->next->next->mot,")")!=0  || strcmp(head->next->next->next->next->next->next->mot,"alors")!=0){
                    printf("Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",k+1);
                    fprintf(pb,"Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",k+1);
                }
            }
            else{
                // Pas assez de tokens
                printf("Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",k+1);
                fprintf(pb,"Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",k+1);
            }
        }
           //Verifions le syntaxe
            if(strcmp(head->mot,"Si")==0){
                if(strcmp(head->next->next->next->mot,"=")!=0 && strcmp(head->next->next->next->mot,"!=")!=0 && strcmp(head->next->next->next->mot,">")!=0 && strcmp(head->next->next->next->mot,">=")!=0 && strcmp(head->next->next->next->mot,"<")!=0 && strcmp(head->next->next->next->mot,"<=")!=0){
                    printf("Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",k+1);
                    fprintf(pb,"Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",k+1);
            }
        }
           freeList(&head);
        }
        if((cc=='\n'|| cc==EOF)){
            k++;
        }
        if(cc!=EOF){
            cc=fgetc(pc);
        }
    }
    //Verifions l ordre,le nombre et le syntaxe des mots-cles conditionnels
        if(strlen(simot)==3 && strcmp(simot,"vld")!=0){
        printf("Erreur syntaxique : conditionnel (ordre invalid) : ligne %d\n",xd);
        fprintf(pb,"Erreur syntaxique : conditionnel (ordre invalid) : ligne %d\n",xd);
      }
    if(strlen(simot)==1){
        printf("Erreur sytaxique : conditionnel (nombre des mots-cles invalid) : ligne %d\n",xd);
        fprintf(pb,"Erreur syntaxique : conditionnel (nombre des mots-cles invalid) : ligne %d\n",xd);
    }
    if(strlen(simot)==2 && strcmp(simot,"vd")!=0){
        printf("Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",xd);
        fprintf(pb,"Erreur syntaxique : conditionnel (syntaxe invalid) : ligne %d\n",xd);
    }
}

void Analyse_semantique(FILE *pd,FILE *pc,FILE *pb){
    printf("\n-------------Analyse semantique-------------\n");
    char cc;
    char cd;
    int i=0,k=0;
    rewind(pd);
    rewind(pc);
    cd=fgetc(pd);
    cc=fgetc(pc);
    char motc[50];
    Noeud *head=NULL;
    char nomvar[20];
    char typevar[20];
    while(cc!=EOF){
        i=0;
        while(cc!=' ' && cc!='\n' && cc!=EOF){
            if(i<19){
            motc[i]=cc;
            }
            cc=fgetc(pc);
            i++;
        }
         motc[i]='\0';
        PutAtEnd(&head,motc);
        i=0;
        motc[0]='\0';
        if(cc=='\n' || cc== EOF){
            // Verification lors de la declaration
            if(strcmp(head->mot,"Variable")==0){
            // Le nom de variable doit etre un seul caractere
                if(strlen(head->next->mot)!=1 ){
                    printf("Erreur semantique : mauvaise declaration de variable( ligne %d )\n",k+1);
                    fprintf(pb,"Erreur semantique : mauvaise declaration de variable( ligne %d )\n",k+1);
                }
                // Sauvegarder le nom et le type
                    strcpy(nomvar,head->next->mot);
                    strcpy(typevar,head->next->next->next->mot);
            }
             //Verifier le variable
            if(strcmp(head->mot,nomvar)==0){
               // Type int : la valeur doit etre un entier
                    if(strcmp(typevar,"int")==0 && estEntier(head->next->next->mot)==0){
                        printf("Erreur semantique : ligne %d ( %s est un %s )\n",k+1,nomvar,typevar);
                        fprintf(pb,"Erreur semantique : ligne %d ( %s est un %s )\n",k+1,nomvar,typevar);
                    }
                    // Type float : la valeur doit etre un float
                    else if (strcmp(typevar,"float")==0 && estFloat(head->next->next->mot)==0){
                        printf("Erreur semantique : ligne %d ( %s est un %s )\n",k+1,nomvar,typevar);
                        fprintf(pb,"Erreur semantique : ligne %d ( %s est un %s )\n",k+1,nomvar,typevar);
                    }
                    // Type char : la valeur doit etre entre guillemets
                    else if (strcmp(typevar,"char")==0 && (head->next->next->mot[0]!='"' || head->next->next->mot[strlen(head->next->next->mot)-1]!='"')){
                        printf("Erreur semantique : ligne %d ( %s est un %s )\n",k+1,nomvar,typevar);
                        fprintf(pb,"Erreur semantique : ligne %d ( %s est un %s )\n",k+1,nomvar,typevar);
                    }
            }
           freeList(&head);
        }
        if((cc=='\n'|| cc==EOF)){
            k++;
        }
        if(cc!=EOF){
            cc=fgetc(pc);
        }
    }
}
int main(){
    FILE *pd;
    pd=fopen("dictionnaire.txt","r");
    FILE *pc;
    pc=fopen("code.txt","r");
    FILE *pb;
    pb=fopen("bugs.txt","w");
    Analyse_lexical(pd,pc,pb);
    Analyse_syntaxique(pd,pc,pb);
    Analyse_semantique(pd,pc,pb);
  return 0;
}