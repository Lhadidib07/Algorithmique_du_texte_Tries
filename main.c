#include <stdio.h>
#include "matrices.c"


int main() {
    Trie T = createTrie(100);
    insertInTrie(T, (unsigned char *)"hello");
    insertInTrie(T, (unsigned char *)"he");
    printf("%d\n", isInTrie(T, (unsigned char *)"hello"));
    printf("%d\n", isInTrie(T, (unsigned char *)"lo"));
    printf("%d\n", isInTrie(T, (unsigned char *)"he"));
    printf("%d\n", isInTrie(T, (unsigned char *)"o"));
    return 0;
}