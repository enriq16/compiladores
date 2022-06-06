/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/************* Definiciones ********************/

//Codigos

#define NUM			284
#define PR_TRUE		256
#define PR_FALSE    257
#define PR_NULL     258
#define STRING		288



// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX 		50
#define TAMHASH 	101

#define ERR_TOKEN   -2
#define EMPTY_STRING 555

/************* Estructuras ********************/

typedef struct entrada{
	//definir los campos de 1 entrada de la tabla de simbolos
	int compLex;
	char lexema[TAMLEX];	
	struct entrada *tipoDato; // null puede representar variable no declarada	
	// aqui irian mas atributos para funciones y procedimientos...
	
} entrada;

typedef struct {
	int compLex;
	entrada *pe;
} token;



/************* Prototipos ********************/
void insertar(entrada e);
entrada* buscar(const char *clave);
void initTabla();
void initTablaSimbolos();
void getToken();

void error_sint(int *tokens, int i);
void addToken();

void match(char* expToken);
void json();
void element();
void array();
void object();
void atribute_list();
void atribute_list_aux();
void element_list();
void element_list_aux();
void atribute();
void atribute_name();
void atribute_value();

/* Para manejo de errores */
int token_in( int *tokens, int tam, int incluir_eof );
void scan_to(int *token_in, int tam, int incluir_eof);
int check_input(int *firsts, int x, int *follow, int y, int caso);
int* unir(int *firsts, int x, int *follows, int y); 


