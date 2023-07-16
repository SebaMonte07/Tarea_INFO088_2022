#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <vector>
#include <list>

using namespace std;

#define mostrarPos 0 //1 para ver indices donde comienza el patron, 0 para no verlos.

struct sufijo{
	int id;
	char *suf;
};
struct compara2{
    bool operator()(sufijo const & a, sufijo const & b) const
    {
        return a.suf < b.suf;
    }
};


////////////////////////////////////////
//Funcion para leer el archivo
char* lectura(char *nomT);

//Funciones para fuerza bruta
void busquedaEx(char* frase , char* patron);

//Funciones para arreglo de sufijos
void buscar(int *arrIndices , char* frase , char* patron , int nF);
int primeraOcurrencia(int *arrIndices, char *texto, int nF,char target);
int ultimaOcurrencia(int *arrIndices, char *texto, int nF,char target);
int *arrSufijos(char* txt , int n);
int compara (sufijo a , sufijo b);
int compvar(const void *uno, const void *dos);
int mi_strncmp(char *s1 , char *s2 , int n);

//funciones para lista enlazada
void listaSufijos(char* texto , int largoF, char* patron);

//Funciones utiles
int largo(char* pal);
void printCP(char* frase, char* patron);


int main(){
	
	char nomT[50];
	cout <<"Ingrese nombre del archivo: ";
	cin.getline(nomT,50);
	
	//Asumimos que se ingresa un patron distinto de vacío!
	char patron[50];
	cout <<"Ingrese patron: ";
	cin.getline(patron,50);

	//Lee el archivo en froma de string, y lo transforma en arreglo de caracteres
	cout << "Leyendo texto..."<<endl;
	char *texto = lectura(nomT);
	cout << "Lectura terminada exitosamente!\n\n";

	//char texto[]="AABAACAADAABAABA";
	//char patron[]="BA";	//AABA
	int largoF = largo(texto);
	unsigned t0,t1,t2,t3;
	
	cout << "Creando arreglo de sufijos ordenado..."<<endl;
	int *indices = arrSufijos(texto , largoF);
	cout << "Creacion exitosa!\n\n";
	
// 0  1  2  3  4  5  6  7  8  9  10  11 12  13  14  15      elemento
// A  A  B  A  A  C  A  A  D  A   A   B  A   A   B   A      frase

	//Imprime arreglo de indices de sufijos ordenados
/*	for(int i=0 ; i<largoF ; i++)
		cout << indices[i]<<" ";
	cout << endl;*/
	//printCP(texto , patron);

	cout << "\n---BUSQUEDA EXHAUSTIVA---\n";
	t0=clock();
	busquedaEx(texto , patron);	
	t1=clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	cout << "Tiempo de ejecucion = "<<time<<" segs"<<endl;
	

	cout << "\n---BUSQUEDA CON ARREGLO DE SUFIJOS---\n";
	t2=clock();
	buscar(indices , texto , patron , largoF);
	t3=clock();
	double time2 = (double(t3-t2)/CLOCKS_PER_SEC);
	cout << "Tiempo de ejecucion = "<<time2<<" segs"<<endl;
	

	cout << "\n---BUSQUEDA CON LISTA ENLAZADA DE SUFIJOS---\n";
	listaSufijos(texto, largoF, patron);
}


///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

//Para guiarnos
// 0  1  2 3 4 5 6  7  8 9 10 11 12 13 14 15    indice
// 15 12 9 0 3 6 13 10 1 4  7 14 11 2  5  8     elemento
// A  A  A A A A  A  A A A  A  B  B B  C  D     primera letra

//Funciones para busqueda por arreglo de sufijos

void buscar(int *arrIndices , char* frase , char* patron , int nF){
	vector<int> vec;
	char target = patron[0];
  int x = primeraOcurrencia(arrIndices, frase, nF,target);
  //cout << "Primera ocurrencia: "<< x << endl;	
	
	int y = ultimaOcurrencia(arrIndices, frase, nF, target);
	//cout << "ultima ocurrencia: "<<y<<endl;
	
	int lP=largo(patron);
	for(int i=x ; i<=y ; i++){
		if(strncmp(&frase[arrIndices[i]],patron,lP) == 0)
			vec.push_back(arrIndices[i]);
	}
	if(vec.empty())
		cout << "Ninguna coincidencia encontrada!"<<endl;
	else{
		qsort(&vec[0],vec.size(),sizeof(int),compvar);
		if(mostrarPos){
			cout <<"Patron encontrado en indice(s): "<<endl;
			for(long unsigned int i=0 ; i<vec.size() ; i++)
				cout << vec[i] << " ";
			cout<<endl;
		}
	}
}


int primeraOcurrencia(int *arrIndices, char *texto, int nF,char target){
  int low = 0, limite = nF-1;
  while(low<=limite){
  	int mid = low + (limite-low)/2;
    if ((mid == 0 || texto[arrIndices[mid - 1]] < target) && texto[arrIndices[mid]] == target)
      return mid;
    else if (target > texto[arrIndices[mid]])
      low = mid + 1;
    else
        limite = mid - 1;
  }
  return -1;
}


int ultimaOcurrencia(int *arrIndices, char *texto, int nF,char target){
  int low = 0, limite = nF-1;
  while(low<=limite){
    int mid = low + (limite-low)/2;
    if ((mid == nF-1 || texto[arrIndices[mid + 1]] > target) && texto[arrIndices[mid]] == target)
			return mid;
    else if (target < texto[arrIndices[mid]])
      limite = mid - 1;
    else
      low = mid + 1;
	}
  return -1;
}


//Funcion strncmp implementada por nosotros 

//implementamos dos formas de strncmp, pero ninguna de estas logro ser mas
//eficiente que el strncmp de la libreria string, es el unico motivo por
//el cual usamos esa funcion.

/*int mi_strncmp(char *s1 , char *s2 , int n){
	for(int i=0 ; i<n ; i++){
		if(s1[i] == s2[i]){
			if(i == (n-1))
				return 0;
		}
		else
			return 1;
	}
	return 0;
}*/

/*int mi_strncmp(char *s1, char *s2, int n)
{
  register unsigned char u1, u2;
  while (n-- > 0)
    {
      u1 = (unsigned char) *s1++;
      u2 = (unsigned char) *s2++;
      if (u1 != u2)
        return u1 - u2;
      if (u1 == '\0')
        return 0;
    }
  return 0;
}*/


int *arrSufijos(char* txt , int n){
	sufijo *sufijos = new sufijo[n];
	
	for(int i=0 ; i<n ; i++){
		sufijos[i].id = i;
		sufijos[i].suf = (txt+i);
	}
	cout <<"Ordenando..."<<endl;
	sort(sufijos,sufijos+n,compara);//Se demora mucho aqui, deberia ser un quicksort
	cout << "Orden completado!"<<endl;

	int *indices = new int[n];
	for(int i=0 ; i<n ; i++)
		indices[i] = sufijos[i].id;

	return indices;
}


int compara (sufijo a , sufijo b){
	//Usamos esta funcion para ordenar el arreglo de sufijos
	if(strcmp(a.suf , b.suf) < 0) return 1;
	else return 0;
}


int compvar(const void *uno, const void *dos){
	//Usamos esta funcion para el quicksort de un vector
  int a = *((int*)uno);
  int b = *((int*)dos);
  if (a<b)
  	return -1;
  if (a == b)
  	return 0;
  return 1;   
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

//Busqueda Exhaustiva
void busquedaEx(char* frase, char* patron){
	int largoF=largo(frase),largoP=largo(patron);
	int i , j , k , pos=0;
	vector<int> arreglo;
 
	for(i=0 ; i<largoF ; i++){
		j=0;
		if(frase[i] == patron[j]){
			pos=i;

			while(frase[i] != '\0' && patron[j] != '\0' && frase[i]==patron[j]){
				i++;
				j++;
			}

			if(patron[j] == '\0'){
				arreglo.push_back(pos);
				i-=largoP;
			}
			else{
				i=pos;
				pos=0;
			}
		}
	}
	
	k=arreglo.size();

	if(k!=0){//Significa que existe por lo menos una ocurrencia del patron
		if(mostrarPos){
			cout << "El patron comienza en las posiciones: " << endl;
			for(i=0 ; i<k ; i++)
				cout << arreglo[i] << " ";
			cout << endl;
		}
	}
	else //Si el arreglo esta vacio, no se encontro ninguna coincidencia
		cout << "No se encontro ninguna coincidencia!"<<endl;
}

////////////////////////////////////////////////////////////////////
//busqueda por lista enlazada
void listaSufijos(char* texto , int largoF, char* patron){
	vector<int> vec;
	list<sufijo> l;
	for(int i = 0; i < largoF; i++){
		sufijo nuevo;
		nuevo.id = i;
		nuevo.suf = (texto+i);
		l.emplace_back(nuevo);
	}
	cout<<"Ordenando lista enlazada.."<<endl;//No tomamos en cuenta el tiempo que demora en ordenar la lista, solo tomamos el tiempo que demora en encontrar las coincidencias de patron
	l.sort(compara2());
	cout<<"Lista ordenada!"<<endl;
	char u= toupper(patron[0]);
	if(toascii('M')<toascii(u)){
		l.reverse();
		cout<<"recorriendo la lista desde el final " <<endl;
	}
	
	int lP=largo(patron);
	//int y=l.size();
	double t2=clock();//Tomamos en cuenta solo el tiempo de busqueda del patron
	for(const auto& sufijo : l){
		if(strncmp(sufijo.suf,patron,lP) == 0)
			vec.push_back(sufijo.id);
	}
	double t3=clock();
	if(vec.empty())
		cout << "Ninguna coincidencia encontrada!"<<endl;
	else{
		qsort(&vec[0],vec.size(),sizeof(int),compvar);
		if(mostrarPos){
			cout <<"Patron encontrado en id(s): "<<endl;
			for(long unsigned int i=0 ; i<vec.size() ; i++)
				cout << vec[i] << " ";
			cout<<endl;
		}
	}	
	double time2 = (double(t3-t2)/CLOCKS_PER_SEC);
	cout << "Tiempo de ejecucion = "<<time2<<" segs"<<endl;

}


////////////////////////////////////////////////////////////////////
//Funcion para leer archivos
char* lectura(char *nomT){
	vector<char> T;
	char letra=0;

	ifstream archivo(nomT);
	if(archivo.fail()){
		cout <<"No se pudo abrir el archivo!"<<endl;;
		exit(1);
	}
	while(archivo.get(letra))
		T.push_back(letra);
	archivo.close();

	int l=T.size();
	char *texto = new char[l];
	for(int i=0;i<l;i++)
		texto[i]=T[i];
	return texto;	
}

////////////////////////////////////////////////////////////////////
//Funciones útiles

int largo(char* pal){
	int i=0;
	while(pal[i] != '\0')
		i++;
	return i;
}

void printCP(char* frase, char* patron){
	cout << "CADENA : ";
	for(int i=0 ; i<largo(frase) ; i++)
		cout << frase[i];
	cout << endl;
	
	cout << "PATRON : ";
	for(int i=0 ; i<largo(patron) ; i++)
		cout << patron[i];		
	cout << endl;
 
}











