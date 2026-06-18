# ProjecteMinions

Aquest projectes és un sistema de gestió per organitzar la producció d'una fàbrica de minions, desenvolupat en C i compilable en l'entorn Matagalls

# Compilació

Per compilar el projecte:
-Fer make
    -Es generarà l'executable "minions" a l'arrel del projecte

# Execució

./minions

# Altres comandes del makefile

make clean
Esborra els fitxers objecte, l'executable "minions" i l'exectuable de tests

make test
Compila i executa els tests TDD, en concret tdd/test.c, que agrupa els tres tests fets seguint RED-GREEN-REFACTOR

# Usuaris hardcoded

Per a poder provar el programa des d'un inici hi ha tres usuaris hardcoded al codi (un de cada rol bàsic que demana l'enunciat: Gru, Minion i Superminion). A més dins del fitxer data/users.txt que s'entrega ja queda guardat un quart usuari, Puyi que té el rol de Superminion, afegit manualment durant les proves, per facilitar comprovar funcionalitats com l'evolució de minions sense haver-lo de registrar de nou.


# Modular

Tal com es demanava en l'enunciat, el projecte s'ha fet modularment i ha seguit segons les funcionalitats generals i especifiques diferents carpetes i estructures perquè estigui ben implementat.