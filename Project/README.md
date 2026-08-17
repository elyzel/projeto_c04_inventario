# Anatomia do Código

## Bibliotecas

```cpp
#include<iostream>
#include<string>

using namespace std;
```

Elas importam bibliotecas da linguagem: entrada/saída no terminal (`std::cin`, `std::cout`) e o tipo de texto `std::string`

## Formatação Visual

```cpp
void criar_texto(string texto)
{
	const string vermelho = "\033[31m"
	const string reset = "\033[0m"
	cout << endl;
	cout << vermelho << texto << reset << endl;
}
```

- __Objetivo__: Imprime o texto recebido com cor vermelha.
- `vermelho` = imprime o texto como vermelho
- `reset` = imprime o texto para a cor padrão

## Funções

```cpp
void inserirItem()

{

    cout << "Funcao Inserir Item em construcao.";

}

void cadastrarSimilaridade()

{

    cout << "Funcao Cadastrar Similaridade em construcao.";

}

void buscarItens()

{

    cout << "Funcao Buscar Itens em construcao.";

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
```

Funções para cumprir o requisito do projeto.

## Controle de Fluxo `Esperar Enter`

```cpp
void esperarEnter()
{
	cout << endl;
	cout << "Pressione Enter para continuar";
	cin.ignore();
	cin.get();
}
```

- __Objetivo:__ Impede que entre diretamente no menu de seleção.

## Apresentação do Menu `exibirMenu`

```cpp
void exibirMenu()

{

    string vermelho = "\033[31m";

    string reset = "\033[0m";

    cout << endl;

    criar_texto("  ======= INVENTARIO D&D ======= ");

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
```

- __Objetivo:__ Centraliza a impressão das opções na tela.

## Ciclo de Vida `executarMenu`

```cpp
void executarMenu()

{

    int opcao;

    while (true)

    {

        exibirMenu();

        cin >> opcao;

        cout << endl;

        switch (opcao)

        {

        case 1:

            inserirItem();

            esperarEnter();

            break;

        case 2:

            cadastrarSimilaridade();

            esperarEnter();

            break;

        case 3:

            buscarItens();

            esperarEnter();

            break;

        case 4:

            verificarExistencia();

            esperarEnter();

            break;

        case 5:

            listarAlfabeticamente();

            esperarEnter();

            break;

        case 6:

            listarRaridade();

            esperarEnter();

            break;;

        case 7:

            buscarPropriedade();

            esperarEnter();

            break;

        case 8:

            contarPropriedades();

            esperarEnter();

            break;;

        case 9:

            removerItens();

            esperarEnter();

            break;

        case 10:

            cout << "Encerrando programa..." << endl;

            return;

        default:

            cout << "Opcao invalida!" << endl;

        }

        cout << endl;

    }

}

int main()

{

    executarMenu();

    return 0;

}
```