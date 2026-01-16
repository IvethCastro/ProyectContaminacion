#include <stdio.h>
#include "funciones.h"

/* -------- LIMITES OMS -------- */
float LIM_PM25 = 15;
float LIM_SO2  = 40;
float LIM_NO2  = 25;

void inicializarZonas(struct Zona zonas[]) {
    char nombres[ZONAS][30] = {
        "Centro Historico",
        "Valle de Los Chillos",
        "Chillogallo",
        "Carapungo",
        "La Carolina"
    };

    for (int i = 0; i < ZONAS; i++) {
        int j = 0;
        while (nombres[i][j] != '\0') {
            zonas[i].nombre[j] = nombres[i][j];
            j++;
        }
        zonas[i].nombre[j] = '\0';
        zonas[i].tieneDatos = 0;
    }
}

/* -------------------------------------------------- */
/* CARGA AUTOMATICA DE 30 DIAS HISTORICOS */

void cargarHistoricoInicial() {
    FILE *f = fopen("historico.dat", "rb");
    if (f != NULL) {
        fclose(f);
        return;
    }

    f = fopen("historico.dat", "wb");
    struct RegistroHistorico r;

    float basePM[ZONAS]  = {18, 16, 20, 17, 15};
    float baseSO2[ZONAS] = {30, 28, 35, 32, 27};
    float baseNO2[ZONAS] = {22, 20, 25, 23, 19};

    for (int z = 0; z < ZONAS; z++) {
        for (int d = 0; d < DIAS_HIST; d++) {
            r.zona = z;
            r.datos.pm25 = basePM[z]  + (d % 5) * 0.3;
            r.datos.so2  = baseSO2[z] + (d % 4) * 0.5;
            r.datos.no2  = baseNO2[z] + (d % 3) * 0.4;
            fwrite(&r, sizeof(r), 1, f);
        }
    }
    fclose(f);
}

/* -------------------------------------------------- */
/* OPCION 1 - INGRESO Y MONITOREO ACTUAL */

void ingresarContaminacion(struct Zona zonas[]) {
    int z, opc;

    do {
        printf("\nID  Zona\n");
        for (int i = 0; i < ZONAS; i++)
            printf("%d   %s\n", i, zonas[i].nombre);

        printf("Ingrese ID de la zona: ");
        scanf("%d", &z);

        printf("PM2.5 actual: ");
        scanf("%f", &zonas[z].actual.pm25);
        printf("SO2 actual: ");
        scanf("%f", &zonas[z].actual.so2);
        printf("NO2 actual: ");
        scanf("%f", &zonas[z].actual.no2);

        zonas[z].tieneDatos = 1;

        monitoreoActual(&zonas[z]);
        guardarHistorico(z, zonas[z].actual);

        printf("\n¿Desea ingresar otra zona? (1=Si / 2=No): ");
        scanf("%d", &opc);

    } while (opc == 1);
}

/* -------------------------------------------------- */

void monitoreoActual(struct Zona *z) {
    printf("\nMonitoreo actual - %s\n", z->nombre);

    if (z->actual.pm25 > LIM_PM25)
        printf("ALERTA: PM2.5 supera limite OMS\n");
    else
        printf("PM2.5 aceptable\n");

    if (z->actual.so2 > LIM_SO2)
        printf("ALERTA: SO2 supera limite OMS\n");
    else
        printf("SO2 aceptable\n");

    if (z->actual.no2 > LIM_NO2)
        printf("ALERTA: NO2 supera limite OMS\n");
    else
        printf("NO2 aceptable\n");
}

/* -------------------------------------------------- */

void guardarHistorico(int zona, struct Contaminacion c) {
    FILE *f = fopen("historico.dat", "ab");
    struct RegistroHistorico r;

    r.zona = zona;
    r.datos = c;

    fwrite(&r, sizeof(r), 1, f);
    fclose(f);
}

/* -------------------------------------------------- */
/* OPCION 2 - PREDICCION */

void prediccionFutura(struct Zona zonas[]) {
    int z;
    struct Clima clima;
    FILE *f = fopen("historico.dat", "rb");
    struct RegistroHistorico r;

    float pm[DIAS_HIST], so2[DIAS_HIST], no2[DIAS_HIST];
    int cont = 0;

    printf("\nID  Zona\n");
    for (int i = 0; i < ZONAS; i++)
        printf("%d   %s\n", i, zonas[i].nombre);

    printf("Ingrese ID de la zona: ");
    scanf("%d", &z);

    while (fread(&r, sizeof(r), 1, f)) {
        if (r.zona == z && cont < DIAS_HIST) {
            pm[cont]  = r.datos.pm25;
            so2[cont] = r.datos.so2;
            no2[cont] = r.datos.no2;
            cont++;
        }
    }
    fclose(f);

    if (cont == 0) {
        printf("No hay datos historicos para esta zona.\n");
        return;
    }

    printf("Temperatura actual (°C): ");
    scanf("%f", &clima.temperatura);
    printf("Velocidad del viento (km/h): ");
    scanf("%f", &clima.viento);
    printf("Humedad (%%): ");
    scanf("%f", &clima.humedad);

    zonas[z].prediccion.pm25 = pm[cont-1] * (1 + clima.humedad / 200);
    zonas[z].prediccion.so2  = so2[cont-1] * (1 + clima.temperatura / 100);
    zonas[z].prediccion.no2  = no2[cont-1] * (1 - clima.viento / 100);

    guardarPrediccion(z, zonas[z].prediccion, clima);

    printf("\nPrediccion de contaminacion (24 horas):\n");
    printf("PM2.5: %.2f\n", zonas[z].prediccion.pm25);
    printf("SO2: %.2f\n", zonas[z].prediccion.so2);
    printf("NO2: %.2f\n", zonas[z].prediccion.no2);
}

/* -------------------------------------------------- */

void guardarPrediccion(int zona, struct Contaminacion p, struct Clima c) {
    FILE *f = fopen("predicciones.dat", "ab");
    struct RegistroPrediccion r;

    r.zona = zona;
    r.prediccion = p;
    r.clima = c;

    fwrite(&r, sizeof(r), 1, f);
    fclose(f);
}

/* -------------------------------------------------- */
/* OPCION 3 - ALERTAS Y RECOMENDACIONES */

void alertasYRecomendaciones(struct Zona zonas[]) {
    int hayAlerta;

    for (int i = 0; i < ZONAS; i++) {

        if (!zonas[i].tieneDatos)
            continue;

        hayAlerta = 0;

        if (zonas[i].prediccion.pm25 > LIM_PM25) hayAlerta = 1;
        if (zonas[i].prediccion.so2  > LIM_SO2)  hayAlerta = 1;
        if (zonas[i].prediccion.no2  > LIM_NO2)  hayAlerta = 1;

        if (!hayAlerta)
            continue;

        printf("\nZona: %s\n", zonas[i].nombre);

        if (zonas[i].prediccion.pm25 > LIM_PM25) {
            printf("PM2.5 elevado:\n");
            printf("- Usar mascarilla\n");
            printf("- Evitar exposicion prolongada\n");
            printf("- Suspender actividades al aire libre\n");
        }

        if (zonas[i].prediccion.so2 > LIM_SO2) {
            printf("SO2 elevado:\n");
            printf("- Reducir trafico vehicular\n");
            printf("- Control de emisiones industriales\n");
        }

        if (zonas[i].prediccion.no2 > LIM_NO2) {
            printf("NO2 elevado:\n");
            printf("- Evitar ejercicio al aire libre\n");
            printf("- Uso de mascarilla\n");
        }
    }
}

/* -------------------------------------------------- */
/* OPCION 4 - PROMEDIOS HISTORICOS */

void promediosHistoricos(struct Zona zonas[]) {
    FILE *f = fopen("historico.dat", "rb");
    struct RegistroHistorico r;

    float sumaPM[ZONAS] = {0}, sumaSO2[ZONAS] = {0}, sumaNO2[ZONAS] = {0};
    int cont[ZONAS] = {0};

    while (fread(&r, sizeof(r), 1, f)) {
        sumaPM[r.zona]  += r.datos.pm25;
        sumaSO2[r.zona] += r.datos.so2;
        sumaNO2[r.zona] += r.datos.no2;
        cont[r.zona]++;
    }
    fclose(f);

    for (int i = 0; i < ZONAS; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);

        if (cont[i] == 0) {
            printf("No hay datos historicos suficientes\n");
            continue;
        }

        printf("Prom PM2.5 (ultimos 30 dias): %.2f\n", sumaPM[i] / cont[i]);
        printf("Prom SO2 (ultimos 30 dias): %.2f\n", sumaSO2[i] / cont[i]);
        printf("Prom NO2 (ultimos 30 dias): %.2f\n", sumaNO2[i] / cont[i]);
    }
}

/* -------------------------------------------------- */
/* OPCION 5 - REPORTE */

void exportarReporte(struct Zona zonas[]) {
    FILE *f = fopen("reporte.txt", "w");

    fprintf(f, "REPORTE DE CONTAMINACION DEL AIRE\n");
    fprintf(f, "================================\n\n");

    for (int i = 0; i < ZONAS; i++) {

        if (!zonas[i].tieneDatos)
            continue;

        fprintf(f, "Zona: %s\n\n", zonas[i].nombre);

        fprintf(f, "Contaminacion actual:\n");
        fprintf(f, "PM2.5: %.2f\n", zonas[i].actual.pm25);
        fprintf(f, "SO2: %.2f\n", zonas[i].actual.so2);
        fprintf(f, "NO2: %.2f\n\n", zonas[i].actual.no2);

        fprintf(f, "Prediccion 24 horas:\n");
        fprintf(f, "PM2.5: %.2f\n", zonas[i].prediccion.pm25);
        fprintf(f, "SO2: %.2f\n", zonas[i].prediccion.so2);
        fprintf(f, "NO2: %.2f\n", zonas[i].prediccion.no2);

        fprintf(f, "\n--------------------------------\n\n");
    }

    fclose(f);
    printf("Reporte exportado correctamente.\n");
}



