# Sistema de Gerenciamento de Pedidos com Max-Heap

Este projeto implementa um **Sistema de Fila de Prioridades de Pedidos** utilizando a estrutura de dados **Max-Heap**, desenvolvida em linguagem C sem a utilização de bibliotecas prontas.

## Estrutura de Dados e Relação de Índices

O Heap é representado concretamente por um **vetor dinâmico** mapeado conceitualmente como uma Árvore Binária Quase Completa.

Para qualquer nó localizado no índice $i$:

* **Pai:** `(i - 1) / 2`
* **Filho Esquerdo:** `2 * i + 1`
* **Filho Direito:** `2 * i + 2`

```text
       [104 (Prio 8)]          <- Raiz (Índice 0 - Maior Prioridade)
        /          \
  [102 (Prio 5)]  [103 (Prio 3)]
      /
[101 (Prio 2)]
```

---

## Descrição das Operações do Heap

1. **`inicializar_heap`**: Aloca o vetor dinâmico e cria a estrutura com capacidade inicial.
2. **`inserir`** ($O(\log n)$): Insere o novo pedido no final do vetor e realiza o subida de nó (*shift-up/bubble-up*) trocando com seus pais até garantir que $Pai \ge Filho$.
3. **`remover`** ($O(\log n)$): Retorna o pedido da raiz (maior prioridade), move o último elemento do vetor para a raiz e invoca `max_heapfy`.
4. **`max_heapfy`** ($O(\log n)$): Compara o nó $i$ com seus filhos esquerdo e direito, promovendo o maior e descendo recursivamente.
5. **`construir`** ($O(n)$): Converte um vetor não ordenado de pedidos em um Max-Heap chamando `max_heapfy` do último nó não-folha `(n/2 - 1)` até a raiz.
6. **`print_heap`**: Exibe visualmente o vetor que representa o Heap.

---

## Instruções de Compilação e Execução

### Compilação
No terminal, execute:

```bash
gcc -std=c99 main.c -o sistema_pedidos
```

### Execução
* **Linux / macOS:**
  ```bash
  ./sistema_pedidos
  ```
* **Windows (PowerShell / CMD):**
  ```bash
  .\sistema_pedidos.exe
  ```

---

## Exemplo de Execução do Programa

```text
===== SISTEMA DE PEDIDOS (MAX-HEAP) =====
1 - Cadastrar pedido
2 - Atender pedido
3 - Exibir pedidos
4 - Exibir quantidade de pedidos
5 - Sair

Escolha: 3

--- PEDIDOS NO HEAP (Tamanho: 4) ---
ID: 104 | Descricao: Pedido urgente | Prioridade: 8
ID: 102 | Descricao: Pedido de 5 produtos | Prioridade: 5
ID: 103 | Descricao: Pedido de 20 produtos | Prioridade: 3
ID: 101 | Descricao: Pedido de 10 produtos | Prioridade: 2

Escolha: 2

Pedido atendido:
ID: 104
Descricao: Pedido urgente
Prioridade: 8
```

---

## Bateria de Testes e Resultados

A aplicação possui uma suíte de testes (Opção 7 do Menu) para validar o comportamento do Max-Heap:

| Caso de Teste | Operação Avaliada | Resultado Esperado | Resultado |
| :--- | :--- | :--- | :---: |
| **Maior na Raiz** | Inserções variadas (Prio 2, 5, 3, 8, 1) | Elemento de prioridade 8 assume o índice 0 | **APROVADO** |
| **Atendimento** | `remover` na raiz | Retorna prioridade 8; novo topo passa a ser 5 | **APROVADO** |
| **Construção** | `construir` sobre vetor desordenado | Reorganiza o vetor em $O(n)$ garantindo Max-Heap | **APROVADO** |
| **Realloc** | Exceder capacidade inicial | Expande capacidade dinamicamente via `realloc` | **APROVADO** |
