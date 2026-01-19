#include <stdio.h>
#include "funciones.h"

int main() {
    Zona zonas[ZONAS];
    int opcion;
    int id;

    inicializarZonas(zonas);
    cargarDatosHistoricos(zonas);

    do {
        printf("\n================ MENU =================\n");
        printf("1. Ingresar datos y Monitoreo contaminacion actual\n");
        printf("2. Proyeccion de contaminacion (24h)\n");
        printf("3. Alertas y recomendaciones\n");
        printf("4. Promedios historicos (30 dias)\n");
        printf("5. Exportar reporte\n");
        printf("6. Mostrar datos actuales\n");
        printf("0. Salir\n");
        printf("======================================\n");
        printf("Seleccione una opcion: ");
        opcion = leerEnteroConRango(0, 6);

        switch (opcion) {

            case 1:
                ingresarContaminacionActual(zonas);
                break;

            case 2:
                
                prediccion(zonas);
                break;

            case 3:
                alertasRecomendaciones(zonas);
                break;

            case 4:
                promediosHistoricos(zonas);
                break;

            case 5:
                exportarReporte(zonas);
                break;
            case 6:
                mostrarDatosActuales(zonas);
                break;
            case 0:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 0);

    return 0;
}

