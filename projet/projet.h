#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h> 
#define TCONNEX 1
#define TCHANG 5   
#define nbstation 304 


typedef struct arete{
    int val;
    int lf;
    int l1;
    int l2;
    int cout;
    int nbl;
} arete_t;

typedef struct station{
    int num;
    char nom[50];
    int ligne[5]; 
    int nbl;
    int marque;
} station_t;


typedef struct maillon{
    int data;
    struct maillon *suivant;
    struct maillon *prec;
    int valprec;
    int valprec2;
} maillon_t;

typedef struct pile{
    int taille;
    maillon_t *tete;
} pile_t;


typedef struct liste{
    int cout;
    int prec;
} liste_t;

maillon_t *new_maillon(int d);
pile_t *new_pile();
void push(pile_t *l, int d);
int pop(pile_t *l);
station_t new_station(int);
void dijkstra(int debut, int arrivee ,arete_t (*t)[nbstation+1],station_t s[]);

