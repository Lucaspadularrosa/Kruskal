#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

typedef pair<pair<char, char>, int> arista; // v1, v2 y costo

typedef struct combina_encuentra
{

	map<char, pair<char, char>> nombres;   // nombre de conjunto y siguiente elemento
	map<char, pair<int, char>> encabezado; // cuenta y primer elemento

} CONJUNTO_CE;

class MenorValor
{
public:
	bool operator()(arista e1, arista e2)
	{
		return e1.second > e2.second;
	}
};

typedef priority_queue<arista, vector<arista>, MenorValor> arbol;

class grafo
{

	vector<char> V;		  // Conjunto de vertices
	vector<arista> E;	  // Conjunto de aristas
	CONJUNTO_CE CE;		  // Estructura para el analisis de adyacencias
	arbol grafo_ordenado; // Cola de prioridad con los pesos de las aristas
	vector<arista> arbol_minimo;

public:
	grafo() {}
	~grafo() {}

	void insertar_vertice(const char &);
	void insertar_arista(const char &, const char &, const int &);
	void inicial(const char &, const char &);
	void insertar();			  // Inserto los elementos del vector de aristas E en la cola de prioridad
	arista sacar_min();			  // Devuelve la arista con mayor prioridad
	char encuentra(const char &); // Encuentra los v�rtices dentro del conjunto COMBINA_ENCUENTRA
	void kruskal();				  // Algoritmo generador del �rbol de cosro m�nimo
	void combina(const char &, const char &);
	friend ostream &operator<<(ostream &, grafo);
};

char grafo::encuentra(const char &x)
{
	return (CE.nombres[x].first);
}

void grafo::inicial(const char &x, const char &v)
{
	CE.nombres[x] = {v, '\0'};
	CE.encabezado[x] = {1, v};
	cout << "Nombre del Conjunto: " << CE.nombres[x].first << " Siguiente Elemento: " << CE.nombres[x].second;
	cout << "Cuenta: " << CE.encabezado[x].first << " Primer elemento: " << CE.encabezado[x].second;
	cout << "\n";
}

void grafo::insertar_vertice(const char &vertice)
{
	V.push_back(vertice);
}

void grafo::insertar_arista(const char &vertice1, const char &vertice2, const int &costo)
{
	E.push_back(make_pair(make_pair(vertice1, vertice2), costo));
}

ostream &operator<<(ostream &os, grafo G)
{

	os << "El grafo es: \n";
	for (auto x : G.E)
		os << x.first.first << " - " << x.first.second << ": Costo = " << x.second << endl;
	return os;
}

void grafo::insertar()
{
	for (auto x : E)
		grafo_ordenado.push(make_pair(make_pair(x.first.first, x.first.second), x.second));
}

arista grafo::sacar_min()
{
	arista aux;

	aux = grafo_ordenado.top();
	grafo_ordenado.pop();
	return (aux);
}

void grafo::kruskal()
{
	int comp_n = V.size();
	arista a;

	char v1, v2, comp_v1, comp_v2; // Inicializo el conjunto combina-encuentra
	for (auto x : V)
		inicial(x, x);

	cout << endl;

	while (comp_n > 1)
	{
		a = sacar_min();
		cout << "\n"
			 << "Arista Minima: " << a.first.first << "-" << a.first.second << " : Costo: " << a.second << endl;
		v1 = a.first.first;
		v2 = a.first.second;
		cout << "Busqueda de vertices: " << v1 << " y " << v2 << endl;
		comp_v1 = encuentra(v1);
		comp_v2 = encuentra(v2);
		cout << "El vertice " << v1 << " pertenece al conjunto " << comp_v1 << endl;
		cout << "El vertice " << v2 << " pertenece al conjunto " << comp_v2 << endl;
		if (comp_v1 != comp_v2)
		{
			combina(v1, v2);
			--comp_n;
			cout << endl;
			cout << "Nombre del Conjunto: " << CE.nombres[v2].first << " | | Sig Elemento: " << CE.nombres[v2].second;
			cout << " | | Cuenta: " << CE.encabezado[v2].first << " | | Primer elemento: " << CE.encabezado[v2].second << "\n";
			arbol_minimo.push_back(make_pair(make_pair(v1, v2), a.second));
		}
		for (auto x : arbol_minimo)
		{
			cout << "\t" << x.first.first << " - " << x.first.second << " costo: " << x.second;
		}
	}
}

void grafo::combina(const char &A, const char &B)
{
	char i;
	if (CE.encabezado[A].first > CE.encabezado[B].first)
	{
		i = CE.encabezado[B].second; // Le asigno a i el primero elemento del conj B
		do
		{

			CE.nombres[i].first = A;
			i = CE.nombres[i].second; //Le asigno a i el siguiente elemento del conjunto

		} while (CE.nombres[i].second != '\0'); //Repito hasta llegar al ultimo elemento del conj

		CE.nombres[i].first = A;
		CE.nombres[i].second = CE.encabezado[A].second; //Agrego la lista A a continuacion del ultimo elemento de B
		CE.encabezado[A].second = CE.encabezado[B].second;
		CE.encabezado[A].first = CE.encabezado[A].first + CE.encabezado[B].first;
	}
	else
	{								 //B es al menos tan grande como A
		i = CE.encabezado[A].second; // Le asigno a i el primero elemento del conj A
		do
		{

			CE.nombres[i].first = B;
			i = CE.nombres[i].second; //Le asigno a i el siguiente elemento del conjunto

		} while (CE.nombres[i].second != '\0'); //Repito hasta llegar al ultimo elemento del conj

		CE.nombres[i].first = B;
		CE.nombres[i].second = CE.encabezado[B].second; //Agrego la lista B a continuacion del ultimo elemento de A
		CE.encabezado[B].second = CE.encabezado[A].second;
		CE.encabezado[B].first = CE.encabezado[B].first + CE.encabezado[A].first;
	}
}

int main()
{
	grafo G;
	G.insertar_vertice('a');
	G.insertar_vertice('b');
	G.insertar_vertice('c');
	G.insertar_vertice('d');

	G.insertar_arista('a', 'b', 52);
	G.insertar_arista('a', 'c', 15);
	G.insertar_arista('a', 'd', 5);
	G.insertar_arista('b', 'c', 40);
	G.insertar_arista('a', 'd', 25);
	G.insertar_arista('c', 'd', 1);

	cout << G << endl;
	G.insertar();
	G.kruskal();
	return 0;
}
