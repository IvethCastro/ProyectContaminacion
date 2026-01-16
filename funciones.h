#ifndef FUNCIONES_H
#define FUNCIONES_H

/* -------- CONSTANTES -------- */
#define ZONAS 5
#define DIAS_HIST 30

/* -------- ESTRUCTURAS -------- */

/* Contaminantes */
struct Contaminacion {
    float pm25;
    float so2;
    float no2;
};

/* Datos climaticos */
struct Clima {
    float temperatura;
    float viento;
    float humedad;
};

/* Zona de la ciudad */
struct Zona {
    char nombre[30];
    struct Contaminacion actual;
    struct Contaminacion prediccion;
    int tieneDatos;   // 0 = no se ingresaron datos actuales, 1 = sí
};

/* Registro historico (archivo binario) */
struct RegistroHistorico {
    int zona;                       // ID de la zona
    struct Contaminacion datos;     // contaminacion de ese dia
};

/* Registro de prediccion (archivo binario) */
struct RegistroPrediccion {
    int zona;
    struct Contaminacion prediccion;
    struct Clima clima;
};

/* -------- FUNCIONES -------- */

/* Inicializacion */
void inicializarZonas(struct Zona zonas[]);
void cargarHistoricoInicial();

/* Opcion 1 - Monitoreo actual */
void ingresarContaminacion(struct Zona zonas[]);
void monitoreoActual(struct Zona *z);
void guardarHistorico(int zona, struct Contaminacion c);

/* Opcion 2 - Prediccion */
void prediccionFutura(struct Zona zonas[]);
void guardarPrediccion(int zona, struct Contaminacion p, struct Clima c);

/* Opcion 3 - Alertas y recomendaciones */
void alertasYRecomendaciones(struct Zona zonas[]);

/* Opcion 4 - Promedios historicos */
void promediosHistoricos(struct Zona zonas[]);

/* Opcion 5 - Reporte */
void exportarReporte(struct Zona zonas[]);

#endif
