#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bd_partidas.h"
#include "bd_times.h"

// Função que Desaloca memória 
void liberarAlocacaoDados(BD_Partidas *bd) {
    int total = bd->qntd;
    for (int idx = total - 1; idx >= 0; idx--) {
        Partidas *item = bd->partidas[idx];
        free(item);
    }
    free(bd);
}

// Função para exibir a Tela principal
void exibirTelaInicial(){
    printf("*** SISTEMA DE JOGOS ***\n");
    printf("\n");
    printf("(1) Buscar time\n");
    printf("(2) Buscar partida\n");
    printf("(3) Atualizar partida\n");
    printf("(4) Remover partida\n");
    printf("(5) Inserir partida\n");
    printf("(6) Visualizar classificacao\n");
    printf("(Q) Finalizar programa\n");
    printf("Digite sua escolha: ");
}

// Inicializa banco de dados
BD_Partidas *inicializarBanco(){
    BD_Partidas *novo_banco = malloc(sizeof(BD_Partidas));
    if (!novo_banco) {
        printf("Falha ao alocar memoria\n");
        exit(1);
    }
    return novo_banco;
}

// Cria nova partida no vetor
Partidas *construirPartida(int codigo, int equipe_a, int equipe_b, int placar_a, int placar_b){
    Partidas *nova_partida = malloc(sizeof(Partidas));
    
    if (nova_partida == NULL) {
        perror("falha ao alocar\n");
        exit(1);
    }
    
    nova_partida->gols_time1 = placar_a;
    nova_partida->gols_time2 = placar_b;
    nova_partida->time1 = equipe_a; //Inicializando os ponteiros/valores dentro da estrutura
    nova_partida->time2 = equipe_b;
    nova_partida->id = codigo;

    return nova_partida;
}

// Registra nova partida no vetor
void inserirNovaPartida(BD_Partidas *bd, int id, int time1, int time2, int gols_time1, int gols_time2){
    int espacos_disponiveis = LIMITE_JOGOS - bd->qntd;
    
    if (espacos_disponiveis <= 0){
        printf("Capacidade maxima atingida\n");
        exit(1);
    }
    
    Partidas *registro = construirPartida(id, time1, time2, gols_time1, gols_time2);
    int *contador = &(bd->qntd);
    bd->partidas[*contador] = registro;
    *contador = *contador + 1;
}

// Mostra menu de busca 
void mostrarOpcoesDeBusca(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printf("*** OPCOES DE BUSCA ***\n");
    printf("(1) Time como mandante\n");
    printf("(2) Time como visitante\n");
    printf("(3) Qualquer participacao\n");
    printf("(4) Retornar\n");
}

// Verifica se time participa da partida (como mandante, visitante ou ambos)
int verificarTimeNaPartida(const char *nome_busca, Partidas *jogo, struct BancoTimes *dados, int tipo){
    int posicao_time;
    
    // tipo: 1=mandante, 2=visitante, 3=qualquer
    if (tipo == 1) {
        posicao_time = jogo->time1;
    } else if (tipo == 2) {
        posicao_time = jogo->time2;
    } else {
        // Checa ambos
        int achou_mandante = verificarTimeNaPartida(nome_busca, jogo, dados, 1);
        int achou_visitante = verificarTimeNaPartida(nome_busca, jogo, dados, 2);
        return (achou_mandante || achou_visitante) ? 1 : 0;
    }
    
    Times *time = buscarTimePorID(dados, posicao_time);
    if (time == NULL) {
        return 0;
    }
    char *nome_time = time->nome_do_time;
    
    // Compara prefixo do nome dos times
    int i = 0;
    while (nome_busca[i] != '\0') {
        if (nome_time[i] != nome_busca[i]) {
            return 0;
        }
        i++;
    }
    
    return 1;
}

// Realiza busca de partidas dentro do vetor
void buscarPartidas(BD_Partidas *dados_partidas, struct BancoTimes *dados_times){
    int selecao;
    char termo_busca[100];

    mostrarOpcoesDeBusca();
    scanf("%d", &selecao);

    if (selecao == 4) {
        printf("Voltando ao menu...\n");
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        return;
    }

    int opcao_valida = (selecao >= 1 && selecao <= 3);
    if (!opcao_valida) {
        printf("Opcao invalida\n");
        return;
    }

    printf("\nDigite o nome ou prefixo do time: ");
    scanf("%s", termo_busca);

    int contador_resultados = 0;
    
    printf("\n+-----+--------------------------------------------------+\n");
    printf("| ID  | Partida                                          |\n");
    printf("+-----+--------------------------------------------------+\n");
    
    for (int idx = 0; idx < dados_partidas->qntd; idx++) {
        Partidas *jogo_atual = dados_partidas->partidas[idx];
        
        // Verifica se o time participa da partida
        int encontrou = verificarTimeNaPartida(termo_busca, jogo_atual, dados_times, selecao);

        if (encontrou != 0) {

            Times *time_casa = buscarTimePorID(dados_times, jogo_atual->time1);
            Times *time_fora = buscarTimePorID(dados_times, jogo_atual->time2);
            
            char *nome_casa = time_casa ? time_casa->nome_do_time : "Desconhecido";
            char *nome_fora = time_fora ? time_fora->nome_do_time : "Desconhecido";
            
            printf("| %-3d | %-20s %2d x %-2d %-20s |\n", 
                   jogo_atual->id, nome_casa, jogo_atual->gols_time1,
                   jogo_atual->gols_time2, nome_fora);
            
            contador_resultados++;
        }
    }
    
    printf("+-----+--------------------------------------------------+\n");
    
    if (contador_resultados == 0) {
        printf("\nNenhuma partida encontrada.\n");
    }
}

// importa dados do arquivo
void importarDadosArquivo(const char *bd_partidas, BD_Partidas *dados){
    FILE *arquivo_dados = fopen(bd_partidas, "r");
    
    if (arquivo_dados == NULL){
        printf("Nao foi possivel abrir %s\n", bd_partidas);
        exit(1);
    }

    char linha_lida[LIMITE_JOGOS];
    int valores[5];

    fgets(linha_lida, sizeof(linha_lida), arquivo_dados);
    dados->qntd = 0;

    while (1) {
        char *resultado_leitura = fgets(linha_lida, sizeof(linha_lida), arquivo_dados);
        
        if (resultado_leitura == NULL) {
            break;
        }
        
        int campos_lidos = sscanf(linha_lida, "%d, %d, %d, %d, %d", 
                                  &valores[0], &valores[1], &valores[2], &valores[3], &valores[4]);
        
        if (campos_lidos == 5) {
            inserirNovaPartida(dados, valores[0], valores[1], valores[2], valores[3], valores[4]);
        }
    }

    fclose(arquivo_dados);
}

// salva dados no arquivo
int salvarDadosArquivo(const char *bd_partidas, BD_Partidas *dados){
    FILE *arquivo_dados = fopen(bd_partidas, "w");
    
    if (arquivo_dados == NULL){
        return -1; // Retorna erro
    }

    // Escreve o cabeçalho
    fprintf(arquivo_dados, "Id,Time1,Time2,Placar1,Placar2\n");

    // Escreve todas as partidas
    for (int i = 0; i < dados->qntd; i++) {
        Partidas *p = dados->partidas[i];
        fprintf(arquivo_dados, "%d, %d, %d, %d, %d\n", 
                p->id, p->time1, p->time2, p->gols_time1, p->gols_time2);
    }

    fclose(arquivo_dados);
    return 0; // Sucesso
}

// Busca partida por ID
Partidas *buscarPartidaPorID(BD_Partidas *bd, int id) {
    for (int i = 0; i < bd->qntd; i++) {
        if (bd->partidas[i]->id == id) {
            return bd->partidas[i];
        }
    }
    return NULL;
}

// Atualiza os dados de uma partida
void atualizarPartida(BD_Partidas *dados_partidas, struct BancoTimes *dados_times, const char *caminho_arquivo){
    int id_busca;
    
    printf("\n[Sistema]\n");
    printf("ID  Time1       Time2       Placar1  Placar2\n");
    
    // Exibe TODAS as partidas
    for (int i = 0; i < dados_partidas->qntd; i++) {
        Partidas *p = dados_partidas->partidas[i];
        Times *time1 = buscarTimePorID(dados_times, p->time1);
        Times *time2 = buscarTimePorID(dados_times, p->time2);
        
        char *nome1 = time1 ? time1->nome_do_time : "Desconhecido";
        char *nome2 = time2 ? time2->nome_do_time : "Desconhecido";
        
        printf("%-3d %-11s %-11s %-8d %d\n", 
               p->id, nome1, nome2, p->gols_time1, p->gols_time2);
    }
    
    printf("\nDigite o ID do registro a ser atualizado:\n");
    printf("\n[Usuario]\n");
    scanf("%d", &id_busca);
    
    Partidas *partida = buscarPartidaPorID(dados_partidas, id_busca);
    
    if (partida == NULL) {
        printf("\n[Sistema]\nPartida nao encontrada.\n");
        return;
    }
    
    Times *time1 = buscarTimePorID(dados_times, partida->time1);
    Times *time2 = buscarTimePorID(dados_times, partida->time2);
    
    char *nome_time1 = time1 ? time1->nome_do_time : "Desconhecido";
    char *nome_time2 = time2 ? time2->nome_do_time : "Desconhecido";
    
    printf("\n[Sistema]\n");
    printf("Digite o novo valor para os campos Placar1 e Placar2 (para\n");
    printf("manter o valor atual de um campo, digite '-'):\n");
    printf("\n[Usuario]\n");
    
    char input1[10], input2[10];
    scanf("%s", input1);
    scanf("%s", input2);
    
    int novo_placar1 = partida->gols_time1;
    int novo_placar2 = partida->gols_time2;
    
    if (strcmp(input1, "-") != 0) {
        novo_placar1 = atoi(input1);
    }
    
    if (strcmp(input2, "-") != 0) {
        novo_placar2 = atoi(input2);
    }
    
    printf("\n[Sistema]\n");
    printf("Confirma os novos valores para o registro abaixo? (S/N)\n");
    printf("\nID  Time1       Time2       Placar1  Placar2\n");
    printf("%-3d %-11s %-11s %-8d %d\n", 
           partida->id, nome_time1, nome_time2, novo_placar1, novo_placar2);
    printf("\n[Usuario]\n");
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    
    if (confirmacao == 'S' || confirmacao == 's') {
        partida->gols_time1 = novo_placar1;
        partida->gols_time2 = novo_placar2;
        
        // Salva as alterações usando o mesmo caminho que foi usado para carregar
        salvarDadosArquivo(caminho_arquivo, dados_partidas);
        
        // ZERA todas as estatísticas antes de recalcular
        zerarEstatisticasTimes((BancoTimes *)dados_times);
        
        // Recalcula as estatísticas do zero
        processarEstatisticas(dados_partidas, (BancoTimes *)dados_times);
        
        printf("\n[Sistema]\n");
        printf("Registro atualizado com sucesso.\n");
        printf("Classificacao atualizada.\n");
    } else {
        printf("\n[Sistema]\n");
        printf("Atualizacao cancelada.\n");
    }
}

// Remove uma partida do sistema
void removerPartida(BD_Partidas *dados_partidas, struct BancoTimes *dados_times, const char *caminho_arquivo){
    int id_busca;
    
    printf("\n[Sistema]\n");
    printf("ID  Time1       Time2       Placar1  Placar2\n");
    
    // Exibe TODAS as partidas
    for (int i = 0; i < dados_partidas->qntd; i++) {
        Partidas *p = dados_partidas->partidas[i];
        Times *time1 = buscarTimePorID(dados_times, p->time1);
        Times *time2 = buscarTimePorID(dados_times, p->time2);
        
        char *nome1 = time1 ? time1->nome_do_time : "Desconhecido";
        char *nome2 = time2 ? time2->nome_do_time : "Desconhecido";
        
        printf("%-3d %-11s %-11s %-8d %d\n", 
               p->id, nome1, nome2, p->gols_time1, p->gols_time2);
    }
    
    printf("\nDigite o ID do registro a ser removido:\n");
    printf("\n[Usuario]\n");
    scanf("%d", &id_busca);
    
    // Busca a partida e seu índice no vetor
    int indice_encontrado = -1;
    Partidas *partida = NULL;
    
    for (int i = 0; i < dados_partidas->qntd; i++) {
        if (dados_partidas->partidas[i]->id == id_busca) {
            partida = dados_partidas->partidas[i];
            indice_encontrado = i;
            break;
        }
    }
    
    if (partida == NULL) {
        printf("\n[Sistema]\nPartida nao encontrada.\n");
        return;
    }
    
    Times *time1 = buscarTimePorID(dados_times, partida->time1);
    Times *time2 = buscarTimePorID(dados_times, partida->time2);
    
    char *nome_time1 = time1 ? time1->nome_do_time : "Desconhecido";
    char *nome_time2 = time2 ? time2->nome_do_time : "Desconhecido";
    
    printf("\n[Sistema]\n");
    printf("Tem certeza de que deseja excluir o registro abaixo? (S/N)\n");
    printf("\nID  Time1       Time2       Placar1  Placar2\n");
    printf("%-3d %-11s %-11s %-8d %d\n", 
           partida->id, nome_time1, nome_time2, partida->gols_time1, partida->gols_time2);
    printf("\n[Usuario]\n");
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    
    if (confirmacao == 'S' || confirmacao == 's') {
        // Libera a memória da partida
        free(partida);
        
        // Move todos os elementos após o índice removido uma posição para trás
        for (int i = indice_encontrado; i < dados_partidas->qntd - 1; i++) {
            dados_partidas->partidas[i] = dados_partidas->partidas[i + 1];
        }
        
        // Decrementa a quantidade
        dados_partidas->qntd--;
        
        // Salva as alterações no arquivo
        salvarDadosArquivo(caminho_arquivo, dados_partidas);
        
        // ZERA todas as estatísticas antes de recalcular
        zerarEstatisticasTimes((BancoTimes *)dados_times);
        
        // Recalcula as estatísticas do zero
        processarEstatisticas(dados_partidas, (BancoTimes *)dados_times);
        
        printf("\n[Sistema]\n");
        printf("Registro removido com sucesso.\n");
    } else {
        printf("\n[Sistema]\n");
        printf("Remocao cancelada.\n");
    }
}

// Insere uma nova partida no sistema
void inserirPartida(BD_Partidas *dados_partidas, struct BancoTimes *dados_times, const char *caminho_arquivo){
    int time1, time2, placar1, placar2;
    
    printf("\n[Sistema]\n");
    printf("Para inserir um novo registro, digite os valores para\n");
    printf("os campos Time1, Time2, Placar1 e Placar2:\n");
    printf("\n[Usuario]\n");
    
    scanf("%d", &time1);
    scanf("%d", &time2);
    scanf("%d", &placar1);
    scanf("%d", &placar2);
    
    // Valida se os times existem
    Times *validacao_time1 = buscarTimePorID(dados_times, time1);
    Times *validacao_time2 = buscarTimePorID(dados_times, time2);
    
    if (validacao_time1 == NULL || validacao_time2 == NULL) {
        printf("\n[Sistema]\nTime invalido.\n");
        return;
    }
    
    // Valida se os placares são não-negativos
    if (placar1 < 0 || placar2 < 0) {
        printf("\n[Sistema]\nPlacar invalido.\n");
        return;
    }
    
    // Gera o próximo ID automaticamente
    int novo_id = 0;
    for (int i = 0; i < dados_partidas->qntd; i++) {
        if (dados_partidas->partidas[i]->id >= novo_id) {
            novo_id = dados_partidas->partidas[i]->id + 1;
        }
    }
    
    char *nome_time1 = validacao_time1->nome_do_time;
    char *nome_time2 = validacao_time2->nome_do_time;
    
    printf("\n[Sistema]\n");
    printf("Confirma a insercao do registro abaixo? (S/N)\n");
    printf("\nID  Time1       Time2       Placar1  Placar2\n");
    printf("%-3d %-11s %-11s %-8d %d\n", 
           novo_id, nome_time1, nome_time2, placar1, placar2);
    printf("\n[Usuario]\n");
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    
    if (confirmacao == 'S' || confirmacao == 's') {
        // Insere a nova partida
        inserirNovaPartida(dados_partidas, novo_id, time1, time2, placar1, placar2);
        
        // Salva as alterações no arquivo
        salvarDadosArquivo(caminho_arquivo, dados_partidas);
        
        // ZERA todas as estatísticas antes de recalcular
        zerarEstatisticasTimes((BancoTimes *)dados_times);
        
        // Recalcula as estatísticas do zero
        processarEstatisticas(dados_partidas, (BancoTimes *)dados_times);
        
        printf("\n[Sistema]\n");
        printf("O registro foi inserido com sucesso.\n");
    } else {
        printf("\n[Sistema]\n");
        printf("Insercao cancelada.\n");
    }
}
