#include <stdio.h>
#include <string.h>
#include "funciones.h"

void monitoreoActual(Zona *z);


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

int soloLetras(char *cadena){
    int i = 0;
    while(cadena[i] != '\0'){
        if(!((cadena[i] >= 'A' && cadena[i] <= 'Z') || 
             (cadena[i] >= 'a' && cadena[i] <= 'z') ||
              cadena[i] == ' ')){
            return 0;
        }
        i++;
    }
    return 1;
}


void guardarZonas(Zona zonas[], int totalZonas) {
    FILE *f = fopen("zonas.dat", "wb");
    if (f == NULL) {
        printf("Error al guardar zonas.\n");
        return;
    }

    fwrite(&totalZonas, sizeof(int), 1, f);
    fwrite(zonas, sizeof(Zona), totalZonas, f);

    fclose(f);
}

void cargarZonas(Zona zonas[], int *totalZonas) {
    FILE *f = fopen("zonas.dat", "rb");

    if (f == NULL) {
        // NO existe el archivo → crear zonas iniciales
        char nombres[5][40] = {
            "Centro Historico",
            "Valle de los Chillos",
            "Chillogallo",
            "Carapungo",
            "La Carolina"
        };

        for (int i = 0; i < 5; i++) {
            strcpy(zonas[i].nombre, nombres[i]);
            zonas[i].hayPrediccion = 0;
            zonas[i].tieneDatos = 0;
            zonas[i].tieneClima = 0;
        }

        *totalZonas = 5;
        guardarZonas(zonas, *totalZonas);
        return;
    }

    fread(totalZonas, sizeof(int), 1, f);
    fread(zonas, sizeof(Zona), *totalZonas, f);

    fclose(f);
}

void mostrarZonas(Zona zonas[], int totalZonas){
    printf("\nID\tZONA\n");
    for(int i = 0; i < totalZonas; i++){
        printf("%d\t%s\n", i, zonas[i].nombre);
    }
} 

void guardarDatosHistoricos(Zona zonas[], int totalZonas){
    FILE *f = fopen("historico.dat", "wb");

    if(f == NULL){
        printf("Error al guardar archivo historico\n");
        return;
    }

    fwrite(zonas, sizeof(Zona), totalZonas, f);
    fclose(f);
}


void cargarDatosHistoricos(Zona zonas[], int totalZonas){
    FILE *f = fopen("historico.dat", "rb");

    if(f == NULL){
        for(int i = 0; i < totalZonas; i++){
            for(int j = 0; j < DIAS; j++){
                zonas[i].historial.dias[j].pm25 = 12 + i;
                zonas[i].historial.dias[j].so2  = 20 + i;
                zonas[i].historial.dias[j].no2  = 35 + i;
                zonas[i].historial.dias[j].co2  = 420 + i * 10;
            }
        }
        guardarDatosHistoricos(zonas, totalZonas);
    } else {
        fread(zonas, sizeof(Zona), totalZonas, f);
        fclose(f);
    }
}


void anadirZona(Zona zonas[], int *totalZonas) {

    if (*totalZonas >= MAX_ZONAS) {
        printf("\nNo se pueden agregar mas zonas.\n");
        return;
    }

    char nombre[40];

    printf("\n============= AÑADIR ZONA =============\n");

    do {
        printf("Ingrese el nombre de la nueva zona de Quito: ");
        fflush(stdin);
        fgets(nombre, 40, stdin);
        nombre[strcspn(nombre, "\n")] = 0;

        if (!soloLetras(nombre)) {
            printf("Nombre invalido. Use solo letras y espacios.\n");
        }

    } while (!soloLetras(nombre));

    strcpy(zonas[*totalZonas].nombre, nombre);

    zonas[*totalZonas].hayPrediccion = 0;
    zonas[*totalZonas].tieneDatos = 0;
    zonas[*totalZonas].tieneClima = 0;

    printf("\nZona añadida correctamente.\n");
    printf("ID asignado: %d\n", *totalZonas);

    (*totalZonas)++;

    guardarZonas(zonas, *totalZonas);
}



void ingresarContaminacionActual(Zona zonas[], int totalZonas) {
    int id, opc;
    Contaminacion c;

    do {
        mostrarZonas(zonas, totalZonas);

        printf("Ingrese ID de la zona: ");
        id = leerEnteroConRango(0, totalZonas - 1);

        /* ----- CONTAMINANTES ----- */
        printf("PM2.5 actual (0-200 ug/m3): ");
        c.pm25 = leerFlotanteConRango(0, 200);

        printf("SO2 actual (0-300 ug/m3): ");
        c.so2 = leerFlotanteConRango(0, 300);

        printf("NO2 actual (0-200 ug/m3): ");
        c.no2 = leerFlotanteConRango(0, 200);

        printf("CO2 actual (350-600 ppm): ");
        c.co2 = leerFlotanteConRango(350, 600);

        /* ----- CLIMA ACTUAL ----- */
        printf("Temperatura actual (C) (-10 a 40): ");
        zonas[id].clima.temperatura = leerFlotanteConRango(-10, 40);

        printf("Velocidad del viento (m/s) (0 a 50): ");
        zonas[id].clima.viento = leerFlotanteConRango(0, 50);

        printf("Humedad relativa (%%) (0 a 100): ");
        zonas[id].clima.humedad = leerFlotanteConRango(0, 100);

        zonas[id].tieneClima = 1;

        /* ----- DESPLAZAR HISTORIAL (30 DÍAS) ----- */
        for (int i = 0; i < DIAS - 1; i++) {
            zonas[id].historial.dias[i] = zonas[id].historial.dias[i + 1];
        }
        zonas[id].historial.dias[DIAS - 1] = c;
        
        zonas[id].actual = c;        // Guardar contaminantes actuales
        zonas[id].tieneDatos = 1;
        
        guardarDatosHistoricos(zonas, totalZonas);
        monitoreoActual(&zonas[id]);

        printf("\nDesea ingresar otra zona? (1. Si / 2. No): ");
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


void prediccion(Zona zonas[], int totalZonas) {
    int id;
    float fc;

    float pm25[DIAS], so2[DIAS], no2[DIAS], co2[DIAS];

    mostrarZonas(zonas, totalZonas);
    printf("Ingrese ID de la zona: ");
    id = leerEnteroConRango(0, totalZonas - 1);

    fc = factorClimatico(
            zonas[id].clima.temperatura,
            zonas[id].clima.viento,
            zonas[id].clima.humedad
         );

    for (int i = 0; i < DIAS; i++) {
        pm25[i] = zonas[id].historial.dias[i].pm25;
        so2[i]  = zonas[id].historial.dias[i].so2;
        no2[i]  = zonas[id].historial.dias[i].no2;
        co2[i]  = zonas[id].historial.dias[i].co2;
    }

    /* === 3. PROMEDIO PONDERADO (ÚLTIMOS 10 DÍAS) === */
    float pm25_pond = promedioPonderado(pm25);
    float so2_pond  = promedioPonderado(so2);
    float no2_pond  = promedioPonderado(no2);
    float co2_pond  = promedioPonderado(co2);

    /* === 4. PROYECCIÓN AJUSTADA POR CLIMA === */
    zonas[id].proy.valor.pm25 = pm25_pond * fc;
    zonas[id].proy.valor.so2  = so2_pond  * fc;
    zonas[id].proy.valor.no2  = no2_pond  * fc;
    zonas[id].proy.valor.co2  = co2_pond  * fc;

    /* === 5. AQI === */
    zonas[id].proy.aqi_pm25 = aqiPM25(zonas[id].proy.valor.pm25);
    zonas[id].proy.aqi_so2  = aqiSO2(zonas[id].proy.valor.so2);
    zonas[id].proy.aqi_no2  = aqiNO2(zonas[id].proy.valor.no2);

    /* === 6. IMPRESIÓN === */
    printf("\nPROYECCION DE CONTAMINACION - %s\n", zonas[id].nombre);
    printf("Contaminante   Proyeccion (concentracion)       Resultado (AQI)\n");
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

    
    guardarPrediccion(zonas, id);
}


void alertasRecomendaciones(Zona zonas[], int totalZonas) {
    int hayAlerta;
    printf("\n--- ALERTAS Y RECOMENDACIONES ---\n");

    for(int i=0;i<totalZonas;i++){
        if(!zonas[i].hayPrediccion) continue;

        hayAlerta=0;
        if(zonas[i].proy.aqi_pm25>100) hayAlerta=1;
        if(zonas[i].proy.aqi_so2>100)  hayAlerta=1;
        if(zonas[i].proy.aqi_no2>100)  hayAlerta=1;
        if(zonas[i].proy.valor.co2>420) hayAlerta=1;
        if(!hayAlerta) continue;

        printf("\n================================\n");
        printf("Zona: %s\n", zonas[i].nombre);
        printf("================================\n");

        // -------- PM2.5 --------
        if(zonas[i].proy.aqi_pm25>100){
            printf("ALERTA PM2.5 (%.2f ug/m3)\n", zonas[i].proy.valor.pm25);
            printf("- Global: Reducir emisiones industriales y de vehiculos.\n");
            printf("- Cultural: Usar mascarilla en espacios abiertos.\n");
            printf("- Social: Evitar actividades al aire libre.\n");
            printf("- Ambiental: Controlar fuentes de combustion.\n\n");
        }

        // -------- SO2 --------
        if(zonas[i].proy.aqi_so2>100){
            printf("ALERTA SO2 (%.2f ug/m3)\n", zonas[i].proy.valor.so2);
            printf("- Global: Controlar procesos industriales.\n");
            printf("- Cultural: Promover uso de energias limpias.\n");
            printf("- Social: Reducir trafico pesado.\n");
            printf("- Ambiental: Mitigar focos de emision.\n\n");
        }

        // -------- NO2 --------
        if(zonas[i].proy.aqi_no2>100){
            printf("ALERTA NO2 (%.2f ug/m3)\n", zonas[i].proy.valor.no2);
            printf("- Global: Reducir circulacion vehicular.\n");
            printf("- Cultural: Realizar movilidad sostenible.\n");
            printf("- Social: Evitar ejercicio al aire libre.\n");
            printf("- Ambiental: Mejorar ventilación urbana.\n\n");
        }

        // -------- CO2 --------
        if(zonas[i].proy.valor.co2>420){
            if(zonas[i].proy.valor.co2<=450)
                printf("CO2 ligeramente elevado (%.0f ppm)\n", zonas[i].proy.valor.co2);
            else if(zonas[i].proy.valor.co2<=500)
                printf("CO2 elevado (%.0f ppm)\n", zonas[i].proy.valor.co2);
            else
                printf("CO2 anomalo para aire exterior (%.0f ppm)\n", zonas[i].proy.valor.co2);

            printf("- Global: Optimizar trafico vehicular.\n");
            printf("- Cultural: Reducir consumo energetico.\n");
            printf("- Social: Limitar desplazamientos.\n");
            printf("- Ambiental: Incrementar areas verdes.\n\n");
        }
    }
}


void promediosHistoricos(Zona zonas[], int totalZonas) {
    
    printf("\n--- PROMEDIOS HISTORICOS (30 DIAS) ---\n");

    for (int i = 0; i < totalZonas; i++) {
        float sumaPM = 0, sumaSO2 = 0, sumaNO2 = 0, sumaCO2 = 0;

        printf("\nZona: %s\n", zonas[i].nombre);

        for (int d = 0; d < DIAS; d++) {
            sumaPM  += zonas[i].historial.dias[d].pm25;
            sumaSO2 += zonas[i].historial.dias[d].so2;
            sumaNO2 += zonas[i].historial.dias[d].no2;
            sumaCO2 += zonas[i].historial.dias[d].co2;
        }

        printf("PM2.5 promedio: %.2f ug/m3\n", sumaPM / DIAS);
        printf("SO2 promedio:   %.2f ug/m3\n", sumaSO2 / DIAS);
        printf("NO2 promedio:   %.2f ug/m3\n", sumaNO2 / DIAS);
        printf("CO2 promedio:   %.0f ppm\n",  sumaCO2 / DIAS);
    }
}

void exportarReporte(Zona zonas[], int totalZonas) {
    FILE *f = fopen("reporte.txt", "w");

    if (f == NULL) {
        printf("Error al crear el archivo de reporte.\n");
        return;
    }

    fprintf(f, "=============================================\n");
    fprintf(f, "   REPORTE DE CALIDAD DEL AIRE - QUITO\n");
    fprintf(f, "=============================================\n\n");

    for (int i = 0; i < totalZonas; i++) {

        Contaminacion actual = zonas[i].historial.dias[DIAS - 1];

        fprintf(f, "---------------------------------------------\n");
        fprintf(f, "Zona: %s\n", zonas[i].nombre);
        fprintf(f, "---------------------------------------------\n");

        fprintf(f, "DATOS ACTUALES:\n");
        fprintf(f, "PM2.5: %.2f ug/m3\n", actual.pm25);
        fprintf(f, "SO2:   %.2f ug/m3\n", actual.so2);
        fprintf(f, "NO2:   %.2f ug/m3\n", actual.no2);
        fprintf(f, "CO2:   %.0f ppm\n",  actual.co2);

        if (!zonas[i].tieneClima) {
            fprintf(f, "Clima actual (Temperatura, viento y humedad): Aún no se ingresa un dato actual.\n");
        } else {
            fprintf(f, "Temperatura: %.1f °C\n", zonas[i].clima.temperatura);
            fprintf(f, "Viento: %.1f m/s\n", zonas[i].clima.viento);
            fprintf(f, "Humedad: %.1f %%\n", zonas[i].clima.humedad);
        }


        /* ===== PREDICCIONES ===== */
        if (zonas[i].hayPrediccion) {
            fprintf(f, "\nPREDICCIONES:\n");

            fprintf(f, "PM2.5 proyectado: %.2f ug/m3 | AQI: %.0f (%s)\n",
                    zonas[i].proy.valor.pm25,
                    zonas[i].proy.aqi_pm25,
                    interpretacionAQI(zonas[i].proy.aqi_pm25));

            fprintf(f, "SO2 proyectado:   %.2f ug/m3 | AQI: %.0f (%s)\n",
                    zonas[i].proy.valor.so2,
                    zonas[i].proy.aqi_so2,
                    interpretacionAQI(zonas[i].proy.aqi_so2));

            fprintf(f, "NO2 proyectado:   %.2f ug/m3 | AQI: %.0f (%s)\n",
                    zonas[i].proy.valor.no2,
                    zonas[i].proy.aqi_no2,
                    interpretacionAQI(zonas[i].proy.aqi_no2));

            fprintf(f, "CO2 proyectado:   %.0f ppm | %s\n",
                    zonas[i].proy.valor.co2,
                    nivelCO2Exterior(zonas[i].proy.valor.co2));
        } else {
            fprintf(f, "\nPREDICCIONES:\n");
            fprintf(f, "No se ha generado prediccion para esta zona.\n");
        }

        fprintf(f, "\n");
    }

    fclose(f);
    printf("\nReporte exportado correctamente a 'reporte.txt'\n");
}

void mostrarDatosActuales(Zona zonas[], int totalZonas) {
    int hayDatos = 0;
    for (int i = 0; i < totalZonas; i++) {
        if (zonas[i].tieneDatos) {
            hayDatos = 1;
            break;
        }
    }

    if (!hayDatos) {
        printf("\n No existen datos actuales registrados.\n");
        return;
    }

    printf("\n================ DATOS ACTUALES =================\n");
    printf("Zona               PM2.5   SO2     NO2     CO2   Temp  Viento  Humedad\n");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < totalZonas ; i++) {

        if (!zonas[i].tieneDatos)
            continue;   

        Contaminacion c = zonas[i].actual;  

        printf("%s\t\t%.1f\t%.1f\t%.1f\t%.0f\t%.1f\t%.1f\t%.1f\n",
       zonas[i].nombre,
       c.pm25,
       c.so2,
       c.no2,
       c.co2,
       zonas[i].clima.temperatura,
       zonas[i].clima.viento,
       zonas[i].clima.humedad);

    }

    printf("=================================================\n");
}



