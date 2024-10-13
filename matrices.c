#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* avec Matrices de transition  */

struct _trie{ 
    int MaxNode; /* nombre max de nœuds du trie */
    int nextNode; /* indice du prochain nœud disponible */
    int **transition; /* matrice de transition */
    char *finite; /* états terminaux */
};

typedef struct _trie *Trie;

Trie createTrie(int maxNode){ 
    Trie T = (Trie)malloc(sizeof(struct _trie));
    T->MaxNode = maxNode;
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
            if(T->nextNode == T->MaxNode)
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

