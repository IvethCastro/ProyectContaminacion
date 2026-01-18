#include <stdio.h>
#include <string.h>
#include "funciones.h"

/* ================= VALIDACIONES ================= */

int leerEnteroConRango(int inicio, int fin){
    int num, val;
    do{
        val = scanf("%d", &num);
        if(val != 1 || num < inicio || num > fin){
            printf("Valor incorrecto. Intente nuevamente: ");
            fflush(stdin);
        }
    } while(val != 1 || num < inicio || num > fin);
    return num;
}

float leerFlotanteConRango(float inicio, float fin){
    float num;
    int val;
    do{
        val = scanf("%f", &num);
        if(val != 1 || num < inicio || num > fin){
            printf("Valor incorrecto. Intente nuevamente: ");
            fflush(stdin);
        }
    } while(val != 1 || num < inicio || num > fin);
    return num;
}

/* ================= INICIALIZACIÓN ================= */

void inicializarZonas(Zona zonas[]) {
    char nombres[ZONAS][40] = {
        "Centro Historico",
        "Valle de los Chillos",
        "Chillogallo",
        "Carapungo",
        "La Carolina"
    };

    for (int i = 0; i < ZONAS; i++) {
        strcpy(zonas[i].nombre, nombres[i]);
        zonas[i].hayPrediccion = 0;
    }
}

void guardarDatosHistoricos(Zona zonas[]) {
    FILE *f = fopen("historico.dat", "wb");
    if (f == NULL) return;
    fwrite(zonas, sizeof(Zona), ZONAS, f);
    fclose(f);
}

void cargarDatosHistoricos(Zona zonas[]) {
    FILE *f = fopen("historico.dat", "rb");

    if (f == NULL) {
        for (int i = 0; i < ZONAS; i++) {
            for (int j = 0; j < DIAS; j++) {
                zonas[i].historial.dias[j].pm25 = 12 + i;
                zonas[i].historial.dias[j].so2  = 20 + i;
                zonas[i].historial.dias[j].no2  = 35 + i;
                zonas[i].historial.dias[j].co2  = 420 + i * 10;
            }
        }
        guardarDatosHistoricos(zonas);
    } else {
        fread(zonas, sizeof(Zona), ZONAS, f);
        fclose(f);
    }
}

void mostrarZonas(Zona zonas[]) {
    printf("\nID\tZONA\n");
    for (int i = 0; i < ZONAS; i++) {
        printf("%d\t%s\n", i, zonas[i].nombre);
    }
}

/* ================= OPCIÓN 1 ================= */

void ingresarContaminacionActual(Zona zonas[]) {
    int id, opc;
    Contaminacion c;

    do {
        mostrarZonas(zonas);

        printf("Ingrese ID de la zona: ");
        id = leerEnteroConRango(0, ZONAS - 1);

        printf("PM2.5 actual: ");
        c.pm25 = leerFlotanteConRango(0, 500);

        printf("SO2 actual: ");
        c.so2 = leerFlotanteConRango(0, 500);

        printf("NO2 actual: ");
        c.no2 = leerFlotanteConRango(0, 500);

        printf("CO2 actual (350-2000): ");
        c.co2 = leerFlotanteConRango(350, 2000);

        /* Desplazar historial */
        for (int i = 0; i < DIAS - 1; i++) {
            zonas[id].historial.dias[i] = zonas[id].historial.dias[i + 1];
        }
        zonas[id].historial.dias[DIAS - 1] = c;

        guardarDatosHistoricos(zonas);

        /* MONITOREO INMEDIATO */
        monitoreoActual(&zonas[id]);

        printf("\n¿Desea ingresar otra zona? (1. Si / 2. No): ");
        opc = leerEnteroConRango(1, 2);

    } while (opc == 1);
}


void monitoreoActual(Zona *z) {
    Contaminacion c = z->historial.dias[DIAS - 1];

    printf("\n--- MONITOREO ACTUAL: %s ---\n", z->nombre);

    /* PM2.5 */
    if (c.pm25 > LIM_PM25)
        printf("ALERTA: PM2.5 supera limite OMS (%.2f ug/m3)\n", c.pm25);
    else
        printf("PM2.5 aceptable (%.2f ug/m3)\n", c.pm25);

    /* SO2 */
    if (c.so2 > LIM_SO2)
        printf("ALERTA: SO2 supera limite OMS (%.2f ug/m3)\n", c.so2);
    else
        printf("SO2 aceptable (%.2f ug/m3)\n", c.so2);

    /* NO2 */
    if (c.no2 > LIM_NO2)
        printf("ALERTA: NO2 supera limite OMS (%.2f ug/m3)\n", c.no2);
    else
        printf("NO2 aceptable (%.2f ug/m3)\n", c.no2);

    /* CO2 */
    if (c.co2 <= 420) {
        printf("CO2 normal (%.0f ppm)\n", c.co2);
    }
    else if (c.co2 <= 450) {
        printf("CO2 ligeramente elevado (%.0f ppm)\n", c.co2);
    }
    else if (c.co2 <= 500) {
        printf("CO2 elevado (%.0f ppm)\n", c.co2);
    }
    else {
        printf("ALERTA: CO2 anomalo para aire exterior (%.0f ppm)\n", c.co2);
    }
}




/* ================= PREDICCIÓN ================= */

float promedioPonderado(float valores[]) {
    float pesos[10] = {0.20,0.15,0.12,0.10,0.09,0.08,0.07,0.07,0.06,0.06};
    float suma = 0;
    int idx = DIAS - 1;

    for (int i = 0; i < 10; i++) {
        suma += valores[idx--] * pesos[i];
    }
    return suma;
}

float calcularAQI(float C, float Clow, float Chigh, float Ilow, float Ihigh) {
    return ((Ihigh - Ilow) / (Chigh - Clow)) * (C - Clow) + Ilow;
}

float aqiPM25(float c) {
    if (c <= 15) return calcularAQI(c, 0, 15, 0, 50);
    else if (c <= 25) return calcularAQI(c, 15, 25, 51, 100);
    else if (c <= 50) return calcularAQI(c, 25, 50, 101, 150);
    else return 200;
}

float aqiSO2(float c) {
    if (c <= 40) return calcularAQI(c, 0, 40, 0, 50);
    else if (c <= 80) return calcularAQI(c, 40, 80, 51, 100);
    else if (c <= 380) return calcularAQI(c, 80, 380, 101, 150);
    else return 200;
}

float aqiNO2(float c) {
    if (c <= 40) return calcularAQI(c, 0, 40, 0, 50);
    else if (c <= 100) return calcularAQI(c, 40, 100, 51, 100);
    else if (c <= 200) return calcularAQI(c, 100, 200, 101, 150);
    else return 200;
}

const char* nivelCO2Exterior(float c) {
    if (c <= 420) return "Nivel ambiental normal";
    else if (c <= 450) return "Ligeramente elevado";
    else if (c <= 500) return "Elevado";
    else return "Anomalo para aire exterior";
}

const char* interpretacionAQI(float aqi) {
    if (aqi <= 50) return "Bueno";
    else if (aqi <= 100) return "Moderado";
    else if (aqi <= 150) return "Dañino (grupos sensibles)";
    else return "Dañino";
}

float factorClimatico(float temp, float viento, float humedad) {
    float factor = 1.0;
    if (temp > 25) factor += 0.05;
    if (viento < 2) factor += 0.05;
    if (humedad > 70) factor += 0.05;
    return factor;
}

void guardarPrediccion(Zona zonas[], int id) {
    FILE *f = fopen("predicciones.dat", "ab");
    if (f == NULL) {
        printf("Error al guardar prediccion.\n");
        return;
    }

    fwrite(&zonas[id], sizeof(Zona), 1, f);
    fclose(f);
}


void prediccion(Zona zonas[], int id) {
    float temp, viento, humedad;
    float fc;

    float pm25[DIAS], so2[DIAS], no2[DIAS], co2[DIAS];

    /* === 1. INGRESO DE FACTORES CLIMÁTICOS === */
    printf("\nIngrese temperatura actual (°C) (-10-40): ");
    temp = leerFlotanteConRango(-10, 40);

    printf("Ingrese velocidad del viento (m/s) (0-50): ");
    viento = leerFlotanteConRango(0, 50);

    printf("Ingrese humedad (%%) (0-100): ");
    humedad = leerFlotanteConRango(0, 100);

    /* === 2. FACTOR CLIMÁTICO === */
    fc = factorClimatico(temp, viento, humedad);

    /* === 3. EXTRAER HISTÓRICOS A ARREGLOS === */
    for (int i = 0; i < DIAS; i++) {
        pm25[i] = zonas[id].historial.dias[i].pm25;
        so2[i]  = zonas[id].historial.dias[i].so2;
        no2[i]  = zonas[id].historial.dias[i].no2;
        co2[i]  = zonas[id].historial.dias[i].co2;
    }

    /* === 4. PROMEDIO PONDERADO (ÚLTIMOS 10 DÍAS) === */
    float pm25_pond = promedioPonderado(pm25);
    float so2_pond  = promedioPonderado(so2);
    float no2_pond  = promedioPonderado(no2);
    float co2_pond  = promedioPonderado(co2);

    /* === 5. PROYECCIÓN AJUSTADA POR CLIMA === */
    zonas[id].proy.valor.pm25 = pm25_pond * fc;
    zonas[id].proy.valor.so2  = so2_pond  * fc;
    zonas[id].proy.valor.no2  = no2_pond  * fc;
    zonas[id].proy.valor.co2  = co2_pond  * fc;

    /* === 6. AQI === */
    zonas[id].proy.aqi_pm25 = aqiPM25(zonas[id].proy.valor.pm25);
    zonas[id].proy.aqi_so2  = aqiSO2(zonas[id].proy.valor.so2);
    zonas[id].proy.aqi_no2  = aqiNO2(zonas[id].proy.valor.no2);

    /* === 7. IMPRESIÓN === */
    printf("\nPROYECCION DE CONTAMINACION - %s\n", zonas[id].nombre);
    printf("Contaminante   Proyeccion (concentracion)       Resultado\n");
    printf("------------------------------------------------------------\n");

    printf("PM2.5          %.2f ug/m3                       %.0f (%s)\n",
           zonas[id].proy.valor.pm25,
           zonas[id].proy.aqi_pm25,
           interpretacionAQI(zonas[id].proy.aqi_pm25));

    printf("SO2            %.2f ug/m3                       %.0f (%s)\n",
           zonas[id].proy.valor.so2,
           zonas[id].proy.aqi_so2,
           interpretacionAQI(zonas[id].proy.aqi_so2));

    printf("NO2            %.2f ug/m3                       %.0f (%s)\n",
           zonas[id].proy.valor.no2,
           zonas[id].proy.aqi_no2,
           interpretacionAQI(zonas[id].proy.aqi_no2));

    printf("CO2            %.0f ppm                         %s\n",
           zonas[id].proy.valor.co2,
           nivelCO2Exterior(zonas[id].proy.valor.co2));

    zonas[id].hayPrediccion = 1;

    /* === 8. GUARDAR PREDICCIÓN EN ARCHIVO === */
    guardarPrediccion(zonas, id);
}



void alertasRecomendaciones(Zona zonas[]) {
    int hayAlerta;

    printf("\n--- ALERTAS Y RECOMENDACIONES ---\n");

    for (int i = 0; i < ZONAS; i++) {

        if (!zonas[i].hayPrediccion)
            continue;

        hayAlerta = 0;

        if (zonas[i].proy.aqi_pm25 > 100) hayAlerta = 1;
        if (zonas[i].proy.aqi_so2  > 100) hayAlerta = 1;
        if (zonas[i].proy.aqi_no2  > 100) hayAlerta = 1;
        if (zonas[i].proy.valor.co2 > 420) hayAlerta = 1;

        if (!hayAlerta)
            continue;   /* No imprime zonas aceptables */

        printf("\n================================\n");
        printf("Zona: %s\n", zonas[i].nombre);
        printf("================================\n");

        /* -------- PM2.5 -------- */
        if (zonas[i].proy.aqi_pm25 > 100) {
            printf("ALERTA PM2.5 (%.2f ug/m3)\n", zonas[i].proy.valor.pm25);
            printf("- Evitar actividades al aire libre\n");
            printf("- Uso de mascarilla\n\n");
        }

        /* -------- SO2 -------- */
        if (zonas[i].proy.aqi_so2 > 100) {
            printf("ALERTA SO2 (%.2f ug/m3)\n", zonas[i].proy.valor.so2);
            printf("- Reducir trafico vehicular\n");
            printf("- Control de emisiones\n\n");
        }

        /* -------- NO2 -------- */
        if (zonas[i].proy.aqi_no2 > 100) {
            printf("ALERTA NO2 (%.2f ug/m3)\n", zonas[i].proy.valor.no2);
            printf("- Evitar ejercicio al aire libre\n");
            printf("- Uso de mascarilla\n\n");
        }

        /* -------- CO2 -------- */
        if (zonas[i].proy.valor.co2 > 420 && zonas[i].proy.valor.co2 <= 450) {
            printf("CO2 ligeramente elevado (%.0f ppm)\n", zonas[i].proy.valor.co2);
            printf("- Optimizar flujo vehicular\n");
            printf("- Incrementar vegetacion urbana\n\n");
        }
        else if (zonas[i].proy.valor.co2 > 450 && zonas[i].proy.valor.co2 <= 500) {
            printf("CO2 elevado (%.0f ppm)\n", zonas[i].proy.valor.co2);
            printf("- Restringir circulacion vehicular\n");
            printf("- Revisar fuentes emisoras\n\n");
        }
        else if (zonas[i].proy.valor.co2 > 500) {
            printf("CO2 anomalo para aire exterior (%.0f ppm)\n", zonas[i].proy.valor.co2);
            printf("- Intervenir fuente inmediata\n");
            printf("- Mejorar ventilacion urbana\n\n");
        }
    }

}

void promediosHistoricos(Zona zonas[]) {
    FILE *f = fopen("historico.dat", "rb");
    Zona zonasArchivo[ZONAS];

    if (f == NULL) {
        printf("No se pudo abrir el archivo historico\n");
        return;
    }

    fread(zonasArchivo, sizeof(Zona), ZONAS, f);
    fclose(f);

    printf("\n--- PROMEDIOS HISTORICOS (30 DIAS) ---\n");

    for (int i = 0; i < ZONAS; i++) {
        float sumaPM = 0, sumaSO2 = 0, sumaNO2 = 0, sumaCO2 = 0;

        printf("\nZona: %s\n", zonasArchivo[i].nombre);

        for (int d = 0; d < DIAS; d++) {
            sumaPM  += zonasArchivo[i].historial.dias[d].pm25;
            sumaSO2 += zonasArchivo[i].historial.dias[d].so2;
            sumaNO2 += zonasArchivo[i].historial.dias[d].no2;
            sumaCO2 += zonasArchivo[i].historial.dias[d].co2;
        }

        printf("PM2.5 promedio: %.2f ug/m3\n", sumaPM / DIAS);
        printf("SO2 promedio:   %.2f ug/m3\n", sumaSO2 / DIAS);
        printf("NO2 promedio:   %.2f ug/m3\n", sumaNO2 / DIAS);
        printf("CO2 promedio:   %.0f ppm\n",  sumaCO2 / DIAS);
    }
}

void exportarReporte(Zona zonas[]) {
    FILE *f = fopen("reporte.txt", "w");

    if (f == NULL) {
        printf("Error al crear el archivo de reporte.\n");
        return;
    }

    fprintf(f, "=============================================\n");
    fprintf(f, "        REPORTE DE CALIDAD DEL AIRE\n");
    fprintf(f, "        Proyeccion 24 horas - Quito\n");
    fprintf(f, "=============================================\n\n");

    for (int i = 0; i < ZONAS; i++) {

        if (!zonas[i].hayPrediccion)
            continue;

        fprintf(f, "Zona: %s\n", zonas[i].nombre);
        fprintf(f, "---------------------------------------------\n");

        fprintf(f, "PM2.5: %.2f ug/m3 | AQI: %.0f (%s)\n",
                zonas[i].proy.valor.pm25,
                zonas[i].proy.aqi_pm25,
                interpretacionAQI(zonas[i].proy.aqi_pm25));

        fprintf(f, "SO2:   %.2f ug/m3 | AQI: %.0f (%s)\n",
                zonas[i].proy.valor.so2,
                zonas[i].proy.aqi_so2,
                interpretacionAQI(zonas[i].proy.aqi_so2));

        fprintf(f, "NO2:   %.2f ug/m3 | AQI: %.0f (%s)\n",
                zonas[i].proy.valor.no2,
                zonas[i].proy.aqi_no2,
                interpretacionAQI(zonas[i].proy.aqi_no2));

        fprintf(f, "CO2:   %.0f ppm | %s\n",
                zonas[i].proy.valor.co2,
                nivelCO2Exterior(zonas[i].proy.valor.co2));

        fprintf(f, "\n");
    }

    fclose(f);

    printf("Reporte exportado correctamente en 'reporte.txt'\n");
}


