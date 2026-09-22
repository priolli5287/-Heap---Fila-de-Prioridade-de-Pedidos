#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char descricao[100];
    int prioridade;
} Pedido;

typedef struct {
    Pedido *dados;
    int tamanho;
    int capacidade;
} Heap;

//  MACROS / FUNÇÕES AUXILIARES DE NAVEGAÇÃO NA ÁRVORE (ÍNDICES)

int pai(int i) { return (i - 1) / 2; }
int esquerdo(int i) { return 2 * i + 1; }
int direito(int i) { return 2 * i + 2; }

void trocar(Pedido *a, Pedido *b) {
    Pedido temp = *a;
    *a = *b;
    *b = temp;
}

// OPERAÇÕES OBRIGATÓRIAS DO MAX-HEAP

Heap* inicializar_heap(int capacidade_inicial) {
    Heap *h = (Heap*) malloc(sizeof(Heap));
    if (h == NULL) return NULL;

    h->dados = (Pedido*) malloc(capacidade_inicial * sizeof(Pedido));
    if (h->dados == NULL) {
        free(h);
        return NULL;
    }

    h->tamanho = 0;
    h->capacidade = capacidade_inicial;
    return h;
}

void max_heapfy(Heap *h, int i) {
    int esq = esquerdo(i);
    int dir = direito(i);
    int maior = i;

    // Compara o elemento atual com o filho esquerdo
    if (esq < h->tamanho && h->dados[esq].prioridade > h->dados[maior].prioridade) {
        maior = esq;
    }

    // Compara o maior até agora com o filho direito
    if (dir < h->tamanho && h->dados[dir].prioridade > h->dados[maior].prioridade) {
        maior = dir;
    }

    // Se o maior não for a raiz da subárvore, realiza a troca e desce na árvore
    if (maior != i) {
        trocar(&h->dados[i], &h->dados[maior]);
        max_heapfy(h, maior);
    }
}

void construir(Heap *h) {
    // Aplica max_heapfy de baixo para cima a partir do último nó não-folha
    for (int i = (h->tamanho / 2) - 1; i >= 0; i--) {
        max_heapfy(h, i);
    }
}

int inserir(Heap *h, Pedido p) {
    if (h == NULL) return 0;

    // Realoca se o vetor atingir a capacidade máxima
    if (h->tamanho >= h->capacidade) {
        int nova_cap = h->capacidade * 2;
        Pedido *temp = (Pedido*) realloc(h->dados, nova_cap * sizeof(Pedido));
        if (temp == NULL) return 0; // Falha na realocação
        h->dados = temp;
        h->capacidade = nova_cap;
    }

    // 1. Insere o novo elemento no final do vetor
    int i = h->tamanho;
    h->dados[i] = p;
    h->tamanho++;

    // 2. Sobe o elemento realizando trocas com o pai até restaurar a propriedade Max-Heap
    while (i != 0 && h->dados[pai(i)].prioridade < h->dados[i].prioridade) {
        trocar(&h->dados[i], &h->dados[pai(i)]);
        i = pai(i);
    }

    return 1;
}

int remover(Heap *h, Pedido *removido) {
    if (h == NULL || h->tamanho <= 0) {
        return 0; // Heap vazio
    }

    // O pedido de maior prioridade está sempre na raiz (índice 0)
    if (removido != NULL) {
        *removido = h->dados[0];
    }

    // Move o último elemento para a raiz
    h->dados[0] = h->dados[h->tamanho - 1];
    h->tamanho--;

    // Reorganiza a estrutura a partir da raiz
    if (h->tamanho > 0) {
        max_heapfy(h, 0);
    }

    return 1;
}

void print_heap(Heap *h) {
    if (h == NULL || h->tamanho == 0) {
        printf("\nNenhum pedido aguardando no Heap.\n");
        return;
    }

    printf("\n--- PEDIDOS NO HEAP (Tamanho: %d) ---\n", h->tamanho);
    for (int i = 0; i < h->tamanho; i++) {
        printf("ID: %d | Descricao: %s | Prioridade: %d\n",
               h->dados[i].id, h->dados[i].descricao, h->dados[i].prioridade);
    }
}

void liberar_heap(Heap *h) {
    if (h != NULL) {
        free(h->dados);
        free(h);
    }
}

// BATERIA DE TESTES AUTOMATIZADOS

void executar_testes() {
    printf("\n====================================================\n");
    printf("     INICIANDO SUÍTE DE TESTES DO MAX-HEAP          \n");
    printf("====================================================\n\n");

    // Teste 1: Inicialização
    Heap *h = inicializar_heap(2);
    printf("[Teste 1] Heap Criado. Capacidade: %d | Tamanho: %d\n", h->capacidade, h->tamanho);

    // Teste 2: Inserção e Propriedade Max-Heap (Pai >= Filhos)
    Pedido p1 = {1, "Pedido A", 2};
    Pedido p2 = {2, "Pedido B", 5};
    Pedido p3 = {3, "Pedido C", 3};
    Pedido p4 = {4, "Pedido D", 8};
    Pedido p5 = {5, "Pedido E", 1};

    inserir(h, p1);
    inserir(h, p2);
    inserir(h, p3);
    inserir(h, p4);
    inserir(h, p5);

    printf("[Teste 2] Inseridos 5 pedidos. Elemento da Raiz (Esperado: Prioridade 8 | ID: 4): Prioridade %d | ID: %d\n",
           h->dados[0].prioridade, h->dados[0].id);

    // Teste 3: Remoção do Máximo
    Pedido atendido;
    remover(h, &atendido);
    printf("[Teste 3] Atendido pedido de maior prioridade (Esperado: ID 4, Prio 8). Obtido: ID %d, Prio %d\n",
           atendido.id, atendido.prioridade);
    printf("Nova raiz apos remocao (Esperado: ID 2, Prio 5). Obtido: ID %d, Prio %d\n",
           h->dados[0].id, h->dados[0].prioridade);

    // Teste 4: Construir Max-Heap a partir de conjunto sem ordem
    Heap *h_unbuilt = inicializar_heap(5);
    h_unbuilt->dados[0] = p1; // Prio 2
    h_unbuilt->dados[1] = p2; // Prio 5
    h_unbuilt->dados[2] = p3; // Prio 3
    h_unbuilt->dados[3] = p4; // Prio 8
    h_unbuilt->dados[4] = p5; // Prio 1
    h_unbuilt->tamanho = 5;

    printf("\n[Teste 4] Estado do vetor antes do 'construir':\n");
    print_heap(h_unbuilt);

    construir(h_unbuilt);
    printf("Estado do vetor apos o 'construir' (Raiz deve ser Prio 8):\n");
    print_heap(h_unbuilt);

    liberar_heap(h);
    liberar_heap(h_unbuilt);

    printf("\n====================================================\n");
    printf("     SUÍTE DE TESTES CONCLUÍDA COM SUCESSO!         \n");
    printf("====================================================\n\n");
}

// SISTEMA INTERATIVO COM O USUÁRIO

void carregar_conjunto_inicial(Heap *h) {
    Pedido iniciais[] = {
        {101, "Pedido de 10 produtos", 2},
        {102, "Pedido de 5 produtos", 5},
        {103, "Pedido de 20 produtos", 3},
        {104, "Pedido urgente", 8},
        {105, "Pedido VIP", 10}
    };

    for (int i = 0; i < 5; i++) {
        if (h->tamanho < h->capacidade) {
            h->dados[h->tamanho++] = iniciais[i];
        } else {
            inserir(h, iniciais[i]);
        }
    }
    // Reorganiza o vetor em formato Max-Heap
    construir(h);
    printf("\n[SUCESSO] Conjunto inicial de 5 pedidos carregado e transformado em Max-Heap via 'construir'!\n");
}

int main() {
    Heap *heap_pedidos = inicializar_heap(4);
    int opcao = -1;

    while (opcao != 5) {
        printf("\n===== SISTEMA DE PEDIDOS (MAX-HEAP) =====\n");
        printf("1 - Cadastrar pedido\n");
        printf("2 - Atender pedido\n");
        printf("3 - Exibir pedidos\n");
        printf("4 - Exibir quantidade de pedidos\n");
        printf("6 - Carregar conjunto inicial e aplicar 'construir'\n");
        printf("7 - Executar suíte de testes automatizados\n");
        printf("5 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Pedido p;
                printf("\nID: ");
                scanf("%d", &p.id);
                getchar(); // Limpar buffer

                printf("Descricao: ");
                fgets(p.descricao, 100, stdin);
                p.descricao[strcspn(p.descricao, "\n")] = 0;

                printf("Prioridade: ");
                scanf("%d", &p.prioridade);

                if (inserir(heap_pedidos, p)) {
                    printf("Pedido cadastrado com sucesso e inserido no Heap!\n");
                } else {
                    printf("Erro ao cadastrar pedido.\n");
                }
                break;
            }
            case 2: {
                Pedido atendido;
                if (remover(heap_pedidos, &atendido)) {
                    printf("\nPedido atendido:\n");
                    printf("ID: %d\n", atendido.id);
                    printf("Descricao: %s\n", atendido.descricao);
                    printf("Prioridade: %d\n", atendido.prioridade);

                    if (heap_pedidos->tamanho > 0) {
                        printf("\nPedidos restantes:\n");
                        print_heap(heap_pedidos);
                    } else {
                        printf("\nTodos os pedidos foram atendidos! Heap vazio.\n");
                    }
                } else {
                    printf("\nNenhum pedido para atender (Heap vazio)!\n");
                }
                break;
            }
            case 3:
                print_heap(heap_pedidos);
                break;
            case 4:
                printf("\nQuantidade de pedidos aguardando atendimento: %d\n", heap_pedidos->tamanho);
                break;
            case 6:
                carregar_conjunto_inicial(heap_pedidos);
                break;
            case 7:
                executar_testes();
                break;
            case 5:
                printf("\nEncerrando o sistema de pedidos...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    }

    liberar_heap(heap_pedidos);
    return 0;
}
