/*******************************
 *
 *  Project Name: Project 5
 *  Description: The Local Movie Database stores the name, year, running length, genre, and revenue for a huge number of movies and allows for the user to make changes based on legal commands.
 *  Date: 4/3/24   
 *  Authors: Samuel Costa, Ashley Gutierrez
 *
 *******************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"

int compare(Movie *root, char *name, int year);
int getIndex(char *genreName);
extern char *GENRE_NAMES[];
int readName(FILE* file, char* name);

int main()
{
	char command[100];
	char name[1024];
	int year;
	int minutes;
	char genreName[100];
	long long revenue;
	char fileName[100];
	int genreIndex;
	Movie *roots[GENRES];

	// Allocates binary search trees for each genre.
	for (int i = 0; i < GENRES; ++i)
	{
		roots[i] = NULL;
	}

	// Continuously reads in user input. 
	do
	{
		printf("Enter command: (type \"help\" for more information) ");
		scanf("%s", command);
		getchar();
		
		// Decides which command to execute based on user input and handles its possible errors. 
		if (strcmp(command, "add") == 0)
		{
			printf( "\nEnter name: ");
			readName(stdin, name);
			printf( "Enter year: ");
			scanf("%d", &year);
			printf( "Enter length in minutes: ");
			scanf("%d", &minutes);
			printf( "Enter genre: ");
			scanf("%s", genreName);
			printf( "Enter revenue: ");
			scanf("%lld", &revenue);
			genreIndex = getIndex(genreName);
			
			//Error checking
			if (year < 1900 || year > 2050)
			{
				printf("Add failed: Invalid year %d\n", year);
			}
			else if (minutes < 0)
			{
				printf("Add failed: Invalid length of %d minutes\n", minutes);
			}
			else if (genreIndex == -1)
			{
				printf("Add failed: Invalid genre %s\n", genreName);
			}
			else if (revenue < 0)
			{
				printf("Add failed: Invalid revenue %lld\n", revenue);
			}
			else
			{
				roots[genreIndex] = insert(roots[genreIndex], name, year, minutes, genreIndex, revenue);
				printf("Add succeeded\n");
			}
		}
		else if (strcmp(command, "clear") == 0)
		{
			for (int i = 0; i < GENRES; ++i)
			{
				clear(roots[i]);
				roots[i] = NULL;
			}
			printf( "\nAll data cleared.\n");
		}
		else if (strcmp(command, "delete") == 0)  
		{
			printf("\nEnter name: ");
			readName(stdin, name);
			printf("Enter year: ");
			scanf("%d", &year);
			printf("Enter genre: ");
			scanf("%s", genreName);
			genreIndex = getIndex(genreName);
			
			//Error checking
			if (year < 1900 || year > 2050)
			{
				printf("Delete failed: Invalid year %d\n", year);
			}
			else if (genreIndex == -1)
			{
				printf( "Delete failed: Invalid genre %s\n", genreName);
			}
			else if (search(roots[genreIndex], name, year) == NULL)
			{
				printf("Delete failed: Movie %s (%d) not found in genre %s\n", name, year, genreName);
			}
			else
			{
				roots[genreIndex] = delete(roots[genreIndex], name, year);
				printf("Delete successful\n");
			}
		}
		else if (strcmp(command, "find") == 0)
		{
			printf("\nEnter name: ");
			readName(stdin, name);
			int matches = 0;
			
			for (int i = 0; i < GENRES; ++i)
			{
				matches += printMatches(roots[i], name);
			}
			printf( "%d matches found.\n", matches);
		}
		else if (strcmp(command, "help") == 0)
		{
			printf("\nCommand\n");
			printf( "\nadd");
			printf( "\n\tAdds a movie to the current list\n");
			printf( "\nclear");
			printf( "\n\tClears the current list of movies\n");
			printf( "\ndelete");
			printf( "\n\tDeletes a movie from the current list\n");
			printf( "\nfind");
			printf( "\n\tSearches for a movie\n");
			printf( "\nhelp");
			printf( "\n\tPrints the list of commands\n");
			printf( "\nhighest");
			printf( "\n\tPrints the highest revenue movie for each genre\n");
			printf( "\nload");
			printf( "\n\tAdds the contents of a file to the list of movies\n");
			printf( "\nquit");
			printf( "\n\tQuits the program\n");
			printf( "\nrevenue");
			printf( "\n\tChanges the revenue of a movie to the specified amount\n");
			printf( "\nsave");
			printf( "\n\tSaves the current list of movies to a file\n");
			printf( "\nstatistics");
			printf( "\n\tPrints out statistics for each genre\n\n");
		}
		else if (strcmp(command, "highest") == 0)
		{
			printf( "\n");
			for (int i = 0; i < GENRES; ++i)
			{
				Movie *maxMovie = highestGrossing(roots[i]);
				
				if (maxMovie == NULL)
				{
					printf( "%-12s\tUnknown\n", GENRE_NAMES[i]);
				}
				else
				{
					printf( "%-12s\t%-20s %d ($%lld)\n", GENRE_NAMES[i], maxMovie->name, maxMovie->year, maxMovie->revenue);
				}
			}
		}
		else if (strcmp(command, "load") == 0)  
		{
			printf("\nEnter a file: ");
			scanf("%s", fileName);
			FILE *file = fopen(fileName, "r");
			
			if(file == NULL)
			{
				printf( "Load failed: File %s not found\n", fileName);
			}
			else
			{
				int adds = 0;
				int errors = 0;

				while (readName(file, name) != EOF)
				{
					fscanf(file, "\t%d\t", &year);
					fscanf(file, "%d\t", &minutes);
					readName(file, genreName);
					fscanf(file, "\t%lld\n", &revenue);
					
					//Erro checking
					genreIndex = getIndex(genreName);
					if (year < 1900 || year > 2050 || minutes < 0 || genreIndex == -1 || revenue < 0)
					{
						++errors;
					}
					else
					{
						roots[genreIndex] = insert(roots[genreIndex], name, year, minutes, genreIndex, revenue);
						++adds;
					}
					
				}
				fclose(file);
				printf( "Loaded %d movies with %d errors\n", adds, errors);
			}
				
		}
		else if (strcmp(command, "revenue") == 0)
		{
			printf( "\nEnter name: ");
			readName(stdin, name);
			printf( "Enter year: ");
			scanf("%d", &year);
			printf( "Enter genre: ");
			scanf("%s", genreName);
			printf( "Enter new revenue: ");
			scanf("%lld", &revenue);
			genreIndex = getIndex(genreName);
			
			//Error checking
			if (year < 1900 || year > 2050)
			{
				printf( "Revenue change failed: Invalid year %d\n", year);
			}
			else if (genreIndex == -1)
			{
				printf( "Revenue change failed: Invalid genre %s\n", genreName);
			}
			else if (revenue < 0)
			{

				printf( "Revenue change failed: Invalid revenue %lld\n", revenue);
			}
			else if (search(roots[genreIndex], name, year) == NULL)
			{
				printf( "Revenue change failed: Movie %s (%d) not found in genre %s\n", name, year, genreName);
			}
			else
			{
				search(roots[genreIndex], name, year)->revenue = revenue;
				printf( "Revenue change successful\n");
			}
		}
		else if (strcmp(command, "save") == 0)
		{
			printf( "\nEnter a file: ");
			scanf("%s", fileName);
			FILE *file = fopen(fileName, "w");

			if (file == NULL)
			{
				printf( "Save failed: File %s not writable\n", fileName);
			}
			else
			{
				for (int i = 0; i < GENRES; ++i)
				{
					printAll(file, roots[i]);
				}
				printf( "Save succeeded\n");
				fclose(file);
			}
		}
		else if (strcmp(command, "statistics") == 0)
		{
			printf( "\n");
			for (int i = 0; i < GENRES; ++i)
			{
				printf( "%-12s\tCount:%10d\tTotal revenue: $%lld\n", GENRE_NAMES[i], count(roots[i]), totalRevenue(roots[i]));
			}
		}
		else if (strcmp(command, "quit") != 0)
		{
			printf( "\nUnknown command: %s\n", command);
		}

		printf( "\n");
	} while (strcmp(command, "quit") != 0);

	 // All memory is freed before quiting program.
	for (int i = 0; i < GENRES; ++i)
	{
		clear(roots[i]);
		roots[i] = NULL;
	}
	printf( "\nAll data cleared.\n");

	return 0;
}

//Insert movie into a tree respecting order constraints using recursion
Movie *insert(Movie *root, char *name, int year, int minutes, int genre, long long revenue)
{
	if (root == NULL)
	{
		root = malloc(sizeof(Movie));
		root->name = strdup(name);
		root->year = year;
		root->minutes = minutes;
		root->genre = genre;
		root->revenue = revenue;
		root->left = NULL;
		root->right = NULL;
	}
	else if (compare(root, name, year) < 0)
	{
		root->left = insert(root->left, name, year, minutes, genre, revenue);
	}
	else if (compare(root, name, year) > 0)
	{
		root->right = insert(root->right, name, year, minutes, genre, revenue);
	}
	return root;
}

//Goes through the tree looking for matches using recursion
Movie *search(Movie *root, char *name, int year)
{

	if (root == NULL)
	{
		return NULL;
	}
	else if (compare(root, name, year) > 0)
	{
		return search(root->right, name, year);
	}
	else if (compare(root, name, year) < 0)
	{
		return search(root->left, name, year);
	}
	else
	{
		return root;
	}
}

//Delete a movie from a tree that matches the name and year entered and free the memory afterwards
Movie* delete (Movie* root, char* name, int year)
{		
	if(root == NULL)
	{
		return NULL;
	}
	else if (compare(root, name, year) < 0)
	{
		root->left = delete(root->left, name, year);
	}
	else if (compare(root, name, year) > 0 )
	{
		root->right = delete(root->right, name, year);
	}
	else
	{
		Movie* oldRoot = root;
		if(root->left == NULL)
		{
			root = root->right;
			free(oldRoot->name);
			free(oldRoot);
		}
		else if(root->right == NULL )
		{
			root = root->left;
			free(oldRoot->name);
			free(oldRoot);	
		}
		else
		{
			//If given root have one child 
			if(root->right->left == NULL)
			{
				root->right->left = root->left;
				root = root->right;
				free(oldRoot->name);
				free(oldRoot); 
			}
			//If given root have both children
			else
			{
				Movie* parent = root->right; 
				Movie* temp = parent->left;
				
				while (temp->left != NULL)
				{
				    parent = temp;
				    temp = temp->left;
				}    
				parent->left = temp->right;
				temp->left = root->left;
				temp->right = root->right;
				root = temp;
				free(oldRoot->name);
				free(oldRoot);
			}	 	
		}
	}
	return root;	
}

// Frees all movies in the tree using a postorder traversal.
void clear(Movie *root)
{
	if (root != NULL) 
	{
		clear(root->left);
		clear(root->right);
		free(root->name);	// Frees the additional memory used when storing a name.
		root->name = NULL;
		free(root);
		root = NULL;
	}
}

// Prints information about the given movie.
void printMovie(Movie *movie)
{
	if (movie != NULL)
	{
		printf( "Name:\t\t%s\n", movie->name);
		printf( "Year:\t\t%d\n", movie->year);
		printf( "Length:\t\t%d minutes\n", movie->minutes);
		printf( "Genre:	\t%s\n", GENRE_NAMES[movie->genre]);

		if (movie->revenue == 0) 
		{
			printf( "Revenue:\tUnknown\n");
		}
		else
		{
			printf( "Revenue:\t$%lld\n", movie->revenue);
		}
		printf( "\n");
	}
}

// Prints all movies in the tree whose name matches the given name using a inorder traversal.
int printMatches(Movie *root, char *name)
{
	int matches = 0;
	if (root != NULL)
	{
		// The movies will be printed from oldest to most recent.
		matches += printMatches(root->left, name);
		
		// Ensures that matches are only printed.
		if (strcmp(name, root->name) == 0)
		{
			printMovie(root);
			++matches;
		}
		
		matches += printMatches(root->right, name);
	}
	return matches;	// Returns the number of successful matches found.
}

// Prints all movies in the tree to the given file using a preorder traversal.
void printAll(FILE *file, Movie *root) 
{
	if (root != NULL)
	{
		fprintf(file, "%s\t%d\t%d\t%s\t%lld\n", root->name, root->year, root->minutes, GENRE_NAMES[root->genre], root->revenue);
		printAll(file, root->left);
		printAll(file, root->right);
	}
}

// Counts the number of movies in the tree and returns it.
int count(Movie *root)
{
	if (root != NULL)
	{
		// Traverses the tree and each time a movie is found, it is added to the count.
		return (1 + count(root->left) + count(root->right));
	}
	return 0;
}

// Sums up the total revenue of the movies in the tree and returns it.
long long totalRevenue(Movie *root)
{
	if (root != NULL)
	{
		// Traverses the tree and each time a movie is found, its revenue is added to the sum.
		return (root->revenue + totalRevenue(root->left) + totalRevenue(root->right));
	}
	return 0;
}

// Finds the movie in tree with the highest revenue or returns NULL if no movie has a revenue greater than 0. Uses inorder traversal in case of ties, so the returned movie will have the oldest year. 
Movie* highestGrossing(Movie *root)
{
	Movie *maxMovie = NULL;  
	if (root != NULL) 
	{
		Movie *maxLeft = highestGrossing(root->left);  
		
		// Checks if maxLeft has a higher revenue than maxMovie or if there is no maxMovie and then sets it to be the new maxMovie.
		if (maxLeft != NULL && (maxMovie == NULL || maxLeft->revenue > maxMovie->revenue))
		{
			maxMovie = maxLeft;
		}
		// Checks if the current movie has a higher revenue than maxMovie or if there is no maxMovie and then sets it to be the new maxMovie.
		if (root->revenue > 0 && (maxMovie == NULL || root->revenue > maxMovie->revenue))
		{
			maxMovie = root;
		}
		Movie *maxRight = highestGrossing(root->right);
		
		// Checks if maxRight has a higher revenue than maxMovie or if there is no maxMovie and then sets it to be the new maxMovie.
		if (maxRight != NULL && (maxMovie == NULL || maxRight->revenue > maxMovie->revenue))
		{
			maxMovie = maxRight;
		}
	}
	return maxMovie;
}

// Finds the approriate index using GENRE_NAMES based off the given genreName.
int getIndex(char *genreName)
{
	int genreIndex = -1;
	int i = 0;

	// Loops through all the possible genres. 
	while (genreIndex == -1 && i < GENRES)
	{
		if (strcmp(genreName, GENRE_NAMES[i]) == 0)
		{
			genreIndex = i;
		}
		++i;
	}
	return genreIndex;	//Returns -1 if index is not found or an index if it is found.
}

//A custom function to check if a movie is the same using name and year
int compare(Movie *root, char *name, int year)
{
	int difference = strcmp(name, root->name);

	if (difference == 0)
	{
		//Checks if the year is the same by getting the difference
		difference = year - root->year; 
	}
	return difference;
}

// Reads in a file and stores a sequence of characters into the given name until reaching a tab, newline, or the end of the file. 
int readName(FILE* file, char* name)
{
	int c = fgetc(file);
	int i = 0;
	while( c != '\t' && c!=EOF && c!='\n')
	{
		name[i] = c;
		++i;
		c = fgetc(file);
	}
	// Adds a null charcter to make the array read like a string.
	name[i]= '\0';
	return c;
}
