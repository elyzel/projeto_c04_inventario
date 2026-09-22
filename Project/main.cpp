/*
Dev 1: Ana Julia - 1048
Dev 2: Caroline Ferreira - 872
Dev 3: Daniel Bernardes - 2353
Dev 4: Eliseu Marinho - 847
Dev 5: Wendel Iury - 972
*/

#include <iostream>
#include <list>
#include <string>
 
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
	list<int> semelhanca;
	Item* proximo;
};

Item* inicio = nullptr;
Item* fim = nullptr;

void preencher_semelhancas(string nome1, string nome2)
{
	Item* primeiro = inicio;
	int primeiro_index = 0;
	
	while ((*primeiro).nome != nome1){
		primeiro = (*primeiro).proximo;
		primeiro_index++;
	}

	Item* outro = inicio;
	int outro_index = 0;
	
	while ((*outro).nome != nome2){
		outro = (*outro).proximo;
		outro_index++;
	}

	if ((*outro).nome == (*primeiro).nome){
		cout << "um item nao pode ter semelhanca com ele mesmo";

	return;
	}
	
	int valor;

	cout << "Digite a semelhanca entre "<< (*primeiro).nome<< " e "<< (*outro).nome<< ": ";

	cin >> valor;
	(*outro).semelhanca.push_back(valor);

	(*primeiro).semelhanca.push_back(valor);
}

void inserirItem()
{
	Item* novoItem = new Item;
	cout << "Digite o nome do item: ";
	cin >> (*novoItem).nome;
	cout << "Digite o dono do item: ";
	cin >> (*novoItem).dono;
	cout << "Digite a propriedade mágica do item: ";
	cin >> (*novoItem).propriedadeMagica;
	cout << "Digite a raridade do item: ";
	cin >> (*novoItem).raridade;
	(*novoItem).proximo = nullptr;

	if (inicio == nullptr)
	{
		inicio = novoItem;
		fim = novoItem;
		(*novoItem).id = 0;
	}
	else
	{
		(*novoItem).id = (*fim).id + 1;
		(*fim).proximo = novoItem;
		fim = novoItem;
	}

	// preencher_semelhancas(novoItem);

	cout << "Item cadastrado com sucesso!" << endl;
}

void cadastrarSimilaridade() 
{
	string nome1, nome2;

	cout << "Informe o nome do item 1 e item 2 respectivamente:" << endl;
	cin >> nome1 >> nome2;

	preencher_semelhancas(nome1, nome2);
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
 
void esperarEnter()
{
    cout << endl;
    cout << "Pressione ENTER para continuar";
    cin.ignore(); //limpa o buffer que esta salvo do enter anterior com cin >> 1 >> enter
    cin.get(); // esperando o proximo enter pra retornar menu
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
 
		if(!(cin >> opcao)){
			cin.clear(); 
			cin.ignore();
			cout << "Entrada invalida! Por favor, digite um numero.";
		}
 
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
			cout << endl << "Opcao invalida!\n";
			esperarEnter();
		}
 
		cout << endl;
	}
}
 
int main()
{
	executarMenu();
 
	return 0;
}