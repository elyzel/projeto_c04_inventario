# 🐉 Sistema de Inventário - Bolsa Devoradora (D&D)

## 📜 Sobre o Projeto
Projeto desenvolvido para a disciplina de **Algoritmos e Estrutura de Dados III** do **Instituto Nacional de Telecomunicações (Inatel)**.

A proposta do sistema é gerenciar um inventário coletivo de uma campanha de RPG de mesa baseado em um item lendário: a **Bolsa Devoradora** (*Bag of Devouring*). Devido à grande quantidade de itens armazenados pelos jogadores, o sistema sincroniza os itens em múltiplas estruturas de dados simultâneas — **Lista Encadeada Simples**, **Árvores Binárias de Busca (ABB)** e **Grafo de Similaridade** — além de contar com validação de **Geometria Computacional** antes de permitir que qualquer item entre na bolsa.

## 👥 Equipe e Orientação
- **Professor:** Jonas Lopes de Vilas Boas
- **Monitoras:** Luana e Bianca
- **Desenvolvedores:**
  - Ana Júlia (1048)
  - Caroline Ferreira
  - Daniel Bernardes Cardoso (2353)
  - Eliseu Marinho de Oliveira (847)
  - Wendel Iury (972)

## 📁 Estrutura do Repositório

```
projeto_c04_inventario/
├── Project/
│   ├── main.cpp                 # Código-fonte principal do sistema (C++)
│   ├── Anatomia_do_Codigo.md    # Documentação técnica e didática detalhada de cada função
│   ├── Anatomia_do_Codigo.pdf   # Versão em PDF da anatomia do código
│   └── README.md                # Visão geral das funções da pasta Project
├── Proposta da Atividade/
│   └── Inventário.PDF           # Enunciado/proposta oficial da disciplina
├── .idea/                       # Configurações de ambiente do CLion/JetBrains
├── .gitignore                   # Arquivos ignorados pelo Git (.exe, etc.)
└── README.md                    # Documentação principal do projeto
```

## 🛠️ Funcionalidades e Entregas

Todas as entregas obrigatórias do projeto estão **100% implementadas e funcionais** em `Project/main.cpp`:

### 📦 Entregas Parciais 1 & 2 (Estruturação e Grafos)
- [x] **Menu Inicial:** Menu iterativo com 10 opções, saída colorida no terminal com sequências ANSI e validação contínua contra entradas não numéricas.
- [x] **Inserção de Itens:** Cadastro com Nome, Dono, Propriedade Mágica, Raridade (0 a 100) e ID numérico gerenciado automaticamente. Suporta textos com espaços e sanitização com `trim`.
- [x] **Grafo de Similaridade:** Cadastro do peso de similaridade entre pares de itens (de 0 a 10), armazenado como matriz de adjacências (vetor `semelhanca` em cada nó). Relação simétrica, sem laços (auto-similaridade proibida) e com validação de existência dos itens.
- [x] **Busca por Similaridade:** Lista os itens que não pertencem ao jogador informado e possuem similaridade estritamente maior que um limiar *X* com um item selecionado.

### 🌳 Entregas Parciais 3 & 4 (Árvores Binárias e Geometria)
- [x] **Duas Árvores Binárias de Busca (ABB):** Cada item cadastrado é inserido simultaneamente em duas árvores:
  - `raiz`: árvore ordenada alfabeticamente pelo nome do item.
  - `raiz_raridade`: árvore ordenada pelo valor da raridade.
- [x] **Listagem Alfabética (Opção 5):** Impressão de todos os itens cadastrados em ordem alfabética através do percurso em ordem (*in-order*) na árvore `raiz`, desconsiderando diferenças entre maiúsculas e minúsculas.
- [x] **Busca Específica (Opção 4):** Verificação rápida da existência de um item pelo nome na árvore `raiz` em tempo $O(h)$.
- [x] **Listagem por Raridade (Opção 6):** Impressão dos itens em ordem decrescente de raridade através do percurso invertido (direita $\to$ nó $\to$ esquerda) na árvore `raiz_raridade`.
- [x] **Agrupamento e Contagem por Propriedade Mágica (Opções 7 e 8):** Busca recursiva na árvore para listar e contar quantos itens possuem determinada propriedade mágica informada.
- [x] **Limpeza de Inventário (Opção 9):** Remoção em cascata de todos os itens com raridade menor que um limite *R*:
  - Desalocação e desvinculação da lista encadeada;
  - Remoção em ambas as árvores com reestruturação de nós (sucessor imediato);
  - Zeramento das arestas no grafo de similaridade;
  - Liberação do ID para reaproveitamento em futuros cadastros.
- [x] **Validação Geométrica (Polígono e Abertura):**
  - Teste de convexidade do contorno com $N$ vértices (3 a 20) via orientação de pontos (determinante/produto vetorial);
  - Cálculo de área por triangulação em leque;
  - Cálculo do menor círculo delimitador (duplas e triplas de pontos) para garantir que o item cabe na abertura circular da bolsa (`RAIO_ABERTURA = 10.0`).
- [x] **Gerenciamento Completo de Memória:** Função `liberar_tudo` que desaloca recursivamente todos os nós de ambas as árvores e todos os nós da lista encadeada antes do encerramento do programa, evitando qualquer vazamento de memória (*memory leaks*).

### ⭐ Funcionalidades Extras (Avançado)
- [ ] **Balanceamento de Árvore:** Rotações automáticas (AVL ou Rubro-Negra).
- [ ] **Fecho Convexo (Convex Hull):** Determinação automática dos vértices convexos a partir dos pixels de uma imagem.

---

## 🧭 Menu do Sistema

| Opção | Ação | Status | Descrição |
|:-----:|------|:------:|-----------|
| 1 | Inserir item | ✅ | Valida contorno geométrico, gera ID livre e insere na lista e nas duas árvores |
| 2 | Cadastrar similaridade de itens | ✅ | Registra peso de similaridade simétrico entre múltiplos pares de IDs |
| 3 | Buscar itens similares | ✅ | Lista itens de outros jogadores com similaridade $> X$ em relação a um item |
| 4 | Verificar a existência de um item | ✅ | Busca binária rápida por nome na árvore `raiz` |
| 5 | Listar itens em ordem alfabética | ✅ | Percurso em ordem (*in-order*) na árvore `raiz` |
| 6 | Listar itens em ordem decrescente de raridade | ✅ | Percurso invertido na árvore `raiz_raridade` |
| 7 | Itens com a mesma propriedade mágica | ✅ | Lista itens filtrados por propriedade mágica |
| 8 | Contar itens com a mesma propriedade mágica | ✅ | Contabiliza ocorrências de uma propriedade mágica |
| 9 | Remover itens menos raros | ✅ | Remove itens com raridade $< R$ de todas as estruturas e libera os IDs |
| 10 | Sair | ✅ | Desaloca toda a memória alocada dinamicamente e encerra a execução |

---

## 💻 Tecnologias e Estruturas de Dados
- **Linguagem:** C++ (padrão C++11 ou superior)
- **Bibliotecas padrão:** `<iostream>`, `<string>`, `<cmath>`
- **Estruturas implementadas manualmente com ponteiros:**
  - **Lista Encadeada Simples:** Mantém a ordem de cadastro e ponteiros `inicio` e `fim`.
  - **Árvores Binárias de Busca (ABB):** Duas árvores simultâneas com nós apontando para os mesmos itens da memória (`Item* data`).
  - **Grafo por Matriz de Adjacência:** Vetor `semelhanca[MAX_ITENS]` embutido em cada item, indexado diretamente pelo ID do item adjacente.
- **Geometria Computacional:**
  - Teste de orientação de três pontos no plano (`lado`);
  - Algoritmo de convexidade de polígonos (`eh_poligono_convexo`);
  - Cálculo de área poligonal via soma de áreas com sinal de triângulos (`area_poligono`);
  - Algoritmo do menor círculo delimitador por duplas e triplas de vértices (`menor_circulo_r2`).

---

## ⚙️ Robustez e Diferenciais de Implementação
- **Leitura com espaços:** Funções `lerTextoNaoVazio` e `trim` permitem cadastrar nomes compostos (ex.: *"Espada Longa Flamejante"*), ao contrário do `cin >>` convencional.
- **Reaproveitamento de IDs:** Vetor booleano `id_em_uso` e função `obter_menor_id_livre` garantem que IDs liberados pela remoção sejam reutilizados, mantendo os identificadores compactos entre `0` e `MAX_ITENS - 1`.
- **Validação de Entrada:** Tratamento com `cin.clear()` e `cin.ignore()` para impedir travamento por tipos de dados incorretos.

---

## 🚀 Como Executar

### Pré-requisitos
- Compilador C++ com suporte a C++11 (ex.: GCC/G++, Clang ou MSVC).

### Passos
1. Clone o repositório:
   ```bash
   git clone https://github.com/carolferreirauai/projeto_c04_inventario.git
   cd projeto_c04_inventario/Project
   ```

2. Compile o código-fonte:
   ```bash
   g++ -std=c++11 main.cpp -o inventario
   ```

3. Execute o programa:
   - **Linux / macOS:**
     ```bash
     ./inventario
     ```
   - **Windows:**
     ```powershell
     .\inventario.exe
     ```

---

## 📚 Documentação Adicional
- [Anatomia do Código (Markdown)](Project/Anatomia_do_Codigo.md): Explicação aprofundada de cada função, estrutura e algoritmo utilizado no projeto.
- [Anatomia do Código (PDF)](Project/Anatomia_do_Codigo.pdf): Versão diagramada em PDF da documentação do código.
- [Proposta da Atividade](Proposta%20da%20Atividade/Inventário.PDF): Enunciado original com as especificações da disciplina.
