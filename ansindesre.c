/*
 *	Analizador Sintáctico descendente recursivo
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Práctica de Programación Nro. 2
 *	
 *	Descripcion:
 *	Implementa un analizador sintáctico descendente recursivo apoyandose en el anlex que se hizo en la 
 *  tarea 1. 
 * 	
 * 	
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
char buff[50*TAMLEX];	// Buffer para IMPRIMIR LINEA
char lexema[TAMLEX];	// Utilizado por el analizador lexico
int delantero = -1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/**************** Funciones **********************/



// Rutinas del analizador lexico
char* print_token();
char* print_token(){

	switch(t.compLex){
		case ',':
			return "COMA";			
		break;
		case ':':
			return "DOS_PUNTOS ";
		break;
		case '[':
		    return "L_CORCHETE ";
		break;
		case ']':
			return "R_CORCHETE ";
		break;
		case '{':
			return "L_LLAVE ";
		break;
		case '}':
			return "R_LLAVE ";
		break;
		case PR_TRUE:
			return "PR_TRUE ";
		break;
		case PR_FALSE:
			return "PR_FALSE ";
		break;
		case PR_NULL:
			return "PR_NULL ";
		break;
		case NUM:
			return "NUMBER ";
		break;
		case STRING:
			return "STRING ";
		break;
		case EMPTY_STRING:
			return "EMPTY_STRING";
	  	break;
	  	case EOF:
	  		return "EOF";
	  	break;					
	}	
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
	char msg[100];
	entrada ee;
	char cc[2] = "";

	int x = 0;

	while((c=fgetc(archivo))!=EOF)
	{
		x++;
		
		strcpy(msg, "");
		strcpy(lexema, "");
		entrada e;	

		if (c==' ' || c=='\t')
			continue;	//eliminar espacios en blanco		
		else if(c=='\n')
		{
			//incrementar el numero de linea
			strcat(buff, "\n ");
			numLinea++;
			continue;
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

				if (i>=TAMLEX){
					//sprintf(msg, "Se excede tamnho buffer.");
					leerHastaFinLinea = 1;					
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
						 	sprintf(msg,"No se esperaba '%c'",c);					
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
			sprintf(msg,"%c no esperado ",c);
			
			lexema[0] = c;
			lexema[1] = '\0';
			leerHastaFinLinea = 1;
		}

		if(leerHastaFinLinea == 1){
			
			sprintf(msg, "Linea %d, error en lexema. Lexema: '%s' %s ", numLinea, lexema, msg);
			strcat(buff, msg);
			t.compLex = ERR_TOKEN;
			strcpy(ee.lexema, lexema);
			t.pe=&ee;	
		}	
			
		addToken();

		break;/* Al encontrar el token salimos de la función. */
	}

	if (c==EOF)
	{
		t.compLex = EOF;
		strcpy(ee.lexema,"EOF");
		//sprintf(ee.lexema,"EOF");
		t.pe=&ee;
	}
	
}

void match(char* expToken){
	if( strcmp(t.pe->lexema, expToken) == 0 ){
		printf("\n#linea: %d, Match(%s)\t\t ", numLinea, t.pe->lexema);
		getToken();
		printf("token_actula: %s", t.pe->lexema);				
	}else{
		printf("\nNo Matching - #linea: %d compLex: '%d' lexema: '%s' | expToken: '%s'", numLinea, t.pe->compLex, t.pe->lexema,expToken);
	}
}

/* Funciones No Terminales */

int pri_json[] = {'{', '['};
int sig_json[] = {EOF};

void json(){// #1		
	if( check_input(pri_json, 2, sig_json, 1) ){
		element();
		check_input(sig_json, 1, pri_json, 2);	
	}	
}



int pri_element[] = {'{', '['};
int sig_element[] = {',', ']', '}', EOF};

void element(){// #2
	if(check_input(pri_element, 2, sig_element, 4)){
		if(t.compLex == '{'){
			object();
		}else if(t.compLex == '['){
			array(); 
		}
		check_input(sig_element, 4, pri_element, 2);
	}	
}

int pri_array[]  = {'['};
int sig_array[]  = {',', ']', '}', EOF};
void array(){// #3
	if( check_input(pri_array, 1, sig_array, 4) ){
		match("[");
		element_list(); 
		match("]");
		check_input(sig_array, 4, pri_array, 1);
	}	
}

int pri_object[] = {'{'};
int sig_object[] = {',', '}', ']', EOF};

void object(){// #4
	if( check_input(pri_object, 1, sig_object, 4) ){
		match("{");
		atribute_list(); 
		match("}");
		check_input(sig_object, 4, pri_object, 1);
	}	
}

int pri_at_list[] = {STRING, EMPTY_STRING};
int sig_at_list[] = {'}'};


/* Puede no venir */
void atribute_list(){// #5
	if( check_input(pri_at_list, 2, sig_at_list, 1) ){	
		if(t.compLex == STRING){
			atribute(); 
			atribute_list_aux();
		}

		check_input(sig_at_list, 1, pri_at_list, 2);
	}	
}


int pri_at_list_aux[] = {',', EMPTY_STRING};
int sig_at_list_aux[] = {'}'};
/* Puede no venir */
void atribute_list_aux(){// #6
	if( check_input(pri_at_list_aux, 2, sig_at_list_aux, 1) ){	
		if(t.compLex == ','){
			match(",");
			atribute();
			atribute_list_aux();
		}
		check_input(sig_at_list_aux, 1, pri_at_list_aux, 2);
	}	
}

int pri_element_list[] = {'{', '[', EMPTY_STRING};
int sig_element_list[] = {']'};

/* Puede no venir */
void element_list(){// #7
	if( check_input(pri_element_list, 3, sig_element_list, 1) ){	
		if(t.compLex == '{' || t.compLex == '['){
			element();
			element_list_aux();
		}
		check_input(sig_element_list, 1, pri_element_list, 3);



	}	
}

int pri_element_list_aux[] = {',', EMPTY_STRING};
int sig_element_list_aux[] = {']'};

/* Puede no venir */
void element_list_aux(){// #8
	if( check_input(pri_element_list_aux, 2, sig_element_list_aux, 1) ){	
		if(t.compLex == ','){
			match(",");
			element();
			element_list_aux();
		}
		check_input(sig_element_list_aux, 1, pri_element_list_aux, 2);
	}
}

int pri_atribute[] = {STRING}; 
int sig_atribute[] = {',', '}'};

void atribute(){// #9
	if(check_input(pri_atribute, 1, sig_atribute,2)){	
		atribute_name();
		match(":");
		atribute_value();

		check_input(sig_atribute, 2, pri_atribute, 1);
	}	
}

int sig_atribute_name[] = {':'};

void atribute_name(){// #10
	if(check_input(pri_atribute, 1, sig_atribute_name, 1)){
		match(t.pe->lexema);
		check_input(sig_atribute_name, 1, pri_atribute, 1);
	}	
}


int pri_atribute_value[] = {NUM,PR_TRUE,PR_FALSE,PR_NULL,STRING,'{','['};
int sig_atribute_value[] = {',', '}'};

void atribute_value(){// #11
	if(check_input(pri_atribute_value, 9, sig_atribute_value, 2)){
	switch(t.compLex){
		case PR_TRUE:
			match("true");
			break;
		case PR_FALSE:
			match("false");
			break;
		case PR_NULL:
			match("null");
			break;
		break;
		case NUM:
		case STRING:
			match(t.pe->lexema);
			break;
		break;
		default:
			element();
		break;
	}
	check_input(sig_atribute_value, 2, pri_atribute_value, 9);
	}
}


/* Para manejo de errores */
int token_in( int *tokens, int tam, int incluir_eof){
	
	for (int i = 0; i < tam; i++){
		
		if(t.compLex == tokens[i]){
			return 1;
		}else if(incluir_eof && t.compLex == EOF){
			return 1;
		}
	}

	return 0;	
}

void scan_to(int *tokens, int tam, int incluir_eof){
	while(!token_in(tokens, tam, incluir_eof)){
		getToken();
	}
}

int check_input(int *firsts, int x, int *follows, int y){
	int i = 0;	
	int result = token_in(firsts, x, 0);

	if(result == 0 && firsts[x-1] == EMPTY_STRING){
		result = 1;
	}


	if(!result){
		error_sint(firsts, x);
		scan_to(unir(firsts, x, follows, y), x+y, 1);	
	}
	return result;	
}

int* unir(int *firsts, int x, int *follows, int y){
	int *aux = malloc(sizeof(int) * (x+y));
	for (int i = 0; i < x+y; ++i){
		if(i < x)
			aux[i] = firsts[i];
		else 
			aux[i] = follows[i-x];
	}
	return aux;
}

void error_sint(int *tokens, int i){
	int x = 0;
	printf("\nError Sintactico, se esperaba: ");
	while(x < i){
		printf("'%s' ", print_token(tokens[x]) );
		x = x + 1;
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

			strcpy(buff, "");
			getToken();
			json();

			printf("\n\ntokens leidos:\n\n%s\n", buff);
		
		/*
		int firsts[]  = {1, 2, 3};
		int follows[] = {4, 5, 6, 7, 8, 9 };	
		int *rest = unir(firsts, 3, follows, 6);

		printf("\n");

		for (int i = 0; i < 9; ++i)
		{
			printf("rest[%d]: %d ", i, rest[i]);
		}
		*/
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}

	return 0;
}
