#ifndef FUNCIONES_H
#define FUNCIONES_H

/* -------- CONSTANTES -------- */
#define ZONAS 5
#define DIAS_HIST 30

struct Contaminacion {
    float pm25;
    float so2;
    float no2;
};

struct Clima {
    float temperatura;
    float viento;
    float humedad;
};

struct Zona {
    char nombre[30];
    struct Contaminacion actual;
    struct Contaminacion prediccion;
    int tieneDatos;   // 0 = no se ingresaron datos actuales, 1 = sí
};

struct RegistroHistorico {
    int zona;                       // ID de la zona
    struct Contaminacion datos;     // contaminacion de ese dia
};

struct RegistroPrediccion {
    int zona;
    struct Contaminacion prediccion;
    struct Clima clima;
};


void inicializarZonas(struct Zona zonas[]);
void cargarHistoricoInicial();

void ingresarContaminacion(struct Zona zonas[]);
void monitoreoActual(struct Zona *z);
void guardarHistorico(int zona, struct Contaminacion c);

void prediccionFutura(struct Zona zonas[]);
void guardarPrediccion(int zona, struct Contaminacion p, struct Clima c);

void alertasYRecomendaciones(struct Zona zonas[]);

void promediosHistoricos(struct Zona zonas[]);

void exportarReporte(struct Zona zonas[]);

#endif

