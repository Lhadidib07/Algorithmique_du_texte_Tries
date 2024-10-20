#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define HASH_SIZE 256

typedef struct _list {
    int startNode; /* etat de depart de la transition */
    int targetNode; /* cible de la transition */
    unsigned char letter; /* etiquette de la transition */
    struct _list *next; /* maillon suivant */
} List;

typedef struct _trie {
    int maxNode; /* Nombre maximal de noeuds du trie */
    int nextNode; /* Indice du prochain noeud disponible */
    List **transition; /* listes d’adjacence */
    char *finite; /* etats terminaux */
} *Trie;

// fonction de hachage 
int hash(unsigned char key, int etat) {
    // Utiliser une constante multiplicative pour perturber la somme
    return (key * 31 + etat * 17) % HASH_SIZE;
}

// Crée un nouveau Trie
Trie createTrieHash(int maxNode) {
    Trie T = (Trie)malloc(sizeof(struct _trie));
    T->maxNode = maxNode;
    T->nextNode = 1; // Commence à 1 car 0 est l'état initial
    // Allouer de la mémoire pour les listes d'adjacence 
    T->transition = (List **)malloc(maxNode * sizeof(List *));
    // Allouer de la mémoire pour les états finaux
    T->finite = (char *)malloc(maxNode * sizeof(char));
    // Initialiser les listes d'adjacence et les états finaux
    for (int i = 0; i < maxNode; i++) {
        T->transition[i] = NULL;
        T->finite[i] = 0;
    }
    return T;
}

// Ajouter une transition à la liste d'adjacence
void addTransition(Trie T, int startNode, int targetNode, unsigned char letter) {
    // Calculer la valeur de hachage
    int hashResult = hash(letter, startNode); // Utiliser la fonction de hachage
    
    // Créer un nouveau maillon de transition
    List *newTransition = (List *)malloc(sizeof(List));
    newTransition->startNode = startNode;
    newTransition->targetNode = targetNode;
    newTransition->letter = letter;
    
    // Utiliser la valeur hachée pour stocker la transition dans l'endroit correct du tableau des transitions
    newTransition->next = T->transition[hashResult];
    T->transition[hashResult] = newTransition; 
}

// Ajouter un mot au Trie
void insertInTrieHash(Trie T, unsigned char *word) {
    int etat = 0, i = 0, hashResult=0;
    bool found = false;
    while (word[i] != '\0') {
        hashResult = hash(word[i], etat);
        List *current = T->transition[hashResult];
        found = false;
       // Parcourir la liste d'adjacence pour trouver la transition
        while (current != NULL) {
            if (current->letter == word[i]) {
                // Si la transition existe, avancer vers le nœud cible
                etat = current->targetNode;
                found = true;
                break;
            }
            current = current->next;
        }
        // Si aucune transition n'a été trouvere pour cette lettre
        if (!found) {
            int nextEtat = T->nextNode++;
            addTransition(T, etat, nextEtat, word[i]);
            etat = nextEtat;
        }
        i++; // passer a la lettre suivante
    } 
    // Marquer l'état final
    T->finite[etat] = 1;
}


void freeTrie(Trie T) {
    for (int i = 0; i < T->maxNode; i++) {
        List *current = T->transition[i];
        while (current != NULL) {
            List *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(T->transition);
    free(T->finite);
    free(T);
}


bool isInTrieHash(Trie T, unsigned char *word) {
    int etat = 0, i = 0,hashResult=0;
    bool found = false;

    // Parcourir le mot lettre par lettre
    while (word[i] != '\0') {
        hashResult = hash(word[i], etat);
        // Parcourir la liste d'adjacence pour trouver la transition
        List *current = T->transition[hashResult];
        found = false;
        while (current != NULL) {
            // Si la transition existe, avancer vers le nœud cible
            if (current->letter == word[i]) {
                etat = current->targetNode;
                found = true;
                break;
            }
            current = current->next;
        }
        // Si aucune transition n'a été trouvée pour cette lettre
        if (!found) {
            return false;
        }
        i++;
    }
    // retourné 1 si vrai 
    return T->finite[etat];
}


void insertPrefixesInTrie(Trie T, unsigned char *word) {
    int len = strlen((char *)word);
    for (int i = 1; i <= len; i++) {
        unsigned char *prefix = (unsigned char *)malloc(i + 1); // +1 pour le caractère nul final 
        strncpy((char *)prefix, (char *)word, i);// copier les i premiers caractères du mot dans le préfixe 
        prefix[i] = '\0';   // Null terminate the prefix
        insertInTrieHash(T, prefix); // insérer le préfixe dans le trie
        free(prefix);
    }
}

void insertSuffixesInTrie(Trie T, unsigned char *word) {
    int len = strlen((char *)word);
    // parcourir le mot lettre par lettre
    for (int i = 0; i < len; i++) {
        // insérer le sous-mot commençant à l'indice i 
        // &word[i] est l'adresse du i-ème caractère du mot
        insertInTrieHash(T, &word[i]);
    }
}

void insertFactorsInTrie(Trie T, unsigned char *word) {
    int len = strlen((char *)word);
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j <= len; j++) {
            unsigned char factor[j - i + 1];
            strncpy((char *)factor, (char *)&word[i], j - i);
            factor[j - i] = '\0';
            insertInTrieHash(T, factor);
        }
    }
}


int main(void) {
    Trie THash = createTrieHash(256);
    Trie Tprefixes = createTrieHash(256);
    Trie Tsufixes  = createTrieHash(256);
    Trie Tfactor  = createTrieHash(256);

    printf("\t insertion de algodutext \n");

    insertInTrieHash(THash, (unsigned char *)"algodutexte");


    printf("\n\t ----------------------tester si un mot existe dans un trie ----------------------\n\n");

    if (isInTrieHash(THash, (unsigned char *)"algodutexte") == 1) {
        printf("Le mot 'algodutexte' existe dans le trie .\n");
    } else {
        printf("Ce mot n existe pas dans ce trie.\n");
    }

    if (isInTrieHash(THash, (unsigned char *)"tpalgo") == 1) {
        printf("Le mot 'tpalgo' existe dasn le trie  .\n");
    } else {
        printf("Le mot 'tpalgo' n existe pas dans ce trie.\n");
    }

    char recherches[][100] = {
        "alg", "algo", "algodutexte", "tpalgo", "texte"
    };

    printf("\n\t ---------- insertion des prefix de algodutext ------------\n");
    insertPrefixesInTrie(Tprefixes, (unsigned char *)"algodutexte");

    printf("\n\t ---------------------- Test Prefix ----------------------\n");
    for(int i=0; i<6; i++){
        printf(" - Prefixe %s trouver ? %s\n", recherches[i], isInTrieHash(Tprefixes, (unsigned char *)recherches[i]) ? "Oui" : "Non");
    }

    printf("\n\t ---------------------- Test Sufix ----------------------\n\n ");

    insertSuffixesInTrie(Tsufixes, (unsigned char *)"algodutexte");
    for(int i=0; i<6; i++){
        printf(" - Suffixe %s trouver ? %s\n",recherches[i] ,isInTrieHash(Tsufixes, (unsigned char *)recherches[i]) ? "Oui" : "Non");
    }
    printf("\n\t ---------------------- Test Facteurs ----------------------\n\n");
    insertFactorsInTrie(Tfactor, (unsigned char *)"algodutexte");
    for(int i=0; i<6; i++){
        printf(" - Facteur %s trouver ? %s\n", recherches[i] ,isInTrieHash(Tfactor, (unsigned char *)recherches[i]) ? "Oui" : "Non");
    }
    return 0;
}

