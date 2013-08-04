Documentation importateur Radiance vers OpenGL (C++)

Objectif de l’application

Cette application a pour but de fournir un importateur du format radiance de Greg Ward (.rad) vers une application OpenGL C++. Elle fournit également une lumière de type diffuse ainsi que l’ombrage des surfaces

Commandes

Souris (bouton gauche) : Rotation de la caméra
Z : Déplacement avant de la caméra
S : Déplacement arrière de la caméra
Q : Déplacement latéral gauche de la caméra
D : Déplacement latéral droit de la caméra
F : Activation ou désactivation des ombres
Echap : Quitter le programme.

Compilation du programme

Ce projet se compile sous Visual Studio 2010 C++ (aussi bien avec la version payante que la version express). Elle utilise les librairies externes suivantes :
- OpenGL (à partir de la version 3.0)
- SDL : Interface graphique de l’application
- Assimp : importe des modèles 3D (Collada) vers l’application
- Devil : Importe des formats d’images (jpg, png, ppm) vers l’application
- Glm : Bibliothèque Mathématique
Pour relier les projets, nous utilisons des variables systèmes, reconnaissables par les variables se nommant par NOMAPPLICATION_PATH par exemple SDL_PATH ou ASSIM_PATH. Si vous êtes sous windows, il faut définir les variables système pour que Visual Studio relie automatiquement chaques librairies au projet.

Création d’une variablement d’environnement sous Windows 7

Les étapes pour créer une variable système sous Windows 7 sont :
- Cliquer sur démarrer
- Ordinateur => Propriétés => Paramètres système avancées => Variables d’environnement…
- Dans la catégorie « variables utilisateur pour NOM_UTILISATEUR » Cliquer sur nouvelle
- Une fenêtre s’ouvre, entrer exactement le même nom que la variable dans le projet Visual Studio, puis dans la ligne « Valeur de la variable » Entrer le chemin vers la librairie.
Les libraires sont maintenant attachés au programme, il suffit de compiler le bouton avec le bouton F7 puis de lancer le programme via F5.

Définir sa scène Radiance

C’est le format rif qui permettra à l’utilisateur de définir sa scène. Pour ajouter des éléments radiances, il faut taper :
scene = obj 1.rad 
scene = obj2.rad 
scene = obj3.rad 
Ainsi de suite
