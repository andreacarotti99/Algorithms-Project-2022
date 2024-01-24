#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITO 4294967295

/********************************************************************VARIABILI GLOBALI**************************************************************************/

typedef struct ElementoCoda {
    unsigned int indice;
    unsigned int qty;
} ElementoCoda;

typedef struct CodaPriorita{
    int currentDimCoda;
    int maxDimCoda;
    ElementoCoda *CODA;
} CodaPriorita;

typedef struct NodoDiArrivo{
    unsigned int peso;
    int isInfiniteDistance;
} NodoDiArrivo;

typedef struct NodoDiPartenza{
    NodoDiArrivo *vettoreNodiDiArrivo;
} NodoDiPartenza;

typedef struct Grafo {
    int maxNumVertici;   /* numero massimo di nodi del grafo */
    int indiceGrafo;
    unsigned int sommaCammini;
    NodoDiPartenza *vettoreNodiDiPartenza; 
} *Grafo;

/********************************************************************DICHIARAZIONE FUNZIONI***********************************************************************/

int leggiLunghezzaGrafo();
int leggiTopK();
int leggiIntero();
int posizioneUltimoElCoda();
int getPadreElemento();
int isCodaPiena();
int estraiPrimoElDaHeapSenzaSistemare();
Grafo leggiGrafo();
Grafo leggiGrafoVeloce();
Grafo creaGrafo();
Grafo resetGrafo();
void decrementaChiave();
void aggiustaMinHeap();
void aggiustaMaxHeap();
void inserisciNelMinHeap();
void scambiaDueElementiCoda();
void creaCoda();
void stampaDistanze();
void stampaTopK();
void controllaEInserisciNelMaxHeap();
void leggiStdIn();
void stampaSommaCamminiMinimiDelGrafo();
void stampaSommaPesiTopk();
void stampaGrafoMatriciale();
void aggiungiElInFondoACodaSenzaSistemare();
unsigned int calcolaSommaCamminiMinimi();
unsigned int leggiNumeroVeloce();
unsigned int djikstra();
unsigned int figlioSx();
unsigned int figlioDx();

/********************************************************************MAIN******************************************************************************/

int main ()
{
    int lunghezzaGrafo;
    int contatoreIndiceGrafo = 0;
    unsigned int topK;
    unsigned int sommaDelleDistanze = 0;
    Grafo G = NULL;
    CodaPriorita *codaTopK = NULL;

    codaTopK = (CodaPriorita *) malloc(sizeof(struct CodaPriorita));
    //lettura prima riga
    lunghezzaGrafo = leggiIntero();
    topK = leggiIntero();
    //lettura seconda riga in poi
    leggiStdIn(G, lunghezzaGrafo, sommaDelleDistanze, codaTopK, &contatoreIndiceGrafo, topK);
    free(codaTopK);
    return 0;
}

/********************************************************************FUNZIONI******************************************************************************/

int leggiIntero(){
    int num = 0;
    char ch;

    ch = getchar();

    while (ch >= '0' && ch <= '9'){
        num = num * 10;
        num = num + (ch - '0');
        ch = getchar();
    }
    return num;
}

void leggiStdIn(Grafo G, int lunghezzaGrafo, unsigned int sommaDelleDistanze, CodaPriorita *codaTopK, int *contatoreIndiceGrafo, unsigned int topK){
    char buffer[15];
    
    creaCoda(codaTopK, topK);
    G = creaGrafo(lunghezzaGrafo);

    while(1)
    {
        if (fgets(buffer, 15, stdin) == NULL){
            return;
        }
        else{
            switch(buffer[0]){
                case 'A':

                    G = resetGrafo(G, lunghezzaGrafo);
                    G = leggiGrafoVeloce(G, lunghezzaGrafo);

                    G->indiceGrafo = *contatoreIndiceGrafo;
                    G->sommaCammini = djikstra(G);
                    

                    (*contatoreIndiceGrafo) = (*contatoreIndiceGrafo) + 1;
                    controllaEInserisciNelMaxHeap(codaTopK, G->indiceGrafo, G->sommaCammini);
                    //stampaSommaCamminiMinimiDelGrafo(G);                                          //debug
                    //stampaTopK(codaTopK);                                                         //debug
                    //stampaGrafoMatriciale(G);                                                     //debug
                    break;
                case 'T':
                    stampaTopK(codaTopK);
                    //stampaSommaPesiTopk(codaTopK);                                               //debug
                    break;
                default:
                    break;
            }
        }    
    }
    free(G);
}

void stampaSommaPesiTopk(CodaPriorita *codaTopK){
    int i;
    for (i=0;i<codaTopK->currentDimCoda;i++){
        if (codaTopK->CODA[i].indice > 10){
            printf("Grafo: %d       Peso: %u\n", codaTopK->CODA[i].indice, codaTopK->CODA[i].qty);
        }
        else {
            printf("Grafo: %d        Peso: %u\n", codaTopK->CODA[i].indice, codaTopK->CODA[i].qty);
        }
    }
}

void stampaTopK(CodaPriorita *codaTopK){
    int i;

    if(codaTopK->currentDimCoda == 0){
        printf("\n");
    }
    else{
        for (i=0; i < codaTopK->currentDimCoda - 1; i++){
            printf("%d ", codaTopK->CODA[i].indice);
        }
            printf("%d", codaTopK->CODA[codaTopK->currentDimCoda - 1].indice);
            printf("\n");
    }    
}

Grafo creaGrafo(int lunghezzaGrafo){
    Grafo G;
    int i, j;

        G = (Grafo) malloc(sizeof(struct Grafo));
        G->vettoreNodiDiPartenza = (NodoDiPartenza *) malloc(lunghezzaGrafo * sizeof(struct NodoDiPartenza));
        G->maxNumVertici = lunghezzaGrafo;
        for (i=0; i<lunghezzaGrafo;i++){
            G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo = (NodoDiArrivo *) malloc(lunghezzaGrafo * sizeof(struct NodoDiArrivo));
            for (j=0;j<lunghezzaGrafo; j++){
                G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].peso = 0;
                G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].isInfiniteDistance = 1;
            }
        }
    return G;
}

Grafo resetGrafo(Grafo G, int lunghezzaGrafo){
    int i, j;

    if (lunghezzaGrafo > 0){
        for (i=0; i<lunghezzaGrafo; i++){
            for (j=0; j<lunghezzaGrafo;j++){
                G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].peso = 0;
                G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].isInfiniteDistance = 1;
            }
        }
    }
    return G;
}


Grafo leggiGrafoVeloce(Grafo G, int lunghezzaGrafo){
    int i,j;
    unsigned int num;

    for (i = 0; i < lunghezzaGrafo; i++){
        for (j = 0; j < lunghezzaGrafo; j++){
            num = leggiNumeroVeloce();
            if (num != 0 && i != j){
                    G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].peso = num;
                    G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].isInfiniteDistance = 0;
                    //printf("Arco da %d a %d, con peso %u aggiunto\n", u, v, G->vettoreNodiDiPartenza[u].vettoreNodiDiArrivo[v].peso);
            }
        }
    }
    return G;
}

unsigned int leggiNumeroVeloce(){
    unsigned int num = 0;
    char ch;
    int toSum;

    ch = getchar();

    while (ch >= '0' && ch <= '9'){
        toSum = ch - '0';
        num = num * 10;
        num = num + toSum;
        ch = getchar();
    }
    return num;
}

void creaCoda(CodaPriorita *codaPriorita, int maxDimensioneCoda){
    codaPriorita->CODA = (ElementoCoda *) malloc(maxDimensioneCoda * sizeof(struct ElementoCoda));
    codaPriorita->maxDimCoda = maxDimensioneCoda;
    codaPriorita->currentDimCoda = 0;
}

int isCodaPiena(CodaPriorita *codaPriorita){
    if (codaPriorita->currentDimCoda == codaPriorita->maxDimCoda) return 1;
    else return 0;
}

void scambiaDueElementiCoda(CodaPriorita *codaPriorita, int primo, int secondo){
    ElementoCoda temp;

    temp = codaPriorita->CODA[primo];
    codaPriorita->CODA[primo] = codaPriorita->CODA[secondo];
    codaPriorita->CODA[secondo] = temp;
}

void controllaEInserisciNelMaxHeap(CodaPriorita *codaPriorita, unsigned int indiceNuovoGrafo, unsigned int sommaCamminiDaInserire){
    ElementoCoda temp;
    int ultimo;

    //SE LA CODA DI TOPK NON E' PIENA LO INSERISCO IN POSIZIONE GIUSTA
    //SE LA CODA DI TOPK E' PIENA CONTROLLO SE LA SOMMACAMMINI DA INSERIRE E' MINORE DEL MAXHEAP(PRIMO EL.) DELLA CODA, SE E' MINORE LA INSERISCO E FIXO LA CODA

    switch (isCodaPiena(codaPriorita)){
        case 0:
            ultimo = posizioneUltimoElCoda(codaPriorita);
            codaPriorita->currentDimCoda = codaPriorita->currentDimCoda + 1;
            temp.indice = indiceNuovoGrafo;
            temp.qty = sommaCamminiDaInserire;
            codaPriorita->CODA[posizioneUltimoElCoda(codaPriorita)] = temp;

            while(ultimo>0 && codaPriorita->CODA[ultimo].qty > codaPriorita->CODA[getPadreElemento(ultimo)].qty){
                scambiaDueElementiCoda(codaPriorita, ultimo, getPadreElemento(ultimo));
                ultimo = getPadreElemento(ultimo);
            }
            aggiustaMaxHeap(codaPriorita, 0);
            break;
        case 1:
            if(sommaCamminiDaInserire < codaPriorita->CODA[0].qty){
           
                estraiPrimoElDaHeapSenzaSistemare(codaPriorita);
                aggiustaMaxHeap(codaPriorita, 0);
                aggiungiElInFondoACodaSenzaSistemare(codaPriorita, indiceNuovoGrafo, sommaCamminiDaInserire);
                aggiustaMaxHeap(codaPriorita, 0);

            }
            break;
        default:
            break;
    }
}

int posizioneUltimoElCoda(CodaPriorita *codaPriorita){
    return (codaPriorita->currentDimCoda - 1);
}

void aggiungiElInFondoACodaSenzaSistemare(CodaPriorita *codaPriorita, unsigned int indiceNuovoGrafo, unsigned int sommaCammini){
        ElementoCoda elDaAggiungere;
        
        codaPriorita->currentDimCoda = codaPriorita->currentDimCoda + 1;
        elDaAggiungere.indice = indiceNuovoGrafo;
        elDaAggiungere.qty = sommaCammini; 
        codaPriorita->CODA[posizioneUltimoElCoda(codaPriorita)] = elDaAggiungere; //es. se la dim. della coda è 6 l'ultimo el. è in posizione 5
}

void aggiustaMaxHeap(CodaPriorita *codaPriorita, int n){
    int posmax, l, r;

    posmax = n;
    l = figlioSx(n);
    r = figlioDx(n);    

    if(l <= posizioneUltimoElCoda(codaPriorita) &&  codaPriorita->CODA[posmax].qty < codaPriorita->CODA[l].qty) {
        posmax = l;
    }
    else {
        posmax = n;
    }
    
    if (r <= posizioneUltimoElCoda(codaPriorita) && codaPriorita->CODA[posmax].qty < codaPriorita->CODA[r].qty){
        posmax = r;
    }
    if(posmax != n){
        scambiaDueElementiCoda(codaPriorita, n, posmax);
        aggiustaMaxHeap(codaPriorita, posmax);   
    }
}


void aggiustaMinHeap(CodaPriorita *codaPriorita, int primo){
    int nuovoElementoInferiore, elementoFiglioSx, elementoFiglioDx;

    nuovoElementoInferiore = primo;
    elementoFiglioSx = figlioSx(primo);
    elementoFiglioDx = figlioDx(primo);

    if(elementoFiglioSx <= posizioneUltimoElCoda(codaPriorita) &&  codaPriorita->CODA[nuovoElementoInferiore].qty > codaPriorita->CODA[elementoFiglioSx].qty){
        nuovoElementoInferiore = figlioSx(primo);
    }    
    if (elementoFiglioDx <= posizioneUltimoElCoda(codaPriorita) && codaPriorita->CODA[nuovoElementoInferiore].qty > codaPriorita->CODA[elementoFiglioDx].qty){
        nuovoElementoInferiore = figlioDx(primo);
    }
    if(nuovoElementoInferiore != primo){
        scambiaDueElementiCoda(codaPriorita, primo, nuovoElementoInferiore);
        aggiustaMinHeap(codaPriorita, nuovoElementoInferiore);

    }
}


/*
* Continuo a scambiare gli elementi finche il nuovo elemento inserito non è nel posto giusto
* [3,4,6,5,9,8] e inserisco l'el con qty 2 la dim della coda ora è 6 quindi aggiungo l'el in posiz 6
* [3,4,6,5,9,8,2]       (6-1)/2 = 2 quindi confronto l'el inserito: [2] con l'el in posiz. 2: [6]
*                       scambio gli elementi poiche 2 < 6 !
* new:
* [3,4,2,5,9,8,6]       attenzione però perchè 2 non è ancora nella posizione finale!
*                       ultimo = (ultimo-1)/2 --> ultimo = 2 > 0! quindi eseguo ancora il ciclo...
*                       (2-1)/2 = 0 
*                       confronto...
* new:
* [2,4,3,5,9,8,6]
*/

void inserisciNelMinHeap(CodaPriorita *codaPriorita, unsigned int newIndice, unsigned int newQty){
    int ultimo;

    aggiungiElInFondoACodaSenzaSistemare(codaPriorita, newIndice, newQty);
    ultimo = posizioneUltimoElCoda(codaPriorita);

    while(ultimo>0 && codaPriorita->CODA[ultimo].qty < codaPriorita->CODA[getPadreElemento(ultimo)].qty){
        
        scambiaDueElementiCoda(codaPriorita, ultimo, getPadreElemento(ultimo));
        ultimo = getPadreElemento(ultimo);
    }
    
}


unsigned int figlioSx(unsigned int i){
    return ((2*i) + 1);
}

unsigned int figlioDx(unsigned int i){
    return ((2*i) + 2);
}


void decrementaChiave(CodaPriorita *codaPriorita, unsigned int nodoDaCercare, unsigned int newQty){
    int i, indicePadre;

    for(i = posizioneUltimoElCoda(codaPriorita); i >= 0; i--){
        /*scorro la coda in cerca dell'elemento fornito*/
        if(codaPriorita->CODA[i].indice == nodoDaCercare){ //indice corrispondente del nodo trovato
            
                codaPriorita->CODA[i].qty = newQty;

                indicePadre = getPadreElemento(i);
                while (i >= 0 && codaPriorita->CODA[i].qty < codaPriorita->CODA[indicePadre].qty){

                    scambiaDueElementiCoda(codaPriorita, i, indicePadre);
                    i = indicePadre;
                    indicePadre = getPadreElemento(i);
                }
            
            return; /*se il indice l'ho trovato ma la priorità non è piu bassa interrompo la ricerca*/
        }
    }
}

int getPadreElemento(int i){
    return (i-1)/2;
}


int estraiPrimoElDaHeapSenzaSistemare(CodaPriorita *codaPriorita){
    int primoEl;

    primoEl = codaPriorita->CODA[0].indice;
    codaPriorita->CODA[0].indice = codaPriorita->CODA[posizioneUltimoElCoda(codaPriorita)].indice;
    codaPriorita->CODA[0].qty = codaPriorita->CODA[posizioneUltimoElCoda(codaPriorita)].qty;
    codaPriorita->currentDimCoda = codaPriorita->currentDimCoda - 1;
    return primoEl;
}



void stampaGrafoMatriciale(Grafo G){
    int i,j;
    printf("Grafo: %d\n", G->indiceGrafo);
    for (i=0;i<G->maxNumVertici;i++){
        for (j=0;j<G->maxNumVertici;j++){
            printf("%u ", G->vettoreNodiDiPartenza[i].vettoreNodiDiArrivo[j].peso);
        }
        printf("\n");
    }
    printf("\n");
}


unsigned int calcolaSommaCamminiMinimi(Grafo G, unsigned int vettoreDelleDistanze[]){
    int i;
    unsigned int sommaDistanza;
    int h = 0;

    sommaDistanza = 0;

    for (i = 1; i < G->maxNumVertici; i++){
        if (G->vettoreNodiDiPartenza[0].vettoreNodiDiArrivo[i].peso == 0){
            h++;
        }
    }
    if (h == G->maxNumVertici - 1)
        return 0;

    for (i = 1; i < G->maxNumVertici; i++){
        if (vettoreDelleDistanze[i] == INFINITO){
           vettoreDelleDistanze[i] = 0;
        }
        sommaDistanza = sommaDistanza + vettoreDelleDistanze[i];
    } 
    return sommaDistanza;
}




void stampaDistanze(unsigned int vettDelleDistanze[], Grafo G){
    int i;
    for (i =0; i < G->maxNumVertici; i++){
        printf("Distanza nodo %d: %u\n", i, vettDelleDistanze[i]);
    }
}

void stampaSommaCamminiMinimiDelGrafo(Grafo G){
    if (G) 
        printf("Indice Grafo: %d, Somma Cammini: %u\n", G->indiceGrafo, G->sommaCammini);
}



unsigned int djikstra(Grafo G){
    CodaPriorita PQ;
    int u, i;
    unsigned int distPassPerU;
    unsigned int * dist = NULL;
    unsigned int sommaCammini;
    
    dist = (unsigned int *) malloc(G->maxNumVertici * sizeof(unsigned int));

    //inizializzazione
    for (i=0; i<G->maxNumVertici; i++){
        dist[i] = INFINITO;
    }
    creaCoda(&PQ, G->maxNumVertici);
    dist[0]=0;

    inserisciNelMinHeap(&PQ, 0, 0);
    for (i=1; i<G->maxNumVertici;i++){
        inserisciNelMinHeap(&PQ, i, INFINITO);
    }

    //printf("\nConsidero il grafo: %d\n", G->indiceGrafo);
    //algoritmo
    while (!(PQ.currentDimCoda == 0)){
        u = estraiPrimoElDaHeapSenzaSistemare(&PQ);
        //printf("Ho estratto: %d\n", u);
        aggiustaMinHeap(&PQ, 0);

        for (i=1; i<G->maxNumVertici;i++){
            if (G->vettoreNodiDiPartenza[u].vettoreNodiDiArrivo[i].peso != 0){
                //printf("dist[%d]: %u e arco da %d a %d: %u\n", u, dist[u], u, i, G->vettoreNodiDiPartenza[u].vettoreNodiDiArrivo[i].peso );
                
                if (dist[u] != INFINITO){

                    distPassPerU = dist[u] + G->vettoreNodiDiPartenza[u].vettoreNodiDiArrivo[i].peso;
                    G->vettoreNodiDiPartenza[u].vettoreNodiDiArrivo[i].isInfiniteDistance = 0;
                    //printf("La distanza da %d a %d non è 0 e distPassPer %d: %u\n", u, i, u, distPassPerU);
                    if (distPassPerU < dist[i]){
                        //printf("Poichè distPassPer %d è < di %u, decremento la chiave, che diventa %u\n", u, dist[i], distPassPerU);
                        decrementaChiave(&PQ, i, distPassPerU);
                        dist[i] = distPassPerU;
                    }
                }
            }
        }
    }
    //printf("\n");
    free(PQ.CODA);
    sommaCammini = calcolaSommaCamminiMinimi(G, dist);
    //sommaCammini = sum(G, dist);
    free(dist);
    return sommaCammini;
}


/********************************************************************DEPRECATO******************************************************************************/

int leggiLunghezzaGrafo()
{
    int lunghezzaGrafo;
    if(scanf( "%d ", &lunghezzaGrafo)>0){};
    return lunghezzaGrafo;
}

int leggiTopK()
{
    int topK;
    if(scanf( "%d\n", &topK)>0){};
    return topK;
}

