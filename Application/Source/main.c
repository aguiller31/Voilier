#include "app.h"

/*
	Informations sur la "Programation Orientée Objet" dans ce programme :
	Le langage C n'étant pas un langage orienté objet, il a fallut établir des règles de programmation afin d'obtenir une POO proche de la POO traditionelle.
	Pour ce faire j'ai suivi la norme proposée par le site developpez.com : https://chgi.developpez.com/c/objet/
	Bien sûr cette norme ne fait qu'approcher la "vraie" POO mais ce choix a été fait pour faciliter le développement, la lisbilité et la maintenance(théorique) de ce code.
========================================
*/

Application * App;
void setup()
{
	App = New_Application();
	App->Setup(App);
	App->Start(App);
}

//même format que Arduino
void loop(){}

int main (void)
{
	setup();
	do
	{
		loop();
	}while(1);
}
