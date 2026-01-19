#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>

#define LIM_PM25 15
#define LIM_SO2 25
#define LIM_NO2 40

#define DIAS 30
#define MAX_ZONAS 20



typedef struct {
    float pm25;
    float so2;
    float no2;
    float co2;
} Contaminacion;

typedef struct {
    Contaminacion dias[DIAS];
} Historial;

typedef struct {
    Contaminacion valor;
    float aqi_pm25;
    float aqi_so2;
    float aqi_no2;
} Proyeccion;

typedef struct {
    float temperatura;
    float viento;
    float humedad;
} Clima;

typedef struct {
    char nombre[40];
    Historial historial;
    Contaminacion actual;
    Proyeccion proy;
    Clima clima;
    int hayPrediccion;
    int tieneDatos;
    int tieneClima;
} Zona;


int leerEnteroConRango(int inicio, int fin);
float leerFlotanteConRango(float inicio, float fin);
int soloLetras(char *cadena);


void mostrarZonas(Zona zonas[], int totalZonas);
void anadirZona(Zona zonas[], int *totalZonas);


void cargarDatosHistoricos(Zona zonas[], int totalZonas);
void guardarDatosHistoricos(Zona zonas[], int totalZonas);
void cargarZonas(Zona zonas[], int *totalZonas);


void ingresarContaminacionActual(Zona zonas[], int totalZonas);
void mostrarDatosActuales(Zona zonas[], int totalZonas);



float promedioPonderado(float valores[]);
float factorClimatico(float temp, float viento, float humedad);

float calcularAQI(float C, float Clow, float Chigh, float Ilow, float Ihigh);
float aqiPM25(float c);
float aqiSO2(float c);
float aqiNO2(float c);

const char* interpretacionAQI(float aqi);
const char* nivelCO2Exterior(float c);

void prediccion(Zona zonas[], int totalZonas);
void alertasRecomendaciones(Zona zonas[], int totalZonas);
void promediosHistoricos(Zona zonas[], int totalZonas);
void exportarReporte(Zona zonas[], int totalZonas);
void monitoreoActual(Zona *z);


#endif

