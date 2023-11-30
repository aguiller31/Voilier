#include "app.h"

/*
	Informations sur la "Programation Orient�e Objet" dans ce programme :
	Le langage C n'�tant pas un langage orient� objet, il a fallut �tablir des r�gles de programmation afin d'obtenir une POO proche de la POO traditionelle.
	Pour ce faire j'ai suivi la norme propos�e par le site developpez.com : https://chgi.developpez.com/c/objet/
	Bien s�r cette norme ne fait qu'approcher la "vraie" POO mais ce choix a �t� fait pour faciliter le d�veloppement, la lisbilit� et la maintenance(th�orique) de ce code.
========================================
*/

Application * App;
void setup()
{
	App = New_Application();
	App->Setup(App);
	App->Start(App);
}

//m�me format que Arduino
void loop(){}

int main (void)
{
	setup();
	do
	{
		loop();
	}while(1);
}
