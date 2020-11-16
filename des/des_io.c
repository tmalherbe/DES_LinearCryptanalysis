#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<unistd.h>
#include"des_core.h"

/**
 * Structure représentant un fichier, composant d'un pointeur sur son contenu mappé en mémoire
 * et de sa taille 
 */


typedef struct{
  int size;
  BYTE *buffer;
}MAP;

/**
 * Fonction projetant un fichier en mémoire.
 * Une structure MAP est retournée.
 *
 */

MAP encode_file(char *src_path){
  int fd;
  MAP file_map;
  struct stat statbuf;  
  
  fd = open(src_path, O_RDONLY);
  if(fd < 0){
    printf("Erreur d'ouverture du fichier %s \n", src_path);
    exit(EXIT_FAILURE);
  }
  if(fstat(fd, &statbuf) < 0){
    printf("Erreur de lecture du fichier %s \n", src_path);
    exit(EXIT_FAILURE);
  }
  file_map.size = statbuf.st_size;

  file_map.buffer = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(!file_map.buffer){
    printf("Erreur de chargement du fichier %s \n", src_path);
    exit(EXIT_FAILURE);
  }  
  
  if(close(fd)<0){
    printf("Erreur de fermeture du fichier %s \n", src_path);
    exit(EXIT_FAILURE);
  }
  return file_map;

}

/**
 * Fonction sauvant dans un fichier le contenu d'un objet de type MAP
 *
 */

void save_file(BYTE *map, char *dst_path, int size){
  int fd;  

  fd = open(dst_path, O_RDWR | O_CREAT/* | S_IRWXU*/);
  if(fd<0){
    printf("Erreur de sauvegarde 1\n");
    exit(EXIT_FAILURE);
  }
  
  if(write(fd, map, size)<0){
    printf("Erreur de sauvegarde 2\n");
    exit(EXIT_FAILURE);
  }
  
  if(close(fd)<0){
    printf("Erreur de sauvegarde 3\n");
    exit(EXIT_FAILURE);
  }

}

/**
 * Fonction effaçant une zone mémoire contenant des données sensibles
 *
 */

void burnStack(void *stack, int length){
    memset(stack, 0, length);
}

