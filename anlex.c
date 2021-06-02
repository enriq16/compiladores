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
char lexema[TAMLEX];	// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=0;			// Numero de Linea

char paraImprimir[100];

/**************** Funciones **********************/


// Rutinas del analizador lexico

void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);	
}

void getToken()
{
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	
	int leer = 1;//en caso de erro hace que se lea hasta el final de la linea

	while((c=fgetc(archivo))!=EOF)
	{
		entrada e;

		if (c==' ' || c=='\t'){
			continue;	//eliminar espacios en blanco		
		}else if(leer < 1){			
			continue;
		}else if(c=='\n')
		{
			//incrementar el numero de linea
			numLinea++;
			break;
		}else if(c == '"'){
						
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
							error("Longitud de Identificador excede tamaño de buffer");			
							break;
						}
					}while( isalpha(c) || isdigit(c) || c == '_' || c == ' ' );
					
					
					if (c == '"'){
						lexema[i] = c;
						i++;
						lexema[i]='\0';

						t.pe=buscar(lexema);
                        t.compLex=t.pe->compLex;
                        if (t.pe->compLex==-1)
                        {
                        	strcpy(e.lexema,lexema);
                        	e.compLex=STRING;
                        	insertar(e);
                        	t.pe=buscar(lexema);
                        	t.compLex=STRING;
                        }
                        strcat(paraImprimir, "STRING ");
					}else{ 
						leer = 0;
						lexema[i] = '\0';						
						sprintf(msg, "El String '%s', no termina correctamente ", lexema); 
						strcat(paraImprimir, msg);
						if(c==EOF)													
							ungetc(c,archivo);
						
						continue;
					}														
				}else if (c == '"'){
					lexema[i] = c;
					i++;
					lexema[i]='\0';
					t.pe=buscar(lexema);
                    t.compLex=t.pe->compLex;
                    if (t.pe->compLex==-1)
                    {
                     strcpy(e.lexema,lexema);
                     e.compLex=STRING;
                     insertar(e);
                     t.pe=buscar(lexema);
                     t.compLex=STRING;
                    }
                    
                    strcat(paraImprimir, "STRING ");										
				}else{
					
					sprintf(msg,"No se esperaba: '%c'",c);
					strcat(paraImprimir, msg);
					estado = -1;
					leer = 0;					
					if(c==EOF)													
						ungetc(c,archivo);
					continue;
				}
		}		
		else if (isdigit(c))
		{		
										
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
							estado = -1;
							leer = 0;							
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
							leer = 0;
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
						
						ungetc(c,archivo);
						
						lexema[++i]='\0';
						acepto=1;

						t.pe=buscar(lexema);
                        t.compLex=t.pe->compLex;
                        if (t.pe->compLex==-1)
                        {
                         strcpy(e.lexema,lexema);
                         e.compLex=NUM;
                         insertar(e);
                         t.pe=buscar(lexema);
                         t.compLex=NUM;
                        }											
												
						strcat(paraImprimir, "NUMBER ");											
						break;
					case -1:
												
						strcat(paraImprimir, msg);
						ungetc(c,archivo);
						continue;													
					}
				}
			
		}		
		else if (c==':')
		{
			

			e.compLex = ':';			
			strcpy(e.lexema,":");

			t.compLex=':';
			t.pe=buscar(":");			
			
			strcat(paraImprimir, "DOS_PUNTOS ");			
		}		
		else if (c==',')
		{   
			

			e.compLex = ',';			
			strcpy(e.lexema,",");
			
			t.compLex=',';
			t.pe=buscar(",");
			
			strcat(paraImprimir, "COMA ");
		}		
		else if (c=='[')
		{
			

			e.compLex = '[';			
			strcpy(e.lexema,"[");

			t.compLex='[';
			t.pe=buscar("[");
			
			strcat(paraImprimir, "L_CORCHETE ");
		}
		else if (c==']')
		{
			

			e.compLex = ']';			
			strcpy(e.lexema,"]");

			t.compLex=']';
			t.pe=buscar("]");
			
			strcat(paraImprimir, "R_CORCHETE ");			
		}		
		else if (c=='{')
		{
			

			e.compLex = '{';			
			strcpy(e.lexema,"{");

			t.compLex='{';
			t.pe=buscar("{");
			
			strcat(paraImprimir, "L_LLAVE ");
		}
		else if (c=='}')
		{
			

			e.compLex = '}';			
			strcpy(e.lexema,"}");

			t.compLex='}';
			t.pe=buscar("}");
			
			strcat(paraImprimir, "R_LLAVE ");
		}
		else if (tolower(c)=='f')
		{
			
			i=0;			
			strcpy(lexema, "");
			lexema[i]=tolower(c);
			i++;
			c=fgetc(archivo);
			if(tolower(c)=='a'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c'", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='l'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c'", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='s'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c'", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='e'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			
			e.compLex = PR_BOOLEAN;
			strcpy(e.lexema, lexema);

			t.compLex = PR_BOOLEAN;
			t.pe=buscar(lexema);
			
			strcat(paraImprimir, "PR_FALSE ");
		}
		else if (tolower(c)=='t')
		{
			
			i=0;			
			strcpy(lexema, "");
			lexema[i]=tolower(c);
			i++;
			c=fgetc(archivo);
			if(tolower(c)=='r'){
				lexema[i]=tolower(c);
				i++;
			}else{
				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='u'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='e'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}


			
			e.compLex = PR_BOOLEAN;
			strcpy(e.lexema, lexema);

			t.compLex = PR_BOOLEAN;
			t.pe=buscar(lexema);
						
			strcat(paraImprimir, "PR_TRUE ");
		}else if(tolower(c)=='n'){
			i=0;			
			strcpy(lexema, "");
			lexema[i]=tolower(c);
			i++;
			c=fgetc(archivo);
			if(tolower(c)=='u'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='l'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			c=fgetc(archivo);
			if(tolower(c)=='l'){
				lexema[i]=tolower(c);
				i++;
			}else{				
				leer = 0;
				sprintf(msg, "No se esperaba '%c' ", c); 
				strcat(paraImprimir, msg);
				ungetc(c,archivo);
				continue;
			}

			
			e.compLex = PR_NULL;
			strcpy(e.lexema, lexema);

			t.compLex = PR_NULL;
			t.pe=buscar(lexema);
					
			strcat(paraImprimir, "PR_NULL ");
		}
		else
		{
			leer = 0;
			sprintf(msg,"--> %c no esperado <--",c);
			strcat(paraImprimir, msg);
		}
	
	}

	if (c==EOF)
	{
		entrada e;
		e.compLex=EOF;
		sprintf(e.lexema,"EOF");
		
		t.compLex=EOF;	
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
			strcpy(paraImprimir, "");
			getToken();
			if(strcmp(paraImprimir, "") != 0){				
				printf("%s\n", paraImprimir);
			}
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
