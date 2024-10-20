# Nom de l'exécutable
EXEC = programme

# Liste des fichiers source
SRC = hashage.c matrices.c

# Liste des fichiers objets générés
OBJ = $(SRC:.c=.o)

# Compilateur et options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Règle par défaut
all: $(EXEC)

# Règle pour créer l'exécutable
$(EXEC): $(OBJ)
    $(CC) -o $@ $^

# Règle pour compiler les fichiers source en fichiers objets
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
    rm -f $(OBJ) $(EXEC)

# Règle pour exécuter le programme
run: $(EXEC)
    ./$(EXEC)

# Règle pour afficher les dépendances
depend: $(SRC)
    makedepend $^

# Inclusion des dépendances générées
-include Makefile.dep

# Commande pour générer les dépendances
Makefile.dep: $(SRC)
    $(CC) -MM $(SRC) > Makefile.dep