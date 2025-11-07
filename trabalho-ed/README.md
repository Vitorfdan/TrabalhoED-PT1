# Trabalho Prático — Estrutura de Dados
## Sistema de Gerenciamento de Times e Partidas

Este projeto foi desenvolvido como parte da disciplina **Estrutura de Dados**, com o objetivo de aplicar os conceitos de **TADs (Tipos Abstratos de Dados)**, ponteiros, modularização e manipulação de arquivos em linguagem C.

O sistema simula o gerenciamento de um campeonato de futebol, permitindo a consulta de times e partidas, além da visualização da tabela de classificação, a partir da leitura de arquivos CSV utilizando estruturas dinâmicas e organização modular de código.

---

## 📁 Estrutura do Projeto

O projeto está organizado da seguinte forma:

```
trabalho-ed/
│
├── include/
│   ├── bd_partidas.h
│   ├── bd_times.h
│   └── times.h
│
├── src/
│   ├── main.c
│   ├── bd_partidas.c
│   ├── bd_times.c
│   └── times.c
│
├── Partidas-times/
│   ├── times.csv
│   ├── partidas_vazio.csv
│   ├── partidas_parcial.csv
│   └── partidas_completo.csv
│
├── arquivosGerados/
│   └── [arquivos .o gerados na compilação]
│
├── Makefile
├── GerenciadorPartidas.exe
├── app.py
└── README.md
```

---

## ⚙️ Estrutura dos Componentes

**main.c**  
Contém o fluxo principal do programa, o menu interativo e a integração entre os módulos do sistema.

**bd_times.h/c**  
Define o TAD responsável por armazenar e manipular os dados dos times, incluindo identificação, nome, vitórias, empates, derrotas, gols marcados/sofridos, saldo e pontuação.

**bd_partidas.h/c**  
Define o TAD responsável pelo gerenciamento das partidas, com dados como identificadores dos times, gols de cada equipe e métodos de busca por mandante, visitante ou qualquer participação.

**times.h/c**  
Implementa funções auxiliares para manipular e exibir informações dos times, além de atualizar as estatísticas de cada equipe conforme os resultados das partidas.

**times.csv e partidas_*.csv**  
Arquivos nos quais os dados dos times e das partidas são armazenados em formato CSV, de onde o sistema busca as informações para processamento.

**Makefile**  
Automatiza o processo de compilação, limpeza e execução do sistema.

---

## 🧠 Conceitos Utilizados

- Tipos Abstratos de Dados (TADs)
- Alocação dinâmica de memória
- Ponteiros e estruturas compostas
- Manipulação e leitura de arquivos texto (CSV)
- Modularização e organização de código
- Controle de fluxo com menus interativos
- Compatibilidade multiplataforma (Windows/Linux)

---

## 🧩 Funcionalidades do Sistema

O sistema foi projetado para gerenciar times e partidas de um campeonato de forma simples e modular.  
As principais funcionalidades implementadas são:

### Menu Interativo
```
*** SISTEMA DE JOGOS ***

(1) Buscar time
(2) Buscar partida
(6) Visualizar classificacao
(Q) Finalizar programa
```

### Funcionalidades Detalhadas

**Carregamento de dados**
- Leitura automática dos arquivos CSV ao iniciar o sistema
- Verificação de erros ao abrir e processar os arquivos
- Suporte a múltiplos cenários (vazio, parcial, completo)

**Busca de times**
- Busca por prefixo do nome do time
- Comparação case-sensitive caractere por caractere
- Exibição dos times encontrados em formato tabular

**Busca de partidas**
- Submenu com opções de filtro:
  - (1) Time como mandante
  - (2) Time como visitante
  - (3) Qualquer participação
  - (4) Retornar ao menu principal
- Exibição formatada com ID, times e placar

**Visualização da classificação**
- Tabela completa com estatísticas de todos os times
- Informações: jogos, vitórias, empates, derrotas, gols pró/contra, saldo e pontos
- Cálculo automático de pontuação (3 pontos por vitória, 1 por empate)

**Estrutura modular e reutilizável**
- Cada funcionalidade separada em seu próprio módulo
- Facilita manutenção e extensão do sistema
- Código limpo e bem comentado

---

## 💻 Compilação e Execução

O projeto utiliza um **Makefile** para simplificar a compilação e execução.

### Comandos Principais

| Ação | Comando |
|------|---------|
| Compilar o projeto | `make` |
| Executar o programa | `make run ARGS="Partidas-times/times.csv Partidas-times/partidas_completo.csv"` |
| Limpar arquivos objeto | `make clean` |
| Recompilar do zero | `make clean && make` |


### Execução

O programa requer **dois argumentos obrigatórios**:
1. Caminho do arquivo de times
2. Caminho do arquivo de partidas

**Exemplos:**

```bash
# Windows - Cenário completo
.\GerenciadorPartidas.exe Partidas-times\times.csv Partidas-times\partidas_completo.csv

# Linux - Cenário parcial
./GerenciadorPartidas Partidas-times/times.csv Partidas-times/partidas_parcial.csv
```

---

## 📄 Formato dos Arquivos CSV

### times.csv
```csv
ID,Time
0,JAVAlis
1,ESCorpiões
2,SemCTRL
```

### partidas_*.csv
```csv
ID,Time1ID,Time2ID,GolsTime1,GolsTime2
0,5,7,4,1
1,9,1,2,5
```

**Observações:**
- Arquivos devem estar em codificação UTF-8
- Espaços após vírgulas são aceitos
- Linhas malformadas são ignoradas automaticamente

---

## 🪟 Ambiente de Desenvolvimento

- **Sistema Operacional:** Windows / Linux
- **Compilador:** GCC
- **Execução:** Terminal/PowerShell compatível com UTF-8
- **Observação Windows:** Execute `chcp 65001` antes de rodar o programa para exibição correta de acentos

---

## 🔮 Melhorias Futuras

- [ ] Implementar opções 3, 4, 5 do menu (adicionar/editar/excluir partidas)
- [ ] Ordenação da classificação por pontos, saldo e gols


