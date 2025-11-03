#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TAM 50

// ------------------ PILA ------------------
typedef struct NodoPila {
    char operacion[50];
    struct NodoPila* siguiente;
} NodoPila;

NodoPila* cima = NULL;

void push(const char* op) {
    NodoPila* nuevo = (NodoPila*)malloc(sizeof(NodoPila));
    strcpy(nuevo->operacion, op);
    nuevo->siguiente = cima;
    cima = nuevo;
}

int pilaVacia() { return cima == NULL; }

char* pop() {
    static char op[50];
    if (pilaVacia()) return "";
    strcpy(op, cima->operacion);
    NodoPila* temp = cima;
    cima = cima->siguiente;
    free(temp);
    return op;
}

void mostrarPila() {
    printf("\n--- Pila (ultimas operaciones ingresadas) ---\n");
    NodoPila* aux = cima;
    while (aux != NULL) {
        printf("%s\n", aux->operacion);
        aux = aux->siguiente;
    }
}

// ------------------ COLA ------------------
typedef struct NodoCola {
    char operacion[50];
    struct NodoCola* siguiente;
} NodoCola;

NodoCola* frente = NULL;
NodoCola* fin = NULL;

void encolar(const char* op) {
    NodoCola* nuevo = (NodoCola*)malloc(sizeof(NodoCola));
    strcpy(nuevo->operacion, op);
    nuevo->siguiente = NULL;
    if (fin == NULL)
        frente = nuevo;
    else
        fin->siguiente = nuevo;
    fin = nuevo;
}

int colaVacia() { return frente == NULL; }

char* desencolar() {
    static char op[50];
    if (colaVacia()) return "";
    strcpy(op, frente->operacion);
    NodoCola* temp = frente;
    frente = frente->siguiente;
    if (frente == NULL) fin = NULL;
    free(temp);
    return op;
}

void mostrarCola() {
    printf("\n--- Cola (operaciones en espera) ---\n");
    NodoCola* aux = frente;
    while (aux != NULL) {
        printf("%s\n", aux->operacion);
        aux = aux->siguiente;
    }
}

// ------------------ LISTA (Historial) ------------------
typedef struct NodoLista {
    char operacion[50];
    double resultado;
    struct NodoLista* siguiente;
} NodoLista;

NodoLista* inicio = NULL;

void agregarHistorial(const char* op, double res) {
    NodoLista* nuevo = (NodoLista*)malloc(sizeof(NodoLista));
    strcpy(nuevo->operacion, op);
    nuevo->resultado = res;
    nuevo->siguiente = NULL;
    if (inicio == NULL)
        inicio = nuevo;
    else {
        NodoLista* aux = inicio;
        while (aux->siguiente != NULL) aux = aux->siguiente;
        aux->siguiente = nuevo;
    }
}

void mostrarHistorial() {
    printf("\n--- Historial de operaciones (lista) ---\n");
    NodoLista* aux = inicio;
    while (aux != NULL) {
        printf("%s = %.2lf\n", aux->operacion, aux->resultado);
        aux = aux->siguiente;
    }
}

// ------------------ HASH ------------------
typedef struct {
    char operacion[50];
    double resultado;
    int ocupado;
} NodoHash;

NodoHash tablaHash[TAM];

int buscarHash(const char* op, double* res) {
    int i;
    for (i = 0; i < TAM; i++) {
        if (tablaHash[i].ocupado && strcmp(tablaHash[i].operacion, op) == 0) {
            *res = tablaHash[i].resultado;
            return 1;
        }
    }
    return 0;
}

void insertarHash(const char* op, double res) {
    int i;
    for (i = 0; i < TAM; i++) {
        if (!tablaHash[i].ocupado) {
            strcpy(tablaHash[i].operacion, op);
            tablaHash[i].resultado = res;
            tablaHash[i].ocupado = 1;
            return;
        }
    }
}

void mostrarHash() {
    int i;
    printf("\n--- Tabla Hash ---\n");
    for (i = 0; i < TAM; i++) {
        if (tablaHash[i].ocupado)
            printf("[%d] %s = %.2lf\n", i, tablaHash[i].operacion, tablaHash[i].resultado);
    }
}

// ------------------ Evaluar Operación ------------------
double evaluarOperacion(const char* op) {
    char *ptr;
    double a, b;
    char buffer[50];

    if (strchr(op, '+')) {
        ptr = strchr(op, '+');
        strcpy(buffer, op);
        a = atof(strtok(buffer, "+"));
        b = atof(ptr + 1);
        return a + b;
    }

    if (strchr(op, '-')) {
        ptr = strchr(op, '-');
        strcpy(buffer, op);
        a = atof(strtok(buffer, "-"));
        b = atof(ptr + 1);
        return a - b;
    }

    if (strchr(op, '*')) {
        ptr = strchr(op, '*');
        strcpy(buffer, op);
        a = atof(strtok(buffer, "*"));
        b = atof(ptr + 1);
        return a * b;
    }

    if (strchr(op, '/')) {
        ptr = strchr(op, '/');
        strcpy(buffer, op);
        a = atof(strtok(buffer, "/"));
        b = atof(ptr + 1);
        return b != 0 ? a / b : 0;
    }

    if (strchr(op, '^')) {
        ptr = strchr(op, '^');
        strcpy(buffer, op);
        a = atof(strtok(buffer, "^"));
        b = atof(ptr + 1);
        return pow(a, b);
    }

    if (strchr(op, '!')) {
        strcpy(buffer, op);
        a = atof(strtok(buffer, "!"));
        double fact = 1;
        int i;
        for (i = 1; i <= (int)a; i++) fact *= i;
        return fact;
    }

    return 0;
}

// ------------------ MAIN ------------------
int main() {
    int opcion;
    char operacion[50];
    double resultado;
    int i;

    for (i = 0; i < TAM; i++) tablaHash[i].ocupado = 0;

    do {
        printf("\n===== SISTEMA DE OPERACIONES MATEMATICAS =====\n");
        printf("1. Ingresar operacion (a pila y cola)\n");
        printf("2. Procesar operacion (de la cola)\n");
        printf("3. Mostrar pila\n");
        printf("4. Mostrar cola\n");
        printf("5. Mostrar historial (lista)\n");
        printf("6. Mostrar hash\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese operacion (ej: 5+3, 4!, 2^3): ");
                scanf("%s", operacion);
                push(operacion);
                encolar(operacion);
                printf("Operacion agregada a pila y cola.\n");
                break;

            case 2:
                if (colaVacia()) {
                    printf("No hay operaciones en espera.\n");
                } else {
                    strcpy(operacion, desencolar());
                    printf("\nProcesando: %s\n", operacion);
                    if (buscarHash(operacion, &resultado)) {
                        printf("Resultado recuperado del hash: %.2lf\n", resultado);
                    } else {
                        resultado = evaluarOperacion(operacion);
                        insertarHash(operacion, resultado);
                        printf("Resultado calculado: %.2lf\n", resultado);
                    }
                    agregarHistorial(operacion, resultado);
                }
                break;

            case 3:
                mostrarPila();
                break;

            case 4:
                mostrarCola();
                break;

            case 5:
                mostrarHistorial();
                break;

            case 6:
                mostrarHash();
                break;

            case 7:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 7);

    return 0;
}

