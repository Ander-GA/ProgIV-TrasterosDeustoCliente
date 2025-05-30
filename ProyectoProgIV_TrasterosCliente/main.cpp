// IMPORTANT: Winsock Library ("ws2_32") should be linked
//Un socket es un mecanimo de comunicaci�n entre 2 procesos cualesquiera
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include "domain/menu.h"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
using namespace std;

int main(int argc, char *argv[]) {

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	/*
	 * En sendBuff guardaremos lo que el cliente le env�a al servidor
	 * En recvBuff guardaremos lo que el servidor le env�a al cliente
	 * */
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP); //INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

	/*EMPIEZA EL PROGRAMA DEL CLIENTE*/

	char opcion, opcionAdmin, opcionAdminTrasteros;
	char opcionUsuario,opcionUsuarioMenu,opcionMenuCatalogo;
	char usuarioAdmin[50], contrasena[50];

	do {
	    opcion = mostrarMenuPrincipal();
		memset(sendBuff, 0, sizeof(sendBuff));
		sprintf(sendBuff, "%c", opcion); // Enviar la opción
		send(s, sendBuff, strlen(sendBuff)+1, 0);
		memset(recvBuff, 0, sizeof(recvBuff));
		recv(s, recvBuff, sizeof(recvBuff), 0);
		cout << recvBuff << endl;
	    switch (opcion) {
	    case '1': {
	        // admin
	        int result = 0; // flag para saber si es correcto o no
	        do { // INICIO DE SESION DEL ADMIN
	        	cout<<"INICIAR SESION ADMIN"<<endl;
	        	cout<<"--------------------"<<endl;
	            cout << "Ingrese usuario: ";
	            cin >> usuarioAdmin;
	            strcpy(sendBuff, usuarioAdmin);
	            send(s, sendBuff, strlen(sendBuff) + 1, 0); // enviar usuario
	            		//Que usuario ha recibido el servidor
	            		recv(s, recvBuff, sizeof(recvBuff), 0);  // recibir
	            		cout<<recvBuff<<endl;
	            cout << "Ingrese contraseña: ";
	            cin >> contrasena;
	            strcpy(sendBuff, contrasena);
	            send(s, sendBuff, strlen(sendBuff) + 1, 0); // enviar contraseña
	            		//Que contraseña ha recibido el servidor
						recv(s, recvBuff, sizeof(recvBuff), 0);  // recibir
						cout<<recvBuff<<endl;
	            memset(sendBuff, 0, sizeof(sendBuff)); // Limpiamos buffer
	            recv(s, recvBuff, sizeof(recvBuff), 0);  // recibimos resultado

	            if (strcmp(recvBuff, "0") == 0) { // (1- INICIO CORRECTO/0- INICIO INCORRECTO)
	                cout << "\033[1;31mInicio de sesion incorrecto\033[0m" << endl;
	            }
	            else {
	                result = 1;
	            }
	        } while (result != 1);

	        cout << "\033[1;32mInicio de sesion correcto\033[0m" << endl;

	        // Una vez iniciado sesion ejecutamos menu del ADMIN
	        do {
	            opcionAdmin = menuAdministrador();
	            memset(sendBuff, 0, sizeof(sendBuff));
				sprintf(sendBuff, "%c", opcionAdmin); // Enviar la opción
				send(s, sendBuff, strlen(sendBuff)+1, 0);
				memset(recvBuff, 0, sizeof(recvBuff));
				recv(s, recvBuff, sizeof(recvBuff), 0);
				cout << recvBuff << endl;


	            switch (opcionAdmin) {
	            case '1': {
	                // AÑADIR TRASTERO
	                int numeroTrastero, metrosCuadrados;
	                float precio;
	                cout << "AÑADIR TRASTERO" << endl;
	                cout << "-----------------" << endl;

	                cout << "Numero del trastero: ";
	                cin >> numeroTrastero;
	                // Mandamos el trastero al servidor
	                sprintf(sendBuff, "%d", numeroTrastero);
	                send(s, sendBuff, strlen(sendBuff) + 1, 0);
	                //Que numero de trastero ha recibido el servidor
	                		memset(recvBuff, 0, sizeof(recvBuff)); // Limpiar buffer antes de recibir
	                		recv(s, recvBuff, sizeof(recvBuff), 0);
	                		cout << recvBuff << endl;
	                cout << "Metros Cuadrados: ";
	                cin >> metrosCuadrados;
	                sprintf(sendBuff, "%d", metrosCuadrados);
					send(s, sendBuff, strlen(sendBuff) + 1, 0);
	                //Que numero de trastero ha recibido el servidor
							memset(recvBuff, 0, sizeof(recvBuff)); // Limpiar buffer antes de recibir
							recv(s, recvBuff, sizeof(recvBuff), 0);
							cout << recvBuff << endl;

					cout << "Precio: ";
					cin >> precio;
					sprintf(sendBuff, "%f", precio);
					send(s, sendBuff, strlen(sendBuff) + 1, 0);
					//Recibir confirmación del servidor para el precio
							memset(recvBuff, 0, sizeof(recvBuff)); // Limpiar buffer antes de recibir
							recv(s, recvBuff, sizeof(recvBuff), 0);
							cout << recvBuff << endl;

	                // LA DISPONIBILIDAD, VALORACION Y NUMERO DE VALORACIONES YA LAS INICIALIZARA EL SERVIDOR
	                if (numeroTrastero <= 0) {
	                    cout << "\033[1;33mEl numero del trastero debe ser mayor a 0\033[0m" << endl;
	                }
	                else {
	                    // Recibimos resultado de si el trastero se ha almacenado correctamente o no
	                    recv(s, recvBuff, sizeof(recvBuff), 0);  // recibimos flag

	                    // recvBuff es cadena, comparo con "1"
	                    if (strcmp(recvBuff, "1") == 0) {
	                        cout << "\033[1;32mTrastero con numero " << numeroTrastero << " añadido correctamente.\033[0m" << endl;
	                    }
	                    else if(strcmp(recvBuff, "4") == 0){
	                        cout << "\033[38;5;214mEl trastero con numero " << numeroTrastero << " ya existe.\033[0m" << endl;
	                    }else if(strcmp(recvBuff, "2") == 0){
	                    	cout << "\033[38;5;214mEl trastero con numero " << numeroTrastero << " no puede añadirse, no hay espacio.\033[0m" << endl;
	                    }else cout << "\033[1;31mNo se pudo añadir el trastero\033[0m" << endl;
	                }
	                break;
	            }
	            case '2':{
	            	char numTrastero[50];
	            	cout << "ELIMINAR TRASTERO" << endl;
	            	cout << "-----------------" << endl;
	            	cout << "Numero del trastero: ";
	            	cin >> numTrastero;

	            	// Enviar número de trastero al servidor
	            	memset(sendBuff, 0, sizeof(sendBuff));
	            	strcpy(sendBuff, numTrastero);
	            	send(s, sendBuff, strlen(sendBuff) + 1, 0);

	            	// Recibir confirmación de qué número recibió el servidor
	            	memset(recvBuff, 0, sizeof(recvBuff));
	            	recv(s, recvBuff, sizeof(recvBuff), 0);
	            	cout << "Servidor recibió el trastero número: " << recvBuff << endl;

	            	// Recibir el flag de resultado de la operación
	            	memset(recvBuff, 0, sizeof(recvBuff));
	            	recv(s, recvBuff, sizeof(recvBuff), 0);

	            	if (strcmp(recvBuff, "1") == 0) {
	            	    cout << "\033[1;32mTrastero con numero " << numTrastero << " se ha borrado correctamente.\033[0m" << endl;
	            	} else {
	            	    cout << "\033[1;31mEl trastero con numero " << numTrastero << " no existe.\033[0m" << endl;
	            	}
	            	break;
	            }
	            case '3':{
	            	// VER TODOS LOS USUARIOS
	            	    int numClientes,dni,telefono;
	            	    char nombre[50], apellidos[50];
	            	    char email[50], direccion[100], contrasena[30];

	            	    memset(recvBuff, 0, sizeof(recvBuff));
	            	    recv(s, recvBuff, sizeof(recvBuff), 0);
	            	    numClientes = atoi(recvBuff);
	            	    cout << "Numero de Usuarios: " << numClientes << endl;

	            	    if (numClientes > 0) {
	            	        // Cabecera de la tabla
	            	        printf("\033[34m%-15s%-15s%-12s%-12s%-20s%-15s%-15s\033[0m\n",
	            	               "NOMBRE", "APELLIDOS", "DNI", "TELEFONO", "EMAIL", "DIRECCION", "CONTRASEÑA");
	            	        printf("=====================================================================================================\n");

	            	        for (int i = 0; i < numClientes; i++) {
	            	            // Recibir nombre
	            	            memset(recvBuff, 0, sizeof(recvBuff));
	            	            recv(s, recvBuff, sizeof(recvBuff), 0);
	            	            strcpy(nombre, recvBuff);

	            	            // Recibir apellidos
	            	            memset(recvBuff, 0, sizeof(recvBuff));
	            	            recv(s, recvBuff, sizeof(recvBuff), 0);
	            	            strcpy(apellidos, recvBuff);

	            	            // Recibir DNI
	            	            memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								dni = atoi(recvBuff);

	            	            // Recibir teléfono
	            	            memset(recvBuff, 0, sizeof(recvBuff));
	            	            recv(s, recvBuff, sizeof(recvBuff), 0);
	            	            telefono = atoi(recvBuff);

	            	            // Recibir email
	            	            memset(recvBuff, 0, sizeof(recvBuff));
	            	            recv(s, recvBuff, sizeof(recvBuff), 0);
	            	            strcpy(email, recvBuff);

	            	            // Recibir dirección
	            	            memset(recvBuff, 0, sizeof(recvBuff));
	            	            recv(s, recvBuff, sizeof(recvBuff), 0);
	            	            strcpy(direccion, recvBuff);

	            	            // Recibir contraseña
	            	            memset(recvBuff, 0, sizeof(recvBuff));
	            	            recv(s, recvBuff, sizeof(recvBuff), 0);
	            	            strcpy(contrasena, recvBuff);

	            	            // Mostrar la información del usuario
	            	            printf("%-15s%-15s%-12d%-12d%-20s%-15s%-15s\n",
	            	                   nombre, apellidos, dni, telefono, email, direccion, contrasena);
	            	        }
	            	    } else {
	            	        cout << "No hay usuarios registrados." << endl;
	            	    }
	                break;
	            }
	            case '4':{
	            	opcionAdminTrasteros = menuTrasterosAdmin();
	            	memset(sendBuff, 0, sizeof(sendBuff));
					sprintf(sendBuff, "%c", opcionAdminTrasteros); // Enviar la opción correcta
					send(s, sendBuff, strlen(sendBuff)+1, 0);

					memset(recvBuff, 0, sizeof(recvBuff));
					recv(s, recvBuff, sizeof(recvBuff), 0);
					cout << recvBuff << endl;
					switch (opcionAdminTrasteros) {
					case '1':{
						//VER TODOS LOS TRASTEROS
						int i, numTrasteros;
						int numero, numValoraciones;
						float metros, precio, valoracion;
						char disponibilidad[20];
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						numTrasteros = atoi(recvBuff);
						cout << "Numero de Trasteros: " << numTrasteros << endl;
						printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
						for(i = 0; i < numTrasteros; i++){
						        // Recibir número del trastero
						        memset(recvBuff, 0, sizeof(recvBuff));
						        recv(s, recvBuff, sizeof(recvBuff), 0);
						        numero = atoi(recvBuff);

						        // Recibir metros cuadrados
						        memset(recvBuff, 0, sizeof(recvBuff));
						        recv(s, recvBuff, sizeof(recvBuff), 0);
						        metros = atof(recvBuff);

						        // Recibir precio
						        memset(recvBuff, 0, sizeof(recvBuff));
						        recv(s, recvBuff, sizeof(recvBuff), 0);
						        precio = atoi(recvBuff);

						        // Recibir valoración
						        memset(recvBuff, 0, sizeof(recvBuff));
						        recv(s, recvBuff, sizeof(recvBuff), 0);
						        valoracion = atof(recvBuff);

						        // Recibir número de valoraciones
						        memset(recvBuff, 0, sizeof(recvBuff));
						        recv(s, recvBuff, sizeof(recvBuff), 0);
						        numValoraciones = atoi(recvBuff);

						        // Recibir disponibilidad
						        memset(recvBuff, 0, sizeof(recvBuff));
						        recv(s, recvBuff, sizeof(recvBuff), 0);
						        strcpy(disponibilidad, recvBuff);

						        // Mostrar la información del trastero
						        printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
						               numero, metros, precio, valoracion, numValoraciones, disponibilidad);
						    }
						break;
					}
					case '2':{
						//VER TODOS LOS TRASTEROS ALQUILADOS
						int i, numTrasteros;
						int numero, numValoraciones;
						float metros, precio, valoracion;
						char disponibilidad[20];
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						numTrasteros = atoi(recvBuff);
						cout << "Numero de Trasteros: " << numTrasteros << endl;
						printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
						for(i = 0; i < numTrasteros; i++){
								// Recibir número del trastero
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								numero = atoi(recvBuff);

								// Recibir metros cuadrados
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								metros = atof(recvBuff);

								// Recibir precio
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								precio = atoi(recvBuff);

								// Recibir valoración
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								valoracion = atof(recvBuff);

								// Recibir número de valoraciones
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								numValoraciones = atoi(recvBuff);

								// Recibir disponibilidad
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								strcpy(disponibilidad, recvBuff);

							   if(strcmp(disponibilidad,"OCUPADO")==0){
								   // Mostrar la información del trastero solo si estan alquilados
									printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
										numero, metros, precio, valoracion, numValoraciones, disponibilidad);
							   }

							}
						break;
					}
					case '3':{
						//VER LOS DISPONIBLES
						int i, numTrasteros;
						int numero, numValoraciones;
						float metros, precio, valoracion;
						char disponibilidad[20];
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						numTrasteros = atoi(recvBuff);
						cout << "Numero de Trasteros: " << numTrasteros << endl;
						printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
						for(i = 0; i < numTrasteros; i++){
								// Recibir número del trastero
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								numero = atoi(recvBuff);

								// Recibir metros cuadrados
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								metros = atof(recvBuff);

								// Recibir precio
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								precio = atoi(recvBuff);

								// Recibir valoración
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								valoracion = atof(recvBuff);

								// Recibir número de valoraciones
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								numValoraciones = atoi(recvBuff);

								// Recibir disponibilidad
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								strcpy(disponibilidad, recvBuff);

							   if(strcmp(disponibilidad,"DISPONIBLE")==0){
								   // Mostrar la información del trastero solo si estan alquilados
									printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
										numero, metros, precio, valoracion, numValoraciones, disponibilidad);
							   }

							}
						break;
					}

					}
	            }
	            case '0': cout<<"Saliendo del menú administrador..."<<endl;
	            default:
	            	cout<<"\033[1;31mOpcion Incorrecta.\033[0m"<<endl;
	                break;
	            }
	        } while (opcionAdmin != '0');

	        break;
	    }
	    case '2': {
	        //USUARIO
	    	do{
	    		//INICIAR SESION/REGISTRARSE
	    		opcionUsuario=menuIniReg();
	    		memset(sendBuff, 0, sizeof(sendBuff));
				sprintf(sendBuff, "%c", opcionUsuario); // Enviar la opción
				send(s, sendBuff, strlen(sendBuff)+1, 0);
				memset(recvBuff, 0, sizeof(recvBuff));
				recv(s, recvBuff, sizeof(recvBuff), 0);
				cout << recvBuff << endl;

				switch (opcionUsuario) {
					case '1'://Iniciar sesion
						int dni,dniUser;
						char contrasena[50];
						cout << "INICIAR SESION USUARIO" << endl;
						cout << "--------------------" << endl;
						cout << "Ingrese dni: ";
						cin >> dni;

						// Enviar DNI como entero (convertido a string para envío)
						memset(sendBuff, 0, sizeof(sendBuff));
						sprintf(sendBuff, "%d", dni); // Convertir int a string
						send(s, sendBuff, strlen(sendBuff) + 1, 0); // enviar DNI

						// Recibir confirmación del DNI
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese contraseña: ";
						cin >> contrasena;

						// Enviar contraseña
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, contrasena);
						send(s, sendBuff, strlen(sendBuff) + 1, 0); // enviar contraseña

						// Recibir confirmación de la contraseña
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						//Para flags
						// RECIBIR RESULTADO DE AUTENTICACIÓN
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout<<recvBuff<<endl;
						dniUser = atoi(recvBuff);
						if(dniUser!=-1){
							//EL USUARIO HA INICIADO SESION
							cout << "\033[1;32mInicio de sesion correcto\033[0m" << endl;

							do{
								opcionUsuarioMenu=menuCliente();
								memset(sendBuff, 0, sizeof(sendBuff));
								sprintf(sendBuff, "%c", opcionUsuarioMenu); // Enviar la opción
								send(s, sendBuff, strlen(sendBuff)+1, 0);
								memset(recvBuff, 0, sizeof(recvBuff));
								recv(s, recvBuff, sizeof(recvBuff), 0);
								cout << recvBuff << endl;

								switch (opcionUsuarioMenu) {
									case '1'://perfil
										// Recibir y mostrar los datos del perfil
										char nombreP[50],apellidoP[50],emailP[50],direccionP[50];
										int dniP,telefonoP;
										// Recibir nombre
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										strcpy(nombreP, recvBuff);

										// Recibir apellidos
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										strcpy(apellidoP, recvBuff);

										// Recibir DNI
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										dniP = atoi(recvBuff);

										// Recibir teléfono
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										telefonoP = atoi(recvBuff);

										// Recibir email
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										strcpy(emailP, recvBuff);

										// Recibir dirección
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										strcpy(direccionP, recvBuff);

										cout<<"---------------"<<endl;
										cout<<"PERFIL"<<endl;
										cout<<"---------------"<<endl;
										cout<<"DATOS DEL USUARIO"<<endl;
										cout<<"- Nombre: "<<nombreP<<endl;
										cout<<"- Apellido: "<<apellidoP<<endl;
										cout<<"- DNI: "<< dniP<<endl;
										cout<<"----------------------"<<endl;
										cout<<"DATOS DE CONTACTO"<<endl;
										cout<<"- Telefono: "<<telefonoP<<endl;
										cout<<"- Email: "<<emailP<<endl;
										cout<<"- Direccion: "<<direccionP<<endl;
										break;
									case '2'://catalogo
										do{
											opcionMenuCatalogo = menuCatalogo();
											memset(sendBuff, 0, sizeof(sendBuff));
											sprintf(sendBuff, "%c", opcionMenuCatalogo); // Enviar la opción
											send(s, sendBuff, strlen(sendBuff)+1, 0);
											memset(recvBuff, 0, sizeof(recvBuff));
											recv(s, recvBuff, sizeof(recvBuff), 0);
											cout << recvBuff << endl;
											switch (opcionMenuCatalogo) {
												case '1':{
													//VER TODO EL CATALOGO
													int i, numTrasteros;
													int numero, numValoraciones;
													float metros, precio, valoracion;
													char disponibilidad[20];
													memset(recvBuff, 0, sizeof(recvBuff));
													recv(s, recvBuff, sizeof(recvBuff), 0);
													numTrasteros = atoi(recvBuff);
													cout << "Numero de Trasteros: " << numTrasteros << endl;
													printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
													for(i = 0; i < numTrasteros; i++){
													        // Recibir número del trastero
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numero = atoi(recvBuff);

													        // Recibir metros cuadrados
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        metros = atof(recvBuff);

													        // Recibir precio
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        precio = atoi(recvBuff);

													        // Recibir valoración
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        valoracion = atof(recvBuff);

													        // Recibir número de valoraciones
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numValoraciones = atoi(recvBuff);

													        // Recibir disponibilidad
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        strcpy(disponibilidad, recvBuff);

													        // Mostrar la información del trastero
													        printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
													               numero, metros, precio, valoracion, numValoraciones, disponibilidad);
													    }
												break;}
												case '2':{//FILTRAR POR PRECIO
													int i, numTrasteros;
													int numero, numValoraciones;
													float metros, precio, valoracion;
													char disponibilidad[20];
													memset(recvBuff, 0, sizeof(recvBuff));
													recv(s, recvBuff, sizeof(recvBuff), 0);
													numTrasteros = atoi(recvBuff);
													cout << "Numero de Trasteros: " << numTrasteros << endl;
													printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
													for(i = 0; i < numTrasteros; i++){
													        // Recibir número del trastero
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numero = atoi(recvBuff);

													        // Recibir metros cuadrados
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        metros = atof(recvBuff);

													        // Recibir precio
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        precio = atoi(recvBuff);

													        // Recibir valoración
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        valoracion = atof(recvBuff);

													        // Recibir número de valoraciones
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numValoraciones = atoi(recvBuff);

													        // Recibir disponibilidad
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        strcpy(disponibilidad, recvBuff);

													        // Mostrar la información del trastero
													        printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
													               numero, metros, precio, valoracion, numValoraciones, disponibilidad);
													    }
													break;}
												case '3':{//FILTRAR POR METROS CUADRADOS
													int i, numTrasteros;
													int numero, numValoraciones;
													float metros, precio, valoracion;
													char disponibilidad[20];
													memset(recvBuff, 0, sizeof(recvBuff));
													recv(s, recvBuff, sizeof(recvBuff), 0);
													numTrasteros = atoi(recvBuff);
													cout << "Numero de Trasteros: " << numTrasteros << endl;
													printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
													for(i = 0; i < numTrasteros; i++){
													        // Recibir número del trastero
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numero = atoi(recvBuff);

													        // Recibir metros cuadrados
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        metros = atof(recvBuff);

													        // Recibir precio
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        precio = atoi(recvBuff);

													        // Recibir valoración
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        valoracion = atof(recvBuff);

													        // Recibir número de valoraciones
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numValoraciones = atoi(recvBuff);

													        // Recibir disponibilidad
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        strcpy(disponibilidad, recvBuff);

													        // Mostrar la información del trastero
													        printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
													               numero, metros, precio, valoracion, numValoraciones, disponibilidad);
													    }
													break;}
												case '4':{//FILTRAR POR VALORACION
													int i, numTrasteros;
													int numero, numValoraciones;
													float metros, precio, valoracion;
													char disponibilidad[20];
													memset(recvBuff, 0, sizeof(recvBuff));
													recv(s, recvBuff, sizeof(recvBuff), 0);
													numTrasteros = atoi(recvBuff);
													cout << "Numero de Trasteros: " << numTrasteros << endl;
													printf("\033[1;34m%s%20s%20s%20s%30s%20s\n\033[0m", "NºTRASTERO", "m²", "PRECIO", "VALORACION", "NUMERO DE VALORACIONES", "DISPONIBILIDAD");
													for(i = 0; i < numTrasteros; i++){
													        // Recibir número del trastero
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numero = atoi(recvBuff);

													        // Recibir metros cuadrados
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        metros = atof(recvBuff);

													        // Recibir precio
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        precio = atoi(recvBuff);

													        // Recibir valoración
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        valoracion = atof(recvBuff);

													        // Recibir número de valoraciones
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        numValoraciones = atoi(recvBuff);

													        // Recibir disponibilidad
													        memset(recvBuff, 0, sizeof(recvBuff));
													        recv(s, recvBuff, sizeof(recvBuff), 0);
													        strcpy(disponibilidad, recvBuff);

													        // Mostrar la información del trastero
													        printf("%10d%20.2f%20.2f%20.2f%30d%20s\n",
													               numero, metros, precio, valoracion, numValoraciones, disponibilidad);
													    }
													break;}
												case '0':
													cout<<"Volviendo al menu Cliente..."<<endl;
													break;

												default:cout<<"\033[1;31mOpcion Incorrecta.\033[0m"<<endl;
													break;
											}
										}while(opcionMenuCatalogo!='0');
										break;
									case '3':{
										//Alquilar Trastero
										int flag;
										char numTrastero[20];
										cout<<"ALQUILAR TRASTERO"<<endl;
										cout<<"--------------------"<<endl;
										cout<<"Introduce el numero de trastero: ";
										cin>>numTrastero;
										// Enviar nombre
										memset(sendBuff, 0, sizeof(sendBuff));
										strcpy(sendBuff, numTrastero);
										send(s, sendBuff, strlen(sendBuff) + 1, 0);

										// Recibir confirmacion de lo que ha recibido el servidor
										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										cout<<recvBuff<<endl;

										memset(recvBuff, 0, sizeof(recvBuff));
										recv(s, recvBuff, sizeof(recvBuff), 0);
										flag = atoi(recvBuff);
										switch (flag) {
											case 0:
												cout<<"\033[0;33mLo sentimos, este trastero no está disponible en nuestro catálogo.\033[0m"<<endl;
												break;
											case 1:
												cout<<"\033[0;32mEl trastero con numero "<<numTrastero<< " ha sido correctamente alquilado por "<<nombreP<<" \033[0m"<<endl;
												break;
											case 2:
												cout<<"\033[0;31mNo existe este trastero en nuestro catálogo.\033[0m"<<endl;
												break;
											default:
												cout<<"\033[0;31mNo se ha podido alquilar el trastero.\033[0m"<<endl;
												break;
										}

										break;
									}
									case '4'://Devolver trastero
										break;
									case '0':cout<<"Volviendo a menu Cliente..."<<endl;
										break;
									default:cout<<"\033[1;31mOpcion Incorrecta.\033[0m"<<endl;
										break;
								}
							}while(opcionUsuarioMenu);
						}else{
							cout << "\033[1;31mDni o contraseña incorrecta.\033[0m" << endl;
						}
						break;
					case '2'://Registrarse
						char nombre[50],apellido[50],email[100],direccion[100],contrasenia[50],confirmarContrasena[50],telefono[20],dniReg[20];
						int resultadoRegistro;

						cout << "REGISTRO DE USUARIO" << endl;
						cout << "-------------------" << endl;

						cout << "Ingrese nombre: ";
						cin >> nombre;

						// Enviar nombre
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, nombre);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación del nombre
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese apellido: ";
						cin >> apellido;

						// Enviar apellido
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, apellido);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación del apellido
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese email: ";
						cin >> email;

						// Enviar email
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, email);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación del email
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese direccion: ";
						cin >> direccion;

						// Enviar direccion
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, direccion);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación de la direccion
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese contraseña: ";
						cin >> contrasenia;

						// Enviar contraseña
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, contrasenia);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación de la contraseña
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Confirme contraseña: ";
						cin >> confirmarContrasena;

						// Enviar confirmar contraseña
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, confirmarContrasena);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación de confirmar contraseña
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese telefono: ";
						cin >> telefono;

						// Enviar telefono
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, telefono);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación del telefono
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						cout << "Ingrese DNI: ";
						cin >> dniReg;

						// Enviar DNI
						memset(sendBuff, 0, sizeof(sendBuff));
						strcpy(sendBuff, dniReg);
						send(s, sendBuff, strlen(sendBuff) + 1, 0);

						// Recibir confirmación del DNI
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						cout << recvBuff << endl;

						// RECIBIR RESULTADO DEL REGISTRO
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						resultadoRegistro = atoi(recvBuff);

						// PROCESAR RESULTADO
						switch (resultadoRegistro) {
						    case 1:
						        // Verde brillante para éxito
						        cout << "\033[1;32m¡Usuario registrado correctamente!\033[0m" << endl;
						        break;
						    case 0:
						        // Rojo para error de DNI duplicado
						        cout << "\033[1;31mError: Este DNI ya está registrado.\033[0m" << endl;
						        break;
						    case 2:
						        // Amarillo para error de contraseña
						        cout << "\033[1;33mError: Las contraseñas no coinciden.\033[0m" << endl;
						        break;
						    default:
						        // Magenta para otros errores desconocidos
						        cout << "\033[1;35mError desconocido en el registro.\033[0m" << endl;
						        break;
						}
						break;
					case '0':
						cout<<"Saliendo del menu Cliente"<<endl;
						break;
					default:
						break;
				}
	    	}while(opcionUsuario!='0');


	        break;
	    }
	    default:
	    	cout<<"\033[1;31mOpcion Incorrecta.\033[0m"<<endl;
	        break;
	    }

	} while (opcion != '0');


	/*ACABA EL PROGRAMA DEL CLIENTE*/
	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}
