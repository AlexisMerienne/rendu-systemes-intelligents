
I. PARTIE N°1 : V1


Avant-propos :

Le code du controller contient quelques commentaires et est normalement assez facile à comprendre si vous voulez y jeter un oeil.


0. Choix de l'architecture

Pour le choix de l'architecture de coordination, l'architecture subsumption a été choisi de par sa facilité de mise en oeuvre (concrètement définir les priorités se fait via une condition "if") et sa pertinence par rapport à l'architecture DAMN (utilisant un système de vote) qui n'aurait pas tellement de sens car nous n'avons que 2 comportements différents dans notre cas : éviter l'obstacle ou suivre la lumière.


1. Mise en place de la solution

Dans la solution que nous vous avons donné, le robot est derrière un baril géant servant d'obstacle a éviter pour aller chercher la lumière qui se trouve juste derrière.

Etapes:
1- Ouvrez Webots
2- Allez dans "File" > "Open World..."
3- Sélectionnez ensuite le fichier "khepera2.wbt" qui se situe dans le dossier "worlds" lui même situé dans le dossier "khepera2", dans le dossier "V1". Chemin complet: "V1" > "khepera2" > "worlds" > "khepera2.wbt"
4- Lancez la simulation en appuyant sur le bouton play (raccourci : sur Windows "ctrl+2" ; sur Mac "cmd+2")
5- Accélérez la simulation en appuyant sur les doubles flèches (raccourci : sur Windows "ctrl+3" ; sur Mac "cmd+3")

Vous noterez que le robot tournera autour de la source lumineuse une fois celle-ci atteinte, il n'y a pas de mécanisme d'arrêt.



II. PARTIE N°2 : V2


Avant-propos:

Le code initialement écrit en C dans la V1, a été traduit en Python car les scripts associés aux containers ont été écrit en Python (plus facile pour établir les communications).


0. Architecture générale

L'architecture reprend celle donné en image sur le TD. Voici les différentes étapes des communications :
1- Le serveur webots envoie au container de coordination les valeurs des capteurs de distance et de luminosité selon la position actuelle.
2- Le container de coordination envoie les valeurs des capteurs de distance vers le container braitenberg qui gère les collisions, ainsi que les valeurs des capteurs de luminosité qu'il envoie au container de luminosité qui gère le suivi par le robot de la lumière.
3- Chacun des deux container de comportements vont calculer les vitesses des moteurs gauche et droit en fonction des données qu'ils auront reçu, et vont renvoyer ces vitesses au container de coordination.
4- Le container de coordination reçoit les vitesses des deux container de comportements.
5- Le container de coordination vérifie que la vitesse envoyé par le container de braitenberg n'est pas (0, 0), si c'est le cas, alors la vitesse des deux moteurs donné par le container de luminosité est envoyé au serveur webots. Sinon, la vitesse des deux moteurs donné par le container braitenberg est envoyé au server webots.
6- Le serveur webots reçoit la vitesse des deux moteurs, met à jour en conséquence la vitesse des moteurs.
7- Retour à l'étape 1.

