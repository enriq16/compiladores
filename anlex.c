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

int leerHastaFinLinea;

char cad[5*TAMLEX];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente pascal
char buff[5*TAMLEX];	// Buffer para IMPRIMIR LINEA
char lexema[TAMLEX];	// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/**************** Funciones **********************/



// Rutinas del analizador lexico

void error(const char* mensaje)
{
	printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);	
}

void addToken(){
	switch(t.compLex){
		case ',':
			strcat(buff, "COMA ");			
		break;
		case ':':
			strcat(buff, "DOS_PUNTOS ");
		break;
		case '[':
		    strcat(buff, "L_CORCHETE ");
		break;
		case ']':
			strcat(buff, "R_CORCHETE ");
		break;
		case '{':
			strcat(buff, "L_LLAVE ");
		break;
		case '}':
			strcat(buff, "R_LLAVE ");
		break;
		case PR_TRUE:
			strcat(buff, "PR_TRUE ");
		break;
		case PR_FALSE:
			strcat(buff, "PR_FALSE ");
		break;
		case PR_NULL:
			strcat(buff, "PR_NULL ");
		break;
		case NUM:
			strcat(buff, "NUMBER ");
		break;
		case STRING:
			strcat(buff, "STRING ");
		break;
		default :
			strcat(buff, "error entro en el default ");
	}	
}

void getToken()
{
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	entrada ee;
	char cc[2] = "";

	while((c=fgetc(archivo))!=EOF)
	{
		strcpy(msg, "");
		strcpy(lexema, "");
		entrada e;	

		if (c==' ' || c=='\t')
			continue;	//eliminar espacios en blanco
		else if(leerHastaFinLinea==1){
			
			while( (c=fgetc(archivo))!=EOF ){
				if(c=='\n'){	break;		}
			}

			numLinea++;
			break;
		}
		else if(c=='\n')
		{
			//incrementar el numero de linea
			numLinea++;
			break;
		}else if(c==','){
			t.compLex=',';
			t.pe=buscar(",");
			
		}else if(c==':'){
			t.compLex=':';
			t.pe=buscar(":");
			
		}else if(c==';'){
			t.compLex=';';
			t.pe=buscar(";");
			
		}else if(c=='['){
			t.compLex='[';
			t.pe=buscar("[");
			
		}else if(c==']'){
			t.compLex=']';
			t.pe=buscar("]");
			
		}else if(c=='{'){
			t.compLex='{';
			t.pe=buscar("{");
			
		}else if(c=='}'){
			t.compLex='}';
			t.pe=buscar("}");
			
		}else if( tolower(c)=='f'){
			lexema[0] = 'f';
			c=fgetc(archivo);			
			if( tolower(c)=='a'){				
				lexema[1] = 'a';
				c=fgetc(archivo);
				if( tolower(c)=='l'){				
					lexema[2] = 'l';
					c=fgetc(archivo);					
					if( tolower(c)=='s'){
						lexema[3] = 's';				
						c=fgetc(archivo);										
						if( tolower(c)=='e'){
							lexema[4] = 'e';
							lexema[5] = '\0';												
							t.compLex=PR_FALSE;
							t.pe=buscar("false");
						}else{
							lexema[4] = '\0';				
							leerHastaFinLinea = 1;				
						}
					}else{
						lexema[3] = '\0';				
						leerHastaFinLinea = 1;
					}	
				}else{
					lexema[2] = '\0';
					leerHastaFinLinea = 1;
				}	
			}else{
				lexema[1] = '\0';
				leerHastaFinLinea = 1;				
			}
						
			
		}else if( tolower(c)=='t'){
			lexema[0] = 't';
			c=fgetc(archivo);			
			if( tolower(c)=='r'){				
				lexema[1] = 'r';
				c=fgetc(archivo);
				if( tolower(c)=='u'){				
					lexema[2] = 'u';
					c=fgetc(archivo);					
					if( tolower(c)=='e'){
						lexema[3] = 'e';				
						lexema[4] = '\0';												
						t.compLex=PR_TRUE;
						t.pe=buscar("true");			
					}else{
						lexema[3] = '\0';				
						leerHastaFinLinea = 1;
					}	
				}else{
					lexema[2] = '\0';
					leerHastaFinLinea = 1;
				}	
			}else{
				lexema[1] = '\0';
				leerHastaFinLinea = 1;				
			}								
			
		}else if(tolower(c)=='n'){
			lexema[0] = 'n';
			c=fgetc(archivo);			
			if( tolower(c)=='u'){				
				lexema[1] = 'u';
				c=fgetc(archivo);
				if( tolower(c)=='l'){				
					lexema[2] = 'l';
					c=fgetc(archivo);					
					if( tolower(c)=='l'){
						lexema[3] = 'l';				
						lexema[4] = '\0';												
						t.compLex=PR_NULL;
						t.pe=buscar("null");			
					}else{
						lexema[3] = '\0';				
						leerHastaFinLinea = 1;
					}	
				}else{
					lexema[2] = '\0';
					leerHastaFinLinea = 1;
				}	
			}else{
				lexema[1] = '\0';
				leerHastaFinLinea = 1;				
			}
			
		}		
		else if (c=='"')/* Este tengo que retocar */
		{
			//es un identificador (o palabra reservada)
			i=0;
			do{
				lexema[i]=c;
				i++;
				c=fgetc(archivo);

				cc[0] = c;
				cc[1] = '\0';
				if (i>=TAMLEX){
					//sprintf(msg, "Se excede tamnho buffer.");
					leerHastaFinLinea = 1;
					break;
				}

			}while(isalpha(c) || isdigit(c) || c=='_' || c==' ' );
			/*
			lexema[i]='X';			
			lexema[++i]='\0';
			printf("%s*\n", lexema);
			*/		
			if(c=='"' && !leerHastaFinLinea){/* Cierre de String */
				lexema[i]=c;			
				lexema[++i]='\0';

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
			}
			else{
				sprintf(msg,"-->codigo char: %d  char: %c<--",c,c);

				leerHastaFinLinea = 1;
				ungetc(c,archivo);
				lexema[i]='\0';	
			}			
		}
		else if (isdigit(c))
		{
				//es un numero
				i=0;
				estado=0;
				acepto=0;
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
							estado=-1;
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
						
						ungetc(c,archivo);
						
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
						t.compLex=NUM;
						break;
					case -1:
						leerHastaFinLinea = 1;
						lexema[++i]='\0';
						acepto = 1;
						ungetc(c,archivo);						
					}
				}
			
		}
		else if (c!=EOF)
		{
			/*sprintf(msg,"%c no esperado",c);
			error(msg);*/
			lexema[0] = c;
			lexema[1] = '\0';
			leerHastaFinLinea = 1;
		}

		if(!leerHastaFinLinea){
			addToken();		
		}else{
			strcpy(buff, "");
			sprintf(buff, "Linea %d, error en lexema. Lexema: '%s' %s", numLinea, lexema, msg);
		}	
	}

	if (c==EOF)
	{
		t.compLex=EOF;
		// strcpy(e.lexema,"EOF");
		sprintf(ee.lexema,"EOF");
		t.pe=&ee;
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
			leerHastaFinLinea = 0;
			strcpy(buff, "");
			getToken();
			if(strcmp(buff, "") != 0)
				printf("\n%s", buff);
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
