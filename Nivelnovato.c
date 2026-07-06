#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ==========================================================
   TETRIS STACK - Nivel Novato: Fila de Pecas Futuras
   ----------------------------------------------------------
   Simula uma FILA CIRCULAR de exatamente 5 pecas.
   Cada peca possui:
     - nome : tipo da peca ('I', 'O', 'T', 'L')
     - id   : identificador numerico exclusivo
   ========================================================== */

#define TAM_FILA 5   /* A fila deve conter exatamente 5 elementos */

/* Struct que representa uma peca do jogo */
typedef struct {
    char nome;   /* Tipo da peca: 'I', 'O', 'T' ou 'L' */
    int  id;     /* Identificador unico da peca */
} Peca;

/* Struct que representa a fila circular */
typedef struct {
    Peca itens[TAM_FILA]; /* Array que armazena as pecas */
    int  frente;          /* Indice da peca da frente */
    int  tras;            /* Indice onde a proxima peca sera inserida */
    int  quantidade;      /* Quantidade atual de pecas na fila */
} Fila;

/* Contador global para gerar ids exclusivos */
int proximoId = 0;

/* ----------------------------------------------------------
   gerarPeca(): cria automaticamente uma nova peca com
   um tipo aleatorio e um id exclusivo e crescente.
   ---------------------------------------------------------- */
Peca gerarPeca() {
    Peca nova;
    char tipos[4] = {'I', 'O', 'T', 'L'};

    nova.nome = tipos[rand() % 4]; /* Escolhe um tipo aleatorio */
    nova.id   = proximoId++;       /* Atribui id unico e incrementa */

    return nova;
}

/* ----------------------------------------------------------
   inicializarFila(): deixa a fila vazia e pronta para uso.
   ---------------------------------------------------------- */
void inicializarFila(Fila *f) {
    f->frente     = 0;
    f->tras       = 0;
    f->quantidade = 0;
}

/* Retorna 1 se a fila estiver cheia, 0 caso contrario */
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

/* Retorna 1 se a fila estiver vazia, 0 caso contrario */
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

/* ----------------------------------------------------------
   enqueue(): insere uma nova peca no final da fila.
   Usa aritmetica circular para reaproveitar o espaco.
   ---------------------------------------------------------- */
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("\n[!] A fila esta cheia! Nao e possivel inserir.\n");
        return;
    }

    f->itens[f->tras] = p;                 /* Coloca a peca na posicao 'tras' */
    f->tras = (f->tras + 1) % TAM_FILA;    /* Avanca de forma circular */
    f->quantidade++;

    printf("\n[+] Peca [%c %d] inserida no final da fila.\n", p.nome, p.id);
}

/* ----------------------------------------------------------
   dequeue(): remove (joga) a peca da frente da fila.
   ---------------------------------------------------------- */
void dequeue(Fila *f) {
    if (filaVazia(f)) {
        printf("\n[!] A fila esta vazia! Nao ha peca para jogar.\n");
        return;
    }

    Peca jogada = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA; /* Avanca de forma circular */
    f->quantidade--;

    printf("\n[>] Peca [%c %d] foi jogada!\n", jogada.nome, jogada.id);
}

/* ----------------------------------------------------------
   visualizarFila(): exibe o estado atual da fila,
   da frente para o final.
   ---------------------------------------------------------- */
void visualizarFila(Fila *f) {
    printf("\n=================================================\n");
    printf("Fila de pecas (frente -> tras):\n");

    if (filaVazia(f)) {
        printf("   [ fila vazia ]\n");
    } else {
        int i;
        int indice = f->frente;

        for (i = 0; i < f->quantidade; i++) {
            printf("   [%c %d]", f->itens[indice].nome, f->itens[indice].id);
            indice = (indice + 1) % TAM_FILA; /* Percorre de forma circular */
        }
        printf("\n");
    }
    printf("=================================================\n");
}

/* ----------------------------------------------------------
   exibirMenu(): mostra as opcoes disponiveis.
   ---------------------------------------------------------- */
void exibirMenu() {
    printf("\n---------------- MENU ----------------\n");
    printf(" 1 - Jogar peca (remover da frente)\n");
    printf(" 2 - Inserir nova peca (no final)\n");
    printf(" 3 - Visualizar fila\n");
    printf(" 0 - Sair\n");
    printf("--------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/* ==========================================================
   FUNCAO PRINCIPAL
   ========================================================== */
int main() {
    Fila fila;
    int opcao;

    srand(time(NULL)); /* Semente para numeros aleatorios */

    inicializarFila(&fila);

    /* Inicializa a fila com 5 pecas geradas automaticamente */
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    printf("\n### TETRIS STACK - Fila de Pecas Futuras ###\n");
    visualizarFila(&fila);

    /* Laco principal do menu */
    do {
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            /* Trata entrada invalida (nao numerica) */
            printf("\n[!] Entrada invalida! Digite um numero.\n");
            while (getchar() != '\n'); /* Limpa o buffer de entrada */
            continue;
        }

        switch (opcao) {
            case 1: /* Jogar peca */
                dequeue(&fila);
                visualizarFila(&fila);
                break;

            case 2: /* Inserir nova peca */
                enqueue(&fila, gerarPeca());
                visualizarFila(&fila);
                break;

            case 3: /* Visualizar fila */
                visualizarFila(&fila);
                break;

            case 0: /* Sair */
                printf("\nEncerrando o jogo. Ate a proxima!\n");
                break;

            default:
                printf("\n[!] Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}