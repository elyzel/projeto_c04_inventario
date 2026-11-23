/*
Dev 1: Ana Julia - 1048
Dev 2: Daniel Bernardes - 2353
Dev 3: Eliseu Marinho - 847
Dev 4: Wendel Iury - 972
*/

#include <iostream>
#include <string>
#include <cmath>

const int MAX_ITENS = 100;
const int SEMELHANCA_MINIMA = 0;
const int SEMELHANCA_MAXIMA = 10;

// abertura da bolsa: um círculo com raio 10 (mesma unidade dos pontos do contorno)
const double RAIO_ABERTURA = 10.0;
const double PI = 3.14159265358979;
const int CRITERIO_ABERTURA = 0;

// quantidade máxima de pontos (vértices) do contorno de um item
const int MAX_PONTOS = 20;

using namespace std;

void criar_texto(string texto)
{
    const string vermelho = "\033[31m";
    const string reset = "\033[0m";
    cout << endl;
    cout << vermelho << texto << reset << endl;
}

struct Item
{
    string nome;
    string dono;
    string propriedadeMagica;
    int id;
    int raridade;
    int semelhanca[MAX_ITENS]; // semelhança[j] = peso da semelhança com o item de id j
    Item* proximo;
};

Item* inicio = nullptr;
Item* fim = nullptr;
int quantidade_de_items = 0;

// id_em_uso[i] é true quando existe um item com id i
// como a semelhança é indexada pelo id, os ids ficam sempre entre 0 e MAX_ITENS - 1,
// e o id de um item removido pode ser reaproveitado por um item novo
bool id_em_uso[MAX_ITENS];

// devolve o menor id que não está em uso
int obter_menor_id_livre()
{
    int i = 0;

    while (i < MAX_ITENS && id_em_uso[i])
    {
        i++;
    }

    return i;
}

// lê um inteiro dentro de [mínimo, máximo], repetindo até o usuário digitar um valor válido
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

// lê um número real (aceita inteiro ou decimal com ponto), repetindo até o usuário digitar algo válido
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

// remove espaços (e tabs) do começo e do fim do texto
// exemplo: "  Espada  " vira "Espada"
string trim(string texto)
{
    int comeco = 0;
    int final = (int)texto.length() - 1;

    while (comeco <= final && (texto[comeco] == ' ' || texto[comeco] == '\t' || texto[comeco] == '\r' || texto[comeco] == '\n'))
    {
        comeco++;
    }

    while (final >= comeco && (texto[final] == ' ' || texto[final] == '\t' || texto[final] == '\r' || texto[final] == '\n'))
    {
        final--;
    }

    return texto.substr(comeco, final - comeco + 1);
}

// lê uma linha de texto, apaga os espaços das pontas e repete até o usuário digitar algo
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

struct ponto
{
    double x;
    double y;
};

double valor_absoluto(double numero)
{
    if (numero < 0)
    {
        return -numero;
    }

    return numero;
}

// área (com sinal) do triângulo abc
// positiva se a -> b -> c está em sentido anti-horário, negativa se horário e 0 se alinhados
double area_triangulo(ponto a, ponto b, ponto c)
{
    return (
        a.x * b.y - a.y * b.x +
        a.y * c.x - a.x * c.y +
        b.x * c.y - b.y * c.x
    ) / 2.0;
}

// retorna 1 se p estiver à esquerda de ab, -1 se estiver à direita e 0 se estiver alinhado
int lado(ponto a, ponto b, ponto p)
{
    double s = area_triangulo(a, b, p);

    if (s > 0)
    {
        return 1;
    }
    if (s < 0)
    {
        return -1;
    }

    return 0;
}

bool eh_poligono_convexo(ponto contorno[], int n)
{
    int sentido = 0; // 0 = ainda não sei, 1 = esquerda, -1 = direita

    for (int i = 0; i < n; i++)
    {
        int j = (i + 1) % n;

        if (contorno[i].x == contorno[j].x && contorno[i].y == contorno[j].y)
        {
            return false; // dois pontos seguidos iguais
        }

        for (int k = 0; k < n; k++)
        {
            if (k == i || k == j)
            {
                continue;
            }

            int l = lado(contorno[i], contorno[j], contorno[k]);

            if (l == 0)
            {
                continue; // ponto alinhado com o lado: não decide nada
            }

            if (sentido == 0)
            {
                sentido = l;
            }
            else if (l != sentido)
            {
                return false;
            }
        }
    }

    return sentido != 0; // sentido 0 = todos os pontos alinhados (não é um polígono)
}

// área do polígono convexo: divide em triângulos a partir do primeiro ponto ("leque")
// e soma a área de cada triângulo com area_triangulo
double area_poligono(ponto contorno[], int n)
{
    double soma = 0;

    for (int i = 1; i < n - 1; i++)
    {
        soma += area_triangulo(contorno[0], contorno[i], contorno[i + 1]);
    }

    return valor_absoluto(soma); // o sinal só indica o sentido (horário/anti-horário)
}

// todos os pontos estão dentro (ou na borda) do círculo de centro (cx, cy) e raio ao quadrado r2?
bool todos_dentro(ponto contorno[], int n, double cx, double cy, double r2)
{
    for (int i = 0; i < n; i++)
    {
        double dx = contorno[i].x - cx;
        double dy = contorno[i].y - cy;

        // pequena folga para erros de arredondamento
        if (dx * dx + dy * dy > r2 * (1 + 1e-9) + 1e-9)
        {
            return false;
        }
    }

    return true;
}

// raio ao quadrado do MENOR círculo que contém todos os pontos
// esse círculo sempre passa por 2 pontos (sendo eles as pontas do diâmetro) ou por 3 pontos
// testamos todas as duplas e todas as triplas e ficamos com o menor círculo que contém tudo
// se um item cabe em algum círculo de raio R (podendo girar), ele cabe no menor círculo dele
// devolve -1 se não achar nenhum (só acontece com pontos alinhados)
double menor_circulo_r2(ponto contorno[], int n)
{
    double melhor = -1;

    // círculos definidos por 2 pontos (diâmetro)
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double cx = (contorno[i].x + contorno[j].x) / 2;
            double cy = (contorno[i].y + contorno[j].y) / 2;
            double dx = contorno[i].x - contorno[j].x;
            double dy = contorno[i].y - contorno[j].y;
            double r2 = (dx * dx + dy * dy) / 4;

            if ((melhor < 0 || r2 < melhor) && todos_dentro(contorno, n, cx, cy, r2))
            {
                melhor = r2;
            }
        }
    }

    // círculos definidos por 3 pontos (circunferência que passa pelos três)
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            for (int k = j + 1; k < n; k++)
            {
                double ax = contorno[i].x, ay = contorno[i].y;
                double bx = contorno[j].x, by = contorno[j].y;
                double px = contorno[k].x, py = contorno[k].y;

                double d = 2 * (ax * (by - py) + bx * (py - ay) + px * (ay - by));

                if (valor_absoluto(d) < 1e-12)
                {
                    continue; // três pontos alinhados: não formam círculo
                }

                double a2 = ax * ax + ay * ay;
                double b2 = bx * bx + by * by;
                double p2 = px * px + py * py;

                double cx = (a2 * (by - py) + b2 * (py - ay) + p2 * (ay - by)) / d;
                double cy = (a2 * (px - bx) + b2 * (ax - px) + p2 * (bx - ax)) / d;

                double dx = ax - cx;
                double dy = ay - cy;
                double r2 = dx * dx + dy * dy;

                if ((melhor < 0 || r2 < melhor) && todos_dentro(contorno, n, cx, cy, r2))
                {
                    melhor = r2;
                }
            }
        }
    }

    return melhor;
}

// lê o contorno do item e decide se ele pode ser adicionado na bolsa
// devolve true se o contorno é um polígono convexo válido e passa pela abertura
bool item_cabe_na_bolsa()
{
    ponto contorno[MAX_PONTOS];

    cout << endl << "Contorno do item: informe os vértices de um polígono convexo, em ordem (horária ou anti-horária)." << endl;
    cout << "A abertura da bolsa é um círculo de raio " << RAIO_ABERTURA << "." << endl;

    int n = lerInteiroNoIntervalo("Quantidade de pontos (3 a " + to_string(MAX_PONTOS) + "): ", 3, MAX_PONTOS);

    for (int i = 0; i < n; i++)
    {
        contorno[i].x = lerNumero("Ponto " + to_string(i + 1) + " - x: ");
        contorno[i].y = lerNumero("Ponto " + to_string(i + 1) + " - y: ");
    }

    if (!eh_poligono_convexo(contorno, n))
    {
        cout << "Contorno inválido: os pontos não formam um polígono convexo (confira a ordem dos pontos)." << endl;
        return false;
    }

    double area = area_poligono(contorno, n);
    
    double r2 = menor_circulo_r2(contorno, n);

    if (r2 < 0)
    {
        cout << "Contorno inválido: não foi possível calcular o tamanho do item." << endl;
        return false;
    }

    double raio = sqrt(r2);

    if (raio > RAIO_ABERTURA + 1e-9)
    {
        cout << "O item não passa pela abertura da bolsa (precisa de um círculo de raio "
            << raio << ")." << endl;
        return false;
    }

    cout << "O item passa pela abertura da bolsa (círculo mínimo de raio " << raio
        << ", área " << area << ")." << endl;

    return true;
}

// ---------------------------------------------------------------------------
// árvores binárias de busca, são DUAS árvores:
//   raiz          -> chave = nome do item  (busca por nome e listagem alfabética)
//   raiz_raridade -> chave = raridade      (listagem por raridade decrescente)
// ---------------------------------------------------------------------------
struct node
{
    Item* data;
    node* left;
    node* right;
};

node* raiz = NULL;
node* raiz_raridade = NULL;

// critérios de ordenação: dizem por qual campo a árvore compara os itens
const int POR_NOME = 0;
const int POR_RARIDADE = 1;

// compara dois itens pelo critério escolhido
// devolve < 0 se 'a' vem antes de 'b', 0 se são iguais nesse critério e > 0 se 'a' vem depois
int comparar_itens(Item* a, Item* b, int criterio)
{
    if (criterio == POR_RARIDADE)
    {
        if (a->raridade < b->raridade)
        {
            return -1;
        }

        if (a->raridade > b->raridade)
        {
            return 1;
        }

        return 0;
    }

    // por_nome: compara os nomes ignorando maiúsculas/minúsculas
    string texto_a = converter_para_minusculas(a->nome);
    string texto_b = converter_para_minusculas(b->nome);

    if (texto_a < texto_b)
    {
        return -1;
    }

    if (texto_a > texto_b)
    {
        return 1;
    }

    return 0;
}

// regra das árvores: menor vai para a esquerda; igual ou maior vai para a direita
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

// remove da árvore o nó que guarda exatamente este item (o Item em si não é apagado aqui)
// como a árvore aceita chaves repetidas, em vez de parar no primeiro nó com a mesma chave
// a busca continua até achar o nó que aponta para este item
// casos:
//   1) sem filho direito: o filho esquerdo (ou NULL) ocupa o lugar do nó;
//   2) sem filho esquerdo: o filho direito ocupa o lugar do nó;
//   3) com dois filhos: o nó recebe o item do menor nó da subárvore direita, e esse nó é apagado
void remover_da_arvore(node* & atual, Item* item, int criterio)
{
    if (atual == NULL)
    {
        return;
    }

    if (atual->data == item)
    {
        node* temp = atual;

        if (atual->right == NULL)
        {
            atual = atual->left;
        }
        else if (atual->left == NULL)
        {
            atual = atual->right;
        }
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

// apaga todos os nós da árvore (os Items não são apagados aqui)
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

void imprimir_item(Item* item)
{
    cout << "Código: " << item->id
        << " | Nome: " << item->nome
        << " | Dono: " << item->dono
        << " | Propriedade mágica: " << item->propriedadeMagica
        << " | Raridade: " << item->raridade << endl;
}

// percorre a árvore em ordem (esquerda -> nó -> direita)
// como a chave da árvore é o nome, o resultado sai em ordem alfabética
void imprimir_em_ordem(node* atual)
{
    if (atual == NULL)
    {
        return;
    }

    imprimir_em_ordem(atual->left);

    imprimir_item(atual->data);

    imprimir_em_ordem(atual->right);
}

// percorre a árvore de raridade ao contrário (direita -> nó -> esquerda):
// os maiores vêm primeiro, então a saída sai em ordem decrescente de raridade
void imprimir_em_ordem_decrescente(node* atual)
{
    if (atual == NULL)
    {
        return;
    }

    imprimir_em_ordem_decrescente(atual->right);

    imprimir_item(atual->data);

    imprimir_em_ordem_decrescente(atual->left);
}

// busca um nome na árvore: se for menor que o do nó atual, desce pela
// esquerda; se for maior, desce pela direita; se for igual, achou
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

// imprime (em ordem alfabética de nome) os itens com a propriedade mágica informada
// (já em minúsculo) e devolve quantos são, a árvore dos nomes não está ordenada por
// propriedade, então percorremos a árvore inteira (em ordem)
int imprimir_propriedade(node* atual, string propriedade)
{
    if (atual == NULL)
    {
        return 0;
    }

    int total = imprimir_propriedade(atual->left, propriedade);

    if (converter_para_minusculas(atual->data->propriedadeMagica) == propriedade)
    {
        imprimir_item(atual->data);
        total++;
    }

    return total + imprimir_propriedade(atual->right, propriedade);
}

// conta os itens com a propriedade mágica informada (já em minúsculo), mesmo percurso da função acima
int contar_propriedade(node* atual, string propriedade)
{
    if (atual == NULL)
    {
        return 0;
    }

    int total = contar_propriedade(atual->left, propriedade);

    if (converter_para_minusculas(atual->data->propriedadeMagica) == propriedade)
    {
        total++;
    }

    return total + contar_propriedade(atual->right, propriedade);
}

// isso é um ponteiro para ponteiro
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

bool preencher_semelhancas(int id1, int id2)
{
    Item* item1;
    int item1_index = 0;

    encontrar_item(id1, &item1, &item1_index);

    Item* item2;
    int item2_index = 0;

    encontrar_item(id2, &item2, &item2_index);

    if (item1 == nullptr || item2 == nullptr)
    {
        cout << "Item não encontrado" << endl;

        return false;
    }

    if ((*item2).id == (*item1).id)
    {
        cout << "Um item não pode ter semelhança com ele mesmo" << endl;

        return false;
    }

    string mensagem = "Digite a semelhança entre o item 1 (" + (*item1).nome + ") e o item 2 ("
        + (*item2).nome + ") [" + to_string(SEMELHANCA_MINIMA) + " a " + to_string(SEMELHANCA_MAXIMA) + "]: ";

    int valor = lerInteiroNoIntervalo(mensagem, SEMELHANCA_MINIMA, SEMELHANCA_MAXIMA);

    // semelhança é indexada pelo id (e não pela posição na lista), porque depois
    // de remover itens a posição na lista deixa de ser igual ao id
    (*item2).semelhanca[(*item1).id] = valor;
    (*item1).semelhanca[(*item2).id] = valor;

    return true;
}

void inserirItem()
{
    if (quantidade_de_items >= MAX_ITENS)
    {
        cout << "Limite de " << MAX_ITENS << " itens atingido." << " Não é possível cadastrar mais itens." << endl;
        return;
    }

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

    // o item só entra na bolsa se o contorno for válido e passar pela abertura
    if (!item_cabe_na_bolsa())
    {
        cout << "Item não cadastrado." << endl;
        delete novoItem;
        return;
    }

    if (inicio == nullptr)
    {
        inicio = novoItem;
        fim = novoItem;
    }
    else
    {
        (*fim).proximo = novoItem;
        fim = novoItem;
    }

    (*novoItem).id = obter_menor_id_livre();
    id_em_uso[(*novoItem).id] = true;
    quantidade_de_items++;

    insert(raiz, novoItem, POR_NOME);               // árvore dos nomes
    insert(raiz_raridade, novoItem, POR_RARIDADE);  // árvore das raridades

    cout << "Item cadastrado com sucesso!" << endl << "Código do item: " << (*novoItem).id << endl;
}

void listarItensCadastrados()
{
    char resposta = 'n';

    cout << "Deseja ver a lista de itens cadastrados? (s/n): ";
    cin >> resposta;
    cin.ignore(10000, '\n');

    if (resposta == 's' || resposta == 'S')
    {
        Item* atual = inicio;

        while (atual != nullptr)
        {
            cout << "Código: " << (*atual).id
                << " | Nome: " << (*atual).nome
                << " | Dono: " << (*atual).dono << endl;

            atual = (*atual).proximo;
        }
    }
}

// lê uma lista de pares de códigos (com o peso S de cada par) e grava no grafo
void cadastrarSimilaridade()
{
    if (inicio == nullptr)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    listarItensCadastrados();

    int pares = lerInteiroNoIntervalo("Quantos pares de itens deseja cadastrar? (1 a " + to_string(MAX_ITENS * (MAX_ITENS - 1) / 2) + "): ",
        1, MAX_ITENS * (MAX_ITENS - 1) / 2);

    for (int p = 1; p <= pares; p++)
    {
        cout << endl << "Par " << p << " de " << pares << endl;

        // os ids vão de 0 a MAX_ITENS - 1 (podem ter "buracos" depois de remover itens);
        // se o id não existir, preencher_semelhancas avisa "Item não encontrado"
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

void buscarItens()
{
    string jogador;
    int valor;
    int codigo;

    if (inicio == nullptr)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    jogador = lerTextoNaoVazio("Digite o nome do jogador: ");

    valor = lerInteiroNoIntervalo("Digite o valor X (listar itens com similaridade MAIOR que X) (" + to_string(SEMELHANCA_MINIMA)
        + " a " + to_string(SEMELHANCA_MAXIMA) + "): ", SEMELHANCA_MINIMA, SEMELHANCA_MAXIMA);

    cout << endl;

    // para usuário saber quais itens ele pode buscar semelhança e saber o código do item
    listarItensCadastrados();

    codigo = lerInteiroNoIntervalo("Digite o código do item que deseja buscar semelhança: ", 0, MAX_ITENS - 1);

    Item* atual;

    Item* itemC = inicio;

    while (itemC != nullptr && (*itemC).id != codigo)
    {
        itemC = (*itemC).proximo;
    }

    if (itemC == nullptr)
    {
        cout << "Item não encontrado!" << endl;
        return;
    }

    cout << endl;
    cout << "Itens que não pertencem a " << jogador << " e têm semelhança maior que "
        << valor << " com " << (*itemC).nome << ":" << endl;

    bool encontrou = false;
    string jogador_minusculo = converter_para_minusculas(jogador);

    atual = inicio;

    while (atual != nullptr)
    {
        // compara o dono ignorando maiúsculas/minúsculas (Daniel == daniel)
        if ((*atual).id != (*itemC).id
            && converter_para_minusculas((*atual).dono) != jogador_minusculo
            && (*itemC).semelhanca[(*atual).id] > valor)
        {
            cout << "Nome: " << (*atual).nome
                << " | Dono: " << (*atual).dono
                << " | Semelhança: " << (*itemC).semelhanca[(*atual).id] << endl;

            encontrou = true;
        }

        atual = (*atual).proximo;
    }

    if (!encontrou)
    {
        cout << "Nenhum item encontrado." << endl;
    }
}

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

void listarAlfabeticamente()
{
    if (raiz == NULL)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    imprimir_em_ordem(raiz);
}

void listarRaridade()
{
    if (raiz_raridade == NULL)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    imprimir_em_ordem_decrescente(raiz_raridade);
}

void buscarPropriedade()
{
    if (raiz == NULL)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    string propriedade = lerTextoNaoVazio("Digite a propriedade mágica: ");

    cout << endl;

    int quantidade = imprimir_propriedade(raiz, converter_para_minusculas(propriedade));

    if (quantidade == 0)
    {
        cout << "Nenhum item com essa propriedade mágica." << endl;
    }
}

void contarPropriedades()
{
    if (raiz == NULL)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    string propriedade = lerTextoNaoVazio("Digite a propriedade mágica: ");

    int quantidade = contar_propriedade(raiz, converter_para_minusculas(propriedade));

    cout << "Itens com a propriedade mágica \"" << propriedade << "\": " << quantidade << endl;
}

// remove todos os itens com raridade menor que R, de TODAS as estruturas:
// lista encadeada, as 2 árvores e o grafo (a semelhança dos outros itens com ele volta a 0)
void removerItens()
{
    if (inicio == nullptr)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    int limite = lerInteiroNoIntervalo("Remover itens com raridade menor que (0 a 100): ", 0, 100);

    int removidos = 0;
    Item* anterior = nullptr;
    Item* atual = inicio;

    while (atual != nullptr)
    {
        Item* proximo = (*atual).proximo; // guarda antes, porque atual pode ser apagado

        if ((*atual).raridade < limite)
        {
            cout << "Removido -> ";
            imprimir_item(atual);

            // 1) tira das duas árvores (antes de apagar o item, pois a busca usa os campos dele)
            remover_da_arvore(raiz, atual, POR_NOME);
            remover_da_arvore(raiz_raridade, atual, POR_RARIDADE);

            // 2) tira do grafo: zera a semelhança de todos os itens com este
            Item* outro = inicio;

            while (outro != nullptr)
            {
                (*outro).semelhanca[(*atual).id] = 0;
                outro = (*outro).proximo;
            }

            // 3) libera o id para um item novo poder usar
            id_em_uso[(*atual).id] = false;

            // 4) tira da lista encadeada
            if (anterior == nullptr)
            {
                inicio = proximo;
            }
            else
            {
                (*anterior).proximo = proximo;
            }

            if (atual == fim)
            {
                fim = anterior;
            }

            // 5) agora sim apaga o item
            delete atual;
            quantidade_de_items--;
            removidos++;
        }
        else
        {
            anterior = atual;
        }

        atual = proximo;
    }

    cout << removidos << " item(ns) removido(s)." << endl;
}

void esperarEnter()
{
    cout << endl;
    cout << "Pressione ENTER para continuar";
    cin.get(); // esperando o próximo enter pra retornar menu (o buffer já foi limpo na leitura)
}

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

    // só o prompt fica vermelho; o reset devolve a cor normal ao terminal
    cout << endl << vermelho << "Escolha uma opção: " << reset;
}

void executarMenu()
{
    int opcao;

    while (true)
    {
        exibirMenu();

        while (!(cin >> opcao) || opcao < 1 || opcao > 10)
        {
            if (cin.eof())
            {
                return;
            }

            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada inválida! Por favor, digite um número de 1 a 10." << endl;
            cout << "Escolha uma opção: ";
        }

        cin.ignore(10000, '\n');

        switch (opcao)
        {
        case 1:
            inserirItem();
            break;

        case 2:
            cadastrarSimilaridade();
            break;

        case 3:
            buscarItens();
            break;

        case 4:
            verificarExistencia();
            break;

        case 5:
            listarAlfabeticamente();
            break;

        case 6:
            listarRaridade();
            break;

        case 7:
            buscarPropriedade();
            break;

        case 8:
            contarPropriedades();
            break;

        case 9:
            removerItens();
            break;

        case 10:
            cout << "Encerrando programa..." << endl;
            return;

        default:
            cout << endl << "Opção inválida!\n";
        }

        esperarEnter();

        cout << endl;
    }
}

// libera toda a memória alocada: os nós das 2 árvores e os Items da lista encadeada
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
