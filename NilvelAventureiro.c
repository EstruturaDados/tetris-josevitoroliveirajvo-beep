#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ==========================================================
   TETRIS STACK - Nivel Aventureiro
   Fila de pecas (sempre cheia) + Pilha de reserva
   ----------------------------------------------------------
   FILA : 5 pecas, circular, sempre cheia (reposicao automatica)
   PILHA: reserva de ate 3 pecas (push / pop)
   Cada peca possui:
     - nome : tipo ('I', 'O', 'T', 'L')
     - id   : identificador numerico exclusivo
   ========================================================== */

#define TAM_FILA  5   /* A fila tem sempre 5 pecas   */
#define TAM_PILHA 3   /* A pilha reserva ate 3 pecas */

/* Struct de uma peca */
typedef struct {
    char nome;
    int  id;
} Peca;

/* Fila circular */
typedef struct {
    Peca itens[TAM_FILA];
    int  frente;
    int  tras;
    int  quantidade;
} Fila;

/* Pilha linear */
typedef struct {
    Peca itens[TAM_PILHA];
    int  topo;   /* indice do topo; -1 = vazia */
} Pilha;

/* Contador global para ids exclusivos */
int proximoId = 0;

/* ---------------- Geracao de pecas ---------------- */
Peca gerarPeca() {
    Peca nova;
    char tipos[4] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id   = proximoId++;
    return nova;
}

/* ---------------- Funcoes da FILA ---------------- */
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->quantidade = 0;
}

int filaCheia(Fila *f)  { return f->quantidade == TAM_FILA; }
int filaVazia(Fila *f)  { return f->quantidade == 0; }

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->itens[f->tras] = p;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->quantidade++;
}

/* Remove e devolve a peca da frente (via ponteiro de saida) */
int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) return 0;
    *removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return 1;
}

/* ---------------- Funcoes da PILHA ---------------- */
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) { return p->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

int push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return 0;
    p->itens[++p->topo] = x;
    return 1;
}

int pop(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) return 0;
    *removida = p->itens[p->topo--];
    return 1;
}

/* ---------------- Exibicao dos estados ---------------- */
void exibirEstado(Fila *f, Pilha *p) {
    int i, indice;

    printf("\n=================================================\n");
    printf("FILA (frente -> tras): ");
    indice = f->frente;
    for (i = 0; i < f->quantidade; i++) {
        printf("[%c %d] ", f->itens[indice].nome, f->itens[indice].id);
        indice = (indice + 1) % TAM_FILA;
    }

    printf("\nPILHA (topo -> base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n=================================================\n");
}

void exibirMenu() {
    printf("\n---------------- MENU ----------------\n");
    printf(" 1 - Jogar peca (remover da frente)\n");
    printf(" 2 - Reservar peca (mover frente p/ pilha)\n");
    printf(" 3 - Usar peca reservada (pop da pilha)\n");
    printf(" 0 - Sair\n");
    printf("--------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/* ==================== PRINCIPAL ==================== */
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    Peca aux;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Fila comeca cheia com 5 pecas */
    for (int i = 0; i < TAM_FILA; i++)
        enqueue(&fila, gerarPeca());

    printf("\n### TETRIS STACK - Nivel Aventureiro ###\n");
    exibirEstado(&fila, &pilha);

    do {
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            printf("\n[!] Entrada invalida! Digite um numero.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1: /* Jogar peca: remove da frente e repoe */
                if (dequeue(&fila, &aux)) {
                    printf("\n[>] Peca [%c %d] foi jogada!\n", aux.nome, aux.id);
                    enqueue(&fila, gerarPeca()); /* mantem a fila cheia */
                } else {
                    printf("\n[!] A fila esta vazia!\n");
                }
                exibirEstado(&fila, &pilha);
                break;

            case 2: /* Reservar peca: frente da fila -> pilha */
                if (pilhaCheia(&pilha)) {
                    printf("\n[!] A pilha de reserva esta cheia (max %d).\n", TAM_PILHA);
                } else {
                    dequeue(&fila, &aux);        /* tira da frente */
                    push(&pilha, aux);           /* guarda na pilha */
                    printf("\n[R] Peca [%c %d] reservada na pilha.\n", aux.nome, aux.id);
                    enqueue(&fila, gerarPeca()); /* repoe a fila */
                }
                exibirEstado(&fila, &pilha);
                break;

            case 3: /* Usar peca reservada: pop da pilha */
                if (pop(&pilha, &aux)) {
                    printf("\n[U] Peca reservada [%c %d] foi usada!\n", aux.nome, aux.id);
                } else {
                    printf("\n[!] Nao ha pecas reservadas na pilha.\n");
                }
                exibirEstado(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o jogo. Ate a proxima!\n");
                break;

            default:
                printf("\n[!] Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}