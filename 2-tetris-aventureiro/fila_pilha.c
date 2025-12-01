/*
 * =====================================================================
 * TETRIS STACK - NIVEL AVENTUREIRO
 * Fila Circular + Pilha de Reserva
 * =====================================================================
 * Descricao: Sistema que combina fila circular (pecas futuras) e
 * pilha (pecas reservadas). O jogador pode reservar pecas estrategicamente
 * e usa-las posteriormente.
 * 
 * Autora: Daniela Chiavenato Marzagao
 * Data: Novembro/2025
 * Disciplina: Analise e Desenvolvimento de Sistemas
 * =====================================================================
 */

// ==================== BIBLIOTECAS ====================
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ==================== CONSTANTES ====================
#define TAM_FILA 5
#define TAM_PILHA 3

// ==================== ESTRUTURA DE DADOS ====================

/*
 * Struct Peca:
 * Representa uma peca do Tetris com tipo e identificador unico
 */
typedef struct {
    char nome;      // Tipo da peca: 'I', 'O', 'T', 'L'
    int id;         // Identificador unico da peca
} Peca;

/*
 * Struct FilaCircular:
 * Implementa uma fila circular para gerenciar as pecas futuras
 */
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

/*
 * Struct Pilha:
 * Implementa uma pilha (LIFO) para pecas reservadas
 */
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;       // Indice do topo da pilha (-1 = vazia)
} Pilha;

// ==================== VARIAVEIS GLOBAIS ====================
int proximoId = 0;

// ==================== PROTOTIPOS - FILA ====================
void inicializarFila(FilaCircular* fila);
int filaVazia(FilaCircular* fila);
int filaCheia(FilaCircular* fila);
void enqueue(FilaCircular* fila, Peca peca);
Peca dequeue(FilaCircular* fila);
void exibirFila(FilaCircular* fila);

// ==================== PROTOTIPOS - PILHA ====================
void inicializarPilha(Pilha* pilha);
int pilhaVazia(Pilha* pilha);
int pilhaCheia(Pilha* pilha);
void push(Pilha* pilha, Peca peca);
Peca pop(Pilha* pilha);
void exibirPilha(Pilha* pilha);

// ==================== PROTOTIPOS - GERAIS ====================
Peca gerarPeca();
void exibirEstado(FilaCircular* fila, Pilha* pilha);
void exibirMenu();
void pausar();

// ==================== FUNCAO PRINCIPAL ====================
int main() {
    FilaCircular fila;
    Pilha pilha;
    int opcao;
    
    // Inicializa gerador de numeros aleatorios
    srand(time(NULL));
    
    // Inicializa estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Preenche a fila inicialmente
    printf("=====================================================\n");
    printf("    TETRIS STACK - FILA + PILHA DE RESERVA\n");
    printf("=====================================================\n");
    printf(">>> Inicializando sistema...\n\n");
    
    for (int i = 0; i < TAM_FILA; i++) {
        Peca novaPeca = gerarPeca();
        enqueue(&fila, novaPeca);
    }
    
    printf(">>> Fila inicializada com %d pecas!\n", TAM_FILA);
    printf(">>> Pilha de reserva pronta (capacidade: %d)!\n", TAM_PILHA);
    pausar();
    
    // Loop principal
    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        printf("\n");
        
        switch (opcao) {
            case 1:
                // Jogar peca (dequeue)
                if (!filaVazia(&fila)) {
                    Peca pecaJogada = dequeue(&fila);
                    printf(">>> PECA JOGADA: [%c %d]\n", pecaJogada.nome, pecaJogada.id);
                    
                    // Gera nova peca
                    Peca novaPeca = gerarPeca();
                    enqueue(&fila, novaPeca);
                    printf(">>> Nova peca gerada: [%c %d]\n", novaPeca.nome, novaPeca.id);
                } else {
                    printf(">>> ERRO: Fila vazia!\n");
                }
                break;
                
            case 2:
                // Reservar peca (fila -> pilha)
                if (pilhaCheia(&pilha)) {
                    printf(">>> ERRO: Pilha de reserva cheia!\n");
                    printf(">>> Use uma peca reservada primeiro (opcao 3).\n");
                } else if (filaVazia(&fila)) {
                    printf(">>> ERRO: Fila vazia!\n");
                } else {
                    Peca pecaReservada = dequeue(&fila);
                    push(&pilha, pecaReservada);
                    printf(">>> PECA RESERVADA: [%c %d]\n", pecaReservada.nome, pecaReservada.id);
                    
                    // Gera nova peca para a fila
                    Peca novaPeca = gerarPeca();
                    enqueue(&fila, novaPeca);
                    printf(">>> Nova peca gerada: [%c %d]\n", novaPeca.nome, novaPeca.id);
                }
                break;
                
            case 3:
                // Usar peca reservada (pop da pilha)
                if (pilhaVazia(&pilha)) {
                    printf(">>> ERRO: Pilha de reserva vazia!\n");
                    printf(">>> Reserve uma peca primeiro (opcao 2).\n");
                } else {
                    Peca pecaUsada = pop(&pilha);
                    printf(">>> PECA RESERVADA USADA: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
                }
                break;
                
            case 0:
                printf(">>> Encerrando Tetris Stack...\n");
                printf(">>> Obrigado por jogar!\n");
                break;
                
            default:
                printf(">>> ERRO: Opcao invalida!\n");
        }
        
        if (opcao != 0) {
            pausar();
        }
        
    } while (opcao != 0);
    
    printf("\n=====================================================\n");
    printf("Jogo finalizado!\n");
    printf("Total de pecas geradas: %d\n", proximoId);
    printf("=====================================================\n\n");
    
    return 0;
}

// ==================== IMPLEMENTACAO - FILA ====================

void inicializarFila(FilaCircular* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

int filaVazia(FilaCircular* fila) {
    return (fila->tamanho == 0);
}

int filaCheia(FilaCircular* fila) {
    return (fila->tamanho == TAM_FILA);
}

void enqueue(FilaCircular* fila, Peca peca) {
    if (filaCheia(fila)) {
        printf(">>> ERRO: Fila cheia!\n");
        return;
    }
    
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->elementos[fila->tras] = peca;
    fila->tamanho++;
}

Peca dequeue(FilaCircular* fila) {
    if (filaVazia(fila)) {
        printf(">>> ERRO: Fila vazia!\n");
        Peca pecaVazia = {'\0', -1};
        return pecaVazia;
    }
    
    Peca pecaRemovida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

void exibirFila(FilaCircular* fila) {
    if (filaVazia(fila)) {
        printf("Fila de pecas: [VAZIA]\n");
        return;
    }
    
    printf("Fila de pecas: ");
    
    int i = fila->frente;
    int contador = 0;
    
    while (contador < fila->tamanho) {
        printf("[%c %d] ", fila->elementos[i].nome, fila->elementos[i].id);
        i = (i + 1) % TAM_FILA;
        contador++;
    }
    
    printf("\n");
}

// ==================== IMPLEMENTACAO - PILHA ====================

void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;  // -1 indica pilha vazia
}

int pilhaVazia(Pilha* pilha) {
    return (pilha->topo == -1);
}

int pilhaCheia(Pilha* pilha) {
    return (pilha->topo == TAM_PILHA - 1);
}

void push(Pilha* pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf(">>> ERRO: Pilha cheia!\n");
        return;
    }
    
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
}

Peca pop(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        printf(">>> ERRO: Pilha vazia!\n");
        Peca pecaVazia = {'\0', -1};
        return pecaVazia;
    }
    
    Peca pecaRemovida = pilha->elementos[pilha->topo];
    pilha->topo--;
    
    return pecaRemovida;
}

void exibirPilha(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        printf("Pilha de reserva: [VAZIA]\n");
        return;
    }
    
    printf("Pilha de reserva (Topo -> Base): ");
    
    // Exibe do topo para a base
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
    }
    
    printf("\n");
}

// ==================== IMPLEMENTACAO - GERAIS ====================

Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

void exibirEstado(FilaCircular* fila, Pilha* pilha) {
    printf("\n=====================================================\n");
    printf("              ESTADO ATUAL DO JOGO\n");
    printf("=====================================================\n");
    
    exibirFila(fila);
    printf("     (%d/%d pecas)\n", fila->tamanho, TAM_FILA);
    
    printf("-----------------------------------------------------\n");
    
    exibirPilha(pilha);
    printf("     (%d/%d pecas)\n", pilha->topo + 1, TAM_PILHA);
    
    printf("=====================================================\n");
}

void exibirMenu() {
    printf("\n=====================================================\n");
    printf("                 MENU DE OPCOES\n");
    printf("=====================================================\n");
    printf("1 - Jogar peca (da fila)\n");
    printf("2 - Reservar peca (fila -> pilha)\n");
    printf("3 - Usar peca reservada (pilha)\n");
    printf("0 - Sair do jogo\n");
    printf("=====================================================\n");
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n');
    getchar();
}

/*
 * =====================================================================
 * EXPLICACOES TECNICAS:
 * =====================================================================
 * 
 * 1. FILA CIRCULAR (FIFO):
 *    - First In, First Out
 *    - Primeira peca a entrar e a primeira a sair
 *    - Simula as "proximas pecas" do Tetris
 *    - Operacoes: enqueue (adicionar), dequeue (remover)
 * 
 * 2. PILHA (LIFO):
 *    - Last In, First Out
 *    - Ultima peca a entrar e a primeira a sair
 *    - Simula a "reserva estrategica" do jogador
 *    - Operacoes: push (adicionar), pop (remover)
 * 
 * 3. FLUXO DO JOGO:
 *    - Fila sempre contem proximas pecas
 *    - Jogador pode jogar peca imediatamente (dequeue)
 *    - Ou reservar para usar depois (dequeue + push)
 *    - Pecas reservadas sao usadas em ordem inversa (LIFO)
 * 
 * 4. ESTRATEGIA:
 *    - Pilha permite guardar pecas "boas" para momento certo
 *    - Capacidade limitada (3) forca decisoes estrategicas
 *    - Fila sempre e reabastecida automaticamente
 * 
 * 5. COMPLEXIDADES:
 *    - Fila: enqueue O(1), dequeue O(1)
 *    - Pilha: push O(1), pop O(1)
 *    - Todas operacoes muito eficientes
 * 
 * 6. DIFERENCA CHAVE:
 *    - FILA: ordem de chegada preservada
 *    - PILHA: ordem invertida (ultimo entra, primeiro sai)
 * =====================================================================
 */