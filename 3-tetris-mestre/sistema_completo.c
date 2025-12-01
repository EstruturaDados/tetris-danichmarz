/*
 * =====================================================================
 * TETRIS STACK - NIVEL MESTRE
 * Sistema Completo com Trocas Estrategicas
 * =====================================================================
 * Descricao: Sistema avancado que implementa fila circular e pilha
 * com operacoes de troca individual e multipla entre as estruturas.
 * Permite estrategias complexas de reorganizacao de pecas.
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

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// ==================== VARIAVEIS GLOBAIS ====================
int proximoId = 0;

// ==================== PROTOTIPOS ====================
// Fila
void inicializarFila(FilaCircular* fila);
int filaVazia(FilaCircular* fila);
int filaCheia(FilaCircular* fila);
void enqueue(FilaCircular* fila, Peca peca);
Peca dequeue(FilaCircular* fila);
Peca frente(FilaCircular* fila);

// Pilha
void inicializarPilha(Pilha* pilha);
int pilhaVazia(Pilha* pilha);
int pilhaCheia(Pilha* pilha);
void push(Pilha* pilha, Peca peca);
Peca pop(Pilha* pilha);
Peca topo(Pilha* pilha);

// Operacoes avancadas
void trocarPecaSimples(FilaCircular* fila, Pilha* pilha);
void trocarMultipla(FilaCircular* fila, Pilha* pilha);

// Gerais
Peca gerarPeca();
void exibirEstado(FilaCircular* fila, Pilha* pilha);
void exibirMenu();
void pausar();

// ==================== FUNCAO PRINCIPAL ====================
int main() {
    FilaCircular fila;
    Pilha pilha;
    int opcao;
    
    srand(time(NULL));
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    printf("=====================================================\n");
    printf("   TETRIS STACK - SISTEMA COMPLETO (MESTRE)\n");
    printf("=====================================================\n");
    printf(">>> Inicializando sistema avancado...\n\n");
    
    // Preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }
    
    printf(">>> Fila inicializada!\n");
    printf(">>> Pilha pronta!\n");
    printf(">>> Trocas estrategicas disponiveis!\n");
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
                // Jogar peca
                if (!filaVazia(&fila)) {
                    Peca p = dequeue(&fila);
                    printf(">>> PECA JOGADA: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                } else {
                    printf(">>> ERRO: Fila vazia!\n");
                }
                break;
                
            case 2:
                // Reservar peca
                if (pilhaCheia(&pilha)) {
                    printf(">>> ERRO: Pilha cheia!\n");
                } else if (filaVazia(&fila)) {
                    printf(">>> ERRO: Fila vazia!\n");
                } else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf(">>> PECA RESERVADA: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
                
            case 3:
                // Usar peca reservada
                if (pilhaVazia(&pilha)) {
                    printf(">>> ERRO: Pilha vazia!\n");
                } else {
                    Peca p = pop(&pilha);
                    printf(">>> PECA USADA: [%c %d]\n", p.nome, p.id);
                }
                break;
                
            case 4:
                // Trocar peca simples (frente fila <-> topo pilha)
                trocarPecaSimples(&fila, &pilha);
                break;
                
            case 5:
                // Trocar 3 primeiras da fila com 3 da pilha
                trocarMultipla(&fila, &pilha);
                break;
                
            case 0:
                printf(">>> Encerrando sistema...\n");
                break;
                
            default:
                printf(">>> ERRO: Opcao invalida!\n");
        }
        
        if (opcao != 0) {
            pausar();
        }
        
    } while (opcao != 0);
    
    printf("\n=====================================================\n");
    printf("Sistema finalizado!\n");
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
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->elementos[fila->tras] = peca;
    fila->tamanho++;
}

Peca dequeue(FilaCircular* fila) {
    Peca p = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    return p;
}

Peca frente(FilaCircular* fila) {
    return fila->elementos[fila->frente];
}

// ==================== IMPLEMENTACAO - PILHA ====================

void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha* pilha) {
    return (pilha->topo == -1);
}

int pilhaCheia(Pilha* pilha) {
    return (pilha->topo == TAM_PILHA - 1);
}

void push(Pilha* pilha, Peca peca) {
    if (pilhaCheia(pilha)) return;
    pilha->topo++;
    pilha->elementos[pilha->topo] = peca;
}

Peca pop(Pilha* pilha) {
    Peca p = pilha->elementos[pilha->topo];
    pilha->topo--;
    return p;
}

Peca topo(Pilha* pilha) {
    return pilha->elementos[pilha->topo];
}

// ==================== OPERACOES AVANCADAS ====================

/*
 * trocarPecaSimples()
 * Troca a peca da frente da fila com o topo da pilha
 */
void trocarPecaSimples(FilaCircular* fila, Pilha* pilha) {
    if (filaVazia(fila)) {
        printf(">>> ERRO: Fila vazia! Impossivel trocar.\n");
        return;
    }
    
    if (pilhaVazia(pilha)) {
        printf(">>> ERRO: Pilha vazia! Impossivel trocar.\n");
        return;
    }
    
    printf(">>> TROCA SIMPLES:\n");
    
    // Guarda as pecas
    Peca pecaFila = frente(fila);
    Peca pecaPilha = topo(pilha);
    
    printf("    Fila (frente): [%c %d]\n", pecaFila.nome, pecaFila.id);
    printf("    Pilha (topo): [%c %d]\n", pecaPilha.nome, pecaPilha.id);
    
    // Remove de ambas
    dequeue(fila);
    pop(pilha);
    
    // Insere trocadas
    push(pilha, pecaFila);
    enqueue(fila, pecaPilha);
    
    printf("\n>>> TROCA REALIZADA COM SUCESSO!\n");
}

/*
 * trocarMultipla()
 * Troca as 3 primeiras pecas da fila com as 3 da pilha
 */
void trocarMultipla(FilaCircular* fila, Pilha* pilha) {
    // Valida se tem 3 em cada
    if (fila->tamanho < 3) {
        printf(">>> ERRO: Fila precisa ter pelo menos 3 pecas!\n");
        printf(">>> Atual: %d pecas\n", fila->tamanho);
        return;
    }
    
    if (pilha->topo + 1 < 3) {
        printf(">>> ERRO: Pilha precisa ter 3 pecas!\n");
        printf(">>> Atual: %d pecas\n", pilha->topo + 1);
        return;
    }
    
    printf(">>> TROCA MULTIPLA (3 x 3):\n");
    
    // Arrays temporarios
    Peca tempFila[3];
    Peca tempPilha[3];
    
    // Remove 3 da fila
    printf("\n    Removendo da fila:\n");
    for (int i = 0; i < 3; i++) {
        tempFila[i] = dequeue(fila);
        printf("      [%c %d]\n", tempFila[i].nome, tempFila[i].id);
    }
    
    // Remove 3 da pilha
    printf("\n    Removendo da pilha:\n");
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = pop(pilha);
        printf("      [%c %d]\n", tempPilha[i].nome, tempPilha[i].id);
    }
    
    // Coloca pecas da pilha na fila
    printf("\n    Inserindo na fila (pecas da pilha):\n");
    for (int i = 0; i < 3; i++) {
        enqueue(fila, tempPilha[i]);
        printf("      [%c %d]\n", tempPilha[i].nome, tempPilha[i].id);
    }
    
    // Coloca pecas da fila na pilha
    printf("\n    Inserindo na pilha (pecas da fila):\n");
    for (int i = 2; i >= 0; i--) {  // Inverte ordem
        push(pilha, tempFila[i]);
        printf("      [%c %d]\n", tempFila[i].nome, tempFila[i].id);
    }
    
    printf("\n>>> TROCA MULTIPLA REALIZADA COM SUCESSO!\n");
}

// ==================== FUNCOES GERAIS ====================

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = proximoId++;
    return p;
}

void exibirEstado(FilaCircular* fila, Pilha* pilha) {
    printf("\n=====================================================\n");
    printf("           ESTADO ATUAL DO SISTEMA\n");
    printf("=====================================================\n");
    
    // Fila
    printf("Fila de pecas: ");
    if (filaVazia(fila)) {
        printf("[VAZIA]\n");
    } else {
        int i = fila->frente;
        int cont = 0;
        while (cont < fila->tamanho) {
            printf("[%c %d] ", fila->elementos[i].nome, fila->elementos[i].id);
            i = (i + 1) % TAM_FILA;
            cont++;
        }
        printf("\n");
    }
    printf("    (%d/%d pecas)\n", fila->tamanho, TAM_FILA);
    
    printf("-----------------------------------------------------\n");
    
    // Pilha
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("[VAZIA]\n");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
        }
        printf("\n");
    }
    printf("    (%d/%d pecas)\n", pilha->topo + 1, TAM_PILHA);
    
    printf("=====================================================\n");
}

void exibirMenu() {
    printf("\n=====================================================\n");
    printf("              MENU DE OPCOES\n");
    printf("=====================================================\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiras (fila) com 3 (pilha)\n");
    printf("0 - Sair\n");
    printf("=====================================================\n");
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    while (getchar() != '\n');
    getchar();
}

/*
 * =====================================================================
 * EXPLICACOES TECNICAS - NIVEL MESTRE:
 * =====================================================================
 * 
 * 1. TROCA SIMPLES (Opcao 4):
 *    - Troca frente da fila <-> topo da pilha
 *    - Mantém ordem das demais pecas
 *    - Operacao O(1)
 *    - Estrategia: pegar peca especifica da pilha
 * 
 * 2. TROCA MULTIPLA (Opcao 5):
 *    - Troca 3 primeiras da fila com 3 da pilha
 *    - Reorganizacao massiva
 *    - Usa arrays temporarios
 *    - Inverte ordem ao inserir na pilha
 *    - Operacao O(n) onde n=3
 * 
 * 3. COMPLEXIDADE DAS OPERACOES:
 *    - Jogar/Reservar/Usar: O(1)
 *    - Troca simples: O(1)
 *    - Troca multipla: O(1) - tamanho fixo
 * 
 * 4. ESTRATEGIAS POSSIVEIS:
 *    - Reservar pecas "boas" para momento certo
 *    - Trocar simples para pegar peca especifica
 *    - Troca multipla para reorganizar sequencia
 *    - Combinar operacoes para otimizar jogadas
 * 
 * 5. INTEGRACAO FILA-PILHA:
 *    - Fila: ordem de chegada (FIFO)
 *    - Pilha: ordem inversa (LIFO)
 *    - Trocas permitem flexibilidade estrategica
 *    - Sistema completo e funcional
 * 
 * 6. VALIDACOES:
 *    - Verifica espacos antes de operacoes
 *    - Garante 3 pecas para troca multipla
 *    - Mensagens claras de erro
 *    - Feedback detalhado das trocas
 * =====================================================================
 */