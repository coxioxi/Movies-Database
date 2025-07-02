#ifndef MOVIE_H
#define MOVIE_H

#include <stdio.h>

#define GENRES 25

typedef enum { ACTION, ADVENTURE, ANIMATION, BIOGRAPHY, COMEDY, CRIME, DOCUMENTARY, DRAMA, FAMILY, FANTASY, FILM_NOIR, HISTORY, HORROR, MUSIC, MUSICAL, MYSTERY, NEWS, REALITY_TV, ROMANCE, SCI_FI, SPORT, TALK_SHOW, THRILLER, WAR, WESTERN } Genre;


typedef struct _Movie {
	char* name;
	int year;
	int minutes;
	Genre genre;
	long long revenue;
		
	struct _Movie* left;
	struct _Movie* right;
} Movie;

Movie* insert (Movie* root, char* name, int year, int minutes, int genre, long long revenue);
Movie* search (Movie* root, char* name, int year);
int printMatches (Movie* root, char* name);
Movie* delete (Movie* root, char* name, int year);
void printAll (FILE* file, Movie* root);
void printMovie(Movie* movie);
void clear (Movie* root);
int count (Movie* root);
long long totalRevenue (Movie* root);
Movie* highestGrossing (Movie* root);

#endif
