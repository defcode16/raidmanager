#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct{
    char chars[128];
}bloque;

struct raid{
    char root[256];
    unsigned long size;
    unsigned long posr0;
    unsigned long posr1;
    int last;
};

struct raid mount;
int mounted=0;

int fileExist(char *path){
    FILE *f = fopen(path,"rb");
    if(f!=NULL){
        fclose(f);
        return 1;
    }else{
        return 0;
    }
}

int writeDisk(char *path,char *name,unsigned long n,int header){
    bloque b;
    //writing disk
   char string[256];
   strcpy(string,path);
   strcat(string,name);
   
   int x=0;
   while(x<128){
       b.chars[x]='\0';
       x++;
   }
   
    //char absolute_path[256];
    FILE *f = fopen(string,"wb");
    if(header){
        printf("Creando unidad %s...\n",name);
        unsigned long i=0;
        while(i<n){
            //strcpy(b.chars,"Hola mundo.. Hola Mundo, Hola Mundo... 000000000000000askjdhasjkdhjkashdkjashd");
            
            fwrite(&b,sizeof(bloque),1,f);
            i++;
        }
        //sleep(1);
    }else{
        struct raid r;
        strcpy(r.root,path);
        r.size = n;
        r.posr0=0;
        r.posr1=0;
        r.last=0;
        fwrite(&r,sizeof(struct raid),1,f);
    }
    //else
       // fwrite('c',1,1,f);
    fclose(f);
}
int createDisk(char* path,unsigned long n){
    //int error;
    //writeDisk(path,"drive.raid",0,0);
    
    writeDisk(path,"disk0.dat",n,1);
    writeDisk(path,"disk1.dat",n,1);
    writeDisk(path,"disk2.dat",n,1);
    writeDisk(path,"disk3.dat",n,1);
    
    writeDisk(path,"drive.dat",n,0);
    return 1;
}

int newraid(char*path,int size){
    if(size>=2&&size<=10){
        char string[256];
        strcpy(string,path);
        strcat(string,"drive.dat");
        //printf("%s",string);
        if(fileExist(string)){
            perror("Ya existe una unidad en esta ubicación");
            return 0;
        }else{
            unsigned long bytes = size*1024*1024;
            unsigned long n = bytes/128;
            if(createDisk(path,n))
                printf("Unidad creada exitosamente\n %sdrive.dat",path);
            else
                perror("Error al intentar crear la unidad");
           
        }
        
    }else
        return 0;
}
int loadraid(char *path){
    FILE *f = fopen(path,"rb");
    if(f!=NULL){
        struct raid r;
        fread(&r, sizeof(struct raid),1,f);
        if(&r!=NULL){
            printf("Root folder -> %s Seleccionado correctamente\n",r.root);
            printf("drive.raid montado correctamente\n");
            mounted = 1;
            mount = r;
            
            return 1;
        }else{
            perror("El archivo es incorrecto o está dañado");
            mounted=0;
            return 0;
        }
    }else{
        perror("No se encontró el fichero\n");
        return 0;
    }
}

int unloadraid(){
    if(!mounted){
        perror("No hay ninguna unidad montada aún");
        return 0;
    }else{
        printf("%sdisk.raid desmontado correctamente\n");
        return 1;
    }
}
int updateRaid(){
   char string[256];
   strcpy(string,mount.root);
   strcat(string,"drive.dat");
    //char absolute_path[256];
    FILE *f = fopen(string,"wb");
    if(f==NULL){
        perror("Error actualizando el archivo RAID\nintente montar nuevamente la unidad\n");
        return 0;
    }else{
        fwrite(&mount,sizeof(struct raid),1,f);
        fclose(f);
        return 1;
    }
}

FILE *openDisk(char *name){
    FILE *f;
    char string[256];
    if(mounted!=0){
        strcpy(string,mount.root);
        strcat(string,name);
        f = fopen(string,"rw+b");
        return f;
    }else{
        perror("No se ha montado ninguna unidad\n");
        return f;
    }
}

int readDisk(char *name){
    printf("\t Leyendo disco %s \n\n",name);
    FILE *disk;
    disk = openDisk(name);
    if(disk!=NULL){
        bloque readed;
        unsigned long x=0;
        while (feof(disk) == 0)
        {
            fread(&readed,sizeof(bloque),1,disk);

            printf("%i. %s",x,readed.chars);

        }
        fclose(disk);
        return 1;
    }else{
        perror("No existe el fichero");
        return 0;
    }
    
}
int writeFileOnDisk(char *path){
    FILE *archivo;
    FILE *disk0;
    FILE *disk1;
    FILE *disk2;
    FILE *disk3;
    disk0 = openDisk("disk0.dat");
    disk1 = openDisk("disk1.dat");
    disk2 = openDisk("disk2.dat");
    disk3 = openDisk("disk3.dat");
    
    char caracteres[128];
    //char linea[128];
    archivo = fopen(path,"r");
    if (archivo == NULL||disk0==NULL||disk1==NULL||disk2==NULL||disk3==NULL)
        return 0;
    printf("\nLeyendo contenido...\n");
    while (feof(archivo) == 0)
    {
    
        fread(caracteres,128,1,archivo);
        //int len = strlen(caracteres);
        strip(caracteres);
        if(mount.last==0){
                bloque for_save;
                strcpy(for_save.chars,caracteres);
                unsigned long pos = sizeof(bloque)*mount.posr0;
                unsigned long max_size = sizeof(bloque)*mount.size;
                if(pos<max_size-pos){
                        fseek(disk0,pos, 0);
                        fseek(disk1,pos, 0);
                
                        fwrite(&for_save,sizeof(bloque),1,disk0);
                        fwrite(&for_save,sizeof(bloque),1,disk1);
                        mount.posr0++;
                }else{
                    perror("Espacio insuficiente\n");
                    return 0;
                }
              
           
            mount.last=1;
        }else{
                bloque for_save;
                strcpy(for_save.chars,caracteres);
                unsigned long pos = sizeof(bloque)*mount.posr1;
                unsigned long max_size = sizeof(bloque)*mount.size;
                if(pos<max_size-pos){
                        fseek(disk2,pos, 0);
                        fseek(disk3,pos, 0);
                
                        fwrite(&for_save,sizeof(bloque),1,disk2);
                        fwrite(&for_save,sizeof(bloque),1,disk3);
                        mount.posr1++;
                }else{
                    perror("Espacio insuficiente\n");
                    return 0;
                }
            
            mount.last=0;
        }
    }
    
    fclose(archivo);
    fclose(disk0);
    fclose(disk1);
    fclose(disk2);
    fclose(disk3);
    updateRaid();
    
    printf("Archivo cargado exitosamente");
    return 1;
}
int writeDiskCSV(FILE *out, char *name){
    fprintf(out,"/* %s */ \n",name);
    FILE *disk;
    disk = openDisk(name);
    if(disk!=NULL){
        bloque readed;
        unsigned long x=0;
        while (feof(disk) == 0)
        {
            fread(&readed,sizeof(bloque),1,disk);
            fprintf(out,"%s,\n\n",readed.chars);
        }
        fprintf(out,"EOF\n");
        fclose(disk);
        return 1;
    }else{
        perror("No existe el fichero");
        return 0;
    }
}
int writeCSV(){
    if(mounted!=0){
        FILE *out;
        out = fopen("output.csv", "w");
        writeDiskCSV(out,"disk0.dat");
        writeDiskCSV(out,"disk1.dat");
        writeDiskCSV(out,"disk2.dat");
        writeDiskCSV(out,"disk3.dat");
    
        fclose(out);
    
        system("nano output.csv");
    }else{
        perror("No se ha montado ninguna unidad\n");
    }
}



