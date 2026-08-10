#include <iostream>
 
using namespace std;
 
void exibirMenu()
{
	string vermelho = "\033[31m";
	string reset = "\033[0m";
	cout << endl;
	cout << vermelho << "              INVENTARIO D&D              " << reset << endl << endl;
	cout << vermelho << " > " << reset << "Inserir item" << endl;
	cout << vermelho << " > " << reset << "Cadastrar similaridade de itens" << endl;
	cout << vermelho << " > " << reset << "Buscar itens similares" << endl;
	cout << vermelho << " > " << reset << "Verificar a existencia de um item" << endl;
	cout << vermelho << " > " << reset << "Listar itens em ordem alfabetica" << endl;
	cout << vermelho << " > " << reset << "Listar itens em ordem descrescente de raridade" << endl;
	cout << vermelho << " > " << reset << "Itens com a mesma propriedade magica" << endl;
	cout << vermelho << " > " << reset << "Contar itens com a mesma propriedade magica" << endl;
	cout << vermelho << " > " << reset << "Remover itens menos raros" << endl;
	cout << vermelho << " > " << reset << "Sair" << endl;
	cout << vermelho << endl;
	cout << "Escolha a opcao desejada: ";
}
 
int main()
{
	int opcao;
 
	while (true)
	{
		exibirMenu();
		cin >> opcao;
		cout << endl;
		cin.ignore();
 
		switch (opcao)
		{
		case 1:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 2:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 3:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 4:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 5:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 6:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 7:
			cout << "Funcao em construcao" << endl;
			break;
 
		case 8:
			cout << "Funcao em construcao" << endl;
			break;
		case 9: 
			cout << "Funcao em construcao" << endl;
			break;
 
		case 10:
			return 0;
 
		default:
			cout << "Opção inválida!" << endl;
		}
	}
}