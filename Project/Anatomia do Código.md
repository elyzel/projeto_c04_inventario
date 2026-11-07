# Anatomia do Código — Inventário D&D (Bolsa Devoradora)

Documentação didática do `main.cpp`, atualizada até a **Entrega Parcial 3** e com os ajustes de robustez pedidos pelo professor.

**O que o programa faz:** é um inventário coletivo de itens de D&D. Cada item fica guardado em **dois lugares ao mesmo tempo**:

|Estrutura|Para que serve|
|---|---|
|**Lista encadeada** (`inicio` → `fim`)|Guarda os itens na ordem de cadastro e permite achar um item pelo código.|
|**Árvore binária de busca** (`raiz`)|Guarda os itens ordenados pelo **nome**, para listar em ordem alfabética e buscar rápido.|

A **semelhança** entre itens é guardada dentro de cada item, em um vetor (funciona como as linhas de uma matriz de adjacência de um grafo).

---

## 1. Bibliotecas e constantes

```cpp
#include <iostream>
#include <string>
const int MAX_ITENS = 100;

const int SEMELHANCA_MINIMA = 0;
const int SEMELHANCA_MAXIMA = 10;

using namespace std;
```

**Objetivo:** preparar o programa.

- `<iostream>`: entrada e saída no terminal (`cin`, `cout`).
- `<string>`: o tipo de texto `string` (também usamos `to_string`, `substr` e `length`).
- `MAX_ITENS`: limite de itens (e tamanho do vetor de semelhanças de cada item).
- `SEMELHANCA_MINIMA` / `SEMELHANCA_MAXIMA`: faixa aceita para a semelhança (0 a 10). Para mudar a faixa, basta alterar só estes dois números.
- `using namespace std;`: permite escrever `cout` em vez de `std::cout`.

---

## 2. Formatação visual — `criar_texto`

```cpp
void criar_texto(string texto)
{
    const string vermelho = "\033[31m";
    const string reset = "\033[0m";
    cout << endl;
    cout << vermelho << texto << reset << endl;
}
```

**Objetivo:** imprimir o texto recebido em vermelho.

- `vermelho`: código especial que faz o terminal pintar o texto de vermelho.
- `reset`: devolve o terminal para a cor padrão.

---

## 3. Estrutura do item — `struct Item`

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

**Objetivo:** definir como um item é guardado na memória.

- `nome`, `dono`, `propriedadeMagica`: textos do item.
- `id`: código do item (0, 1, 2…), gerado automaticamente na ordem de cadastro.
- `raridade`: número de 0 (comum) a 100 (raríssimo).
- `semelhanca[]`: na posição `j` fica a semelhança deste item com o item de índice `j`.
- `proximo`: ponteiro para o próximo item da lista encadeada (`nullptr` = não há próximo).
- `inicio` / `fim`: primeiro e último item da lista. `quantidade_de_items`: quantos itens existem.

---

## 4. Validação de entradas

### 4.1 `lerInteiroNoIntervalo`

```cpp
int lerInteiroNoIntervalo(string mensagem, int minimo, int maximo)
{
    int valor;
    cout << mensagem;

    while (!(cin >> valor) || valor < minimo || valor > maximo)
    {
        if (cin.eof())
        {
            return minimo;
        }

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor inválido!" << endl << "Digite um número de " << minimo << " a " << maximo << ": ";
    }

    cin.ignore(10000, '\n');
    return valor;
}
```

**Objetivo:** ler um número inteiro e **só aceitar** valores entre `minimo` e `maximo`.

- `!(cin >> valor)` é verdadeiro quando o usuário digita algo que não é número (por exemplo `abc`).
- `cin.clear()` tira o `cin` do estado de erro; `cin.ignore(10000, '\n')` descarta o que foi digitado errado.
- O `while` repete até o valor ser válido.
- `cin.eof()` evita laço infinito se a entrada acabar.
- O `cin.ignore` final limpa o ENTER que sobrou, para o próximo `getline` não ler uma linha vazia.

**Onde é usada agora:** raridade, valor da semelhança, os códigos dos itens e o valor mínimo da busca.

### 4.2 `trim`

```cpp
string trim(string texto)
{
    int comeco = 0;
    int final = (int)texto.length() - 1;

    while (comeco <= final && (texto[comeco] == ' ' || texto[comeco] == '\t' || ...))
    {
        comeco++;
    }

    while (final >= comeco && (texto[final] == ' ' || texto[final] == '\t' || ...))
    {
        final--;
    }

    return texto.substr(comeco, final - comeco + 1);
}
```

**Objetivo:** remover espaços do **começo e do fim** do texto. `" Espada "` vira `"Espada"`.

- O primeiro `while` avança `comeco` enquanto houver espaço no início.
- O segundo recua `final` enquanto houver espaço no fim.
- `substr` devolve só o pedaço que sobrou. Se o texto só tinha espaços, devolve `""`.

### 4.3 `lerTextoNaoVazio`

```cpp
string lerTextoNaoVazio(string mensagem)
{
    string texto;
    cout << mensagem;

    while (true)
    {
        getline(cin, texto);

        if (cin.eof())
        {
            return "";
        }

        texto = trim(texto);

        if (texto != "")
        {
            return texto;
        }

        cout << "O campo não pode ficar vazio!" << endl << mensagem;
    }
}
```

**Objetivo:** ler uma linha de texto (pode ter espaços, como "Espada Longa"), apagar os espaços das pontas e **não aceitar campo vazio**.

**Onde é usada:** nome, dono e propriedade mágica (cadastro), nome do jogador (busca) e nome do item (opção 4).

> Como o nome é salvo já sem espaços nas pontas, `"Espada "` e `"Espada"` passam a ser o mesmo item na árvore.

---

## 5. Árvore binária de busca (chave = nome)

### 5.1 Estrutura e `converter_para_minusculas`

```cpp
struct node
{
    Item* data;
    struct node* left;
    struct node* right;
};

node* raiz = NULL;

string converter_para_minusculas(string texto)
{
    for (int i = 0; i < (int)texto.length(); i++)
    {
        if (texto[i] >= 'A' && texto[i] <= 'Z')
        {
            texto[i] = texto[i] + ('a' - 'A');
        }
    }
    return texto;
}
```

**Objetivo:**

- `node`: um nó da árvore. `data` aponta para o item (não copia), `left` guarda nomes menores e `right` guarda nomes maiores ou iguais.
- `raiz`: topo da árvore (`NULL` = árvore vazia).
- `converter_para_minusculas`: transforma `A–Z` em `a–z`. É usada para que **"Espada" e "espada" sejam considerados iguais** nas comparações.

### 5.2 `insert`

```cpp
void insert(node* & current, Item* data)
{
    if (current == NULL)
    {
        current = new node;
        current->data = data;
        current->left = NULL;
        current->right = NULL;
    }
    else if (converter_para_minusculas(data->nome) < converter_para_minusculas(current->data->nome))
    {
        insert(current->left, data);
    }
    else
    {
        insert(current->right, data);
    }
}
```

**Objetivo:** inserir um item na árvore, mantendo-a ordenada pelo nome.

- Se o lugar está vazio (`NULL`), cria o nó ali.
- Se o nome é menor que o do nó atual, desce pela **esquerda**; senão, desce pela **direita**.
- `node* &` (referência a ponteiro) permite que a função altere o ponteiro original, ligando o novo nó ao pai.
- É **recursiva**: chama a si mesma até achar um lugar vazio.

### 5.3 `imprimir_em_ordem`

```cpp
void imprimir_em_ordem(node* atual)
{
    if (atual == NULL)
    {
        return;
    }

    imprimir_em_ordem(atual->left);

    cout << "Código: " << atual->data->id
        << " | Nome: " << atual->data->nome
        << " | Dono: " << atual->data->dono
        << " | Propriedade mágica: " << atual->data->propriedadeMagica
        << " | Raridade: " << atual->data->raridade << endl;

    imprimir_em_ordem(atual->right);
}
```

**Objetivo:** imprimir todos os itens em **ordem alfabética**.

Percurso **em ordem** (esquerda → nó → direita): primeiro todos os nomes menores, depois o nó atual, depois os maiores. Como a árvore é ordenada pelo nome, a saída já sai alfabética, sem precisar de ordenação extra.

### 5.4 `existe_na_arvore`

```cpp
bool existe_na_arvore(node* atual, string nome)
{
    if (atual == NULL)
    {
        return false;
    }

    string nome_buscado = converter_para_minusculas(nome);
    string nome_do_no = converter_para_minusculas(atual->data->nome);

    if (nome_buscado == nome_do_no)
    {
        return true;
    }

    if (nome_buscado < nome_do_no)
    {
        return existe_na_arvore(atual->left, nome);
    }

    return existe_na_arvore(atual->right, nome);
}
```

**Objetivo:** dizer se existe um item com aquele nome (ignorando maiúsculas/minúsculas).

- Chegou em `NULL`: não achou → `false`.
- Nomes iguais: achou → `true`.
- Senão, desce para a esquerda (se o buscado é menor) ou para a direita (se é maior). A cada passo descarta metade dos itens, por isso a busca é rápida.

---

## 6. Funções de apoio da lista e do grafo

### 6.1 `encontrar_item`

```cpp
void encontrar_item(int id, Item** item_sendo_buscado, int* item_sendo_buscado_index)
{
    *item_sendo_buscado = inicio;
    *item_sendo_buscado_index = 0;

    while (*item_sendo_buscado != nullptr && (*(*item_sendo_buscado)).id != id)
    {
        *item_sendo_buscado = (*(*item_sendo_buscado)).proximo;
        (*item_sendo_buscado_index)++;
    }
}
```

**Objetivo:** percorrer a lista encadeada procurando o item com o `id` informado.

- Devolve **dois resultados** pelos parâmetros: o ponteiro do item (`nullptr` se não achou) e a posição dele na lista.
- `Item**` é um ponteiro para ponteiro: serve para a função poder **mudar** o ponteiro que foi passado a ela.

### 6.2 `preencher_semelhancas`

```cpp
bool preencher_semelhancas(int id1, int id2)
{
    // ... busca os dois itens com encontrar_item ...

    if (item1 == nullptr || item2 == nullptr) { /* "Item não encontrado" */ return false; }
    if ((*item2).id == (*item1).id)           { /* "não pode ter semelhança com ele mesmo" */ return false; }

    string mensagem = "Digite a semelhança entre o item 1 (" + (*item1).nome + ") e o item 2 ("
        + (*item2).nome + ") [" + to_string(SEMELHANCA_MINIMA) + " a " + to_string(SEMELHANCA_MAXIMA) + "]: ";

    int valor = lerInteiroNoIntervalo(mensagem, SEMELHANCA_MINIMA, SEMELHANCA_MAXIMA);

    (*item2).semelhanca[item1_index] = valor;
    (*item1).semelhanca[item2_index] = valor;

    return true;
}
```

**Objetivo:** registrar a semelhança (aresta com peso) entre dois itens.

- Confere se os dois itens existem e se não são o mesmo.
- Lê o valor **de 0 a 10**; letras ou números fora da faixa são recusados.
- Grava o valor **nos dois itens** (A→B e B→A), pois a semelhança vale nos dois sentidos (grafo não direcionado).

---

## 7. Funcionalidades do menu

### 7.1 `inserirItem` (opção 1)

```cpp
void inserirItem()
{
    if (quantidade_de_items >= MAX_ITENS) { /* mensagem de limite */ return; }

    Item* novoItem = new Item;

    for (int i = 0; i < MAX_ITENS; i++)
    {
        (*novoItem).semelhanca[i] = 0;
    }

    (*novoItem).nome = lerTextoNaoVazio("Digite o nome do item: ");
    (*novoItem).dono = lerTextoNaoVazio("Digite o dono do item: ");
    (*novoItem).propriedadeMagica = lerTextoNaoVazio("Digite a propriedade mágica do item: ");
    (*novoItem).raridade = lerInteiroNoIntervalo("Digite a raridade do item (0 a 100): ", 0, 100);
    (*novoItem).proximo = nullptr;

    // encaixa no fim da lista encadeada
    // define id = quantidade_de_items e soma 1
    insert(raiz, novoItem); // guarda na arvore
}
```

**Objetivo:** cadastrar um novo item.

1. Verifica o limite de 100 itens.
2. Cria o item com `new` e zera todas as semelhanças.
3. Lê os campos de texto (sem aceitar vazio e sem espaços nas pontas) e a raridade (0 a 100).
4. Coloca o item no **fim da lista encadeada** e define o `id`.
5. Insere o **mesmo item** na árvore (`insert(raiz, novoItem)`). Atenção: o item não é copiado, os dois guardam o mesmo endereço.
6. Mostra o código do item cadastrado.

### 7.2 `listarItensCadastrados` (auxiliar)

```cpp
void listarItensCadastrados()
{
    char resposta = 'n';

    cout << "Deseja ver a lista de itens cadastrados? (s/n): ";
    cin >> resposta;
    cin.ignore(10000, '\n');

    if (resposta == 's' || resposta == 'S')
    {
        // percorre a lista encadeada imprimindo código, nome e dono
    }
}
```

**Objetivo:** ajudar o usuário a descobrir o código dos itens antes de pedir um código.

- `char resposta = 'n';` começa com valor definido: se a leitura falhar, assume "não" em vez de usar lixo de memória.

### 7.3 `cadastrarSimilaridade` (opção 2)

```cpp
void cadastrarSimilaridade()
{
    int id1, id2;

    if (inicio == nullptr) { /* "Nenhum item cadastrado." */ return; }

    listarItensCadastrados();

    id1 = lerInteiroNoIntervalo("Informe o código do item 1: ", 0, quantidade_de_items - 1);
    id2 = lerInteiroNoIntervalo("Informe o código do item 2: ", 0, quantidade_de_items - 1);

    bool preencheu_semelhanca = preencher_semelhancas(id1, id2);
    // mostra "Semelhança preenchida" ou "Não foi possível preencher semelhança"
}
```

**Objetivo:** ligar dois itens com um valor de semelhança.

Os códigos só são aceitos entre `0` e `quantidade_de_items - 1`, que são exatamente os códigos que existem. Depois a função chama `preencher_semelhancas`.

### 7.4 `buscarItens` (opção 3)

```cpp
jogador = lerTextoNaoVazio("Digite o nome do jogador: ");

valor = lerInteiroNoIntervalo("Digite o valor mínimo de similaridade (0 a 10): ", SEMELHANCA_MINIMA, SEMELHANCA_MAXIMA);

listarItensCadastrados();

codigo = lerInteiroNoIntervalo("Digite o código do item que deseja buscar semelhança: ", 0, quantidade_de_items - 1);

// ... acha itemC (o item de código "codigo") ...

string jogador_minusculo = converter_para_minusculas(jogador);

while (atual != nullptr)
{
    if ((*atual).id != (*itemC).id
        && converter_para_minusculas((*atual).dono) != jogador_minusculo
        && (*itemC).semelhanca[(*atual).id] >= valor)
    {
        // imprime nome, dono e semelhança
    }
    atual = (*atual).proximo;
}
```

**Objetivo:** listar os itens que **não são do jogador J** e que têm semelhança **mínima X** com o item de código C.

Um item aparece na lista se cumprir as três condições:

1. não é o próprio item C;
2. o dono é diferente de J, **ignorando maiúsculas/minúsculas** (`Daniel` = `daniel`);
3. a semelhança com C é **maior ou igual (`>=`)** ao valor mínimo.

O critério é o mesmo no pedido ("valor mínimo"), na mensagem do resultado ("semelhança mínima de X") e no código (`>=`).

> Atenção: pares de itens sem semelhança cadastrada têm valor 0. Por isso, se o valor mínimo digitado for 0, todos esses itens também aparecem.

### 7.5 `verificarExistencia` (opção 4)

```cpp
void verificarExistencia()
{
    string nome = lerTextoNaoVazio("Digite o nome do item que deseja procurar: ");

    if (existe_na_arvore(raiz, nome))
    {
        cout << "Item encontrado!" << endl;
    }
    else
    {
        cout << "Item não encontrado!" << endl;
    }
}
```

**Objetivo:** perguntar um nome e usar a árvore para dizer se o item existe. Os espaços das pontas são removidos antes da busca.

### 7.6 `listarAlfabeticamente` (opção 5)

```cpp
void listarAlfabeticamente()
{
    if (raiz == NULL) { /* "Nenhum item cadastrado." */ return; }

    imprimir_em_ordem(raiz);
}
```

**Objetivo:** mostrar todas as informações dos itens em ordem alfabética, usando o percurso em ordem da árvore.

### 7.7 Funções ainda em construção (opções 6, 7, 8 e 9)

```cpp
void listarRaridade()       { cout << "Função Listar por Raridade em construção."; }
void buscarPropriedade()    { cout << "Função Buscar por Propriedade em construção."; }
void contarPropriedades()   { cout << "Função Contar Propriedades em construção."; }
void removerItens()         { cout << "Função Remover Itens em construção."; }
```

**Objetivo:** reservar o lugar das funcionalidades das próximas entregas (raridade decrescente, propriedade mágica e remoção). Já estão ligadas ao menu.

---

## 8. Controle de fluxo — `esperarEnter`

```cpp
void esperarEnter()
{
    cout << endl;
    cout << "Pressione ENTER para continuar";
    cin.get();
}
```

**Objetivo:** pausar até o usuário apertar ENTER, para ele conseguir ler o resultado antes de o menu aparecer de novo. Não precisa de `cin.ignore()` antes porque todas as leituras já limpam o ENTER que sobra.

---

## 9. Apresentação do menu — `exibirMenu`

```cpp
void exibirMenu()
{
    string vermelho = "\033[31m";
    string reset = "\033[0m";

    cout << endl;

    criar_texto("==========================================");
    criar_texto("             INVENTÁRIO D&D               ");
    criar_texto("==========================================");
    criar_texto("");
    criar_texto(" 1 > Inserir item");
    // ... opções 2 a 9 ...
    criar_texto(" 10 > Sair");

    cout << endl << vermelho << "Escolha uma opção: " << reset;
}
```

**Objetivo:** centralizar a impressão do título e das opções. Só o prompt "Escolha uma opção" fica vermelho; o `reset` devolve a cor normal logo depois.

---

## 10. Ciclo de vida — `executarMenu`

```cpp
void executarMenu()
{
    int opcao;

    while (true)
    {
        exibirMenu();

        while (!(cin >> opcao) || opcao < 1 || opcao > 10)
        {
            if (cin.eof()) { return; }

            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada inválida! Por favor, digite um número de 1 a 10." << endl;
            cout << "Escolha uma opção: ";
        }

        cin.ignore(10000, '\n');

        switch (opcao)
        {
        case 1:  inserirItem();           break;
        case 2:  cadastrarSimilaridade(); break;
        case 3:  buscarItens();           break;
        case 4:  verificarExistencia();   break;
        case 5:  listarAlfabeticamente(); break;
        case 6:  listarRaridade();        break;
        case 7:  buscarPropriedade();     break;
        case 8:  contarPropriedades();    break;
        case 9:  removerItens();          break;
        case 10: cout << "Encerrando programa..." << endl; return;
        default: cout << endl << "Opção inválida!\n";
        }

        esperarEnter();
        cout << endl;
    }
}
```

**Objetivo:** manter o programa rodando até o usuário escolher **10 > Sair**.

- O `while (true)` repete: mostra o menu, lê a opção, executa, espera ENTER.
- O `while` interno só aceita números de 1 a 10; letras ou números fora da faixa pedem nova digitação.
- O `switch` chama a função certa para cada opção. O `esperarEnter()` aparece uma só vez, depois do `switch`, em vez de repetido em cada `case`.
- `return` na opção 10 sai da função e o programa termina.

---

## 11. Ponto de entrada — `main`

```cpp
int main()
{
    executarMenu();
    return 0;
}
```

**Objetivo:** iniciar o programa chamando o menu. `return 0` indica que terminou sem erros.

---

## 12. Resumo das mudanças desta versão

|Ajuste|Onde|O que mudou|
|---|---|---|
|`cin >>` sem validação|`preencher_semelhancas`, `cadastrarSimilaridade`, `buscarItens`|Passou a usar `lerInteiroNoIntervalo`. Códigos aceitam de `0` a `quantidade_de_items - 1`.|
|Campos vazios e espaços no fim|nome, dono, propriedade, jogador e busca da opção 4|Novas funções `trim` e `lerTextoNaoVazio`.|
|`char resposta;` sem valor|`listarItensCadastrados`|Agora `char resposta = 'n';`.|
|"Valor mínimo" × "maior que"|`buscarItens`|Critério único: **mínimo**, com `>=` no código e nas mensagens.|
|Dono com caixa diferente|`buscarItens`|Compara com `converter_para_minusculas` dos dois lados.|
|Semelhança sem faixa|`preencher_semelhancas`|Limitada de **0 a 10** (constantes no topo do arquivo).|
|Acentos que sobraram|`exibirMenu`, `inserirItem`|"Escolha uma opção", "Código do item", "INVENTÁRIO D&D".|

---

## 13. Situação por entrega

|Entrega|Requisito|Situação|
|---|---|---|
|Parcial 1|Menu e funções em construção|Feito|
|Parcial 2|Inserir itens, similaridade (grafo), listar itens de outros jogadores com semelhança ≥ X|Feito|
|Parcial 3|Inserir na árvore, imprimir em ordem alfabética, verificar existência pelo nome|Feito|
|Parcial 4|Raridade decrescente, contar propriedade mágica, remover por raridade, polígono convexo|A fazer|