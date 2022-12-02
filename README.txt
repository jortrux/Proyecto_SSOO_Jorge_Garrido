# Proyecto_SSOO_Jorge_Garrido
Trabajo hecho por Jorge Garrido del Moral


puede que el orden en el que he creado las funciones no sea muy convencional porque por ejemplo,
la funcion de volcar cache la hice la segunda, eso es porque me parecia que solo tenia que copiar la funcion de limpiar ram y cambiar las asignaciones de los valores de
tbl por un fputc (que me dio error en su momento y cambio a un fprintf) al fichero de contentsCache pero no la he podido testear hasta recientemente donde he visto que,
el fputc() me casteaba los valores y no se mostraba ni en decimal ni en hexadecimal

he tenido ayuda para la funcion de parsear direccion pero la estoy estudiando y cambiano
pero basicamente, utilizo "Bitwise Operators" para tratar directamente con los bits de la direccion de memoria como en los ejercicios de memoria de clase
donde cogia la tira de bits y los dividia entre etiqueta bloque y palabra
y de ahi saco los valores de la etiqueta la palabra y el bloque para las demas funciones

en paralelo, intento hacerlo con integers y haciendo divisiones para coger solo la parte entera de la division pero me surgen problemas porque el bloque esta entre la
palabra y la etiqueta y solo puedo descontar uno de esos dos haciendo divisiones
ejem:
pal 4
etq 3
bloq 8
101 11010010 1011 /
101110100101011/10^12=101,110100101011  (si hago el casteo a int se desprecian los decimales y la etiqueta seria 101)
101110100101011/10^4=10111010010,1011 (etiqueta+bloque y palabra en decimal, se desprecia, le resto etiqueta*10^8 para que solo queden los 8 bits de bloque)
y luego lo mismo con palabra que seria los 15 bits menos los 11 de bloque mas etiqueta pero multiplicado por 10^4 para que al hacer la resta solo queden los ultimos 4 bits

a ver si me da tiempo a implementarlo sin errores


y la funcion de tratar fallo unicamente consistia en aumentar el tiempoGlobal, y encontrar el inicio del bloque sabiendo ya su tamaño que es de 16bytes
a la vez una vez se ha detectado el fallo, se carga la etiqueta en la linea donde ha dado el fallo


para identificar los fallos he hehcho iteraciones por el accesos_memoria.txt hasta el end of file y dentro de las iteraciones con los valores que iba leyendo
parseaba la direccion de memoria y despues comprobaba el error en funcion de si la etiqueta ya estaba cargada en ram o no

y el resto del codigo es lo clasico de leer ficheros y recorrerlos

no se si era necesaria esta explicacion en el README pero prefiero tenerlo explicado que queda mas claro cuales eran mis intenciones por si mi codigo era muy confuso
y me gusta que quede claro que se lo que hago
