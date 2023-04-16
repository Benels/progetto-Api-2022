#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct albero_rb {
    int colore;
    struct albero_rb *papi;
    struct albero_rb *figlio_sx;
    struct albero_rb *figlio_dx;
    char parola[];
};

struct albero_rb foglie;
struct albero_rb * NIL;
struct albero_rb * head_rb; 

#define ROSSO   0
#define NERO    1

struct Rimanenti {
    struct Rimanenti * next;
    char parola[];
};

char p='+';
char s='/';
char v='|';

int insert_special_counter=0;
int lenght=0;
int tentativi=0;
int not_exists_flag=0;
int sum_occ=0;
int usa_perf=0;
int occ_rif[64];
int occ_rif_tmp[64];
char riferimento[128];
char per_confrontare[128];
int Occorrenze_temp[64];
int occorrenze[64];
bool where_banned[128][64];
bool banlist[64];
bool banlist_temp[64];
bool see_esatto[64];

/*sottoprogrammi albero rosso nero*/
struct albero_rb *crea_rb(struct albero_rb **, char *);
void rb_insert(struct albero_rb **, struct albero_rb *);
void rb_fixup(struct albero_rb **, struct albero_rb *);
void rotate_dx(struct albero_rb **, struct albero_rb *);
void rotate_sx(struct albero_rb **, struct albero_rb *);
void ricerca_parola_rb(struct albero_rb *, char *);
void stampa_rb(struct albero_rb * );

/*sottoprogrammi generici*/
void nuova_partita();
int va_bene(char * );
void controlleiscion(char * );
int diminuzione(int );
void guarda_occ_rif(char * );

/*sottoprogrammi liste*/
void push(struct Rimanenti** , char * );
void ricerca_lista(struct Rimanenti** , char * ) ;
void eliminatesta(struct Rimanenti ** );
void filtra_lista(struct Rimanenti ** );
void stampa_lista(struct Rimanenti * );
void distruggi(struct Rimanenti** );
struct Rimanenti *merge(struct Rimanenti *, struct Rimanenti *);
struct Rimanenti * ordina_lista(struct Rimanenti * );


/**
 * 
 * Creates a red and black tree
 * 
 */

struct albero_rb *crea_rb(struct albero_rb **h_rb, char *par)
{
    struct albero_rb * nuovo = NULL;
    nuovo= malloc(sizeof(struct albero_rb)+lenght+2);
    nuovo->colore = ROSSO;
    nuovo->figlio_sx = NULL;
    nuovo->figlio_dx = NULL;
    nuovo->papi = NULL;
    strcpy(nuovo->parola, par);
    
    rb_insert(h_rb, nuovo);
    return nuovo;
}


/**
 * 
 * Inserts a new node in the given rb tree
 * 
 */

void rb_insert(struct albero_rb **h_rb, struct albero_rb *nuovo)
{
    struct albero_rb *x = *h_rb;
    struct albero_rb *y = NIL;

    while(x != NIL){
        y = x;
       if(strcmp(nuovo->parola, x->parola)< 0)
            x = x->figlio_sx;
        else
            x = x->figlio_dx;
    }
    nuovo->papi = y;

    if(y == NIL)
        *h_rb = nuovo;
    else if(strcmp(nuovo->parola, y->parola) < 0)
        y->figlio_sx = nuovo;
    else
        y->figlio_dx = nuovo;

    nuovo->figlio_sx  = NIL;
    nuovo->figlio_dx = NIL;
    nuovo->colore = ROSSO;
    rb_fixup(h_rb, nuovo);
    return;
}

void rb_fixup(struct albero_rb **h_rb, struct albero_rb *nodo_fix)
{
    struct albero_rb *y=NIL;
    if (nodo_fix == *h_rb) {
        (*h_rb)->colore = NERO;
    } else {
        if (nodo_fix->papi->colore == ROSSO) {
            if (nodo_fix->papi == nodo_fix->papi->papi->figlio_sx) {
                y = nodo_fix->papi->papi->figlio_dx;
                if (y->colore == ROSSO) {
                    nodo_fix->papi->colore = NERO;
                    y->colore = NERO;
                    nodo_fix->papi->papi->colore = ROSSO;
                    rb_fixup(h_rb, nodo_fix->papi->papi);
                } else {
                    if (nodo_fix == nodo_fix->papi->figlio_dx) {
                        nodo_fix = nodo_fix->papi;
                        rotate_sx(h_rb, nodo_fix);
                        nodo_fix->papi = nodo_fix->papi;
                    }
                    nodo_fix->papi->colore = NERO;
                    nodo_fix->papi->papi->colore = ROSSO;
                    rotate_dx(h_rb, nodo_fix->papi->papi);
                }
            } else {
                 y = nodo_fix->papi->papi->figlio_sx;
                if (y->colore == ROSSO) {
                    nodo_fix->papi->colore = NERO;
                    y->colore = NERO;
                    nodo_fix->papi->papi->colore = ROSSO;
                    rb_fixup(h_rb, nodo_fix->papi->papi);
                } else {
                    if (nodo_fix == nodo_fix->papi->figlio_sx) {
                        nodo_fix = nodo_fix->papi;
                        rotate_dx(h_rb, nodo_fix);
                        nodo_fix->papi = nodo_fix->papi;
                    }
                    nodo_fix->papi->colore = NERO;
                    nodo_fix->papi->papi->colore = ROSSO;
                    rotate_sx(h_rb, nodo_fix->papi->papi);
                }
            }
        }
    }
}

void rotate_dx(struct albero_rb **h_rb, struct albero_rb *x)
{
    struct albero_rb *y = x->figlio_sx;

    x->figlio_sx = y->figlio_dx;
    if (y->figlio_dx != NIL)
        y->figlio_dx->papi = x;
    y->papi = x->papi;
    if (x->papi == NIL)
        *h_rb = y;
    else if (x == x->papi->figlio_dx)
        x->papi->figlio_dx = y;
    else
        x->papi->figlio_sx = y;
    y->figlio_dx = x;
    x->papi = y;
}



void rotate_sx(struct albero_rb **h_rb, struct albero_rb *x)
{
    struct albero_rb *y = x->figlio_dx;

    x->figlio_dx = y->figlio_sx;
    if (y->figlio_sx != NIL)
        y->figlio_sx->papi = x;
    y->papi = x->papi;
    if (x->papi == NIL)
        *h_rb = y;
    else if (x == x->papi->figlio_sx)
        x->papi->figlio_sx = y;
    else
        x->papi->figlio_dx = y;
    y->figlio_sx = x;
    x->papi = y;
}


/**
 * 
 * Traverses the tree, prints "not exists" if the string given as a parameter is not in the tree
 * 
 */

void ricerca_parola_rb(struct albero_rb *curr, char *par)
{
    int res=0;
    while(curr != NIL) {
        res = strncmp(par, curr->parola, lenght);
        if(res== 0){
            not_exists_flag=1;
            return;
        }
        else if(res < 0){
            curr = curr->figlio_sx;
        }
        else{
            curr = curr->figlio_dx;
        }
    }
    if(not_exists_flag==0)
    {
        printf("not_exists\n");    
    }
}


/**
 * 
 * Prints the whole tree
 * 
 */

void stampa_rb(struct albero_rb * curr)
{
    if (curr){
        stampa_rb(curr->figlio_sx);
        printf("%s", curr->parola);
        stampa_rb(curr->figlio_dx);
    }
}


/**
 * 
 * Creates a linked list which contains all the words in the rb tree that fulfil the constraints given by the 'va_bene' function
 * 
 */

void da_rb_a_lista(struct albero_rb * curr, struct Rimanenti ** hrim)
{
    if (curr!=NIL){
        da_rb_a_lista(curr->figlio_sx, hrim);
        if(va_bene(curr->parola)==1){
            push(hrim, curr->parola);
            insert_special_counter++;
        }
        da_rb_a_lista(curr->figlio_dx, hrim);
    }
}


/**
 * 
 * Insert a new element as the head of the linked list
 * 
 */

void push(struct Rimanenti** h_rimanente, char * par)
{
    struct Rimanenti* nuovo = (struct Rimanenti*) malloc(sizeof(struct Rimanenti)+lenght+3); 
    strcpy(nuovo->parola, par);
    nuovo->next = (*h_rimanente);
    (*h_rimanente)= nuovo;
}


/**
 * 
 * Traverses the linked list in order to find a given word
 *      Once the word if found, that occurrence is eliminated from the list
 *      If the word is not in the list, prints "not_exists"
 * 
 */

void ricerca_lista(struct Rimanenti** h, char * par) 
{
  struct Rimanenti * curr = *h, *prec = *h;

  while (curr != NULL) {
  if (strcmp(curr->parola, par) == 0){
    not_exists_flag=1;
    if(prec == curr){
        //deletes the first element
        *h = curr->next;
        free(curr);
    }else{
        //deletes a non-first element
        prec->next = curr->next;
        free(curr);
    }
    return;
  } 
  prec = curr;
  curr = curr->next;
  }
  printf("not_exists\n");
}


/**
 * 
 * Filters the linked list in order to delete every element that doesn't fulfil every constraint given by the 'va_bene' funcion
 *  
 */

void filtra_lista(struct Rimanenti** hrimanenti)
{
   struct Rimanenti* curr = *hrimanenti;
   struct Rimanenti* succ;
   struct Rimanenti * last_ok = * hrimanenti;
   struct Rimanenti * copiah = * hrimanenti;

   int count=0;
   while(copiah != NULL && va_bene(copiah->parola)==0){//the head of the list has to be eliminated
        *hrimanenti=copiah->next;
        free(copiah);
        copiah=*hrimanenti;
   }

   curr=copiah;
   last_ok=copiah;
   while (curr != NULL)
   {
        if(va_bene(curr->parola)==0){
           succ = curr->next;
           last_ok->next=succ;
           free(curr);
           curr = succ;             
        } else{
            count++;
            last_ok=curr;
            succ = curr->next;
            curr = succ; 
        }
   }
   printf("%d\n", count);
}


/**
 * 
 * Prints the whole linked list
 * 
 */

void stampa_lista(struct Rimanenti * h)
{
    struct Rimanenti * temp = h;
    while(temp != NULL){
        printf("%s", temp->parola);
        temp=temp->next;
    }
}



/**
 * 
 * Destroys the linked list
 * 
 */

void distruggi(struct Rimanenti** hrimanenti)
{
   struct Rimanenti* curr = *hrimanenti;
   struct Rimanenti* succ;
 
   while (curr != NULL)
   {
       succ = curr->next;
       free(curr);
       curr = succ;
   }
   *hrimanenti = NULL;
}



/**
 * 
 * Helper for the 'ordina_lista' function
 * 
 */ 

struct Rimanenti *merge(struct Rimanenti *parte_sx, struct Rimanenti *parte_dx)
{
    if(parte_sx == NULL){
        return parte_dx;
    } 
    if(parte_dx == NULL){
        return parte_sx;
    } 
    if(strcmp(parte_sx->parola , parte_dx->parola)<=0)
    {
        parte_sx->next = merge(parte_sx->next, parte_dx);
        return parte_sx;
    }
    else
    {
        parte_dx->next = merge(parte_sx, parte_dx->next);
        return parte_dx;
    }
}


/**
 * 
 * Merge Sort for the linked list
 * 
 */

struct Rimanenti * ordina_lista(struct Rimanenti * head)
{
    if(head == NULL)
        return NULL;
    if(head->next == NULL)
        return head;

    struct Rimanenti  *a=NULL, *b=NULL, *pre=NULL;
    a = b = head;
    while(a && a->next)
    {
        pre = b;
        a = a->next->next;
        b = b->next;
    }
    pre->next = NULL;
    return merge(ordina_lista(head), ordina_lista(b));
}


/**
 * 
 * Controls that the word given as a parameter fulfils certain constraints decided by the project's specifications
 * 
 */

int va_bene(char * par)
{

    int va_bene_flag=1;
    int ascii_val=0;
    int i=0;
    int j=0;

    for(i=0;i<64;i++){
        Occorrenze_temp[i]=0;
    }
    for( i=0; i<lenght; i++){
        if(per_confrontare[i] != '+'){
            if(per_confrontare[i]!=par[i]){
                va_bene_flag = 0;
                break;
            } 
        }
        ascii_val=(int)(par[i]);
        ascii_val=diminuzione(ascii_val);
        if(banlist[ascii_val]!=0 ){
            va_bene_flag = 0;
            break;
        }
        if(where_banned[i][ascii_val]!=0){
            va_bene_flag = 0;
            break;
        }
        Occorrenze_temp[ascii_val]++;
    }
    if(va_bene_flag==1){
        for( j=0; j<64; j++){
            if(see_esatto[j]!=0){
                if(Occorrenze_temp[j] != occorrenze[j]){
                    va_bene_flag = 0;
                    break;
                }
            }
            if(see_esatto[j]==0){
                if(Occorrenze_temp[j] < occorrenze[j]){
                    va_bene_flag = 0;
                    break;
                }                
            }
            if(usa_perf==0){
                if(Occorrenze_temp[j] < occorrenze[j]){
                    va_bene_flag = 0;
                    break;
                }               
            }
            if(usa_perf==1){
                if(Occorrenze_temp[j] != occorrenze[j]){
                    va_bene_flag = 0;
                    break;
                }               
            }
        }   
    }else{
        return 0;
    }
    if(va_bene_flag==1)
        return 1;
    else
        return 0;
}


/**
 * 
 * Creates specific constraints given by the interaction of the project's contraints and the given word
 * 
 */

void controlleiscion(char * par)
{
    char *risposta;
    int asciival=0;
    int i=0;
    risposta=(char*)calloc((lenght+1), 1);

    for(i=0; i<64; i++){
        Occorrenze_temp[i]=0;
        banlist_temp[i]=0;
        occ_rif_tmp[i]=occ_rif[i];

    }
    if(risposta) {
        risposta[lenght]='\0';
        sum_occ=0;
        for(i=0;i<lenght;i++){
            if(par[i]==riferimento[i]){
                risposta[i]=p;
                asciival=(int)(par[i]);
                asciival=diminuzione(asciival);
                occ_rif_tmp[asciival]--;
                Occorrenze_temp[asciival]++; 
                per_confrontare[i]=par[i];
                sum_occ++;
            }
        }
        for(i=0;i<lenght;i++){
            if(risposta[i]!=p){
                asciival=(int)(par[i]);
                asciival=diminuzione(asciival);
                if(occ_rif_tmp[asciival]>0){
                    occ_rif_tmp[asciival]--;
                    risposta[i]=v;
                    Occorrenze_temp[asciival]++;
                    where_banned[i][asciival]=1;
                    sum_occ++;

                }else{
                    risposta[i]=s;
                    banlist_temp[asciival]=1;
                    where_banned[i][asciival]=1;
                    see_esatto[asciival]=1;
                }
            }
        }
        for(i=0;i<lenght;i++){
            asciival=(int)(par[i]);
            asciival=diminuzione(asciival);
            if(Occorrenze_temp[asciival]>0 && banlist_temp[asciival]>0){
                see_esatto[asciival]=1;
                banlist_temp[asciival]=0;
            }if(Occorrenze_temp[asciival]>0 && sum_occ==lenght){
                see_esatto[asciival]=1;
                banlist_temp[asciival]=0;
            }if(Occorrenze_temp[asciival]==0 && sum_occ==lenght){
                see_esatto[asciival]=1;
                banlist_temp[asciival]=1;
            }
            if(banlist_temp[asciival]>0 && Occorrenze_temp[asciival]==0){
                banlist[asciival]=1;
                banlist_temp[asciival]=0;
            }
            if(Occorrenze_temp[asciival]>=occorrenze[asciival]){
                occorrenze[asciival]=Occorrenze_temp[asciival];
            }
            if (sum_occ==lenght){
                
                usa_perf=1;
            }

        }
        printf("%s\n", risposta);
        free(risposta);
    }
    return;
}



/**
 * 
 * Helper function used to deal with the ascii values
 * 
 */
int diminuzione(int ascii_val)
{
    int a=0;
    switch(ascii_val){
        case 45:a=0;break;
        case 95: a=37;break;
        default: 
            if(ascii_val<=57 && ascii_val>=48){
                a=ascii_val-47;
            }else if(ascii_val<=90 && ascii_val>=56){
                a=ascii_val-54;
            }else if(ascii_val<=122 && ascii_val>=97){
                a=ascii_val-59;
            }; break;
    }
    return a;
}


/**
 * 
 * Finds the number of occurrencies of every letter of the alphabet in the given word
 * 
 */
void guarda_occ_rif(char * r)
{
    int i=0;
    int asciival=0;
    for(i=0;i<lenght;i++){
        asciival=(int)r[i];
        asciival=diminuzione(asciival);
        occ_rif[asciival]++;
    }     
}


/**
 * 
 * Creates a new game
 * 
 */
void nuova_partita()
{
    int i=0;
    char parola[128];
    struct Rimanenti * hrim = NULL;
    if(fgets(riferimento, lenght+1, stdin)){

        for(i=0; i<64; i++){
            occorrenze[i]=0;
            banlist[i]=0;
            see_esatto[i]=0;
            occ_rif[i]=0;
        }
        memset(per_confrontare, p, lenght);
        per_confrontare[lenght]='\0';
        memset(where_banned, 0, sizeof(where_banned));
        usa_perf=0;
        guarda_occ_rif(riferimento);

        if(fscanf (stdin, "%d", &tentativi)){
        }
        while('\n'!=getchar()){}; 

        while(tentativi>0){
            if(fgets(parola, 128, stdin)){
                if (parola[0]=='+' && parola[1]=='i'){
                    while(fgets(parola, 128, stdin)){
                        if(parola[0]=='+' && parola[1]=='i')
                             break;
                        else{
                            crea_rb(&head_rb, parola);
                        }
                    }
                }else if(parola[0]=='+' && parola[1]=='s'){
                    stampa_rb(head_rb);
                }else if(memcmp(parola, riferimento, lenght)==0){
                    printf("ok\n");
                    return;
                }else{
                    not_exists_flag=0;
                    ricerca_parola_rb(head_rb, parola);
                    if (not_exists_flag==0)
                    {
                    continue;
                    }
                    if(not_exists_flag == 1){
                        controlleiscion(parola);
                        insert_special_counter=0;
                        distruggi(&hrim);
                        da_rb_a_lista (head_rb, &hrim);
                        printf("%d\n", insert_special_counter);
                        tentativi--;
                        if(feof(stdin)){
                            return;
                        }
                        break;   
                    }                   
                }
            }
        }
        
        if(tentativi == 0){
            printf("ko\n");
            distruggi(&hrim);
            return;
        }
        if(feof(stdin)){
            return;
        }
        
        while(tentativi>0){
            if(fgets(parola, 128, stdin)){
                if (parola[0]=='+' && parola[1]=='i'){
                    while(fgets(parola, 128, stdin)){                        
                        if(parola[0]=='+' && parola[1]=='i')
                             break;
                        else{
                            if(va_bene(parola)==1){
                                push(&hrim, parola);
                                crea_rb(&head_rb, parola);
                            }else{
                                crea_rb(&head_rb, parola);
                            }
                        }
                    }
                }else if(parola[0]=='+' && parola[1]=='s'){
                    hrim=ordina_lista(hrim);
                    stampa_lista(hrim);
                }else if(memcmp(parola, riferimento, lenght)==0){
                    printf("ok\n");
                    distruggi(&hrim);
                    return;
                }else{
                    not_exists_flag=0;
                    ricerca_parola_rb(head_rb, parola);
                    if(not_exists_flag == 1){
                        controlleiscion(parola);
                        filtra_lista(&hrim);
                        tentativi--;
                        if(feof(stdin)){
                            distruggi(&hrim);
                            return;
                        }   
                    }                   
                }
            }
        }
        if(tentativi == 0){
            printf("ko\n");
        }
        if(feof(stdin)){
            distruggi(&hrim);
            return;
        }
        distruggi(&hrim);
        return;
    }
}


int main()
{
    if(fscanf (stdin, "%d", &lenght)){
        while('\n'!=getchar()){}; 
        char parola[128];

        NIL = &foglie;
        NIL->colore = NERO; 
        head_rb = NIL;
        while(fgets(parola, 128, stdin)){
            if(parola[0]=='+' && parola[1]=='n'){
                nuova_partita();
            }
            else if(parola[0]=='+' && parola[1]=='i'){
                    while(fgets(parola, 128, stdin)){
                        if(parola[0]=='+' && parola[1]=='i')
                             break;
                        else{
                            crea_rb(&head_rb, parola);
                        }
                    }
            }else if(parola[0]=='+' && parola[1]=='s'){
                   stampa_rb(head_rb);
            }else{
                crea_rb(&head_rb, parola);
            }
        }
    }
    return 0;
}