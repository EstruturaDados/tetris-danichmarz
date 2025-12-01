/*
 * =====================================================================
 * TETRIS STACK - NIVEL NOVATO
 * Fila Circular de Pecas
 * =====================================================================
 * Descricao: Sistema de fila circular para gerenciar pecas futuras
 * do Tetris Stack. Implementa operacoes FIFO (First In, First Out).
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
 * Implementa uma fila circular para gerenciar as pecas
 */
typedef struct {
    Peca elementos[TAM_FILA];    // Array que armazena as pecas
    int frente;                   // Indice da frente da fila
    int tras;                     // Indice do final da fila
    int tamanho;                  // Quantidade atual de elementos
} FilaCircular;

// ==================== VARIAVEIS GLOBAIS ====================
int proximoId = 0;               // Contador global para IDs unicos

// ==================== PROTOTIPOS DAS FUNCOES ====================
void inicializarFila(FilaCircular* fila);
int filaVazia(FilaCircular* fila);
int filaCheia(FilaCircular* fila);
Peca gerarPeca();
void enqueue(FilaCircular* fila, Peca peca);
Peca dequeue(FilaCircular* fila);
void exibirFila(FilaCircular* fila);
void exibirMenu();
void pausar();

// ==================== FUNCAO PRINCIPAL ====================
int main() {
    FilaCircular fila;
    int opcao;
    
    // Inicializa gerador de numeros aleatorios
    srand(time(NULL));
    
    // Inicializa a fila
    inicializarFila(&fila);
    
    // Preenche a fila inicialmente com 5 pecas
    printf("=====================================================\n");
    printf("         TETRIS STACK - FILA DE PECAS\n");
    printf("=====================================================\n");
    printf(">>> Inicializando fila de pecas...\n\n");
    
    for (int i = 0; i < TAM_FILA; i++) {
        Peca novaPeca = gerarPeca();
        enqueue(&fila, novaPeca);
    }
    
    printf(">>> Fila inicializada com %d pecas!\n", TAM_FILA);
    pausar();
    
    // Loop principal do jogo
    do {
        exibirFila(&fila);
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
                    
                    // Gera e adiciona nova peca automaticamente
                    Peca novaPeca = gerarPeca();
                    enqueue(&fila, novaPeca);
                    printf(">>> Nova peca gerada: [%c %d]\n", novaPeca.nome, novaPeca.id);
                } else {
                    printf(">>> ERRO: Fila vazia!\n");
                }
                break;
                
            case 2:
                // Inserir nova peca (enqueue)
                if (!filaCheia(&fila)) {
                    Peca novaPeca = gerarPeca();
                    enqueue(&fila, novaPeca);
                    printf(">>> Nova peca inserida: [%c %d]\n", novaPeca.nome, novaPeca.id);
                } else {
                    printf(">>> ERRO: Fila cheia! Jogue uma peca primeiro.\n");
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

// ==================== IMPLEMENTACAO DAS FUNCOES ====================

/*
 * inicializarFila()
 * Inicializa a fila circular com valores padrao
 */
void inicializarFila(FilaCircular* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

/*
 * filaVazia()
 * Verifica se a fila esta vazia
 * Retorna: 1 se vazia, 0 caso contrario
 */
int filaVazia(FilaCircular* fila) {
    return (fila->tamanho == 0);
}

/*
 * filaCheia()
 * Verifica se a fila esta cheia
 * Retorna: 1 se cheia, 0 caso contrario
 */
int filaCheia(FilaCircular* fila) {
    return (fila->tamanho == TAM_FILA);
}

/*
 * gerarPeca()
 * Gera uma peca aleatoria com tipo e ID unico
 * Retorna: nova peca gerada
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Seleciona tipo aleatorio
    novaPeca.nome = tipos[rand() % 4];
    
    // Atribui ID unico
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

/*
 * enqueue()
 * Adiciona uma peca ao final da fila circular
 * Parametros: fila (ponteiro), peca a ser adicionada
 */
void enqueue(FilaCircular* fila, Peca peca) {
    if (filaCheia(fila)) {
        printf(">>> ERRO: Fila cheia! Impossivel adicionar peca.\n");
        return;
    }
    
    // Calcula proxima posicao (circular)
    fila->tras = (fila->tras + 1) % TAM_FILA;
    
    // Adiciona a peca
    fila->elementos[fila->tras] = peca;
    fila->tamanho++;
}

/*
 * dequeue()
 * Remove e retorna a peca da frente da fila
 * Parametros: fila (ponteiro)
 * Retorna: peca removida
 */
Peca dequeue(FilaCircular* fila) {
    if (filaVazia(fila)) {
        printf(">>> ERRO: Fila vazia! Impossivel remover peca.\n");
        Peca pecaVazia = {'\0', -1};
        return pecaVazia;
    }
    
    // Pega a peca da frente
    Peca pecaRemovida = fila->elementos[fila->frente];
    
    // Avanca a frente (circular)
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

/*
 * exibirFila()
 * Mostra o estado atual da fila de pecas
 */
void exibirFila(FilaCircular* fila) {
    printf("\n=====================================================\n");
    printf("              ESTADO ATUAL DA FILA\n");
    printf("=====================================================\n");
    
    if (filaVazia(fila)) {
        printf(">>> Fila vazia!\n");
        printf("=====================================================\n");
        return;
    }
    
    printf("Fila de pecas: ");
    
    // Percorre a fila de forma circular
    int i = fila->frente;
    int contador = 0;
    
    while (contador < fila->tamanho) {
        printf("[%c %d] ", fila->elementos[i].nome, fila->elementos[i].id);
        i = (i + 1) % TAM_FILA;
        contador++;
    }
    
    printf("\n");
    printf("-----------------------------------------------------\n");
    printf("Pecas na fila: %d/%d\n", fila->tamanho, TAM_FILA);
    printf("Frente: posicao %d | Tras: posicao %d\n", fila->frente, fila->tras);
    printf("=====================================================\n");
}

/*
 * exibirMenu()
 * Exibe o menu de opcoes do jogo
 */
void exibirMenu() {
    printf("\n=====================================================\n");
    printf("                 MENU DE OPCOES\n");
    printf("=====================================================\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair do jogo\n");
    printf("=====================================================\n");
}

/*
 * pausar()
 * Pausa a execucao aguardando ENTER
 */
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
 * 1. FILA CIRCULAR:
 *    - Estrutura FIFO (First In, First Out)
 *    - Usa modulo (%) para retornar ao inicio do array
 *    - Reaproveita espaco sem necessidade de deslocar elementos
 *    - Complexidade O(1) para enqueue e dequeue
 * 
 * 2. INDICES DA FILA:
 *    - frente: indica onde esta o primeiro elemento
 *    - tras: indica onde esta o ultimo elemento
 *    - tamanho: controla quantos elementos existem
 * 
 * 3. OPERACAO CIRCULAR:
 *    - (tras + 1) % TAM_FILA: avanca tras circularmente
 *    - (frente + 1) % TAM_FILA: avanca frente circularmente
 *    - Quando chega no fim do array, volta para o inicio
 * 
 * 4. GERACAO DE PECAS:
 *    - Tipos: I, O, T, L (pecas classicas do Tetris)
 *    - ID unico gerado sequencialmente
 *    - Tipo escolhido aleatoriamente
 * 
 * 5. FLUXO DO JOGO:
 *    - Fila sempre mantida com pecas
 *    - Ao jogar uma peca, nova e gerada automaticamente
 *    - Jogador pode ver proximas pecas na fila
 * 
 * 6. VANTAGENS DA FILA CIRCULAR:
 *    - Nao precisa realocar memoria
 *    - Nao precisa deslocar elementos
 *    - Operacoes muito rapidas
 *    - Uso eficiente de memoria
 * =====================================================================
 */
