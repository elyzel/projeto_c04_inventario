# Anatomia do Código — Inventário D&D (Bolsa Devoradora)

Documentação didática do `main.cpp`, atualizada até a **Entrega Parcial 4** (todas as funcionalidades implementadas, incluindo o contorno poligonal dos itens).

**O que o programa faz:** é um inventário coletivo de itens de D&D. Cada item fica guardado em **três lugares ao mesmo tempo** (sempre o mesmo `Item`, nunca uma cópia):

|Estrutura|Para que serve|
|---|---|
|**Lista encadeada** (`inicio` → `fim`)|Guarda os itens na ordem de cadastro e permite achar um item pelo código.|
|**Árvore binária de busca por nome** (`raiz`)|Lista em ordem alfabética, busca por nome e percurso para propriedade mágica.|
|**Árvore binária de busca por raridade** (`raiz_raridade`)|Lista em ordem decrescente de raridade.|

A **semelhança** entre itens é guardada dentro de cada item, em um vetor indexado pelo **id** do outro item (funciona como as linhas de uma matriz de adjacência de um grafo não direcionado).

Antes de entrar em qualquer uma dessas estruturas, o item passa por um filtro geométrico: o contorno dele precisa ser um **polígono convexo** e precisa **passar pela abertura da bolsa**.

---

## 1. Bibliotecas e constantes

```cpp
#include <iostream>
#include <string>
#include <cmath>
const int MAX_ITENS = 100;

const int SEMELHANCA_MINIMA = 0;
const int SEMELHANCA_MAXIMA = 10;

const double RAIO_ABERTURA = 10.0;
const double PI = 3.14159265358979;

const int CRITERIO_CIRCULO = 0;
const int CRITERIO_AREA = 1;
const int CRITERIO_ABERTURA = CRITERIO_CIRCULO;

const int MAX_PONTOS = 20;

using namespace std;
```

**Objetivo:** preparar o programa.

- `<iostream>`: entrada e saída no terminal (`cin`, `cout`).
- `<string>`: o tipo `string` (também usamos `to_string`, `substr` e `length`).
- `<cmath>`: só para `sqrt`, usado no raio do menor círculo.
- `MAX_ITENS`: limite de itens. Também é o tamanho do vetor de semelhanças de cada item e o limite dos ids (de `0` a `MAX_ITENS - 1`).
- `SEMELHANCA_MINIMA` / `SEMELHANCA_MAXIMA`: faixa aceita para a semelhança (0 a 10). Para mudar a faixa, basta alterar só estes dois números.
- `RAIO_ABERTURA`: raio da abertura da bolsa, na mesma unidade dos pontos do contorno. **O enunciado não define o tamanho nem o formato da abertura**, então o valor 10 é uma hipótese a confirmar com o professor ou o monitor.
- `PI`: usado só no critério por área.
- `CRITERIO_ABERTURA`: escolhe como decidir se o item passa pela abertura. Basta trocar `CRITERIO_CIRCULO` por `CRITERIO_AREA` (veja a seção 6).
- `MAX_PONTOS`: quantidade máxima de vértices do contorno de um item.
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

## 3. Estrutura do item e controle de ids

```cpp
struct Item
{
    string nome;
    string dono;
    string propriedadeMagica;
    int id;
    int raridade;
    int semelhanca[MAX_ITENS]; // semelhanca[j] = peso da semelhanca com o item de id j
    Item* proximo;
};

Item* inicio = nullptr;
Item* fim = nullptr;
int quantidade_de_items = 0;

bool id_em_uso[MAX_ITENS];

int obter_menor_id_livre()
{
    int i = 0;

    while (i < MAX_ITENS && id_em_uso[i])
    {
        i++;
    }

    return i;
}
```

**Objetivo:** definir como um item é guardado na memória e como os códigos (ids) são distribuídos.

- `nome`, `dono`, `propriedadeMagica`: textos do item.
- `id`: código do item, gerado automaticamente. É sempre o **menor id livre**.
- `raridade`: número de 0 (comum) a 100 (raríssimo).
- `semelhanca[]`: na posição `j` fica a semelhança deste item com o item de **id** `j`. Não é a posição na lista, porque depois de remover itens a posição deixa de ser igual ao id.
- `proximo`: ponteiro para o próximo item da lista encadeada (`nullptr` = não há próximo).
- `inicio` / `fim`: primeiro e último item da lista. `quantidade_de_items`: quantos itens existem.
- `id_em_uso[i]`: é `true` quando existe um item com id `i`. Variável global, então começa toda em `false`.
- `obter_menor_id_livre`: procura o primeiro id não usado. Por isso, o id de um item **removido pode ser reaproveitado** por um item novo.

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

**Onde é usada:** raridade, quantidade de pontos do contorno, valor da semelhança, códigos dos itens, quantidade de pares, valor X da busca e limite de raridade da remoção.

### 4.2 `lerNumero`

```cpp
double lerNumero(string mensagem)
{
    double valor;
    cout << mensagem;

    while (!(cin >> valor))
    {
        if (cin.eof())
        {
            return 0;
        }

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor inválido!" << endl << mensagem;
    }

    cin.ignore(10000, '\n');
    return valor;
}
```

**Objetivo:** ler um número **real** (inteiro ou decimal com ponto) para as coordenadas do contorno. Não tem faixa, porque as coordenadas podem ser negativas. Atenção: `3,5` com vírgula é lido como `3`, porque o `cin` para na vírgula.

### 4.3 `trim`

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

- O primeiro `while` avança `comeco` enquanto houver espaço, tab ou quebra de linha no início.
- O segundo recua `final` enquanto houver no fim.
- `substr` devolve só o pedaço que sobrou. Se o texto só tinha espaços, devolve `""`.

### 4.4 `lerTextoNaoVazio`

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

**Onde é usada:** nome, dono e propriedade mágica (cadastro), nome do jogador (busca), nome do item (opção 4) e propriedade mágica (opções 7 e 8).

> Como o nome é salvo já sem espaços nas pontas, `"Espada "` e `"Espada"` passam a ser o mesmo item na árvore.

### 4.5 `converter_para_minusculas`

```cpp
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

**Objetivo:** transformar `A–Z` em `a–z`. É usada para que **"Espada" e "espada" sejam considerados iguais** nas comparações (nome, dono e propriedade mágica).

> Limitação: letras acentuadas (como `É`) não são convertidas. Na ordem alfabética, um nome começando com letra acentuada pode ficar fora do lugar esperado.

---

## 5. Geometria

Ideias da aula de Algoritmos Geométricos e Fecho Convexo. O contorno do item é uma lista de pontos que precisa formar um **polígono convexo** (entrega 11).

### 5.1 `struct ponto`, `valor_absoluto`, `area_triangulo` e `lado`

```cpp
struct ponto
{
    double x;
    double y;
};

double area_triangulo(ponto a, ponto b, ponto c)
{
    return (
        a.x * b.y - a.y * b.x +
        a.y * c.x - a.x * c.y +
        b.x * c.y - b.y * c.x
    ) / 2.0;
}

int lado(ponto a, ponto b, ponto p)
{
    double s = area_triangulo(a, b, p);

    if (s > 0) { return 1; }
    else if (s < 0) { return -1; }

    return 0;
}
```

**Objetivo:** as peças básicas, iguais às de `geometrico.cpp` (aula), mas com `double`.

- `ponto`: um ponto do plano. Usa `double` para aceitar coordenadas decimais.
- `valor_absoluto`: devolve o número sem sinal (evita usar `<cmath>` para isso).
- `area_triangulo`: área **com sinal** do triângulo `abc`. Positiva se `a → b → c` está em sentido anti-horário, negativa se horário e zero se os pontos estão alinhados.
- `lado`: devolve `1` se `p` está à esquerda de `ab`, `-1` se está à direita e `0` se está alinhado.

### 5.2 `eh_poligono_convexo`

```cpp
bool eh_poligono_convexo(ponto contorno[], int n)
{
    int sentido = 0; // 0 = ainda nao sei, 1 = esquerda, -1 = direita

    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;

        if (contorno[i].x == contorno[j].x && contorno[i].y == contorno[j].y)
        {
            return false; // dois pontos seguidos iguais
        }

        for (int k = 0; k < n; k++)
        {
            if (k == i || k == j) { continue; }

            int l = lado(contorno[i], contorno[j], contorno[k]);

            if (l == 0) { continue; }

            if (sentido == 0) { sentido = l; }
            else if (l != sentido) { return false; }
        }
    }

    return sentido != 0;
}
```

**Objetivo:** dizer se os pontos, **dados em ordem**, formam um polígono convexo.

- Para cada lado `i → j`, olha todos os outros pontos `k`. Num polígono convexo, todos ficam do **mesmo lado** de cada aresta, e esse lado é o mesmo para todas as arestas.
- O primeiro ponto não alinhado define o `sentido`. Se qualquer outro ponto aparecer do lado oposto, não é convexo.
- Pontos alinhados com a aresta (`l == 0`) não decidem nada.
- Dois vértices seguidos iguais são recusados.
- `sentido == 0` no final significa que todos os pontos estão alinhados (não é um polígono).
- Ordem errada também é recusada: por exemplo, o quadrado informado como `(0,0) (2,2) (0,2) (2,0)` forma uma "gravata", e isso reprova.

### 5.3 `area_poligono`

```cpp
double area_poligono(ponto contorno[], int n)
{
    double soma = 0;

    for (int i = 1; i < n - 1; i++)
    {
        soma += area_triangulo(contorno[0], contorno[i], contorno[i + 1]);
    }

    return valor_absoluto(soma);
}
```

**Objetivo:** calcular a área do polígono convexo.

Divide o polígono em triângulos a partir do primeiro ponto (um "leque") e soma a área de cada um. O `valor_absoluto` final existe porque o sinal só indica se a ordem foi horária ou anti-horária.

### 5.4 `todos_dentro`

```cpp
bool todos_dentro(ponto contorno[], int n, double cx, double cy, double r2)
{
    for (int i = 0; i < n; i++)
    {
        double dx = contorno[i].x - cx;
        double dy = contorno[i].y - cy;

        if (dx * dx + dy * dy > r2 * (1 + 1e-9) + 1e-9)
        {
            return false;
        }
    }
    return true;
}
```

**Objetivo:** dizer se todos os pontos estão dentro (ou na borda) do círculo de centro `(cx, cy)` e raio ao quadrado `r2`. A pequena folga (`1e-9`) evita erro de arredondamento com `double`.

### 5.5 `menor_circulo_r2`

**Objetivo:** devolver o **raio ao quadrado** do menor círculo que contém todos os pontos.

- O menor círculo sempre passa por **2 pontos** (extremos de um diâmetro) ou por **3 pontos**.
- A função testa **todas as duplas** e **todas as triplas** e fica com o menor círculo que contém tudo (`todos_dentro`).
- Para 3 pontos, calcula o centro da circunferência que passa pelos três. Se estiverem alinhados (`d` perto de 0), pula.
- Devolve `-1` se não achar nenhum círculo (só acontece com pontos alinhados).
- É força bruta, mas com no máximo 20 pontos (`MAX_PONTOS`) o custo é pequeno.

> Por que o menor círculo? Se um item cabe em algum círculo de raio R (podendo girar), ele cabe no menor círculo que o contém. Girar o item não muda o raio desse círculo.

---

## 6. A abertura da bolsa — `item_cabe_na_bolsa`

```cpp
bool item_cabe_na_bolsa()
{
    ponto contorno[MAX_PONTOS];

    // lê n (3 a MAX_PONTOS) e os n pontos com lerNumero

    if (!eh_poligono_convexo(contorno, n)) { /* "Contorno inválido..." */ return false; }

    double area = area_poligono(contorno, n);

    if (CRITERIO_ABERTURA == CRITERIO_AREA)
    {
        // passa se area <= PI * RAIO_ABERTURA * RAIO_ABERTURA
    }

    double r2 = menor_circulo_r2(contorno, n);
    // se r2 < 0: "Contorno inválido"
    double raio = sqrt(r2);
    // passa se raio <= RAIO_ABERTURA
}
```

**Objetivo:** ler o contorno do item e decidir se ele pode entrar na bolsa. Devolve `true` só se o contorno for um polígono convexo válido **e** passar pela abertura.

Passos:

1. Lê a quantidade de pontos (3 a 20) e as coordenadas `x` e `y` de cada um.
2. Se não for convexo, recusa e manda conferir a ordem dos pontos.
3. Calcula a área do polígono.
4. Decide se passa pela abertura conforme `CRITERIO_ABERTURA`:

|Critério|Regra|Observação|
|---|---|---|
|`CRITERIO_CIRCULO` (padrão)|Passa se o raio do menor círculo que contém o item é ≤ `RAIO_ABERTURA`|Considera o item girando. Um item comprido e fino só passa se o comprimento couber no diâmetro.|
|`CRITERIO_AREA`|Passa se a área do polígono é ≤ `PI * RAIO_ABERTURA²`|Mais simples, mas deixa passar itens compridos e finos que na prática não cabem.|

A comparação usa uma folga de `1e-9` para absorver erro de arredondamento.

---

## 7. Árvores binárias de busca (nome e raridade)

### 7.1 Estrutura e critérios

```cpp
struct node
{
    Item* data;
    struct node* left;
    struct node* right;
};

node* raiz = NULL;           // chave = nome
node* raiz_raridade = NULL;  // chave = raridade

const int POR_NOME = 0;
const int POR_RARIDADE = 1;
```

**Objetivo:** guardar os itens ordenados por dois critérios diferentes.

- `node`: um nó da árvore. `data` aponta para o item (não copia), `left` guarda os menores e `right` guarda os maiores **ou iguais**.
- `raiz` e `raiz_raridade`: topos das duas árvores (`NULL` = árvore vazia).
- `POR_NOME` / `POR_RARIDADE`: dizem por qual campo a árvore compara os itens. A mesma função de inserção e de remoção serve para as duas.
- A propriedade mágica **não tem árvore própria**: para listar ou contar por propriedade, percorremos a árvore dos nomes inteira.

### 7.2 `comparar_itens`

```cpp
int comparar_itens(Item* a, Item* b, int criterio)
{
    if (criterio == POR_RARIDADE)
    {
        // compara a->raridade com b->raridade
        // devolve -1, 1 ou 0
    }

    // POR_NOME: compara os nomes em minúsculas
    string texto_a = converter_para_minusculas(a->nome);
    string texto_b = converter_para_minusculas(b->nome);
    // devolve -1, 1 ou 0
}
```

**Objetivo:** comparar dois itens pelo critério escolhido. Devolve negativo se `a` vem antes de `b`, `0` se são iguais nesse critério e positivo se `a` vem depois. Nomes são comparados ignorando maiúsculas/minúsculas.

### 7.3 `insert`

```cpp
void insert(node* & current, Item* data, int criterio)
{
    if (current == NULL)
    {
        current = new node;
        current->data = data;
        current->left = NULL;
        current->right = NULL;
    }
    else if (comparar_itens(data, current->data, criterio) < 0)
    {
        insert(current->left, data, criterio);
    }
    else
    {
        insert(current->right, data, criterio);
    }
}
```

**Objetivo:** inserir um item na árvore, mantendo-a ordenada pelo critério escolhido.

- Se o lugar está vazio (`NULL`), cria o nó ali.
- Se o item é menor que o do nó atual, desce pela **esquerda**; se for igual ou maior, desce pela **direita**. Por isso a árvore aceita chaves repetidas (nomes iguais, raridades iguais).
- `node* &` (referência a ponteiro) permite alterar o ponteiro original, ligando o novo nó ao pai.
- É **recursiva**: chama a si mesma até achar um lugar vazio.

### 7.4 `find_less_save_right` e `remover_da_arvore`

```cpp
node* find_less_save_right(node* & current)
{
    if (current->left != NULL)
    {
        return find_less_save_right(current->left);
    }

    node* copy = current;
    current = current->right;
    return copy;
}

void remover_da_arvore(node* & atual, Item* item, int criterio)
{
    if (atual == NULL) { return; }

    if (atual->data == item)
    {
        node* temp = atual;

        if (atual->right == NULL)      { atual = atual->left; }
        else if (atual->left == NULL)  { atual = atual->right; }
        else
        {
            temp = find_less_save_right(atual->right);
            atual->data = temp->data;
        }

        delete temp;
    }
    else if (comparar_itens(item, atual->data, criterio) < 0)
    {
        remover_da_arvore(atual->left, item, criterio);
    }
    else
    {
        remover_da_arvore(atual->right, item, criterio);
    }
}
```

**Objetivo:** remover da árvore o nó que guarda **exatamente aquele item** (o `Item` em si **não** é apagado aqui).

- Baseada em `find_less_save_right` e `remove` de `abb.cpp` (aula).
- Diferença para o `remove` da aula: como a árvore aceita chaves repetidas, não basta parar no primeiro nó com a mesma chave. A função compara o **ponteiro** (`atual->data == item`) e continua descendo até achar o nó certo.
- Os três casos são os mesmos da aula:
  1. sem filho direito: o filho esquerdo (ou `NULL`) ocupa o lugar do nó;
  2. sem filho esquerdo: o filho direito ocupa o lugar do nó;
  3. com dois filhos: o nó recebe o item do **menor nó da subárvore direita**, e esse nó é apagado.
- `find_less_save_right` desce até o menor nó da subárvore (o mais à esquerda), liga o pai dele ao filho direito e devolve o nó solto.

### 7.5 `destruct`

```cpp
void destruct(node* & current)
{
    if (current != NULL)
    {
        destruct(current->left);
        destruct(current->right);
        delete current;
        current = NULL;
    }
}
```

**Objetivo:** apagar todos os nós da árvore (os `Item` não são apagados aqui). Igual ao `destruct` de `abb.cpp`.

### 7.6 `imprimir_item`

```cpp
void imprimir_item(Item* item)
{
    cout << "Código: " << item->id
        << " | Nome: " << item->nome
        << " | Dono: " << item->dono
        << " | Propriedade mágica: " << item->propriedadeMagica
        << " | Raridade: " << item->raridade << endl;
}
```

**Objetivo:** imprimir todas as informações de um item em uma linha. É reaproveitada pelas listagens e pela remoção.

### 7.7 `imprimir_em_ordem` (alfabética)

```cpp
void imprimir_em_ordem(node* atual)
{
    if (atual == NULL) { return; }

    imprimir_em_ordem(atual->left);
    imprimir_item(atual->data);
    imprimir_em_ordem(atual->right);
}
```

**Objetivo:** imprimir todos os itens em **ordem alfabética**.

Percurso **em ordem** (esquerda → nó → direita): primeiro os menores, depois o nó atual, depois os maiores. Como a árvore é ordenada pelo nome, a saída já sai alfabética, sem ordenação extra.

### 7.8 `imprimir_em_ordem_decrescente` (raridade)

```cpp
void imprimir_em_ordem_decrescente(node* atual)
{
    if (atual == NULL) { return; }

    imprimir_em_ordem_decrescente(atual->right);
    imprimir_item(atual->data);
    imprimir_em_ordem_decrescente(atual->left);
}
```

**Objetivo:** imprimir os itens por **raridade decrescente**.

É o percurso ao contrário (direita → nó → esquerda) na árvore de raridade: os maiores vêm primeiro.

### 7.9 `existe_na_arvore`

```cpp
bool existe_na_arvore(node* atual, string nome)
{
    if (atual == NULL) { return false; }

    string nome_buscado = converter_para_minusculas(nome);
    string nome_do_no = converter_para_minusculas(atual->data->nome);

    if (nome_buscado == nome_do_no) { return true; }

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
- Senão, desce para a esquerda (se o buscado é menor) ou para a direita (se é maior). A cada passo descarta uma parte dos itens, por isso a busca é rápida.

### 7.10 `imprimir_propriedade` e `contar_propriedade`

```cpp
int contar_propriedade(node* atual, string propriedade)
{
    if (atual == NULL) { return 0; }

    int total = contar_propriedade(atual->left, propriedade);

    if (converter_para_minusculas(atual->data->propriedadeMagica) == propriedade)
    {
        total++;
    }

    return total + contar_propriedade(atual->right, propriedade);
}
```

**Objetivo:** listar (`imprimir_propriedade`) ou contar (`contar_propriedade`) os itens com uma propriedade mágica.

- A árvore dos nomes não está ordenada por propriedade, então as duas funções **percorrem a árvore inteira** em ordem. Por isso a listagem sai em ordem alfabética de nome.
- O texto recebido já vem em minúsculas; a propriedade de cada item é convertida na hora da comparação.
- `imprimir_propriedade` tem a mesma estrutura, mas imprime o item (`imprimir_item`) a cada acerto. As duas devolvem a quantidade.

---

## 8. Funções de apoio da lista e do grafo

### 8.1 `encontrar_item`

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
- A posição na lista não é mais usada para guardar semelhanças (isso agora é feito pelo `id`), mas a função continua devolvendo as duas informações.

### 8.2 `preencher_semelhancas`

```cpp
bool preencher_semelhancas(int id1, int id2)
{
    // ... busca os dois itens com encontrar_item ...

    if (item1 == nullptr || item2 == nullptr) { /* "Item não encontrado" */ return false; }
    if ((*item2).id == (*item1).id)           { /* "não pode ter semelhança com ele mesmo" */ return false; }

    string mensagem = "Digite a semelhança entre o item 1 (...) e o item 2 (...) [0 a 10]: ";

    int valor = lerInteiroNoIntervalo(mensagem, SEMELHANCA_MINIMA, SEMELHANCA_MAXIMA);

    (*item2).semelhanca[(*item1).id] = valor;
    (*item1).semelhanca[(*item2).id] = valor;

    return true;
}
```

**Objetivo:** registrar a semelhança (aresta com peso) entre dois itens.

- Confere se os dois itens existem e se não são o mesmo.
- Lê o valor **de 0 a 10**; letras ou números fora da faixa são recusados.
- Grava o valor **nos dois itens** (A→B e B→A), pois a semelhança vale nos dois sentidos (grafo não direcionado). O índice do vetor é o **id** do outro item.

---

## 9. Funcionalidades do menu

### 9.1 `inserirItem` (opção 1)

```cpp
void inserirItem()
{
    if (quantidade_de_items >= MAX_ITENS) { /* mensagem de limite */ return; }

    Item* novoItem = new Item;
    // zera semelhanca[]

    // lê nome, dono, propriedade mágica (lerTextoNaoVazio) e raridade (0 a 100)

    if (!item_cabe_na_bolsa())
    {
        cout << "Item não cadastrado." << endl;
        delete novoItem;
        return;
    }

    // encaixa no fim da lista encadeada

    (*novoItem).id = obter_menor_id_livre();
    id_em_uso[(*novoItem).id] = true;
    quantidade_de_items++;

    insert(raiz, novoItem, POR_NOME);
    insert(raiz_raridade, novoItem, POR_RARIDADE);
}
```

**Objetivo:** cadastrar um novo item.

1. Verifica o limite de 100 itens.
2. Cria o item com `new` e zera todas as semelhanças.
3. Lê os campos de texto (sem aceitar vazio e sem espaços nas pontas) e a raridade (0 a 100).
4. Chama `item_cabe_na_bolsa`. Se o contorno não for convexo ou o item não passar pela abertura, mostra "Item não cadastrado.", apaga o item (`delete`) e **não altera nenhuma estrutura**.
5. Coloca o item no **fim da lista encadeada**.
6. Define o `id` (menor id livre) e marca o id como em uso.
7. Insere o **mesmo item** nas duas árvores (nome e raridade). O item não é copiado, os três guardam o mesmo endereço.
8. Mostra o código do item cadastrado.

### 9.2 `listarItensCadastrados` (auxiliar)

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

### 9.3 `cadastrarSimilaridade` (opção 2)

```cpp
void cadastrarSimilaridade()
{
    if (inicio == nullptr) { /* "Nenhum item cadastrado." */ return; }

    listarItensCadastrados();

    int pares = lerInteiroNoIntervalo("Quantos pares de itens deseja cadastrar? (...)", 1, MAX_ITENS * (MAX_ITENS - 1) / 2);

    for (int p = 1; p <= pares; p++)
    {
        int id1 = lerInteiroNoIntervalo("Informe o código do item 1: ", 0, MAX_ITENS - 1);
        int id2 = lerInteiroNoIntervalo("Informe o código do item 2: ", 0, MAX_ITENS - 1);

        if (preencher_semelhancas(id1, id2))
        {
            cout << "Semelhança preenchida" << endl;
        }
        else
        {
            cout << "Não foi possível preencher semelhança (par ignorado)" << endl;
        }
    }
}
```

**Objetivo:** ler uma **lista de pares** de itens (como pede o enunciado) e ligar cada par com um valor de semelhança.

- Primeiro pergunta **quantos pares** serão cadastrados (de 1 até o máximo possível, `100 × 99 / 2 = 4950`).
- Para cada par, lê dois códigos e chama `preencher_semelhancas`.
- Os códigos aceitos vão de `0` a `MAX_ITENS - 1`, porque depois de remover itens podem existir "buracos" nos ids. Se o código não existir, `preencher_semelhancas` avisa "Item não encontrado".
- Um par inválido é **ignorado com aviso** e a leitura segue para o próximo par.

### 9.4 `buscarItens` (opção 3)

```cpp
jogador = lerTextoNaoVazio("Digite o nome do jogador: ");

valor = lerInteiroNoIntervalo("Digite o valor X (listar itens com similaridade MAIOR que X) (0 a 10): ", SEMELHANCA_MINIMA, SEMELHANCA_MAXIMA);

listarItensCadastrados();

codigo = lerInteiroNoIntervalo("Digite o código do item que deseja buscar semelhança: ", 0, MAX_ITENS - 1);

// ... acha itemC (o item de código "codigo"); se não existir: "Item não encontrado!" ...

string jogador_minusculo = converter_para_minusculas(jogador);

while (atual != nullptr)
{
    if ((*atual).id != (*itemC).id
        && converter_para_minusculas((*atual).dono) != jogador_minusculo
        && (*itemC).semelhanca[(*atual).id] > valor)
    {
        // imprime nome, dono e semelhança
    }
    atual = (*atual).proximo;
}
```

**Objetivo:** listar os itens que **não são do jogador J** e que têm semelhança **maior que X** com o item de código C (como pede o enunciado).

Um item aparece na lista se cumprir as três condições:

1. não é o próprio item C;
2. o dono é diferente de J, **ignorando maiúsculas/minúsculas** (`Daniel` = `daniel`);
3. a semelhança com C é **estritamente maior (`>`)** que X.

O critério é o mesmo no pedido ("MAIOR que X"), na mensagem do resultado ("semelhança maior que X") e no código (`>`).

> Como pares sem semelhança cadastrada valem 0, com `>` eles **não** aparecem nem quando X = 0. Só aparecem itens com relação cadastrada de valor 1 ou mais.

### 9.5 `verificarExistencia` (opção 4)

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

**Objetivo:** perguntar um nome e usar a árvore dos nomes para dizer se o item existe. Os espaços das pontas são removidos antes da busca.

### 9.6 `listarAlfabeticamente` (opção 5)

```cpp
void listarAlfabeticamente()
{
    if (raiz == NULL) { /* "Nenhum item cadastrado." */ return; }

    imprimir_em_ordem(raiz);
}
```

**Objetivo:** mostrar todas as informações dos itens em ordem alfabética, usando o percurso em ordem da árvore dos nomes.

### 9.7 `listarRaridade` (opção 6)

```cpp
void listarRaridade()
{
    if (raiz_raridade == NULL) { /* "Nenhum item cadastrado." */ return; }

    imprimir_em_ordem_decrescente(raiz_raridade);
}
```

**Objetivo:** mostrar todos os itens em ordem **decrescente de raridade**, usando o percurso invertido da árvore de raridade. Itens com a mesma raridade aparecem juntos.

### 9.8 `buscarPropriedade` (opção 7 — funcionalidade extra)

```cpp
string propriedade = lerTextoNaoVazio("Digite a propriedade mágica: ");

int quantidade = imprimir_propriedade(raiz, converter_para_minusculas(propriedade));

if (quantidade == 0)
{
    cout << "Nenhum item com essa propriedade mágica." << endl;
}
```

**Objetivo:** listar todos os itens com a propriedade mágica digitada (ignorando maiúsculas/minúsculas), em ordem alfabética de nome.

> Esta opção **não está no enunciado**. É uma funcionalidade extra. O professor pediu para combinar extras com o monitor.

### 9.9 `contarPropriedades` (opção 8)

```cpp
string propriedade = lerTextoNaoVazio("Digite a propriedade mágica: ");

int quantidade = contar_propriedade(raiz, converter_para_minusculas(propriedade));

cout << "Itens com a propriedade mágica \"" << propriedade << "\": " << quantidade << endl;
```

**Objetivo:** pedir uma propriedade mágica e mostrar **quantos itens** a possuem (entrega 9).

### 9.10 `removerItens` (opção 9)

```cpp
void removerItens()
{
    if (inicio == nullptr) { /* "Nenhum item cadastrado." */ return; }

    int limite = lerInteiroNoIntervalo("Remover itens com raridade menor que (0 a 100): ", 0, 100);

    int removidos = 0;
    Item* anterior = nullptr;
    Item* atual = inicio;

    while (atual != nullptr)
    {
        Item* proximo = (*atual).proximo;

        if ((*atual).raridade < limite)
        {
            // imprime "Removido -> ..." (imprimir_item)

            // 1) tira das duas árvores
            // 2) zera a semelhança dos outros itens com este
            // 3) libera o id
            // 4) tira da lista encadeada
            // 5) apaga o item (delete)
        }
        else
        {
            anterior = atual;
        }

        atual = proximo;
    }

    cout << removidos << " item(ns) removido(s)." << endl;
}
```

**Objetivo:** remover todos os itens com raridade **menor** que R (entrega 10), de **todas as estruturas**.

Percorre a lista com dois ponteiros (`anterior` e `atual`). Para cada item a remover, na **ordem** abaixo:

1. **Duas árvores:** `remover_da_arvore(raiz, ...)` e `remover_da_arvore(raiz_raridade, ...)`. Isso vem **antes** de apagar o item, porque a busca na árvore usa os campos dele (nome e raridade).
2. **Grafo:** percorre todos os itens e zera `semelhanca[id do removido]`, para o id reaproveitado não herdar semelhanças antigas.
3. **Id:** `id_em_uso[id] = false`, liberando o código para um item novo.
4. **Lista encadeada:** religa `anterior` ao `proximo` (ou atualiza `inicio`). Se o removido era o último, atualiza `fim`.
5. **Memória:** só então faz `delete` no item e atualiza os contadores.

O `proximo` é guardado no começo de cada volta, porque `atual` pode ser apagado.

---

## 10. Controle de fluxo — `esperarEnter`

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

## 11. Apresentação do menu — `exibirMenu`

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
    criar_texto(" 2 > Cadastrar similaridade de itens");
    criar_texto(" 3 > Buscar itens similares");
    criar_texto(" 4 > Verificar a existência de um item");
    criar_texto(" 5 > Listar itens em ordem alfabética");
    criar_texto(" 6 > Listar itens em ordem descrescente de raridade");
    criar_texto(" 7 > Itens com a mesma propriedade mágica");
    criar_texto(" 8 > Contar itens com a mesma propriedade mágica");
    criar_texto(" 9 > Remover itens menos raros");
    criar_texto(" 10 > Sair");

    cout << endl << vermelho << "Escolha uma opção: " << reset;
}
```

**Objetivo:** centralizar a impressão do título e das opções. Só o prompt "Escolha uma opção" fica vermelho; o `reset` devolve a cor normal logo depois.

---

## 12. Ciclo de vida — `executarMenu`

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
- `return` na opção 10 sai da função e o programa continua para a liberação de memória.

---

## 13. Liberação de memória e ponto de entrada

```cpp
void liberar_tudo()
{
    destruct(raiz);
    destruct(raiz_raridade);

    Item* atual = inicio;

    while (atual != nullptr)
    {
        Item* proximo = (*atual).proximo;
        delete atual;
        atual = proximo;
    }

    inicio = nullptr;
    fim = nullptr;
    quantidade_de_items = 0;
}

int main()
{
    executarMenu();

    liberar_tudo();

    return 0;
}
```

**Objetivo:** encerrar sem vazar memória.

- `liberar_tudo` apaga os **nós das duas árvores** (`destruct`) e depois os **itens** percorrendo a lista. Os nós não são donos dos itens, então os itens só são apagados uma vez, pela lista.
- `main` inicia o menu, libera a memória ao sair e devolve `0` (terminou sem erros).

---

## 14. Resumo das mudanças desta versão

|Ajuste|Onde|O que mudou|
|---|---|---|
|Contorno do item (entrega 11)|`item_cabe_na_bolsa`, seção 5|Novas funções de geometria: `lado`, `area_triangulo`, `eh_poligono_convexo`, `area_poligono`, `menor_circulo_r2`. O item só é cadastrado se for convexo e passar pela abertura.|
|Abertura da bolsa|constantes do topo|`RAIO_ABERTURA`, `CRITERIO_ABERTURA` (círculo mínimo ou área) e `MAX_PONTOS`.|
|Duas árvores|`raiz` e `raiz_raridade`|`insert`, `comparar_itens` e `remover_da_arvore` recebem o critério (`POR_NOME` ou `POR_RARIDADE`).|
|Raridade decrescente (entrega 8)|`imprimir_em_ordem_decrescente`, `listarRaridade`|Percurso invertido na árvore de raridade.|
|Propriedade mágica (entrega 9 e extra 7)|`imprimir_propriedade`, `contar_propriedade`|Percorrem a árvore dos nomes inteira; sem árvore própria.|
|Remoção (entrega 10)|`removerItens`, `remover_da_arvore`|Remove da lista, das duas árvores e do grafo, e libera o id.|
|Ids reaproveitáveis|`id_em_uso`, `obter_menor_id_livre`|O id vai de 0 a `MAX_ITENS - 1`; o de um item removido pode ser reutilizado.|
|Semelhança indexada por id|`semelhanca[]`, `preencher_semelhancas`|Antes era pela posição na lista; deixou de funcionar depois de remover itens.|
|"Maior que X"|`buscarItens`|Critério passou de `>=` ("mínimo") para `>`, como diz o enunciado.|
|Lista de pares|`cadastrarSimilaridade`|Pergunta quantos pares e repete a leitura, em vez de um par por vez.|
|Leitura de reais|`lerNumero`|Nova função para as coordenadas do contorno.|
|Liberação de memória|`liberar_tudo`|Apaga os nós das duas árvores e todos os itens ao sair.|

---

## 15. Situação por entrega

|Entrega|Requisito|Situação|
|---|---|---|
|Parcial 1|Menu e funções em construção|Feito|
|Parcial 2|Inserir itens, similaridade (grafo), listar itens de outros jogadores com semelhança > X|Feito|
|Parcial 3|Inserir na árvore, imprimir em ordem alfabética, verificar existência pelo nome|Feito|
|Parcial 4|Raridade decrescente, contar propriedade mágica, remover por raridade, polígono convexo|Feito|
|Extras (5 pontos)|Balanceamento das árvores por rotação; fecho convexo dos pixels de um item|Não implementados|

### Pontos a confirmar com o professor ou o monitor

- **Abertura da bolsa:** o enunciado não define tamanho nem formato. O código assume um círculo de raio 10 (`RAIO_ABERTURA`).
- **Id do item:** o enunciado diz que o usuário informa o id ao inserir; o código gera o menor id livre automaticamente.
- **Opção 7 do menu:** é uma funcionalidade extra, fora do enunciado.
- **Acentos:** a comparação ignora maiúsculas/minúsculas só para letras sem acento.
