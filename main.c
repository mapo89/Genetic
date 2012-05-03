/* 
 * File:   main.c
 * Author: alex
 *
 * Created on 7 marzo 2012, 16.28
 */
#include <stdio.h>
#include <stdlib.h>
#include "pieces.h"
#include "popolation.h"
#define SOGLIA_ESCALATION 1000
#define MAX_POP 50000


int main(int argc, char** argv) {
    /* Dichiarazione variabili */
    int row,col;//numero righe e colonne matrice dei pezzi
    int npieces,//numero pezzi
        *border,// vettore dei pezzi di bordo di npieces el.per ogni pezzo dice se è di bordo.val è pari al num di trinagoli grigi(0=centro,1=bordo,2=angolo)
            temp, // massimo di una generazione
            escalation, // contiene il numero di iterazioni non miglioranti consecutive
            i;
    int **pieces;//vettore dei colori del pezzo
    solution_t best;//contiene migliore soluzione trovata
    population_t *population; // puntatore a popolazione
    
    srand(time(NULL)); // randomizzazione del generatore di numeri pseudocasuali
    if (argc != 4){
        fprintf(stderr,"Usage: %s input_file output_file stats_file",argv[0]);
        exit(2);
    } 
    pieces=build_pieces(argv[1],&border,&npieces,&row,&col);
    population=build_population(pieces,border,npieces,row,col);
    const int MAX_PT=(row-1)*col+(col-1)*row;//costante di punti max dipende 
    escalation=0;
    //printf("Punteggio Massimo: %d\n",MAX_PT);
    sorted_popolation(population,pieces);
    test_evolution(population,&best,MAX_PT);
    best.fitness=population->soluzioni[0].fitness;
    best.matrice_pezzi=matcp(population->soluzioni[0],row,col);
    if(!(is_best(population,row,col))){
        while(population->pop_dim<=MAX_POP)
        for(i=0;(i<MAX_ITERATIONS)&&(best.fitness!=MAX_PT);i++){
            temp=pop_evolution(pieces,npieces,population,row,col,border);
                if(temp>best.fitness){
                    best.fitness=population->soluzioni[0].fitness;
                    best.matrice_pezzi=matcp(population->soluzioni[0],row,col);
                    escalation=0;
                }
                else {
                    escalation++;
                    if (escalation>SOGLIA_ESCALATION){
                        expand_population(pieces,npieces,population,row,col,border);
                        population->current_iteration=0;
                        escalation=0;
                        break;
                    }
                }
                test_evolution(population,&best,MAX_PT);
        }
    }
    write_best_solution(argv[2],best,row,col);
    write_evolution(population,argv[3]);
    //deallocazione memoria heap
    dealloc_population(population,row,col);
    free(population);
    dealloc_pieces(pieces,npieces);
    free(border);
    return (EXIT_SUCCESS);
}

