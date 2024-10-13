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

int main() {

    unsigned char mot[100];
    char reponse[10];
    bool chercher = true; 
    bool insert = true;
    int choixInsertion = -1;
    int choix = -1;
    int tester = true; 

    printf("\n\t ************  le trie avec le hachage  ************\n");

    Trie THash = createTrieHash(256);
        if (THash == NULL || THash->transition == NULL || THash->finite == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    Trie Tprefixes = createTrieHash(256);
        if (Tprefixes == NULL || Tprefixes->transition == NULL || Tprefixes->finite == NULL) {
        fprintf(stderr, "Memory allocation failed pour le trie des  prefixes !\n");
        exit(EXIT_FAILURE);
    }

    Trie Tsufixes  = createTrieHash(256);
        if (Tsufixes == NULL || Tsufixes->transition == NULL || Tsufixes->finite == NULL) {
        fprintf(stderr, "Memory allocation failed pour le trie des  sufixes  !\n");
        exit(EXIT_FAILURE);
    }

    Trie Tfactor  = createTrieHash(256);
        if (Tfactor == NULL || Tfactor->transition == NULL || Tfactor->finite == NULL) {
        fprintf(stderr, "Memory allocation failed pour le trie des facteur  !\n");
        exit(EXIT_FAILURE);
    }


    insert = true;
    do{ 
        printf(" 1. Insertion des mots dans le trie \n");
        printf(" 2. Recherche d'un mot dans le trie \n");
        printf(" 3. Insertion de prefix ,sufix et facotor d'un mot \n");
        printf(" 4. Recherches de prefix ,sufix et facotor d'un mot \n");

        printf(" 0. Pour sortir \n");
        printf("entrez votre choix : \t");
        scanf("%d", &choix);

        switch (choix)
        {
            case 0:
                tester = false;
            break;
            case 1:
                printf("****** faire l'insertion des mots dans le trie \n");
                mot[0] = '\0';
                printf(" entrez le mot : \t");
                scanf("%s", mot);
                insertInTrieHash(THash, mot);
            break;

            case 2:
                mot[0] = '\0';
                printf("****** Entrez le mot que vous vouler chercher dans le trie : \t");
                scanf("%s", mot);
                printf("\n \t  \"%s\" est dans le trie ? %s\n", mot, isInTrieHash(THash, mot) ? "Oui" : "Non");
            break;

            case 3:
                mot[0] = '\0';
                printf("****** insertion de prefix ,sufix et facotor d'un mot \n");
                printf("entrez votre mot : \t");
                scanf("%s", mot);
                do{ 
                    printf("\n*********************%s*******************************\n",mot);
                    printf(" 1. insertion des prefixe  \n");
                    printf(" 2. insertion des suffixe  \n");
                    printf(" 3. insertion des facteur  \n");
                    printf(" 0. Pour sortir \n");
                    printf("faites votre choix : \t");
                    scanf("%d",&choixInsertion);
                    switch (choixInsertion)
                    {
                        case 1:
                            printf("\n insertion des prefixe d'un mot dans le trie \n");
                            insertPrefixesInTrie(Tprefixes, mot);
                            break;
                        case 2:
                            printf(" insertion des suffixe d'un mot dans le trie \n");
                            insertSuffixesInTrie(Tsufixes, mot);
                        break;
                        case 3: 
                            printf(" insertion des facteur d'un mot dans le trie \n");
                            insertFactorsInTrie(Tfactor, mot);
                        break;
                        case 0:
                        break;
                        
                        default:
                            printf(" choix invalide \n");
                        break;
                    }
                }while(choixInsertion != 0);
                

            break;
            case 4:
           
                do{ 
                    mot[0] = '\0';
                    printf("****** Entrez le mot que vous vouler chercher dans le trie : \t");
                    scanf("%s", mot);
                    printf("\n*********************%s*******************************\n",mot);
                    printf(" 1. Rechercher dans prefixe trie \n");
                    printf(" 2. Rechercher dans sufixe trie   \n");
                    printf(" 3. Rechercher dans facteur trie  \n");
                    printf(" 0. Pour sortir \n");
                    printf("faites votre choix : \t");
                    scanf("%d",&choixInsertion);
                    switch (choixInsertion)
                    {
                        case 1:
                            printf("\n insertion des prefixe d'un mot dans le trie \n");
                            insertPrefixesInTrie(Tprefixes, mot);
                            printf("\n \t  \"%s\" est dans le trie des préfixes ? %s\n", mot, isInTrieHash(Tprefixes, mot) ? "Oui" : "Non");

                            break;
                        case 2:
                            printf(" insertion des suffixe d'un mot dans le trie \n");
                            insertSuffixesInTrie(Tsufixes, mot);
                            printf("\n \t  \"%s\" est dans le trie des suffixes? %s\n", mot, isInTrieHash(Tsufixes, mot) ? "Oui" : "Non");

                        break;
                        case 3: 
                            printf(" insertion des facteur d'un mot dans le trie \n");
                            insertFactorsInTrie(Tfactor, mot);
                            printf("\n \t  \"%s\" est dans le trie des facteurs  ? %s\n", mot, isInTrieHash(Tfactor, mot) ? "Oui" : "Non");

                        break;
                        case 0:
                        break;
                        
                        default:
                            printf(" choix invalide \n");
                        break;
                    }
                }while(choixInsertion != 0);
                

            break;
            
            default:
                printf(" choix invalide \n");
            break;
        }
    }while (tester);
   
    


    return 0;
}