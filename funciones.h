#ifndef FUNCIONES_H
#define FUNCIONES_H

#define LIM_PM25 15
#define LIM_SO2 40
#define LIM_NO2 40


#define ZONAS 5
#define DIAS 30

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
    char nombre[40];
    Historial historial;
    Proyeccion proy;
    int hayPrediccion;
} Zona;

/* ================= VALIDACIONES ================= */

int leerEnteroConRango(int inicio, int fin);
float leerFlotanteConRango(float inicio, float fin);

/* ================= INICIALIZACIÓN Y ARCHIVOS ================= */

void inicializarZonas(Zona zonas[]);
void cargarDatosHistoricos(Zona zonas[]);
void guardarDatosHistoricos(Zona zonas[]);
void guardarPrediccion(Zona zonas[], int id);

/* ================= UTILIDADES ================= */

void mostrarZonas(Zona zonas[]);
void monitoreoActual(Zona *z);

float promedioPonderado(float valores[]);
float factorClimatico(float temp, float viento, float humedad);

const char* interpretacionAQI(float aqi);
const char* nivelCO2Exterior(float c);

/* ================= AQI ================= */

float calcularAQI(float C, float Clow, float Chigh, float Ilow, float Ihigh);
float aqiPM25(float c);
float aqiSO2(float c);
float aqiNO2(float c);

/* ================= OPCIONES DEL MENÚ ================= */

void ingresarContaminacionActual(Zona zonas[]);
void prediccion(Zona zonas[], int id);
void alertasRecomendaciones(Zona zonas[]);
void promediosHistoricos(Zona zonas[]);
void exportarReporte(Zona zonas[]);

#endif
 
