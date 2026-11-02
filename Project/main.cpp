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

// Arvore binaria de busca (chave = nome)
struct node
{
    Item* data;
    struct node* left;
    struct node* right;
};

node* raiz = NULL;

void insert(node* & current, Item* data)
{
    if (current == NULL)
    {
        current = new node;
        current->data = data;
        current->left = NULL;
        current->right = NULL;
    }
    else if (data->nome < current->data->nome)
    {
        insert(current->left, data);
    }
    else
    {
        insert(current->right, data);
    }
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
        cout << "Item nao encontrado" << endl;

        return false;
    }

    if ((*item2).id == (*item1).id)
    {
        cout << "Um item nao pode ter semelhanca com ele mesmo";

        return false;
    }

    int valor;

    cout << "Digite a semelhanca entre o item 1 (" << (*item1).nome << ") e o item 2 (" << (*item2).nome << "): ";

    cin >> valor;
    cin.ignore(10000, '\n');

    (*item2).semelhanca[item1_index] = valor;
    (*item1).semelhanca[item2_index] = valor;

    return true;
}

void inserirItem()
{
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
    cout << "Digite a raridade do item: ";
    cin >> (*novoItem).raridade;
    cin.ignore(10000, '\n');
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

    cout << "Item cadastrado com sucesso! Codigo do item: " << (*novoItem).id << endl;
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
            cout << "Codigo: " << (*atual).id
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

    cout << "Informe o codigo do item 1: ";
    cin >> id1;
    cin.ignore(10000, '\n');
    cout << "Informe o codigo do item 2: ";
    cin >> id2;
    cin.ignore(10000, '\n');

    bool preencheu_semelhanca = preencher_semelhancas(id1, id2);

    if (preencheu_semelhanca)
    {
        cout << "Semelança preenchida" << endl;
    }
    else
    {
        cout << "Nao foi possivel preencher semelhanca" << endl;
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

    cout << "Digite o valor minimo de similaridade: ";
    cin >> valor;
    cin.ignore(10000, '\n');

    cout << endl;

    // Para usuario saber quais itens ele pode buscar semelhanca e saber o codigo do item
    listarItensCadastrados();

    cout << "Digite o codigo do item que deseja buscar semelhanca: ";
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
        cout << "Item nao encontrado!" << endl;
        return;
    }

    cout << endl;
    cout << "Itens que nao pertencem a " << jogador << " e tem semelhanca maior que "
        << valor << " com " << (*itemC).nome << ":" << endl;

    bool encontrou = false;

    atual = inicio;

    while (atual != nullptr)
    {
        if ((*atual).id != (*itemC).id && (*atual).dono != jogador && (*itemC).semelhanca[(*atual).id] > valor)
        {
            cout << "Nome: " << (*atual).nome
                << " | Dono: " << (*atual).dono
                << " | Semelhanca: " << (*itemC).semelhanca[(*atual).id] << endl;

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
    cout << "Funcao Verificar Existencia em construcao.";
}

void listarAlfabeticamente()
{
    cout << "Funcao Listar Alfabeticamente em construcao.";
}

void listarRaridade()
{
    cout << "Funcao Listar por Raridade em construcao.";
}

void buscarPropriedade()
{
    cout << "Funcao Buscar por Propriedade em construcao.";
}

void contarPropriedades()
{
    cout << "Funcao Contar Propriedades em construcao.";
}

void removerItens()
{
    cout << "Funcao Remover Itens em construcao.";
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
    criar_texto(" 4 > Verificar a existencia de um item");
    criar_texto(" 5 > Listar itens em ordem alfabetica");
    criar_texto(" 6 > Listar itens em ordem descrescente de raridade");
    criar_texto(" 7 > Itens com a mesma propriedade magica");
    criar_texto(" 8 > Contar itens com a mesma propriedade magica");
    criar_texto(" 9 > Remover itens menos raros");
    criar_texto(" 10 > Sair");

    cout << vermelho << endl;

    cout << endl << "Escolha uma opcao: ";
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
            cout << "Entrada invalida! Por favor, digite um numero de 1 a 10." << endl;
            cout << "Escolha uma opcao: ";
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
            cout << endl << "Opcao invalida!\n";
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
