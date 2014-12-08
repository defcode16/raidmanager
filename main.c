
/* 
 * File:   input.c
 * Author: dennis
 *
 * Created on 19 de agosto de 2013, 05:58 PM
 */


#include <stdlib.h>
#include <stdio.h>

void clr(){
    system("clear");
}
int menu(){
    int o;
    clr();
    printf("\n\t==============================\n");
    printf("\tRaid Manager vr. 2.11.34-alpha\n");
    printf("\t==============================\n");
    printf("\t\t2013 (c) Dennis Higueros\n\n");
    printf("\tOPCIONES:\n");
    printf("\t\t0. Salir\n");
    printf("\t\t1. Crear Unidad RAID\n");
    printf("\t\t2. Montar Unidad RAID\n");
    printf("\t\t3. Desmontar Unidad RAID\n");
    printf("\t\t4. Utilizar Unidad RAID\n");
    printf("\t\t5. Mostrar Contenido (CSV)\n\n");
    printf("raid-manager> ");
    scanf("%i",&o);
    return o;
    
}
void menuNewRaid(){
    clr();
    int n;
    printf("\n\tNueva Unidad RAID\n\n");
    printf("Ingrese la ruta de la nueva unidad\n");
    printf("raid-manager> ");
    char *path = readln(256);
    printf("\n %s Ruta Seleccionada!\nIngrese el tamaño de la nueva unidad (mb) \n",path);
    printf("Min: 2(mb) Max: 10(mb)\n");
    scanf("%i",&n);
    newraid(path,n);
    printf("\nPresione cualquier tecla para continuar...");
    readchar();
    
}

int menuMontarRaid(){
    clr();
    printf("\n\tMontar Unidad RAID\n\n");
    printf("Ingrese la ruta de la unidad (drive.dat)\n");
    printf("raid-manager> ");
    char *manager = readln(256);
    loadraid(manager);
    readchar();
}

void menuDesmontarRaid(){
    unloadraid();
    readchar();
}

void cargaMasiva(){
    clr();
    printf("Ingresa la ruta del archivo de texto\n");
    printf("raid-manager> ");
    char *path = readln(256);
    writeFileOnDisk(path);
    readchar();
    
}
void cargaConsola(){
    clr();
    printf("Presione cualquier tecla para iniciar el editor\n");
    readchar();
    clr();
    system("nano fichero.txt");
    printf("Leyendo fichero");
    writeFileOnDisk("fichero.txt");
    remove("fichero.txt");
    readchar();
}
void menuUtilizarRaid(){
    int o = -1;
    while(o!=0){
        clr();
        printf("\n\t\tUtilizar Raid\n\n");
        printf("\tOPCIONES:\n\n");
        printf("\t0. Regresar\n");
        printf("\t1. Consola\n");
        printf("\t2. Carga Masiva\n");
        printf("raid-manager> ");
        scanf("%i",&o);
        switch(o){
            case 0:
                break;
            case 1:
                cargaConsola();
                break;
            case 2:
                cargaMasiva();
                break;
            default:
                perror("Opción incorrecta");
                readchar();
                break;
        }
    }
}
void testDisk(){
    clr();
    printf("Ingrese el nombre del disco\n");
    printf("raid-manager> ");
    char *path = readln(256);
    readDisk(path);
    readchar();
}
void menuWriteCSV(){
    clr();
    printf("Generando archivo CSV...");
    writeCSV();
    
}
int main(void){
    int o = -1;
    while(o!=0){
        o = menu();
        switch(o){
            case 0:
                printf("Adiós\n");
                sleep(1);
                break;
            case 1:
                menuNewRaid();
                break;
            case 2:
                menuMontarRaid();
                break;
            case 3:
                menuDesmontarRaid();
                break;
            case 4:
                menuUtilizarRaid();
                break;
            case 5:
                //testDisk();
                menuWriteCSV();
                break;
            default:
                perror("\nOpción incorrecta");
                readchar();
                break;
        }
    }
    
    return EXIT_SUCCESS;
}
