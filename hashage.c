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
Trie createTrie(int maxNode) {
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
void insertInTrie(Trie T, unsigned char *word) {
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
        // Si aucune transition n'a été trouvée pour cette lettre
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

// Fonction pour afficher le Trie
void printTrie(Trie T) {
    printf("Affichage du Trie:\n");
    for (int i = 0; i < T->maxNode; i++) {
        if (T->transition[i] != NULL) {
            List *current = T->transition[i];
            while (current != NULL) {
                printf("Node %d -> Letter: '%c' -> Target Node: %d\n", 
                       current->startNode, current->letter, current->targetNode);
                current = current->next;
            }
        }
    }
}

int main() {
    Trie T = createTrie(256);

    if (T == NULL || T->transition == NULL || T->finite == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }


    // 4. Vérifier les états finaux
    printf("Final states:\n");
    for (int i = 0; i < T->nextNode; i++) {
        if (T->finite[i]) {
            printf("State %d is a final state\n", i);
        }
    }

    
    printTrie(T);


    return 0;
}