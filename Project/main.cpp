/*
Dev 1: Ana Julia - 1048
Dev 2: Caroline Ferreira - 872
Dev 3: Daniel Bernardes - 2353
Dev 4: Eliseu Marinho - 847
Dev 5: Wendel Iury - 972
*/

#include <iostream>
#include <string>
const int MAX_ITENS = 100;

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
    int semelhanca[MAX_ITENS];
    Item* proximo;
};

Item* inicio = nullptr;
Item* fim = nullptr;
int quantidade_de_items = 0;

// Le um inteiro dentro de [minimo, maximo], repetindo ate o usuario digitar um valor valido.
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

// Arvore binaria de busca (chave = nome)
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

// Percorre a arvore em ordem (esquerda -> no -> direita).
// Como a chave da arvore e o nome, o resultado sai em ordem alfabetica.
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

// Busca um nome na arvore: se for menor que o do no atual, desce pela
// esquerda; se for maior, desce pela direita; se for igual, achou.
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

// Isso é um ponteiro para ponteiro
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

    int valor;

    cout << "Digite a semelhança entre o item 1 (" << (*item1).nome << ") e o item 2 (" << (*item2).nome << "): ";

    cin >> valor;
    cin.ignore(10000, '\n');

    (*item2).semelhanca[item1_index] = valor;
    (*item1).semelhanca[item2_index] = valor;

    return true;
}

void inserirItem()
{
    if (quantidade_de_items >= MAX_ITENS)
    {
        cout << "Limite de " << MAX_ITENS << " itens atingido." << "Não é possível cadastrar mais itens." << endl;
        return;
    }

    Item* novoItem = new Item;

    for (int i = 0; i < MAX_ITENS; i++)
    {
        (*novoItem).semelhanca[i] = 0;
    }

    cout << "Digite o nome do item: ";
    getline(cin, (*novoItem).nome);
    cout << "Digite o dono do item: ";
    getline(cin, (*novoItem).dono);
    cout << "Digite a propriedade mágica do item: ";
    getline(cin, (*novoItem).propriedadeMagica);
    (*novoItem).raridade = lerInteiroNoIntervalo("Digite a raridade do item (0 a 100): ", 0, 100);
    (*novoItem).proximo = nullptr;

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

    (*novoItem).id = quantidade_de_items;
    quantidade_de_items++;

    insert(raiz, novoItem); // guarda na arvore

    cout << "Item cadastrado com sucesso!" << endl << "Codigo do item: " << (*novoItem).id << endl;
}

void listarItensCadastrados()
{
    char resposta;

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

void cadastrarSimilaridade()
{
    int id1, id2;

    if (inicio == nullptr)
    {
        cout << "Nenhum item cadastrado." << endl;
        return;
    }

    listarItensCadastrados();

    cout << "Informe o código do item 1: ";
    cin >> id1;
    cin.ignore(10000, '\n');
    cout << "Informe o código do item 2: ";
    cin >> id2;
    cin.ignore(10000, '\n');

    bool preencheu_semelhanca = preencher_semelhancas(id1, id2);

    if (preencheu_semelhanca)
    {
        cout << "Semelhança preenchida" << endl;
    }
    else
    {
        cout << "Não foi possível preencher semelhança" << endl;
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

    cout << "Digite o nome do jogador: ";
    getline(cin, jogador);

    cout << "Digite o valor mínimo de similaridade: ";
    cin >> valor;
    cin.ignore(10000, '\n');

    cout << endl;

    // Para usuario saber quais itens ele pode buscar semelhanca e saber o codigo do item
    listarItensCadastrados();

    cout << "Digite o código do item que deseja buscar semelhança: ";
    cin >> codigo;
    cin.ignore(10000, '\n');

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
    cout << "Itens que não pertencem a " << jogador << " e tem semelhança maior que "
        << valor << " com " << (*itemC).nome << ":" << endl;

    bool encontrou = false;

    atual = inicio;

    while (atual != nullptr)
    {
        if ((*atual).id != (*itemC).id && (*atual).dono != jogador && (*itemC).semelhanca[(*atual).id] > valor)
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
    string nome;

    cout << "Digite o nome do item que deseja procurar: ";
    getline(cin, nome);

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
    cout << "Função Listar por Raridade em construção.";
}

void buscarPropriedade()
{
    cout << "Função Buscar por Propriedade em construção.";
}

void contarPropriedades()
{
    cout << "Função Contar Propriedades em construção.";
}

void removerItens()
{
    cout << "Função Remover Itens em construção.";
}

void esperarEnter()
{
    cout << endl;
    cout << "Pressione ENTER para continuar";
    cin.get(); // esperando o proximo enter pra retornar menu (o buffer ja foi limpo na leitura)
}

void exibirMenu()
{
    string vermelho = "\033[31m";
    string reset = "\033[0m";

    cout << endl;

    criar_texto("==========================================");
    criar_texto("             INVENTARIO D&D               ");
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

    // So o prompt fica vermelho; o reset devolve a cor normal ao terminal
    cout << endl << vermelho << "Escolha uma opcao: " << reset;
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

int main()
{
    executarMenu();

    return 0;
}