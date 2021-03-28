compiladores
============

Compiladores FPUNA

Proyecto Base: https://github.com/jpaciello/compiladores
Autor: Enrique Ruiz 


Implementar un analizador léxico para el lenguaje Json simplificado. Recibe un archivo fuente
Json, debe realizar un análisis léxico reconociendo tokens, y luego por cada línea del fuente
imprimir una línea en un archivo de salida con la secuencia de componentes léxicos
encontrados separados por espacios

Ejemplo Entrada

{  
   "personas":[  
      {  
         "ci":1234567,
         "nombre":"Julio Prez",
         "casado":TRUE,
         "hijos":[  

         ]
      },
      {  
         "ci":7654321,
         "nombre":"Juan Gmez",
         "casado":false,
         "hijos":[  
            {  
               "nombre":"Jorge",
               "edad":18
            },
            {  
               "nombre":"Valeria",
               "edad":16
            }
         ]
      }
   ]
}

Ejemplo Salida


L_LLAVE
	STRING DOS_PUNTOS L_CORCHETE
	      L_LLAVE  
		 STRING DOS_PUNTOS NUMBER COMA
		 STRING DOS_PUNTOS STRING COMA
		 STRING DOS_PUNTOS PR_FALSE COMA
		 STRING DOS_PUNTOS L_CORCHETE  

		 R_CORCHETE
	      R_LLAVE COMA
	      L_LLAVE  
		 STRING DOS_PUNTOS NUMBER COMA
		 STRING DOS_PUNTOS STRING COMA
		 STRING DOS_PUNTOS PR_FALSE COMA
		 STRING DOS_PUNTOS L_CORCHETE  
		    L_LLAVE  
			STRING DOS_PUNTOS STRING COMA
			STRING DOS_PUNTOS NUMBER
		    R_LLAVE COMA
		    L_LLAVE  
			STRING DOS_PUNTOS STRING COMA
			STRING DOS_PUNTOS NUMBER
		    R_LLAVE
		 R_CORCHETE
	      R_LLAVE
	R_CORCHETE
R_LLAVE



Para compilar: gcc anlex.c tablaSimbolos.c -o jsonlexer



Observación:
string --> "^\"([a-zA-Z_])+([a-zA-Z_]|\s)*\"$"
NO considero las palabras con acento: en windows por ejemplo 'ó' viene como un número negativo y no el código ascci correspondiente.