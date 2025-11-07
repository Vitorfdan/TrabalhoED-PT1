#include <stdio.h>
#include "bd_times.h"
#include "bd_partidas.h"

int main(){
    char escolha;

    BancoTimes *banco_times = inicializarBancoTimes();
    BD_Partidas *banco_partidas = inicializarBanco();
    
    importarTimes("Partidas-times/times.csv", banco_times);
    importarDadosArquivo("Partidas-times/partidas_completo.csv", banco_partidas);
    
    processarEstatisticas(banco_partidas, banco_times);
    
    while (1)
    {
        exibirTelaInicial();
        scanf(" %c", &escolha);

        if(escolha == 'q' || escolha == 'Q') {
            printf("Encerrando sistema...\n");
            break;
        }

        switch(escolha) {
            case '1':
                buscarTimes(banco_times);
                break;
            case '2':
                buscarPartidas(banco_partidas, banco_times);
                break;
            case '3':
                printf("em desenvolvimento\n");
                break;
            case '4':
                printf("em desenvolvimento\n");
                break;
            case '5':
                printf("em desenvolvimento\n");
                break;
            case '6':
                mostrarTabelaClassificacao(banco_times);
                break;
            default:
                printf("Opcao invalida\n");
        }
    }

    liberarAlocacaoTimes(banco_times);
    liberarAlocacaoDados(banco_partidas);
    
    return 0;
}