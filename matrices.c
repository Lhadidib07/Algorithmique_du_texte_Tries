#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* avec Matrices de transition  */

struct _trie{ 
    int maxNode; /* nombre max de nœuds du trie */
    int nextNode; /* indice du prochain nœud disponible */
    int **transition; /* matrice de transition */
    char *finite; /* états terminaux */
};

typedef struct _trie *Trie;

Trie createTrie(int maxNode){ 
    Trie T = (Trie)malloc(sizeof(struct _trie));
    T->maxNode = maxNode;
    T->nextNode = 1;
    T->transition = (int **)malloc(maxNode * sizeof(int *));
    for(int i = 0; i < maxNode; i++)
        T->transition[i] = (int *)malloc(256 * sizeof(int));
    for(int i = 0; i < maxNode; i++)
        for(int j = 0; j < 256; j++)
            T->transition[i][j] = -1;
    T->finite = (char *)malloc(maxNode * sizeof(char));
    for(int i = 0; i < maxNode; i++)
        T->finite[i] = 0;
    return T;
}

bool insertInTrie(Trie T, unsigned char *w){ 
    int node = 0;
    for(int i = 0; w[i] != '\0'; i++){ 
        if(T->transition[node][w[i]] == -1){ 
            if(T->nextNode == T->maxNode)
                return false;
            T->transition[node][w[i]] = T->nextNode++;
        }
        node = T->transition[node][w[i]];
    }
    T->finite[node] = 1;
    return true;
}

int isInTrie(Trie trie, unsigned char *w){ 
    int node = 0;
    for(int i = 0; w[i] != '\0'; i++){ 
        if(trie->transition[node][w[i]] == -1)
            return 0;
        node = trie->transition[node][w[i]];
    }
    return trie->finite[node];
}

int main(){

    unsigned char mot[100];
    char reponse[10];
    bool chercher = true; 
    bool insert = true;

    printf("\n\t ************ vous avez choisi le trie avec des matrices de transition ************ \n");
    Trie T = createTrie(100);
    while (insert == true) { 
        // vider le buffer
        mot[0] = '\0';
        printf(" entrez le mot : \t");
        scanf("%s", mot);
        insertInTrie(T, mot);
        printf("vouler vous inserer un autre mot dans le trie y/n : \t");
        scanf("%s", reponse);
        if (reponse[0] == 'n') {
            insert = false;
        }
    }

    printf("\n \t ************ nous allons chercher les mots dans le trie ************ \n");
    while(chercher){
        mot[0] = '\0';
        printf("****** Entrez le mot que vous vouler chercher dans le trie : \t");
        scanf("%s", mot);

        if(isInTrie(T, mot)){
            printf("le mot %s est dans le trie\n", mot);
        }else{
            printf("le mot %s n'est pas dans le trie\n", mot);
        }

        printf("chercher un autre mot dans le trie y/n : \t");
        scanf("%s", reponse);
        if(reponse[0] == 'n'){
            chercher = false;
        }
    }
    return 0;
}