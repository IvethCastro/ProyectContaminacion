#include <stdio.h>
#include "funciones.h"

int main() {
    Zona zonas[ZONAS];
    int opcion;

    /* Inicializacion */
    inicializarZonas(zonas);
    cargarHistoricoInicial();

    do {
        printf("\n SISTEMA DE GESTION DE CONTAMINACION\n");
        printf("====================================\n");
        printf("1. Ingresar contaminacion actual\n");
        printf("2. Prediccion de niveles futuros (24h)\n");
        printf("3. Alertas y recomendaciones\n");
        printf("4. Promedios historicos (30 dias)\n");
        printf("5. Exportar reporte\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {

            case 1:
                ingresarContaminacion(zonas);
                break;

            case 2:
                prediccionFutura(zonas);
                break;

            case 3:
                alertasYRecomendaciones(zonas);
                break;

            case 4:
                promediosHistoricos(zonas);
                break;

            case 5:
                exportarReporte(zonas);
                break;

            case 6:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while (opcion != 6);

    return 0;
}


