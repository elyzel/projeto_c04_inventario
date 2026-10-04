/*
Dev 1: Ana Julia - 1048
Dev 2: Caroline Ferreira - 872
Dev 3: Daniel Bernardes - 2353
Dev 4: Eliseu Marinho - 847
Dev 5: Wendel Iury - 972
*/

#include <iostream>
#include <string>
#include <vector>
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

// Isso é um ponteiro para ponteiro
void encontrar_item(string nome, Item** item_sendo_buscado, int* item_sendo_buscado_index) {
	*item_sendo_buscado = inicio;
	*item_sendo_buscado_index = 0;

	while ((*(*item_sendo_buscado)).nome != nome) {
		*item_sendo_buscado = (*(*item_sendo_buscado)).proximo;
		(*item_sendo_buscado_index)++;
	}
}

bool preencher_semelhancas(string nome1, string nome2)
{
	Item* item1;
	int item1_index = 0;

	encontrar_item(nome1, &item1, &item1_index);

	Item* item2;
	int item2_index = 0;
	
	encontrar_item(nome2, &item2, &item2_index);

	if ((*item2).nome == (*item1).nome){
		cout << "Um item nao pode ter semelhanca com ele mesmo";

		return false;
	}
	
	int valor;

	cout << "Digite a semelhanca entre "<< (*item1).nome<< " e "<< (*item2).nome<< ": ";

	cin >> valor;

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
	}
	else
	{
		(*fim).proximo = novoItem;
		fim = novoItem;
	}

	(*novoItem).id = quantidade_de_items;
	quantidade_de_items++;

	cout << "Item cadastrado com sucesso!" << endl;
}

void cadastrarSimilaridade() 
{
	string nome1, nome2;

	cout << "Informe o nome do item 1 e item 2 respectivamente:" << endl;
	cin >> nome1 >> nome2;

	bool preencheu_semelhanca = preencher_semelhancas(nome1, nome2);

	if (preencheu_semelhanca) {
		cout << "Semelança preenchida" << endl;
	}else {
		cout << "Nao foi possivel preencher semelhanca" << endl;
	}
}
 
void buscarItens() 
{
	string jogador;
	int valor; 
	int codigo;

	cout << "Digite o nome do jogador: ";
	cin >> jogador;

	cout << "Digite o valor minimo de similaridade: ";
	cin >> valor;

	cout << endl;

	cout << "Itens cadastrados: " << endl; // Para usuario saber quais itens ele pode buscar semelhanca e saber o codigo do item

	Item* atual = inicio;

	while (atual != nullptr)
	{
		cout << "Codigo: " << (*atual).id 
			<< " | Nome: " << (*atual).nome 
			<< " | Dono: " << (*atual).dono << endl;

		atual = (*atual).proximo;
	}

	cout << "Digite o codigo do item que deseja buscar semelhanca: ";
	cin >> codigo; 

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

	atual = inicio;

	while (atual != nullptr)
	{
		if ((*atual).id != (*itemC).id && (*atual).dono != jogador && (*itemC).semelhanca[(*atual).id] > valor)
		{
			cout << "Nome: " << (*atual).nome 
				<< " | Dono: " << (*atual).dono 
				<< " | Semelhanca: " << (*itemC).semelhanca[(*atual).id] << endl;
		}

		atual = (*atual).proximo;
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