#include <stdio.h>
#include "bd_times.h"
#include "bd_partidas.h"

int main(){
    char escolha;

    BancoTimes *banco_times = inicializarBancoTimes();
    BD_Partidas *banco_partidas = inicializarBanco();
    
    importarTimes("Partidas-times/times.csv", banco_times);
    importarDadosArquivo("Partidas-times/partidas_completo.csv", banco_partidas);
    
    // Zera antes de processar pela primeira vez
    zerarEstatisticasTimes(banco_times);
    processarEstatisticas(banco_partidas, banco_times);
    
    while (1)
    {
        exibirTelaInicial();
        scanf(" %c", &escolha);

        if(escolha == 'q' || escolha == 'Q') {
            printf("Encerrando sistema...\n");
            break;
        }

        if (escolha == '1') {
            buscarTimes(banco_times);
        } else if (escolha == '2') {
            buscarPartidas(banco_partidas, banco_times);
        } else if (escolha == '3') {
            atualizarPartida(banco_partidas, banco_times, "Partidas-times/partidas_completo.csv");
        } else if (escolha == '4') {
            removerPartida(banco_partidas, banco_times, "Partidas-times/partidas_completo.csv");
        } else if (escolha == '5') {
            inserirPartida(banco_partidas, banco_times, "Partidas-times/partidas_completo.csv");
        } else if (escolha == '6') {
            mostrarTabelaClassificacao(banco_times);
        } else {
            printf("Opcao invalida\n");
        }
    }

    liberarAlocacaoTimes(banco_times);
    liberarAlocacaoDados(banco_partidas);
    
    return 0;
}