# Anatomia do Código — Guia Rápido

Este documento apresenta uma visão geral das funções, estruturas de dados e organização do código em [`main.cpp`](main.cpp).

> [!TIP]
> Para uma explicação aprofundada, didática e passo a passo de cada linha e função do código, consulte o arquivo detalhado:
> 📖 **[Anatomia_do_Codigo.md](Anatomia_do_Codigo.md)** (ou a versão diagramada em **[Anatomia_do_Codigo.pdf](Anatomia_do_Codigo.pdf)**).

---

## 📚 Bibliotecas e Constantes

```cpp
#include <iostream>
#include <string>
#include <cmath>

const int MAX_ITENS = 100;
const int SEMELHANCA_MINIMA = 0;
const int SEMELHANCA_MAXIMA = 10;
const double RAIO_ABERTURA = 10.0;
const double PI = 3.14159265358979;
const int CRITERIO_ABERTURA = 0;
const int MAX_PONTOS = 20;

using namespace std;
```

- `<iostream>`: Entrada e saída padrão (`cin`, `cout`).
- `<string>`: Manipulação de strings (`length`, `substr`, `to_string`).
- `<cmath>`: Cálculo da raiz quadrada (`sqrt`), utilizada no cálculo do raio do menor círculo delimitador.
- `MAX_ITENS`: Capacidade máxima de itens no inventário e tamanho da matriz de adjacências.
- `RAIO_ABERTURA`: Raio da abertura circular da bolsa (10.0 unidades).
- `MAX_PONTOS`: Limite de vértices para o contorno poligonal de cada item (20).

---

## 🧱 Estruturas de Dados

### 1. `Item` (Lista Encadeada e Grafo)
```cpp
struct Item
{
    string nome;
    string dono;
    string propriedadeMagica;
    int id;
    int raridade;
    int semelhanca[MAX_ITENS]; // matriz de adjacências indexada pelo id
    Item* proximo;
};
```
- Armazena os dados de cada item cadastrado.
- `semelhanca[MAX_ITENS]`: Representa a linha do grafo de similaridade do item.
- `proximo`: Ponteiro para encadeamento na lista simples (`inicio` e `fim`).

### 2. `node` (Árvores Binárias de Busca)
```cpp
struct node
{
    Item* data;
    node* left;
    node* right;
};

node* raiz = NULL;           // Árvore 1: Chave = Nome do item (ordem alfabética)
node* raiz_raridade = NULL;  // Árvore 2: Chave = Raridade do item (ordem decrescente)
```
- Os nós armazenam um **ponteiro** para o `Item` da lista, evitando duplicação de dados na memória.

### 3. `ponto` (Geometria Computacional)
```cpp
struct ponto
{
    double x;
    double y;
};
```
- Representa um vértice bidimensional $(x, y)$ do contorno do item.

---

## 📐 Geometria Computacional e Validação de Entrada

| Função | Finalidade |
|---|---|
| `area_triangulo(a, b, c)` | Calcula a área orientada com sinal via determinante para determinar a orientação dos pontos. |
| `lado(a, b, p)` | Retorna se o ponto $p$ está à esquerda ($+1$), à direita ($-1$) ou alinhado ($0$) ao segmento $ab$. |
| `eh_poligono_convexo(contorno, n)` | Valida se os $n$ pontos formam um polígono estritamente convexo. |
| `area_poligono(contorno, n)` | Calcula a área total do polígono convexo por triangulação ("leque"). |
| `menor_circulo_r2(contorno, n)` | Determina o menor círculo que envolve todos os vértices (testando pares e triplas). |
| `item_cabe_na_bolsa()` | Valida o polígono informado pelo usuário e verifica se o raio mínimo $\le$ `RAIO_ABERTURA`. |

---

## 🌳 Manipulação das Árvores Binárias

| Função | Finalidade |
|---|---|
| `comparar_itens(a, b, criterio)` | Compara dois itens por nome (`POR_NOME`, ignorando maiúsculas/minúsculas) ou por raridade (`POR_RARIDADE`). |
| `insert(current, data, criterio)` | Insere recursivamente o item na árvore especificada. |
| `remover_da_arvore(atual, item, criterio)` | Remove o nó correspondente da árvore mantendo sua consistência (substituindo pelo menor da subárvore direita). |
| `destruct(current)` | Libera recursivamente todos os nós da árvore ao encerrar o sistema. |
| `imprimir_em_ordem(atual)` | Percurso *in-order* (esquerda $\to$ nó $\to$ direita): imprime em ordem alfabética. |
| `imprimir_em_ordem_decrescente(atual)` | Percurso invertido (direita $\to$ nó $\to$ esquerda): imprime em ordem decrescente de raridade. |
| `existe_na_arvore(atual, nome)` | Busca binária rápida $O(h)$ pelo nome do item. |
| `imprimir_propriedade(atual, prop)` | Percorre a árvore listando itens que possuem a propriedade mágica. |
| `contar_propriedade(atual, prop)` | Percorre a árvore contabilizando itens com a propriedade mágica. |

---

## 📋 Funções do Menu Principal

| Opção | Função | Descrição | Status |
|:-----:|--------|-----------|:------:|
| 1 | `inserirItem` | Valida contorno geométrico, gera ID livre e insere na lista e nas duas árvores. | ✅ |
| 2 | `cadastrarSimilaridade` | Registra peso de semelhança simétrico entre pares de IDs informados. | ✅ |
| 3 | `buscarItens` | Lista itens que não pertencem a um jogador e têm semelhança $> X$ com um item. | ✅ |
| 4 | `verificarExistencia` | Busca pelo nome na árvore `raiz` via `existe_na_arvore`. | ✅ |
| 5 | `listarAlfabeticamente` | Imprime todos os itens em ordem alfabética via `imprimir_em_ordem`. | ✅ |
| 6 | `listarRaridade` | Imprime itens em ordem decrescente de raridade via `imprimir_em_ordem_decrescente`. | ✅ |
| 7 | `buscarPropriedade` | Imprime todos os itens com uma dada propriedade mágica. | ✅ |
| 8 | `contarPropriedades` | Conta quantos itens possuem uma dada propriedade mágica. | ✅ |
| 9 | `removerItens` | Remove itens com raridade $< R$ de todas as estruturas e libera seus IDs. | ✅ |
| 10 | `liberar_tudo` / Sair | Desaloca todos os nós de ambas as árvores e da lista encadeada e encerra. | ✅ |

---

## 🧹 Gerenciamento de Memória

A função `liberar_tudo()` garante que o programa encerre sem nenhum vazamento de memória (*memory leak*):
1. Chama `destruct(raiz)` para desalocar todos os nós da árvore por nome.
2. Chama `destruct(raiz_raridade)` para desalocar todos os nós da árvore por raridade.
3. Percorre a lista encadeada `inicio` $\to$ `fim` com `delete atual` para desalocar cada estrutura `Item`.
