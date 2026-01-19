#include <stdio.h>
#include "funciones.h"

int main() {
    Zona zonas[MAX_ZONAS];
    int totalZonas = 0;
    int opcion;
    
    cargarZonas(zonas, &totalZonas);
    cargarDatosHistoricos(zonas, totalZonas);

    do {
        
        printf("\n========= SISTEMA DE MONITOREO AMBIENTAL - QUITO =========\n");
        printf("1. Mostrar zonas\n");
        printf("2. Añadir zona\n");
        printf("3. Ingresar datos y Monitoreo contaminacion actual\n");
        printf("4. Mostrar datos actuales\n");
        printf("5. Proyeccion de contaminacion (24h)\n");
        printf("6. Alertas y recomendaciones\n");
        printf("7. Promedios historicos (30 dias) \n");
        printf("8. Exportar reporte\n");
        printf("9. Salir\n");
        printf("Seleccione una opcion: ");

        opcion = leerEnteroConRango(1, 9);

        switch(opcion){

            case 1:
                mostrarZonas(zonas, totalZonas);
                break;

            case 2:
                anadirZona(zonas, &totalZonas);
                break;

            case 3:
                ingresarContaminacionActual(zonas, totalZonas);
                break;

            case 4:
                mostrarDatosActuales(zonas, totalZonas);
                break;

            case 5:
                prediccion(zonas, totalZonas);
                break;

            case 6:
                alertasRecomendaciones(zonas, totalZonas);
                break;

            case 7:
                promediosHistoricos(zonas, totalZonas);
                break;

            case 8:
                exportarReporte(zonas, totalZonas);
                break;

            case 9:
                printf("\nSaliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 9);

    return 0;
}

