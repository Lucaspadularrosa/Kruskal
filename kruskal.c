#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int tipo_nombre; // tipo_nombre nombre = 4; === int nombre = 4;
typedef int tipo_elemento;
typedef int vertices;

#define VERTICES 7

typedef struct _ARISTAS
{
    vertices u; //Contiene dos vertices de tipo entero y su costo
    vertices v;
    int costo;
} aristas;

typedef struct _RAMA
{
    struct _ARISTAS a; //Arista que contiene vertice u, v y el costo
    struct _RAMA *sig; //*sig apunta al proximo vertice
} rama;

typedef struct _ENCABEZADO //Conjunto encabezado
{
    int cuenta;          //Inicializas en 1
    int primer_elemento; //Nombre del vertice
} encabezado;

typedef struct _NOMBRE //Conjunto nombres
{
    tipo_nombre nombre_conjunto; //Nombres de los vertices
    int siguiente_elemento;      //Inicia en 0
} nombre;

typedef struct _CONJUNTO_CE
{
    nombre nombres[VERTICES];
    encabezado encabezamientos_conjunto[VERTICES];
} conjunto_CE;

rama *ultimo;
void inicial(tipo_nombre, tipo_elemento, conjunto_CE *);
void combina(tipo_nombre, tipo_nombre, conjunto_CE *);
tipo_nombre encuentra(int, conjunto_CE *);
void kruskal(rama *);
void inserta(int, int, int, rama **);
aristas *sacar_min(rama **);
void lista(rama *);

int main()
{
    rama *arbol = NULL;       //Puntero tipo rama inicializado en Null
    inserta(1, 2, 3, &arbol); // vertice u, vetice v, costo, direccion de memoria
    inserta(1, 3, 1, &arbol);
    inserta(2, 3, 8, &arbol);
    inserta(2, 4, 7, &arbol);
    inserta(2, 5, 5, &arbol);
    inserta(4, 5, 6, &arbol);
    inserta(4, 7, 9, &arbol);
    inserta(5, 7, 2, &arbol);
    inserta(5, 6, 1, &arbol);
    inserta(6, 7, 4, &arbol);

    lista(arbol);
    printf("\n\n");
    kruskal(arbol);

    return 0;
}

void inicial(tipo_nombre A, tipo_elemento x, conjunto_CE *C)
{
    // Segunda tabla
    (*C).nombres[x].nombre_conjunto = A;    // Indice (*C).nombres[1].nombre_conjunto = 1;
    (*C).nombres[x].siguiente_elemento = 0; // Vertice y siguiente elemento
    // Primera tabla
    (*C).encabezamientos_conjunto[A].cuenta = 1;
    (*C).encabezamientos_conjunto[A].primer_elemento = x;
}

void combina(tipo_nombre A, tipo_nombre B, conjunto_CE *C)
{
    if ((*C).encabezamientos_conjunto[A].cuenta > (*C).encabezamientos_conjunto[B].cuenta) //Comparo la longitud de los dos conjuntos
    {
        int i = (*C).encabezamientos_conjunto[B].primer_elemento; //Me paro en el primer elemento del conjunto B
        while ((*C).nombres[i].siguiente_elemento != 0)
        {
            (*C).nombres[i].nombre_conjunto = A; //Al elemento i le asigno como nombre de su conjunto el conjunto A
            i = (*C).nombres[i].siguiente_elemento;
        }
        (*C).nombres[i].nombre_conjunto = A;                                                                 //Asigno como siguiente elemento al primer elemento de A
        (*C).nombres[i].siguiente_elemento = (*C).encabezamientos_conjunto[A].primer_elemento;               //Asigno como primer elemento de A al primer elemento de B
        (*C).encabezamientos_conjunto[A].primer_elemento = (*C).encabezamientos_conjunto[B].primer_elemento; //Actualizo la cuenta de elementos de A
        (*C).encabezamientos_conjunto[A].cuenta = (*C).encabezamientos_conjunto[A].cuenta + (*C).encabezamientos_conjunto[B].cuenta;
    }
    else
    {
        int j;
        j = (*C).encabezamientos_conjunto[A].primer_elemento;
        while ((*C).nombres[j].siguiente_elemento != 0)
        {
            (*C).nombres[j].nombre_conjunto = B;
            j = (*C).nombres[j].siguiente_elemento;
        }

        (*C).nombres[j].nombre_conjunto = B;
        (*C).nombres[j].siguiente_elemento = (*C).encabezamientos_conjunto[B].primer_elemento;
        (*C).encabezamientos_conjunto[B].primer_elemento = (*C).encabezamientos_conjunto[A].primer_elemento;
        (*C).encabezamientos_conjunto[B].cuenta = (*C).encabezamientos_conjunto[B].cuenta + (*C).encabezamientos_conjunto[A].cuenta;
    }
}

tipo_nombre encuentra(int x, conjunto_CE *C)
{
    //Busco conjunto y retorno su nombre
    return (*C).nombres[x].nombre_conjunto;
}

void kruskal(rama *A)
{
    int comp_n;              // Cantidad de componentes
    conjunto_CE componentes; // Conjunto componentes
    vertices u, v;           // Vertice u y v
    int comp_siguiente;      // Componente siguiente
    int comp_u, comp_v;      // Los componentes que contienen los vertices u y v
    aristas *a;              // Puntero tipo arista
    comp_siguiente = 1;      // Inicializamos comp_siguiente en 1
    comp_n = VERTICES;       // comp_n = 7

    for (int n = 1; n <= VERTICES; n++) //Asigno valor inicial a un componente para que contenga los vertices de V.
    {
        inicial(comp_siguiente, n, &componentes); // inicial (1,1,&componentes)
        comp_siguiente++;
    }
    printf("Arbol de costo minimo:\n");

    while (comp_n > 1)
    {
        //Retorno la arista de mayor prioridad
        a = sacar_min(&A);
        u = a->u;
        v = a->v;
        comp_u = encuentra(u, &componentes);
        comp_v = encuentra(v, &componentes);

        if (comp_u != comp_v) //Si los vertices pertenecen a distintos conjuntos entonces los combino dependiendo de la cantidad de elementos que tengan.
        {
            combina(comp_u, comp_v, &componentes);
            comp_n = comp_n - 1;
            printf("(%d-%d)--Costo: %d\n", u, v, a->costo);
        }
    }
}

void inserta(int u, int v, int costo, rama **A)
{
    if (*A == NULL) //Si la cola esta vacia
    {
        *A = (rama *)malloc(sizeof(rama)); //Reservo memoria
        //Agrego elementos a la cola
        (*A)->a.u = u;
        (*A)->a.v = v;
        (*A)->a.costo = costo;
        (*A)->sig = NULL;
        ultimo = (*A);
        printf("Primer arista agregada con exito..\n");
    }
    else //Si el costo de la nueva arista es menor que el costo de la arista con mayor prioridad, agrego al inicio
    {
        rama *primero; //Puntero primero y es tipo rama
        primero = (*A);
        if ((*A)->a.costo > costo)
        {
            rama *nuevo = (rama *)malloc(sizeof(rama));
            nuevo->a.costo = costo;
            nuevo->a.u = u;
            nuevo->a.v = v;
            nuevo->sig = (*A);
            (*A) = nuevo;
            ultimo = (*A)->sig;
            printf("Arista agregada al inicio de la cola con exito..\n");
        }
        else
        {
            // Si el costo de la arista a insertar es mayor al costo de mi última arista entonces lo agrego al final de la cola de prioridad
            if (ultimo->a.costo < costo)
            {
                rama *nuevo = (rama *)malloc(sizeof(rama));
                nuevo->a.costo = costo;
                nuevo->a.u = u;
                nuevo->a.v = v;
                ultimo->sig = nuevo;
                nuevo->sig = NULL;
                ultimo = nuevo;
            }
            else
            {
                rama *aux2;

                //Recorro la cola de prioridad hasta posicionarme en la arista de costo mayor a la que estoy por insertar
                while ((*A)->a.costo <= costo && (*A)->sig != ultimo)
                {
                    aux2 = (*A);
                    (*A) = (*A)->sig;
                }
                if ((*A)->a.costo > costo)
                {
                    rama *aux = (rama *)malloc(sizeof(rama));
                    aux->a.costo = costo;
                    aux->a.u = u;
                    aux->a.v = v;
                    aux2->sig = aux;
                    aux->sig = (*A);
                    (*A) = primero;
                }
                else
                {
                    rama *aux = (rama *)malloc(sizeof(rama));
                    aux->a.costo = costo;
                    aux->a.u = u;
                    aux->a.v = v;
                    (*A)->sig = aux;
                    aux->sig = ultimo;
                    (*A) = primero;
                }
            }
        }
    }
}

aristas *sacar_min(rama **A)
{
    //Retorno la arista de mayor prioridad en la cola
    aristas *aux2 = (aristas *)malloc(sizeof(aristas)); //Reservo memoria
    rama *temp;
    temp = (*A);
    aux2->u = temp->a.u;
    aux2->v = temp->a.v;
    aux2->costo = temp->a.costo;
    //Apunto *A a siguiente
    (*A) = (*A)->sig;
    free(temp); //Libero espacio en memoria
    return aux2;
}

void lista(rama *A) //Muestro la lista de aristas
{
    printf("Elementos del Árbol: \n");
    rama *temp = A;
    do
    {
        printf("Arista: (%d,%d) Costo: %d\n", temp->a.u, temp->a.v, temp->a.costo);
        temp = temp->sig;
    } while (temp != ultimo);
}
