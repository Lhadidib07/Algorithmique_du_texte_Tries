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
    int etat = 0;
    int i = 0;
    while (word[i] != '\0') {
        int hashResult = hash(word[i], etat);
        List *current = T->transition[hashResult];
        bool found = false;
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
    int etat = 0;
    int i = 0;
    while (word[i] != '\0') {
        int hashResult = hash(word[i], etat);
        List *current = T->transition[hashResult];
        bool found = false;
        while (current != NULL) {
            if (current->letter == word[i]) {
                etat = current->targetNode;
                found = true;
                break;
            }
            current = current->next;
        }
        if (!found) {
            return false;
        }
        i++;
    }
    return T->finite[etat];
}


void insertPrefixesInTrie(Trie T, unsigned char *word) {
    int len = strlen((char *)word);
    for (int i = 1; i <= len; i++) {
        unsigned char prefix[i + 1];
        strncpy((char *)prefix, (char *)word, i);
        prefix[i] = '\0';
        insertInTrieHash(T, prefix);
    }
}

void insertSuffixesInTrie(Trie T, unsigned char *word) {
    int len = strlen((char *)word);
    for (int i = 0; i < len; i++) {
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

    printf("----------------------Test insertInTrie(t, \"algodutexte\")----------------------\n");

    insertInTrieHash(THash, (unsigned char *)"algodutexte");

    if (isInTrieHash(THash, (unsigned char *)"algodutexte") == 1) {
        printf("Le mot 'algodutexte' existe deja.\n");
    } else {
        printf("Ce mot n existe pas dans ce trie.\n");
    }

    if (isInTrieHash(THash, (unsigned char *)"tpalgo") == 1) {
        printf("Le mot 'tpalgo' existe deja .\n");
    } else {
        printf("Le mot 'tpalgo' n existe pas dans ce trie.\n");
    }

    char recherches[][100] = {
        "alg", "algo", "algodutexte", "tpalgo", "texte"
    };
    printf("---------------------- Test Prefix ----------------------\n");

    insertPrefixesInTrie(Tprefixes, "algodutexte");
    for(int i=0; i<6; i++){
        printf(" - Prefixe %s trouver ? %s\n", recherches[i], isInTrieHash(Tprefixes,recherches[i]) ? "Oui" : "Non");
    }

    printf("---------------------- Test Sufix ----------------------\n");

    insertSuffixesInTrie(Tsufixes,"algodutexte");
    for(int i=0; i<6; i++){
        printf(" - Suffixe %s trouver ? %s\n",recherches[i] ,isInTrieHash(Tsufixes, recherches[i]) ? "Oui" : "Non");
    }
    printf("---------------------- Test Facteurs ----------------------\n");
    insertFactorsInTrie(Tfactor, "algodutexte");
    for(int i=0; i<6; i++){
        printf(" - Facteur %s trouver ? %s\n", recherches[i] ,isInTrieHash(Tfactor, recherches[i]) ? "Oui" : "Non");
    }
    return 0;
}

