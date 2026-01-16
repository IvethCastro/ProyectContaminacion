#ifndef FUNCIONES_H
#define FUNCIONES_H

/* -------- CONSTANTES -------- */
#define ZONAS 5
#define DIAS_HIST 30

typedef struct {
    float pm25;
    float so2;
    float no2;
} Contaminacion;


typedef struct {
    float temperatura;
    float viento;
    float humedad;
} Clima;


typedef struct {
    char nombre[30];
    Contaminacion actual;
    Contaminacion prediccion;
    int tieneDatos;   
} Zona;

typedef struct {
    int zona;
    Contaminacion datos;
} RegistroHistorico;

typedef struct {
    int zona;
    Contaminacion prediccion;
    Clima clima;
} RegistroPrediccion;

void inicializarZonas(Zona zonas[]);
void cargarHistoricoInicial();


void ingresarContaminacion(Zona zonas[]);
void monitoreoActual(Zona *z);
void guardarHistorico(int zona, Contaminacion c);

void prediccionFutura(Zona zonas[]);
void guardarPrediccion(int zona, Contaminacion p, Clima c);
void alertasYRecomendaciones(Zona zonas[]);
void promediosHistoricos(Zona zonas[]);
void exportarReporte(Zona zonas[]);

#endif



