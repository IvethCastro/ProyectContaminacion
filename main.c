#include <stdio.h>
#include "funciones.h"

int main() {
    struct Zona zonas[ZONAS];
    int opcion;

    inicializarZonas(zonas);

    do {
        printf("\n=== MENU ===\n");
        printf("1. Ingresar contaminacion actual\n");
        printf("2. Prediccion niveles futuros\n");
        printf("3. Alertas y recomendaciones\n");
        printf("4. Promedios historicos\n");
        printf("5. Exportar reporte\n");
        printf("0. Salir\n");
        printf("Opcion: ");
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
        }
    } while (opcion != 0);

    return 0;
}
