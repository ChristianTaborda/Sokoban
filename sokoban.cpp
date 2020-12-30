
/*****************************************************
 * Christian Camilo Taborda Campiño		1632081-3743 * 
 * Cristian Camilo Vallecilla Cuellar 	1628790-3743 *
 * Santiago Hernandez Arias				1631281-3743 *
 *****************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

//Clase para manejar posiciones:
class Posicion{	
	private: 
		int x, y;
	public:
		Posicion();
		Posicion(string pos);	
		Posicion(int px, int py);
		int getX();
		int getY();
		void setX(int px);
		void setY(int py);
};

//Constructor vacío:
Posicion::Posicion(){}

//Constructor con coordenadas:
Posicion::Posicion(int px, int py){
	x = px;
	y = py;
}

//Constructor con string:
Posicion::Posicion(string pos){
	int coma = pos.find(",");
	x = stoi(pos.substr(0,coma));
	y = stoi(pos.substr(coma + 1));
}

//Retorna la coordenada X:
int Posicion::getX(){
	return x;
}

//Retorna la coordenada Y:
int Posicion::getY(){
	return y;
}

//Setea la coordenada X:
void Posicion::setX(int px){
	x = px;
}

//Setea la coordenada Y:
void Posicion::setY(int py){
	y = py;
}

//Clase para manejar los nodos:
class Nodo{	
	private:
		int profundidad;
		string regla;
		Posicion jugador;
		vector <Posicion> cajas;
	public:
		Nodo();
		Nodo(int p, string r, Posicion j, vector <Posicion> c);
		Posicion getCaja(int caja);
		vector <Posicion> getCajas();
		Posicion getJugador();
		string getRegla();
		int getProfundidad();		
};

//Constructor vacío:
Nodo::Nodo(){}

//Constructor completo:
Nodo::Nodo(int p, string r, Posicion j, vector <Posicion> c){
	profundidad = p;
	regla = r;
	jugador = j;
	cajas = c;
}

//Retorna la posición de una caja:
Posicion Nodo::getCaja(int caja){
	return cajas.at(caja);
}

//Retorna un vector de cajas:
vector <Posicion> Nodo::getCajas(){
	return cajas;
}

//Retorna la posición del jugador
Posicion Nodo::getJugador(){
	return jugador;
}

//Retorna la regla del nodo:
string Nodo::getRegla(){
	return regla;
}

//Retorna la profundidad del nodo:
int Nodo::getProfundidad(){
	return profundidad;
}

//Variables globales para el mapa y los objetivos:
string ** mapa;
vector <Posicion> objetivos;

//Lee un fichero línea a línea y las almacena en un vector:
vector <string> leerFichero(string nombre){
	vector <string> lineas;
	ifstream fichero(nombre);
	if(fichero.is_open()){
		char linea[50];
		while(!fichero.eof()){
			fichero >> linea;
			lineas.push_back(linea);
		}
		fichero.close();		
	}else{
		cout << "\nNo se encuentra el fichero.\n";
	}
	lineas.erase(lineas.end());
	return lineas;
}

//Verifica si una línea de texto corresponde a una fila del juego:
bool verificarFila(string linea){
	bool salida = true;
	for(int x=0; x<linea.length(); x++){
		if(linea[x] == ','){
			salida = false;
		}
	}
	return salida;
}

//Cuenta la cantidad de filas para la matriz del juego:
int contarFilas(vector <string> lineas){
	int filas = 0;
	while(verificarFila(lineas.at(filas))){
		filas++;
	}
	return filas;
}

//Retorna la posición de una caja en un vector de acuerdo a sus coordenadas:
int buscarCaja(vector <Posicion> cajas, Posicion P){
	for(int x=0; x<cajas.size(); x++){
		if(P.getX() == cajas.at(x).getX() && P.getY() == cajas.at(x).getY()){
			return x;
		}
	}
}

//Almacena los objetivos del mapa en un vector:
void buscarObjetivos(int f, int c){
	for(int x=0; x<f; x++){
		for(int y=0; y<c; y++){
			if(mapa[x][y].compare("X") == 0){
				objetivos.push_back(Posicion(x,y));
			}
		}
	}
}

//Indica si las coordenadas de una caja están en un vector de posiciones de cajas:
bool validarCaja(Posicion caja, vector <Posicion> posiciones){
	bool salida = false;
	for(int x=0; x<posiciones.size(); x++){
		if(posiciones.at(x).getX() == caja.getX() && posiciones.at(x).getY() == caja.getY()){
			salida = true;
		}
	}
	return salida;
}

//Valida que las posiciones de las cajas coincidan con los objetivos:
bool validarJuego(Nodo estado){
	bool salida = true;
	for(int x=0; x<objetivos.size(); x++){
		if(!validarCaja(estado.getCaja(x), objetivos)){
			salida = false;
		}
	}
	return salida;
}

//Indica si hay una caja en una posición del mapa:
bool hayCaja(int posX, int posY, vector <Posicion> cajas){
	bool salida = false;
	for(int x=0; x<cajas.size(); x++){
		if(cajas.at(x).getX() == posX && cajas.at(x).getY() == posY){
			salida = true;
		}
	}
	return salida;
}

//Compara si el contenido de dos vectores de posiciones de cajas son iguales:
bool compararCajas(vector <Posicion> C1, vector <Posicion> C2){
	bool salida = true;
	for(int x=0; x<C1.size(); x++){
		if(!validarCaja(C1.at(x), C2)){
			salida = false;
		}
	}
	return salida;
}

//Indica si un nodo ya fue considerado en un árbol de nodos:
bool verificarRepetido(vector <Nodo> arbol, Nodo n){
	bool salida = false;
	Posicion jugador;
	vector <Posicion> cajas;
	for(int x=0; x<arbol.size(); x++){
		jugador = arbol.at(x).getJugador();
		cajas = arbol.at(x).getCajas();
		if(jugador.getX() == n.getJugador().getX() && jugador.getY() == n.getJugador().getY()){
			if(compararCajas(cajas, n.getCajas())){
				salida = true;
			}
		}
	}	
	return salida;
}

//Valida si una jugada termina por derrota:
bool esquina(Posicion pos){
	bool salida = false;
	if(mapa[pos.getX()-1][pos.getY()].compare("W") == 0 && mapa[pos.getX()][pos.getY()-1].compare("W") == 0
		&& mapa[pos.getX()][pos.getY()].compare("X") != 0){
		salida = true;
	}
	if(mapa[pos.getX()+1][pos.getY()].compare("W") == 0 && mapa[pos.getX()][pos.getY()+1].compare("W") == 0
		&& mapa[pos.getX()][pos.getY()].compare("X") != 0){
		salida = true;
	}
		return salida;
}

//Aplica búsqueda preferente por amplitud, profundidad o profundidad iterativa y retorna la solución del problema:
string buscarSolucion(vector <Nodo> arbol, vector <Nodo> repetidos, string tipo, int limite){
	Nodo estadoActual = arbol.at(0);
	int tope;
	bool repetido;
	while(!validarJuego(estadoActual)){
		tope = 0;
		repetidos.push_back(estadoActual);
		arbol.erase(arbol.begin());
		if(tipo.compare("PI") == 0){
			if(estadoActual.getProfundidad() < limite){
				Posicion J = estadoActual.getJugador();
				vector <Posicion> C = estadoActual.getCajas();
				//ARRIBA
				if(hayCaja(J.getX()-1, J.getY(), C)){
					if(!(hayCaja(J.getX()-2, J.getY(), C) || mapa[J.getX()-2][J.getY()].compare("W") == 0)){
						Posicion NJ = J;
						vector <Posicion> NC = C;
						NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX()-1, NJ.getY())));
						Posicion PC = Posicion(NJ.getX()-2, NJ.getY());
						NC.push_back(PC);
						NJ.setX(NJ.getX()-1);
						Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "U", NJ, NC);
						repetido = verificarRepetido(repetidos, estadoNuevo);
						if(!repetido && !esquina(PC)){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
							if(!repetido){
								repetidos.push_back(estadoNuevo);
							}
						}							
					}
				}else if(!(mapa[J.getX()-1][J.getY()].compare("W") == 0)){
					Posicion NJ = J;
					NJ.setX(NJ.getX()-1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "U", NJ, C);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						repetidos.push_back(estadoNuevo);
						tope++;
					}
				}		
				//ABAJO
				if(hayCaja(J.getX()+1, J.getY(), C)){
					if(!(hayCaja(J.getX()+2, J.getY(), C) || mapa[J.getX()+2][J.getY()].compare("W") == 0)){
						Posicion NJ = J;
						vector <Posicion> NC = C;
						NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX()+1, NJ.getY())));
						Posicion PC = Posicion(NJ.getX()+2, NJ.getY());
						NC.push_back(PC);
						NJ.setX(NJ.getX()+1);
						Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "D", NJ, NC);
						repetido = verificarRepetido(repetidos, estadoNuevo);
						if(!repetido && !esquina(PC)){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
							if(!repetido){
								repetidos.push_back(estadoNuevo);
							}
						}
					}
				}else if(!(mapa[J.getX()+1][J.getY()].compare("W") == 0)){
					Posicion NJ = J;
					NJ.setX(NJ.getX()+1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "D", NJ, C);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
						repetidos.push_back(estadoNuevo);
					}
				}
				//IZQUIERDA
				if(hayCaja(J.getX(), J.getY()-1, C)){
					if(!(hayCaja(J.getX(), J.getY()-2, C) || mapa[J.getX()][J.getY()-2].compare("W") == 0)){
						Posicion NJ = J;
						vector <Posicion> NC = C;				
						NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX(), NJ.getY()-1)));
						Posicion PC = Posicion(NJ.getX(), NJ.getY()-2);
						NC.push_back(PC);
						NJ.setY(NJ.getY()-1);
						Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "L", NJ, NC);
						repetido = verificarRepetido(repetidos, estadoNuevo);
						if(!repetido && !esquina(PC)){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
							if(!repetido){
								repetidos.push_back(estadoNuevo);
							}
						}
					}
				}else if(!(mapa[J.getX()][J.getY()-1].compare("W") == 0)){
					Posicion NJ = J;
					NJ.setY(NJ.getY()-1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "L", NJ, C);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
						repetidos.push_back(estadoNuevo);
					}
				}
				//DERECHA
				if(hayCaja(J.getX(), J.getY()+1, C)){
					if(!(hayCaja(J.getX(), J.getY()+2, C) || mapa[J.getX()][J.getY()+2].compare("W") == 0)){
						Posicion NJ = J;
						vector <Posicion> NC = C;
						NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX(), NJ.getY()+1)));
						Posicion PC = Posicion(NJ.getX(), NJ.getY()+2);
						NC.push_back(PC);
						NJ.setY(NJ.getY()+1);
						Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "R", NJ, NC);
						repetido = verificarRepetido(repetidos, estadoNuevo);
						if(!repetido && !esquina(PC)){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
							if(!repetido){
								repetidos.push_back(estadoNuevo);
							}
						}
					}
				}else if(!(mapa[J.getX()][J.getY()+1].compare("W") == 0)){
					Posicion NJ = J;
					NJ.setY(NJ.getY()+1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "R", NJ, C);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
						repetidos.push_back(estadoNuevo);
					}
				}	
			}
			if(!arbol.empty()){
				estadoActual = arbol.at(0);
			}else{
				return "\nNo se ha logrado resolver el problema.\n";
			}
		}else{
			Posicion J = estadoActual.getJugador();
			vector <Posicion> C = estadoActual.getCajas();
			//ARRIBA
			if(hayCaja(J.getX()-1, J.getY(), C)){
				if(!(hayCaja(J.getX()-2, J.getY(), C) || mapa[J.getX()-2][J.getY()].compare("W") == 0)){
					Posicion NJ = J;
					vector <Posicion> NC = C;
					NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX()-1, NJ.getY())));
					Posicion PC = Posicion(NJ.getX()-2, NJ.getY());
					NC.push_back(PC);
					NJ.setX(NJ.getX()-1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "U", NJ, NC);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido && !esquina(PC)){
						if(tipo.compare("A") == 0){
							arbol.push_back(estadoNuevo);
						}else if(tipo.compare("P") == 0){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
						}
						if(!repetido){
							repetidos.push_back(estadoNuevo);
						}
					}
				}
			}else if(!(mapa[J.getX()-1][J.getY()].compare("W") == 0)){
				Posicion NJ = J;
				NJ.setX(NJ.getX()-1);
				Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "U", NJ, C);
				repetido = verificarRepetido(repetidos, estadoNuevo);
				if(!repetido){
					if(tipo.compare("A") == 0){
						arbol.push_back(estadoNuevo);
					}else if(tipo.compare("P") == 0){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
					}
					repetidos.push_back(estadoNuevo);
				}
			}	
			//ABAJO
			if(hayCaja(J.getX()+1, J.getY(), C)){
				if(!(hayCaja(J.getX()+2, J.getY(), C) || mapa[J.getX()+2][J.getY()].compare("W") == 0)){
					Posicion NJ = J;
					vector <Posicion> NC = C;
					NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX()+1, NJ.getY())));
					Posicion PC = Posicion(NJ.getX()+2, NJ.getY());
					NC.push_back(PC);
					NJ.setX(NJ.getX()+1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "D", NJ, NC);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido && !esquina(PC)){
						if(tipo.compare("A") == 0){
							arbol.push_back(estadoNuevo);
						}else if(tipo.compare("P") == 0){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
						}
						if(!repetido){
							repetidos.push_back(estadoNuevo);
						}
					}
				}
			}else if(!(mapa[J.getX()+1][J.getY()].compare("W") == 0)){
				Posicion NJ = J;
				NJ.setX(NJ.getX()+1);
				Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "D", NJ, C);
				repetido = verificarRepetido(repetidos, estadoNuevo);
				if(!repetido){
					if(tipo.compare("A") == 0){
						arbol.push_back(estadoNuevo);
					}else if(tipo.compare("P") == 0){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
					}
					repetidos.push_back(estadoNuevo);
				}
			}
			//IZQUIERDA
			if(hayCaja(J.getX(), J.getY()-1, C)){
				if(!(hayCaja(J.getX(), J.getY()-2, C) || mapa[J.getX()][J.getY()-2].compare("W") == 0)){
					Posicion NJ = J;
					vector <Posicion> NC = C;				
					NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX(), NJ.getY()-1)));
					Posicion PC = Posicion(NJ.getX(), NJ.getY()-2);
					NC.push_back(PC);
					NJ.setY(NJ.getY()-1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "L", NJ, NC);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido && !esquina(PC)){
						if(tipo.compare("A") == 0){
							arbol.push_back(estadoNuevo);
						}else if(tipo.compare("P") == 0){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
						}
						if(!repetido){
							repetidos.push_back(estadoNuevo);
						}
					}
				}
			}else if(!(mapa[J.getX()][J.getY()-1].compare("W") == 0)){
				Posicion NJ = J;
				NJ.setY(NJ.getY()-1);
				Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "L", NJ, C);
				repetido = verificarRepetido(repetidos, estadoNuevo);
				if(!repetido){
					if(tipo.compare("A") == 0){
						arbol.push_back(estadoNuevo);
					}else if(tipo.compare("P") == 0){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
					}
					repetidos.push_back(estadoNuevo);
				}
			}	
			//DERECHA
			if(hayCaja(J.getX(), J.getY()+1, C)){
				if(!(hayCaja(J.getX(), J.getY()+2, C) || mapa[J.getX()][J.getY()+2].compare("W") == 0)){
					Posicion NJ = J;
					vector <Posicion> NC = C;
					NC.erase(NC.begin() + buscarCaja(NC,Posicion(NJ.getX(), NJ.getY()+1)));
					Posicion PC = Posicion(NJ.getX(), NJ.getY()+2);
					NC.push_back(PC);
					NJ.setY(NJ.getY()+1);
					Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "R", NJ, NC);
					repetido = verificarRepetido(repetidos, estadoNuevo);
					if(!repetido && !esquina(PC)){
						if(tipo.compare("A") == 0){
							arbol.push_back(estadoNuevo);
						}else if(tipo.compare("P") == 0){
							arbol.insert(arbol.begin() + tope, estadoNuevo);
							tope++;
						}
						if(!repetido){
							repetidos.push_back(estadoNuevo);
						}
					}
				}
			}else if(!(mapa[J.getX()][J.getY()+1].compare("W") == 0)){
				Posicion NJ = J;
				NJ.setY(NJ.getY()+1);
				Nodo estadoNuevo = Nodo(estadoActual.getProfundidad()+1, estadoActual.getRegla() + "R", NJ, C);
				repetido = verificarRepetido(repetidos, estadoNuevo);
				if(!repetido){
					if(tipo.compare("A") == 0){
						arbol.push_back(estadoNuevo);
					}else if(tipo.compare("P") == 0){
						arbol.insert(arbol.begin() + tope, estadoNuevo);
						tope++;
					}
					repetidos.push_back(estadoNuevo);
				}
			}				
			if(!arbol.empty()){
				estadoActual = arbol.at(0);
			}else{
				return "\nNo se ha logrado resolver el problema.\n";
			}
		}			
	}
	return estadoActual.getRegla();
}

int main(int argc, char * argv[]) {	
	
	vector <string> lineas = leerFichero(argv[1]);
	int columnas = lineas.at(0).length();
	int filas = contarFilas(lineas);
	
	//Inicialización del mapa del juego y los objetivos:
	mapa = new string * [filas];
	for(int x=0; x<filas; x++){
		mapa[x] = new string[columnas];
	}	
	for(int x=0; x<filas; x++){
		for(int y=0; y<columnas; y++){
			mapa[x][y] = lineas.at(x)[y];
		}
	}	
	buscarObjetivos(filas, columnas);
	
	//Inicialización del estado inicial:
	vector <Posicion> cajas;
	for(int x=filas+1; x<lineas.size(); x++){
		cajas.push_back(Posicion(lineas.at(x))); 
	}	
	vector <Nodo> estadoInicial;
	vector <Nodo> repetidos;
	estadoInicial.push_back(Nodo(0,"",Posicion(lineas.at(filas)), cajas));
	
	string solucion;
	
	//Búsqueda preferente por amplitud:
	solucion = buscarSolucion(estadoInicial, repetidos, "A",-1);
	cout << solucion << endl;
	cout << endl;
	
	//Búsqueda preferente por profundidad:
	solucion = buscarSolucion(estadoInicial, repetidos, "P",-1);
	cout << solucion << endl;
	cout << endl;
	
	//Búsqueda por profundidad iterativa:
	int limite = 0;
	solucion = "";
	while(solucion.compare("\nNo se ha logrado resolver el problema.\n") == 0 || solucion.compare("") == 0){
		solucion = buscarSolucion(estadoInicial, repetidos, "PI", limite);
		limite++;
	}
	cout << solucion << endl;	
	
	return 0;	
	
}

