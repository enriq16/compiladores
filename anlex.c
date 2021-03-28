/*
 *	Analizador Léxico	
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Práctica de Programación Nro. 1
 *	
 *	Descripcion:
 *	Implementa un analizador léxico que reconoce números, identificadores, 
 * 	palabras reservadas, operadores y signos de puntuación para un lenguaje
 * 	con sintaxis tipo Pascal.
 *	
 */

/*********** Inclusión de cabecera **************/
#include "anlex.h"


/************* Variables globales **************/

int consumir;			/* 1 indica al analizador lexico que debe devolver
						el sgte componente lexico, 0 debe devolver el actual */

char cad[5*TAMLEX];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente pascal
char buff[2*TAMBUFF];	// Buffer para lectura de archivo fuente
char paraImprimir[100];
char lexema[TAMLEX];	// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea = 0;		// Numero de Linea
int lineaVacia;			//Indica si se va a imprimir la linea 'paraImprimir'

/**************** Funciones **********************/


// Rutinas del analizador lexico

void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n", numLinea+1, mensaje);	
}

void getToken()
{
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	entrada e;
	int leer = 0;//en caso de erro hace que se lea hasta el final de la linea

	while((c=fgetc(archivo))!=EOF)
	{
		
		if (c=='\n'){
			//incrementar el numero de linea
			numLinea++;
			break;					
			
		}
		else if(c==' ' || c=='\t' || leer)
		{
			continue;	//eliminar espacios en blanco o leer hasta fin de linea			
		}else if(c == '"'){
			entrada e;
			i = 0;
			strcpy(lexema, "");
			
			lexema[i]=c;
			i++;
			c = fgetc(archivo);
			//printf("c: '%c'", c);
				if ( isalpha(c) || isdigit(c) || c == '_' )
				{
					//es un identificador (o palabra reservada)
					
					do{
						lexema[i]=c;
						i++;
						c=fgetc(archivo);
						//printf(" c: '%c'", c);
						if (i>=TAMLEX){
							leer = 1;
							lineaVacia = 1;
							error("Longitud de Identificador excede tamaño de buffer");			
							break;
						}
					}while( isalpha(c) || isdigit(c) || c == '_' || c == ' ' );
					
					if(leer)
						continue;

					if (c == '"'){
						lexema[i] = c;
						i++;
					}else{
						leer = 1;
						lineaVacia = 1;
						sprintf(msg, "No cierra string '%i'", c); 
						error(msg);
						continue;
					}

					lexema[i]='\0';
					c = 0;
					/*
					if (c!=EOF)
						ungetc(c,archivo);
					else
						c=0;
					*/
					t.pe=buscar(lexema);
					t.compLex = t.pe->compLex;

					if (t.pe->compLex==-1)
					{
						strcpy(e.lexema,lexema);
						e.compLex = STRING;
						insertar(e);
						t.pe=buscar(lexema);
						t.compLex = STRING;
					}
					
					lineaVacia = 0;
					strcat(paraImprimir, "STRING ");

				}else if (c == '"'){
					lineaVacia = 1;
					sprintf(msg,"1No se esperaba: '%i', string vacio",c);
					error(msg);
					estado = -1;
				}else{
					lineaVacia = 1;
					sprintf(msg,"2No se esperaba: '%c'",c);
					error(msg);
					estado = -1;
				}
		}		
		else if (isdigit(c))
		{
				//es un numero
				entrada e;
				i=0;
				estado=0;
				acepto=0;

				strcpy(lexema, "");
				lexema[i]=c;
				
				while(!acepto)
				{
					switch(estado){
					case 0: //una secuencia netamente de digitos, puede ocurrir . o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							lexema[++i]=c;
							estado=0;
						}
						else if(c=='.'){
							lexema[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e'){
							lexema[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
					
					case 1://un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							lexema[++i]=c;
							estado=2;
						}						
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							error(msg);
							estado=-1;
							leer = 1;
							lineaVacia = 1;
						}
						break;
					case 2://la fraccion decimal, pueden seguir los digitos o e
						c=fgetc(archivo);
						if (isdigit(c))
						{
							lexema[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							lexema[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://una e, puede seguir +, - o una secuencia de digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							lexema[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							lexema[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 4://necesariamente debe venir por lo menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							lexema[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
						}
						break;
					case 5://una secuencia de digitos correspondiente al exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							lexema[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}break;
					case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
						if (c!=EOF)
							ungetc(c,archivo);
						else
							c=0;

						lexema[++i]='\0';
						acepto=1;
						t.pe=buscar(lexema);
						if (t.pe->compLex==-1)
						{
							strcpy(e.lexema,lexema);
							e.compLex=NUM;
							insertar(e);
							t.pe=buscar(lexema);
						}
						t.compLex = NUM;
						
						lineaVacia = 0;
						strcat(paraImprimir, "NUMBER ");
											

						break;
					case -1:
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else
							error(msg);
						exit(1);
					}
				}
			
		}		
		else if (c==':')
		{
			
			t.compLex=':';
			t.pe=buscar(":");
			lineaVacia = 0;
			strcat(paraImprimir, "DOS_PUNTOS ");
			
		}		
		else if (c==',')
		{
			t.compLex=',';
			t.pe=buscar(",");
			lineaVacia = 0;
			strcat(paraImprimir, "COMA ");
		}		
		else if (c=='[')
		{
			t.compLex='[';
			t.pe=buscar("[");
			lineaVacia = 0;
			strcat(paraImprimir, "L_LLAVE ");
		}
		else if (c==']')
		{
			t.compLex=']';
			t.pe=buscar("]");
			lineaVacia = 0;
			strcat(paraImprimir, "R_LLAVE ");			
		}		
		else if (c=='{')
		{
			t.compLex='{';
			t.pe=buscar("{");
			lineaVacia = 0;
			strcat(paraImprimir, "L_CORCHETE ");
		}
		else if (c=='}')
		{
			t.compLex='}';
			t.pe=buscar("}");
			lineaVacia = 0;
			strcat(paraImprimir, "R_CORCHETE ");
		}
		else if (tolower(c)=='f')
		{
			entrada e;
			i=0;			
			strcpy(lexema, "");
			lexema[i]=c;
			i++;
			c=fgetc(archivo);
			if(tolower(c)=='a'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='l'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='s'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='e'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}


			t.compLex = PR_BOOLEAN;
			t.pe=buscar("false");
			


			lineaVacia = 0;
			strcat(paraImprimir, "PR_FALSE ");
		}
		else if (tolower(c)=='t')
		{
			entrada e;
			i=0;			
			strcpy(lexema, "");
			lexema[i]=c;
			i++;
			c=fgetc(archivo);
			if(tolower(c)=='r'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='u'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='e'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			
			t.compLex = PR_BOOLEAN;
			t.pe=buscar("true");		


			lineaVacia = 0;
			strcat(paraImprimir, "PR_TRUE ");
		}else if(tolower(c)=='n'){
			i=0;			
			strcpy(lexema, "");
			lexema[i]=c;
			i++;
			c=fgetc(archivo);
			if(tolower(c)=='u'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='l'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='l'){
				lexema[i]=c;
				i++;
			}else{
				lineaVacia = 1;
				leer = 1;
				sprintf(msg, "No se esperaba '%c'", c); 
				error(msg);								
				continue;
			}

			
			t.compLex = PR_NULL;
			t.pe=buscar("PR_NULL");		


			lineaVacia = 0;
			strcat(paraImprimir, "PR_NULL ");
		}
		else if (c!=EOF)
		{
			sprintf(msg,"--> %c no esperado <--",c);
			error(msg);
		}
	}

	if (c==EOF)
	{
		t.compLex=EOF;
		// strcpy(e.lexema,"EOF");
		sprintf(e.lexema,"EOF");
		t.pe=&e;
	}
	
}

int main(int argc,char* args[])
{
	// inicializar analizador lexico

	initTabla();
	initTablaSimbolos();
	
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		while (t.compLex!=EOF){
			lineaVacia = 1;
			strcpy(paraImprimir, "");
			getToken();
			if(lineaVacia == 0){				
				printf("%d: %s\n", numLinea, paraImprimir);
			}	
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);



	}

	return 0;
}
