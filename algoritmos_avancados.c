#include <stdio.h>
#include <stdlib.h>     // Para malloc, free, NULL
#include <string.h>     // Para strcmp, strcpy, strdup
#include <ctype.h>      // Para tolower

// --- Constantes Globais ---
#define MAX_NOME 50
#define TAMANHO_HASH 10 // Tamanho da Tabela Hash

// --- Estrutura 1: Árvore Binária do Mapa (Nível Novato) ---
struct Sala {
    char nome[MAX_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
};

// --- Estrutura 2: Árvore Binária de Busca (BST) de Pistas (Nível Aventureiro) ---
struct NoPista {
    char pista[MAX_NOME];
    struct NoPista *esquerda;
    struct NoPista *direita;
};

// --- Estrutura 3: Tabela Hash (Suspeitos) (Nível Mestre) ---
struct NoHash {
    char pista[MAX_NOME];     // Chave
    char suspeito[MAX_NOME];  // Valor
    struct NoHash *proximo; // Para tratar colisões (encadeamento)
};

// --- Array da Tabela Hash ---
struct NoHash *tabelaHash[TAMANHO_HASH];

// --- Protótipos das Funções ---

// Funções Utilitárias
void limparBufferEntrada();
void pausarParaContinuar();
unsigned int funcaoHash(const char *chave);

// Funções da Árvore do Mapa (Nível Novato)
struct Sala* criarSala(const char *nome);
void explorarSalas(struct Sala *salaAtual, struct NoPista **raizPistas);

// Funções da Árvore de Busca (BST) de Pistas (Nível Aventureiro)
struct NoPista* inserirPista(struct NoPista *no, const char *pista);
void exibirPistasEmOrdem(struct NoPista *no);

// Funções da Tabela Hash (Nível Mestre)
void inicializarHash();
void inserirNaHash(const char *pista, const char *suspeito);
const char* buscarNaHash(const char *pista);
void exibirAssociacoesHash();
void encontrarSuspeitoMaisCitado(struct NoPista *raizPistas);

// --- Função Principal (main) ---
int main() {
    // 1. Setup - Inicializa TUDO
    struct Sala *raizMapa = NULL;
    struct NoPista *raizPistas = NULL;
    inicializarHash(); // Zera a tabela hash

    // 2. Criação do Mapa (Nível Novato) - Feito manualmente no código
    raizMapa = criarSala("Hall de Entrada");
    raizMapa->esquerda = criarSala("Biblioteca");
    raizMapa->direita = criarSala("Sala de Jantar");
    raizMapa->esquerda->esquerda = criarSala("Escritorio"); // Onde acha a "Carta"
    raizMapa->direita->direita = criarSala("Cozinha");      // Onde acha a "Faca"

    // 3. Setup das Evidências (Nível Mestre)
    // No mundo real, isso seria descoberto, mas aqui pré-populamos
    inserirNaHash("Carta", "Mordomo");
    inserirNaHash("Faca", "Cozinheiro");
    inserirNaHash("Veneno", "Jardineiro"); // Pista extra
    
    printf("--- DETECTIVE QUEST (NIVEL MESTRE) ---\n");
    printf("Mansao carregada. Tabela Hash de suspeitos preenchida.\n");
    printf("Explore a mansao para encontrar as pistas.\n");
    pausarParaContinuar();

    // 4. Inicia a exploração (Nível Novato + Aventureiro)
    // Passamos o endereço de raizPistas (ponteiro para ponteiro)
    // para que a função explorarSalas possa MODIFICAR a raiz.
    explorarSalas(raizMapa, &raizPistas); 

    // 5. Relatório Final (Nível Mestre)
    system("clear"); // Use "cls" no Windows
    printf("\n--- RELATORIO FINAL DA INVESTIGACAO ---\n");

    printf("\n[Pistas Coletadas (Em ordem alfabetica)]:\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistasEmOrdem(raizPistas);
    }

    printf("\n[Associacoes da Tabela Hash (Pista -> Suspeito)]:\n");
    exibirAssociacoesHash();

    printf("\n[Deducao do Culpado (Suspeito mais citado)]:\n");
    encontrarSuspeitoMaisCitado(raizPistas); // Analisa as pistas encontradas
    
    // NOTA: Faltaria liberar a memória de todas as árvores e da hash
    // (mas para uma entrega rápida, o programa termina aqui).

    return 0;
}

// --- Implementação das Funções Utilitárias ---

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausarParaContinuar() {
    printf("\nPressione Enter para continuar...");
    getchar();
}

/*
 * funcaoHash(): Função de hash simples (soma ASCII)
 */
unsigned int funcaoHash(const char *chave) {
    unsigned long hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash += chave[i];
    }
    return hash % TAMANHO_HASH;
}

// --- Implementação da Árvore do Mapa (Nível Novato) ---

/*
 * criarSala(): Aloca memória para uma nova sala e define seu nome.
 */
struct Sala* criarSala(const char *nome) {
    struct Sala* novaSala = (struct Sala*) malloc(sizeof(struct Sala));
    if (novaSala != NULL) {
        strncpy(novaSala->nome, nome, MAX_NOME);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

/*
 * explorarSalas(): Loop interativo de navegação pela mansão.
 */
void explorarSalas(struct Sala *salaAtual, struct NoPista **raizPistas) {
    char escolha;
    while (1) { // Loop infinito de exploração
        system("clear");
        printf("Voce esta em: %s\n", salaAtual->nome);
        
        // Lógica de encontrar pistas (Nível Aventureiro / Mestre)
        if (strcmp(salaAtual->nome, "Escritorio") == 0) {
            printf("Voce encontrou uma pista: 'Carta'\n");
            *raizPistas = inserirPista(*raizPistas, "Carta");
        }
        if (strcmp(salaAtual->nome, "Cozinha") == 0) {
            printf("Voce encontrou uma pista: 'Faca'\n");
            *raizPistas = inserirPista(*raizPistas, "Faca");
        }

        // Lógica de Navegação
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("\nEste eh um beco sem saida. Voce retornara ao Hall.\n");
            pausarParaContinuar();
            return; // Sai da exploração
        }

        printf("\nEscolha seu caminho:\n");
        if (salaAtual->esquerda != NULL) printf("(e) Esquerda -> %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL) printf("(d) Direita  -> %s\n", salaAtual->direita->nome);
        printf("(s) Sair da mansao\n");
        printf("Escolha: ");

        scanf(" %c", &escolha);
        limparBufferEntrada();
        escolha = tolower(escolha);

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("\nSaindo da mansao...\n");
            pausarParaContinuar();
            return; // Sai da exploração
        } else {
            printf("\nCaminho invalido. Tente novamente.\n");
            pausarParaContinuar();
        }
    }
}

// --- Implementação da Árvore de Busca (BST) de Pistas (Nível Aventureiro) ---

/*
 * inserirPista(): Insere uma nova pista na BST (em ordem alfabética).
 * Usa recursão.
 */
struct NoPista* inserirPista(struct NoPista *no, const char *pista) {
    // 1. Caso Base: Nó vazio, encontramos onde inserir
    if (no == NULL) {
        struct NoPista* novoNo = (struct NoPista*) malloc(sizeof(struct NoPista));
        strncpy(novoNo->pista, pista, MAX_NOME);
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        printf("(Pista '%s' adicionada ao inventario!)\n", pista);
        pausarParaContinuar();
        return novoNo;
    }

    // 2. Caso Recursivo: Decide para onde ir
    if (strcmp(pista, no->pista) < 0) {
        // Pista nova é "menor" (vem antes no alfabeto), vai para a esquerda
        no->esquerda = inserirPista(no->esquerda, pista);
    } else if (strcmp(pista, no->pista) > 0) {
        // Pista nova é "maior", vai para a direita
        no->direita = inserirPista(no->direita, pista);
    }
    // Se (strcmp == 0), a pista já existe, não faz nada.
    
    return no; // Retorna o nó (inalterado)
}

/*
 * exibirPistasEmOrdem(): Percorre a BST em "in-order" (Esquerda, Raiz, Direita)
 * o que resulta em uma impressão em ordem alfabética.
 */
void exibirPistasEmOrdem(struct NoPista *no) {
    if (no != NULL) {
        exibirPistasEmOrdem(no->esquerda);
        printf("- %s\n", no->pista);
        exibirPistasEmOrdem(no->direita);
    }
}

// --- Implementação da Tabela Hash (Nível Mestre) ---

/*
 * inicializarHash(): Zera todos os ponteiros da tabela.
 */
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

/*
 * inserirNaHash(): Insere uma associação Pista -> Suspeito.
 * Trata colisões usando encadeamento simples (adiciona no início da lista).
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned int indice = funcaoHash(pista); // Calcula onde salvar

    // Cria o novo nó
    struct NoHash *novoNo = (struct NoHash*) malloc(sizeof(struct NoHash));
    strncpy(novoNo->pista, pista, MAX_NOME);
    strncpy(novoNo->suspeito, suspeito, MAX_NOME);
    
    // Adiciona no início da lista (tratamento de colisão)
    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;
}

/*
 * buscarNaHash(): Busca um suspeito associado a uma pista.
 */
const char* buscarNaHash(const char *pista) {
    unsigned int indice = funcaoHash(pista);
    
    struct NoHash *atual = tabelaHash[indice];
    
    // Percorre a lista encadeada naquele índice
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Encontrou!
        }
        atual = atual->proximo;
    }
    
    return "Desconhecido"; // Não encontrou
}

/*
 * exibirAssociacoesHash(): Mostra o conteúdo da tabela hash.
 */
void exibirAssociacoesHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        struct NoHash *atual = tabelaHash[i];
        if (atual == NULL) {
            printf("Indice %d: [ Vazio ]\n", i);
        } else {
            printf("Indice %d: ", i);
            while (atual != NULL) {
                printf("[Pista: %s -> Suspeito: %s] -> ", atual->pista, atual->suspeito);
                atual = atual->proximo;
            }
            printf("NULL\n");
        }
    }
}

/*
 * encontrarSuspeitoMaisCitado(): Percorre as PISTAS COLETADAS,
 * busca cada uma na HASH, e conta qual suspeito aparece mais.
 * (Esta é uma lógica complexa de Nível Mestre!)
 */
void encontrarSuspeitoMaisCitado(struct NoPista *raizPistas) {
    if (raizPistas == NULL) {
        printf("Nao ha pistas suficientes para uma deducao.\n");
        return;
    }

    // Esta é uma solução simples. Uma solução real usaria outra hash
    // ou lista para contar, mas vamos simplificar para o prazo.
    // Vamos apenas listar os suspeitos das pistas que encontramos.
    
    // Lista de suspeitos encontrados (simplificado)
    char suspeitosEncontrados[10][MAX_NOME];
    int contagemSuspeitos[10] = {0};
    int totalSuspeitosUnicos = 0;

    // Usaremos uma função interna (recursiva) para percorrer a árvore de pistas
    // e preencher nossa lista de contagem.
    // Esta é uma técnica avançada (função dentro de função não existe em C,
    // então temos que simular percorrendo e buscando).

    printf("Analisando pistas coletadas...\n");

    // Lógica de Contagem:
    // 1. Percorre a árvore de pistas
    // 2. Para cada pista, busca o suspeito na hash
    // 3. Adiciona o suspeito a uma lista e conta
    
    // Como não podemos criar funções dinamicamente, faremos o percurso
    // e a busca de forma separada, o que é menos eficiente mas mais claro.
    // Para esta entrega rápida, vamos apenas buscar e mostrar.
    
    struct NoPista *pilha[100]; // Pilha para percurso iterativo
    int topo = -1;
    struct NoPista *atual = raizPistas;
    
    // Lista de suspeitos citados (com repetição)
    char citados[100][MAX_NOME];
    int totalCitados = 0;

    // Percurso In-Order Iterativo (sem recursão)
    while (atual != NULL || topo != -1) {
        while (atual != NULL) {
            pilha[++topo] = atual;
            atual = atual->esquerda;
        }
        atual = pilha[topo--];
        
        // --- Processa o nó (a pista) ---
        const char *suspeito = buscarNaHash(atual->pista);
        if (strcmp(suspeito, "Desconhecido") != 0) {
            strncpy(citados[totalCitados++], suspeito, MAX_NOME);
        }
        // --- Fim do processamento ---
        
        atual = atual->direita;
    }

    if (totalCitados == 0) {
        printf("Nenhuma pista encontrada esta associada a um suspeito conhecido.\n");
        return;
    }

    // Agora, encontrar o mais frequente na lista "citados"
    char culpado[MAX_NOME];
    int maxContagem = 0;
    
    for (int i = 0; i < totalCitados; i++) {
        int contagemAtual = 0;
        for (int j = 0; j < totalCitados; j++) {
            if (strcmp(citados[i], citados[j]) == 0) {
                contagemAtual++;
            }
        }
        
        if (contagemAtual > maxContagem) {
            maxContagem = contagemAtual;
            strncpy(culpado, citados[i], MAX_NOME);
        }
    }
    
    printf("\n--- Veredito ---\n");
    printf("Com base nas pistas coletadas, o suspeito mais provavel eh...\n");
    printf("!!! %s !!! (Citado %d vez(es))\n", culpado, maxContagem);
}