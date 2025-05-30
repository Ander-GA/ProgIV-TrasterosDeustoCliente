#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
using namespace std;

//Simula que limpia la consola empujando el contenido para arriba
void limpiarConsola() {
    for (int i = 0; i < 3; i++) {
        cout<<"..............................................................................................................."<<endl;
    }
}
char mostrarMenuPrincipal() {
    char opcion;
    cout<<"---Menu PRINCIPAL----"<<endl;
    cout<<"1. Area Administrador"<<endl;
    cout<<"2. Area Cliente"<<endl;
    cout<<"0. Cerrar Programa"<<endl;
    cout<<"Seleccione una opcion: ";

    cin>>opcion;
    return opcion;
}

char menuIniReg() {
    char opcion;
    cout<<"\n--- MENÚ CLIENTE ---"<<endl;
    cout<<"1. Iniciar Sesión"<<endl;
    cout<<"2. Registrarse"<<endl;
    cout<<"0. Cerrar"<<endl;
    cout<<"Seleccione una opción: ";

    cin>>opcion;
    return opcion;
}

char menuCliente() {
	char opcion;
	cout<<"MENÚ"<<endl;
	cout<<"1. Perfil"<<endl;
	cout<<"2. Catalogo"<<endl;
	cout<<"3. Alquilar trastero"<<endl;
	cout<<"4. Devolver trastero"<<endl;
	cout<<"0. Cerrar Sesion"<<endl;
	cout<<"Seleccione una opcion: ";

	cin>>opcion;
	return opcion;
}
char menuCatalogo(){
	char opcion;

	limpiarConsola();
	cout<<"CATALOGO"<<endl;
	cout<<"----------------------------------------"<<endl;
	cout<<"1. Ver todo el Catatalogo"<<endl;
	cout<<"2. Filtrar por Precio"<<endl;
	cout<<"3. Filtrar por Metros Cuadrados"<<endl;
	cout<<"4. Filtrar por Valoracion"<<endl;
	cout<<"0. Volver atras"<<endl;
	cout<<"Seleccione una opcion: ";
	cin>>opcion;
	return opcion;
}


char menuAdministrador() {
    char opcion;

    limpiarConsola();
    cout<<"Menu ADMINISTRADOR"<<endl;
    cout<<"1. Añadir Trastero"<<endl;
    cout<<"2. Eliminar Trastero"<<endl;
    cout<<"3. Ver Clientes"<<endl;
    cout<<"4. Ver Trasteros Disponibles"<<endl;
    cout<<"0. Salir"<<endl;
    cout<<"Seleccione una opcion: "<<endl;

    cin>>opcion;
    return opcion;
}


char menuTrasterosAdmin(){
	char opcion;
	    cout<<"VISUALIZAR TRASTEROS"<<endl;
	    cout<<"1. VER TODOS LOS TRASTEROS"<<endl;
	    cout<<"2. VER TRASTEROS ALQUILADOS"<<endl;
	    cout<<"3. VER TRASTEROS DISPONIBLES"<<endl;
	    cout<<"0. Salir"<<endl;
	    cout<<"Seleccione una opcion: ";
	    cin>>opcion;
	    return opcion;
}



