# Anatomia do Código

Explicação das partes de `main.cpp`, na ordem em que aparecem no arquivo.

## Bibliotecas

```cpp
#include <iostream>
#include <string>

using namespace std;
```

Importam as bibliotecas da linguagem: entrada/saída no terminal (`cin`, `cout`) e o tipo de texto `string`. O projeto não usa nenhuma outra biblioteca (nem `vector`): as estruturas são feitas à mão com ponteiros.

## Constante `MAX_ITENS`

```cpp
const int MAX_ITENS = 100;
```

Quantidade máxima de itens. Ela define o tamanho do vetor de similaridades de cada item, por isso o cadastro é recusado quando o limite é atingido.

## Formatação Visual `criar_texto`

```cpp
void criar_texto(string texto)
{
    const string vermelho = "\033[31m";
    const string reset = "\033[0m";
    cout << endl;
    cout << vermelho << texto << reset << endl;
}
```

- __Objetivo__: Imprime o texto recebido em vermelho.
- `vermelho` = código ANSI que liga a cor vermelha.
- `reset` = código ANSI que volta para a cor padrão do terminal.

## Estruturas de Dados

### `Item` (lista encadeada)

```cpp
struct Item
{
    string nome;
    string dono;
    string propriedadeMagica;
    int id;
    int raridade;
    int semelhanca[MAX_ITENS];
    Item* proximo;
};

Item* inicio = nullptr;
Item* fim = nullptr;
int quantidade_de_items = 0;
```

- Cada item guarda seus dados, o `id` (gerado automaticamente, começando em 0) e a `raridade` (de 0 a 100).
- `semelhanca[]` é a linha do item na matriz de adjacências do grafo: `semelhanca[j]` é o peso da similaridade com o item de `id` j (0 = sem similaridade cadastrada).
- `proximo` liga os itens em uma lista encadeada; `inicio` e `fim` apontam para o primeiro e o último.

### `node` (árvore binária de busca)

```cpp
struct node
{
    Item* data;
    struct node* left;
    struct node* right;
};

node* raiz = NULL;
```

- Cada nó guarda um **ponteiro** para o `Item`, e não uma cópia. Assim, lista, grafo e árvore enxergam o mesmo item.
- A chave da árvore é o nome do item. Nomes menores ficam à esquerda; nomes maiores ou iguais, à direita.

## Funções da Árvore

### `minusculas`

```cpp
string minusculas(string texto)
```

- __Objetivo__: Devolve uma cópia do texto com as letras em minúsculo.
- Necessária porque o C++ compara strings pelo código de cada caractere: `'Z'` (90) vale menos que `'a'` (97), então sem ela "Zebra" viria antes de "adaga".
- Não trata letras acentuadas.

### `insert`

```cpp
void insert(node* & current, Item* data)
```

- __Objetivo__: Insere um item na árvore, de forma recursiva.
- `node*&` é uma **referência para ponteiro**: permite que a função altere o ponteiro original (por exemplo, `raiz` ou `left` de algum nó) quando encontra um lugar vazio.
- Compara os nomes com `minusculas` para ignorar maiúsculas e minúsculas.

### `imprimir_em_ordem`

```cpp
void imprimir_em_ordem(node* atual)
```

- __Objetivo__: Imprime todas as informações dos itens em ordem alfabética.
- Faz o percurso **em ordem**: visita a subárvore esquerda, imprime o nó atual e visita a subárvore direita. Como a chave é o nome, a saída já sai ordenada.

### `existe_na_arvore`

```cpp
bool existe_na_arvore(node* atual, string nome)
```

- __Objetivo__: Diz se existe um item com aquele nome.
- Se o nome é igual ao do nó, achou. Se é menor, continua pela esquerda; se é maior, pela direita. Chegando em `NULL`, não existe.
- Ignora maiúsculas e minúsculas.

## Funções do Grafo e da Lista

### `encontrar_item`

```cpp
void encontrar_item(int id, Item** item_sendo_buscado, int* item_sendo_buscado_index)
```

- __Objetivo__: Procura na lista encadeada o item com o `id` informado.
- Usa **ponteiro para ponteiro** (`Item**`) para devolver o item encontrado (ou `nullptr`) e `int*` para devolver a posição dele na lista.

### `preencher_semelhancas`

```cpp
bool preencher_semelhancas(int id1, int id2)
```

- __Objetivo__: Lê o valor de similaridade entre dois itens e grava nos dois.
- A relação é simétrica (`item1 -> item2` e `item2 -> item1`).
- Devolve `false` se algum item não existir ou se os dois códigos forem iguais.

## Funções de Entrada

### `lerInteiroNoIntervalo`

```cpp
int lerInteiroNoIntervalo(string mensagem, int minimo, int maximo)
```

- __Objetivo__: Lê um número inteiro entre `minimo` e `maximo`, repetindo a pergunta até o usuário digitar um valor válido.
- Usada para ler a raridade (0 a 100).

## Funções do Menu

| Opção | Função | Descrição | Status |
|:-----:|--------|-----------|:------:|
| 1 | `inserirItem` | Lê os dados do item, coloca na lista e na árvore. Recusa se `MAX_ITENS` foi atingido. | ✅ |
| 2 | `cadastrarSimilaridade` | Pede dois códigos e chama `preencher_semelhancas`. | ✅ |
| 3 | `buscarItens` | Lista itens que não são de um jogador e têm similaridade maior que X com um item. | ✅ |
| 4 | `verificarExistencia` | Lê um nome e imprime "Item encontrado!" ou "Item não encontrado!" usando `existe_na_arvore`. | ✅ |
| 5 | `listarAlfabeticamente` | Imprime os itens em ordem alfabética usando `imprimir_em_ordem`. | ✅ |
| 6 | `listarRaridade` | Em construção. | 🚧 |
| 7 | `buscarPropriedade` | Em construção. | 🚧 |
| 8 | `contarPropriedades` | Em construção. | 🚧 |
| 9 | `removerItens` | Em construção. | 🚧 |
| 10 | (sair) | Encerra o programa. | ✅ |

`listarItensCadastrados` é uma função auxiliar: pergunta se o usuário quer ver a lista de itens com seus códigos, e é usada nas opções 2 e 3.

## Controle de Fluxo `esperarEnter`

```cpp
void esperarEnter()
{
    cout << endl;
    cout << "Pressione ENTER para continuar";
    cin.get();
}
```

- __Objetivo:__ Pausa antes de voltar ao menu, para o usuário conseguir ler o resultado.
- Não precisa de `cin.ignore()` aqui porque todas as leituras anteriores já limpam o `Enter` que sobra no buffer.

## Apresentação do Menu `exibirMenu`

```cpp
cout << endl << vermelho << "Escolha uma opcao: " << reset;
```

- __Objetivo:__ Centraliza a impressão das opções na tela, usando `criar_texto`.
- O prompt "Escolha uma opcao" fica vermelho, e o `reset` devolve a cor normal logo depois, para o resto do programa (e o terminal) não ficar vermelho.

## Ciclo de Vida `executarMenu`

```cpp
while (!(cin >> opcao) || opcao < 1 || opcao > 10)
{
    if (cin.eof())
    {
        return;
    }

    cin.clear();
    cin.ignore(10000, '\n');
    cout << "Entrada invalida! Por favor, digite um numero de 1 a 10." << endl;
    cout << "Escolha uma opcao: ";
}
```

- __Objetivo:__ Repete o menu até o usuário escolher a opção 10.
- O `while` valida a entrada: se não for número, ou estiver fora de 1 a 10, limpa o erro do `cin` (`clear`), descarta o que foi digitado (`ignore`) e pergunta de novo.
- Depois de uma opção válida, um `switch` chama a função correspondente e, no fim, `esperarEnter`.

## Função Principal

```cpp
int main()
{
    executarMenu();
    return 0;
}
```