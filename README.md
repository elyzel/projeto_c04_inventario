# 🐉 Sistema de Inventário - Bolsa Devoradora (D&D)

## 📜 Sobre o Projeto
Projeto desenvolvido para a disciplina de **Algoritmos e Estrutura de Dados III** do **Instituto Nacional de Telecomunicações (Inatel)**. 

A proposta do sistema é gerenciar um inventário coletivo de uma campanha de RPG de mesa baseado em um item lendário: a **Bolsa Devoradora**. Devido à quantidade astronômica de itens armazenados pelos jogadores, o sistema utiliza **Árvores Binárias de Busca** e **Grafos** para organizar e permitir buscas rápidas, estruturadas e eficientes.

## 👥 Equipe e Orientação
- **Professor:** Jonas Lopes de Vilas Boas
- **Monitoras:** Luana e Bianca
- **Desenvolvedores (Grupo):**
  - Ana Julia
  - Caroline
  - Daniel
  - Eliseu
  - Wendel

## 🛠️ Funcionalidades e Entregas

O projeto foi estruturado em entregas parciais, contemplando as seguintes funcionalidades:

### 📦 Entregas Parciais 1 & 2 (Estruturação e Grafos)
- [ ] **Menu Inicial:** Interface iterativa para navegação entre as funcionalidades.
- [ ] **Inserção de Itens:** Cadastro contendo Nome, Dono, Propriedade Mágica, ID e Raridade (0 a 100, onde 0 é comum e 100 raríssimo).
- [ ] **Grafo de Similaridade:** Cadastro de relações e pesos (similaridade) entre pares de itens utilizando grafos.
- [ ] **Busca por Similaridade:** Listagem de itens que não pertencem a um jogador específico e que possuem similaridade maior que *X* em relação a um determinado item.

### 🌳 Entregas Parciais 3 & 4 (Árvores Binárias e Geometria)
- [ ] **Armazenamento em Árvore:** Inserção dos itens em uma Árvore Binária de Busca.
- [ ] **Listagem Alfabética:** Impressão dos itens em ordem alfabética utilizando a árvore.
- [ ] **Busca Específica:** Verificação da existência de um item pelo nome (retornando "Item encontrado!" ou "Item não encontrado!").
- [ ] **Listagem por Raridade:** Impressão dos itens em ordem decrescente de raridade utilizando a árvore.
- [ ] **Agrupamento:** Retorno da quantidade de itens que possuem as mesmas propriedades mágicas.
- [ ] **Limpeza de Inventário:** Remoção de todos os elementos com raridade menor que um valor *R*.
- [ ] **Validação Geométrica (Polígonos):** Leitura de uma lista de pontos definindo um polígono convexo para decidir se o item pode ser adicionado à bolsa.

### ⭐ Funcionalidades Extras (Avançado)
- [ ] **Balanceamento de Árvore:** Implementação de mecanismos de rotação para manter a árvore balanceada e proporcionar buscas performáticas.
- [ ] **Fecho Convexo (Convex Hull):** Dado os pixels de imagem de um item, determinar quais pontos formam um polígono convexo que o engloba.

## 💻 Tecnologias e Estruturas de Dados
- **Linguagem:** C / C++ / Java *(Atualizar conforme a escolha do grupo)*
- **Estruturas Principais:**
  - Árvores Binárias de Busca (BST)
  - Grafos (Matriz ou Lista de Adjacências)
- **Conceitos:** Algoritmos de Busca, Geometria Computacional, Balanceamento de Árvores.

## 🚀 Como Executar
1. Clone o repositório:
   ```bash
   git clone https://github.com/seu-usuario/nome-do-repositorio.git
   ```
2. Compile o código-fonte (exemplo em C++):
   ```bash
   g++ main.cpp -o inventario
   ```
3. Execute o programa:
   ```bash
   ./inventario
   ```
