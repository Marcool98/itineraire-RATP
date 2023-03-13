#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h> 
#include "projet.h" 


int main(){
    char a3[6],texte[50];
    int a1,a2,e,depart,arrivee,aux = 0,x,i=0;
    // On ouvre les fichiers csv pour récupérer les données.
     FILE *f = fopen("Metro Paris Data - Aretes.csv","r"); 
    if (f == NULL){
        return EXIT_FAILURE;
    } 
    fgets(texte,50,f);
    //On initialise le tableau d'arêtes.
    arete_t t[nbstation+1][nbstation+1];
    for(int i =1; i< nbstation+1;i++){
        for(int j =1; j< nbstation+1; j++){
            t[i][j].val =0;   
            t[i][j].nbl =0; 
            t[i][j].l1 = 0;  
            t[i][j].l2 = 0;  
        } 
    } 

    // On affiche la liste des stations avec leur numéros.
    station_t s[nbstation+1];
    for(int i = 1; i<nbstation+1;i++){
        s[i] = new_station(i);
        s[i].nbl = 0;
        printf( "%s,%d\n",s[i].nom,s[i].num);
    } 
    //On demande à l'utilisateur de rentrer deux stations.
    printf("Veuillez entrer un numéro de station de départ :");
    scanf("%d",&aux);
    depart = aux;
    printf("Veuillez entrer un numéro de station d'arrivée :");
    scanf("%d",&aux);
    arrivee = aux;
    // On rentre nos données dans le tableau d'arêtes et dans le tableau de stations.
    e = fscanf(f,"%d,%d,%s\n",&a1,&a2,a3);
    while(e != EOF){
            if(!strcmp(a3,"3bis"))
                strcpy(a3,"15");
            if(!strcmp(a3,"7bis"))
                strcpy(a3,"16");
            x = atoi(a3);   
            if(t[a1][a2].val == 1){
                i++;
                t[a1][a2].l2 = x;
                t[a1][a2].nbl ++;
                e = fscanf(f,"%d,%d,%s\n",&a1,&a2,a3);
            }
            else{
                i = 0;
                t[a1][a2].val =1;
                t[a1][a2].l1 = x; 
                t[a1][a2].nbl++; 
                e = fscanf(f,"%d,%d,%s\n",&a1,&a2,a3);
            } 
    } 
    fclose(f);
    printf("\n");
    dijkstra(depart,arrivee,t,s);
    return EXIT_SUCCESS;
} 




void dijkstra(int debut, int arrivee ,arete_t (*t)[nbstation+1],station_t s[]){
    liste_t l[nbstation +1];
    pile_t *p = new_pile();
    int coutfinal = 0,aux, aux2;
    for (int i = 1; i < nbstation+1; i++){
        for(int j = 1; j < nbstation+1; j++){
            t[i][j].cout = 0;
            t[i][j].lf = t[i][j].l1 ;   
        } 
        s[i].marque  = 0;
        l[i].cout =  INT_MAX;
    }
    l[debut].cout = 0;   
    for(int i = 1; i < nbstation; i++) {
        int num_min = INT_MAX,coutmin = INT_MAX;       
        for(int j = 1; j < nbstation+1; j++){
            if(!s[j].marque && l[j].cout <= coutmin){
                num_min = j;
                coutmin = l[j].cout;
                if(s[j].num == arrivee){
                    coutfinal = coutmin;
                } 
            } 
        }
        s[num_min].marque = 1;
        for( int k = 1 ; k < nbstation+1; k++){
            if(t[num_min][k].val == 1  && !s[k].marque){
                if(( i==1 || t[l[num_min].prec][num_min].lf  == t[num_min][k].l1 || t[l[num_min].prec][num_min].lf == t[num_min][k].l2)){
                    t[num_min][k].cout = TCONNEX;    
                    if( i != 1  && t[l[num_min].prec][num_min].lf == t[num_min][k].l2  ){
                        t[num_min][k].lf = t[num_min][k].l2;
                    }  
                }   
                else{
                    t[num_min][k].cout = TCONNEX + TCHANG;
                    aux = num_min;
                    while( aux != debut &&  t[l[aux].prec][aux].nbl > 1 && t[aux][k].nbl > 1){
                        if(t[l[aux].prec][aux].l2 == t[aux][k].l1 ){
                            t[num_min][k].cout = TCONNEX;
                            t[l[aux].prec][aux].lf = t[aux][k].l1;
                        } 
                        if((t[aux][k].l2 !=0 && t[l[aux].prec][k].l2 != 0 && t[l[aux].prec][aux].l2 == t[aux][k].l2 && t[l[aux].prec][aux].lf !=t[aux][k].l1 )){
                            t[num_min][k].cout = TCONNEX;
                            t[l[aux].prec][aux].lf = t[l[aux].prec][aux].l2;
                            t[aux][k].lf =  t[l[aux].prec][aux].l2;    
                        } 
                        aux = l[aux].prec; 
                    }        
                } 
                if(l[num_min].cout + t[num_min][k].cout < l[k].cout){
                    l[k].cout = l[num_min].cout + t[num_min][k].cout ; 
                    l[k].prec = num_min;    
                } 
            } 
        }  
    }  
    aux2 = arrivee;
    int z = 0;
    printf("Pour aller de la station %s à la station %s, il faut : \n",s[debut].nom,s[arrivee].nom);
    while (aux2 != debut){
        push(p,aux2);
        if(z == 1){
            p->tete->suivant->valprec = aux2;
            if(p->taille > 2)
                p->tete->suivant->suivant->valprec2 = aux2;
        } 
        aux2 = l[aux2].prec;
        z = 1;
    } 
    push(p,aux2);
    z = 0;
    while ( p->taille != 0  ){
        if( z == 0 ){
            if(t[p->tete->data][p->tete->suivant->data].lf == 15)
                printf("Prendre la ligne 3bis\n"); 
            else if(t[p->tete->data][p->tete->suivant->data].lf == 16)
                printf("Prendre la ligne 7bis\n"); 
            else
                printf("Prendre la ligne %d :\n", t[p->tete->data][p->tete->suivant->data].lf); 
        } 
        aux2 = pop(p);
        z += 1;
        if (z >= 2)
            printf("Aller à %s en %d min(s).\n", s[aux2].nom,l[aux2].cout );
        if(z >= 2 && p->taille != 0 && p->tete->valprec2 !=0 && p->tete->suivant != NULL 
        && t[p->tete->valprec2][p->tete->valprec].lf != t[p->tete->valprec][p->tete->data].lf ){
            if(t[p->tete->valprec][p->tete->data].lf == 15)
                printf("Faites un changement pour la ligne 3bis\n");
            else if(t[p->tete->valprec][p->tete->data].lf == 16)
                printf("Faites un changement pour la ligne 7bis\n");
            else 
                printf("Faites un changement pour la ligne %d\n",t[p->tete->valprec][p->tete->data].lf);
        } 
    } 
    printf("Vous êtes arrivé à destination, ce trajet vous prendra %d minute(s).\n ",coutfinal );
} 


// Cette fonction sert à initialiser une station avec son nom et son numéro.
station_t new_station(int n){
    FILE *g = fopen("Metro Paris Data - Stations.csv","rb");
    if (g == NULL){
        assert(0);
    } 
    station_t s;
    char a[50],b[5] ;  
    int e =0, aux =1;
    s.num = n;
    fgets(a,50,g);
    while(e != EOF  ){
        e = fscanf(g,"%[^,], %[^\n]",a,b);
        fgetc(g);
        if(aux == s.num){
            strcpy(s.nom,a);
        } 
        aux++;
    } 
    fclose(g);
    return s;
 }

// Cette fonction initialise un nouveau maillon.
maillon_t *new_maillon(int d){
    maillon_t *m = malloc(sizeof(maillon_t));
    if (m == NULL)  
        assert(0);
    m->data = d;
    m->valprec = 0;
    m->valprec2 = 0;
    return m;
} 
// Cette fonction initialise une nouvelle pile;
pile_t *new_pile(){
    pile_t *p = malloc(sizeof(pile_t));
    if(p == NULL)
        assert(0);
    p->taille = 0;
    p->tete = NULL;
    return p;
} 

// Cette fonction ajoute un maillon en tête de pile 
void push(pile_t *p, int d){
    maillon_t *m = new_maillon(d);
    m->suivant = p->tete;
    p->tete = m;
    p->taille += 1;
}

// Cette fonction retire la tête de la pile
int pop(pile_t *p){
    maillon_t *t = p->tete;
    int r = t->data;
    p->tete = p->tete->suivant;
    free(t);
    p->taille -= 1;
    return r;
 }