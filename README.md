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

## 📁 Estrutura do Repositório

```
projeto_c04_inventario/
├── Project/
│   ├── main.cpp                 # Código-fonte do sistema (C++)
│   ├── README.md                # Anatomia do Código (explicação das funções)
│   └── Anatomia do Código.pdf   # Versão em PDF da anatomia do código
├── Proposta da Atividade/
│   └── Inventário.PDF           # Enunciado/proposta da atividade
├── .idea/                       # Configurações do CLion/JetBrains
├── .gitignore                   # Ignora a pasta output/ e arquivos .exe
└── README.md                    # Este arquivo
```

## 🛠️ Funcionalidades e Entregas

O status abaixo reflete o que existe hoje em `Project/main.cpp`.

### 📦 Entregas Parciais 1 & 2 (Estruturação e Grafos)
- [x] **Menu Inicial:** Menu iterativo (opções 1 a 10), com texto colorido no terminal e validação de entrada não numérica.
- [x] **Inserção de Itens:** Cadastro com Nome, Dono, Propriedade Mágica, Raridade e ID (gerado automaticamente). Os itens ficam numa lista encadeada.
- [x] **Grafo de Similaridade:** Cadastro do peso de similaridade entre dois itens, em uma matriz de adjacências (vetor `semelhanca` dentro de cada item). A relação é simétrica e um item não pode ser semelhante a ele mesmo.
- [x] **Busca por Similaridade:** Lista os itens que não pertencem a um jogador informado e cuja similaridade com um item escolhido (pelo código) é maior que *X*.

### 🌳 Entregas Parciais 3 & 4 (Árvores Binárias e Geometria)
- [~] **Armazenamento em Árvore:** Cada item cadastrado já é inserido em uma Árvore Binária de Busca, usando o nome como chave (função `insert`). Falta usar a árvore nas demais operações.
- [ ] **Listagem Alfabética:** Impressão dos itens em ordem alfabética utilizando a árvore *(stub: "em construção")*.
- [ ] **Busca Específica:** Verificação da existência de um item pelo nome *(stub)*.
- [ ] **Listagem por Raridade:** Impressão em ordem decrescente de raridade *(stub)*.
- [ ] **Agrupamento:** Itens com a mesma propriedade mágica e contagem *(stubs das opções 7 e 8)*.
- [ ] **Limpeza de Inventário:** Remoção de todos os itens com raridade menor que *R* *(stub)*.
- [ ] **Validação Geométrica (Polígonos):** Leitura de pontos de um polígono convexo para decidir se o item pode entrar na bolsa.

### ⭐ Funcionalidades Extras (Avançado)
- [ ] **Balanceamento de Árvore:** Rotações para manter a árvore balanceada.
- [ ] **Fecho Convexo (Convex Hull):** Dados os pixels da imagem de um item, determinar o polígono convexo que o engloba.

Legenda: `[x]` pronto · `[~]` parcial · `[ ]` pendente.

## 🧭 Menu do Sistema

| Opção | Ação | Status |
|:-----:|------|:------:|
| 1 | Inserir item | ✅ |
| 2 | Cadastrar similaridade de itens | ✅ |
| 3 | Buscar itens similares | ✅ |
| 4 | Verificar a existência de um item | 🚧 |
| 5 | Listar itens em ordem alfabética | 🚧 |
| 6 | Listar itens em ordem decrescente de raridade | 🚧 |
| 7 | Itens com a mesma propriedade mágica | 🚧 |
| 8 | Contar itens com a mesma propriedade mágica | 🚧 |
| 9 | Remover itens menos raros | 🚧 |
| 10 | Sair | ✅ |

## 💻 Tecnologias e Estruturas de Dados
- **Linguagem:** C++ (apenas `<iostream>` e `<string>`)
- **Estruturas utilizadas:**
  - Lista encadeada simples (armazenamento dos itens)
  - Árvore Binária de Busca, com chave = nome do item
  - Grafo por matriz de adjacências (similaridade), limitado a `MAX_ITENS = 100` itens
- **Conceitos:** Ponteiros e ponteiros para ponteiros, recursão, algoritmos de busca, geometria computacional e balanceamento de árvores (a implementar).

## ⚠️ Limitações Conhecidas
- O limite é de 100 itens (`MAX_ITENS`), por causa do tamanho fixo da matriz de similaridade.
- Nome, dono e propriedade mágica são lidos com `cin >>`, então não aceitam espaços.
- Cadastrar similaridade com um nome inexistente não é tratado.

## 🚀 Como Executar
1. Clone o repositório:
   ```bash
   git clone https://github.com/carolferreirauai/projeto_c04_inventario.git
   cd projeto_c04_inventario/Project
   ```
2. Compile o código-fonte:
   ```bash
   g++ main.cpp -o inventario
   ```
3. Execute o programa:
   ```bash
   ./inventario
   ```

## 📚 Documentação
- [Anatomia do Código](Project/README.md): explicação das funções do programa.
- [Proposta da Atividade](Proposta%20da%20Atividade/Inventário.PDF): enunciado original do projeto.
