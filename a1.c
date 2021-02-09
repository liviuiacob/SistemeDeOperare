#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int afisareDirector(char*path)
{
    DIR* dir=opendir(path);
    struct dirent *entry;
    if (dir!=NULL){
        printf("SUCCESS\n");
        
        while((entry=readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                printf("%s/%s\n", path, entry->d_name);
        }
    free(path);
    closedir(dir);
    return 0;
    }
    else
        return -1;
}

void afisareRecursivaDirector(char*path, int ok)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        printf("ERROR\ninvalid directory path");
        return;
    }
    else
        if (ok==0){
            printf("SUCCESS\n");
            ok=1;    
        }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode)) {
                     afisareRecursivaDirector(fullPath, ok);
                }
            }
        }
    }
    closedir(dir);
}

int seTerminaCu(char* nume, char*end)
{
    int i=0, j=0;
    for (i=strlen(end), j=strlen(nume); i>=0; i--, j--)
    {
        if (nume[j]!=end[i])
            return 0;
    }
    return 1;
}

int afisareEndsWith(char* path, char*end)
{
    DIR* dir=opendir(path);
    struct dirent *entry;
    if (dir!=NULL){
        printf("SUCCESS\n");
        
        while((entry=readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                if (seTerminaCu(entry->d_name, end)==1)
                    printf("%s/%s\n", path, entry->d_name);
        }
    //free(path);
    closedir(dir);
    return 0;
    }
    else
        return -1;
}
/*
void transformarePermisiuni(unsigned int* permissions, char* p)
{
    //printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    p[0]=(fileStat.st_mode & S_IRUSR) ? "r" : "-";
    p[1]=(fileStat.st_mode & S_IWUSR) ? "w" : "-";
    p[2]=(fileStat.st_mode & S_IXUSR) ? "x" : "-";
    p[3]=(fileStat.st_mode & S_IRGRP) ? "r" : "-";
    p[4]=(fileStat.st_mode & S_IWGRP) ? "w" : "-";
    p[5]=(fileStat.st_mode & S_IXGRP) ? "x" : "-";
    p[6]=(fileStat.st_mode & S_IROTH) ? "r" : "-";
    p[7]=(fileStat.st_mode & S_IWOTH) ? "w" : "-";
    p[8]=(fileStat.st_mode & S_IXOTH) ? "x" : "-";
}
*/
int afisarePermisiuni (char* path, char* permissions)
{
    DIR* dir=opendir(path);
    struct dirent *entry;
    struct stat statbuf;
    if (dir!=NULL){
        
        char fullPath[512];
        char permisiuni[9];
              
        printf("SUCCESS\n");
        
        while((entry=readdir(dir))!=NULL){
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
                if (lstat(fullPath, &statbuf)==0){
                    //trebuie transformat permisiuni in tip de forma rwx----
                    //snprintf(permisiuni, 9, "%o", statbuf.st_mode);
                    //transformarePermisiuni(&statbuf.st_mode, permisiuni);
                    permisiuni[0]=(statbuf.st_mode & S_IRUSR) ? 'r' : '-';
                    permisiuni[1]=(statbuf.st_mode & S_IWUSR) ? 'w' : '-';
                    permisiuni[2]=(statbuf.st_mode & S_IXUSR) ? 'x' : '-';
                    permisiuni[3]=(statbuf.st_mode & S_IRGRP) ? 'r' : '-';
                    permisiuni[4]=(statbuf.st_mode & S_IWGRP) ? 'w' : '-';
                    permisiuni[5]=(statbuf.st_mode & S_IXGRP) ? 'x' : '-';
                    permisiuni[6]=(statbuf.st_mode & S_IROTH) ? 'r' : '-';
                    permisiuni[7]=(statbuf.st_mode & S_IWOTH) ? 'w' : '-';
                    permisiuni[8]=(statbuf.st_mode & S_IXOTH) ? 'x' : '-';
                   // permisiuni[9]='\0';
                    //if (strcmp(permisiuni, permissions)==0)
                    int ok=1;
                    for (int i=0; i<9; i++)
                        if (permisiuni[i]!=permissions[i])
                            ok=0;
                    if (ok)
                        printf("%s\n", fullPath);
                    
                }
                
            }
        }
    free(path);
    closedir(dir);
    return 0;
    }
    else
        return -1;
}

void parse(char* path) {

    int fd=-1;
    fd= open(path, O_RDONLY);
    if (fd==-1){
        perror("Could not open input file");
        return;
    }
    char magic[2]; 
    int header_size=0, no_of_sections=0, version=0;
    char name[14];
    int type=0, offset=0, size=0;
    
    read(fd, magic, 2);
	read(fd, &header_size, 2);
    if(strcmp(magic, "2E") != 0) {
        printf("ERROR\nwrong magic");
        return;
    }

	read(fd, &version, 4);
    if(version < 24 || version > 101) {
        printf("ERROR\nwrong version");
        return;
    }

    read(fd, &no_of_sections, 1);
	if (no_of_sections < 8 || no_of_sections > 10) {
	    printf("ERROR\nwrong sect_nr");
		return;
	}

    for (int i = 0; i < no_of_sections; i++) {  
        read (fd, name, 14);
        read (fd, &type, 1);
        read (fd, &offset, 4);
        read (fd, &size, 4); 
        
		if (type != 32 && type != 18 && type != 67) {
		    printf("ERROR\nwrong sect_types");
			return;
		}
	}
	lseek(fd, 0, SEEK_SET);
	lseek(fd, 9, SEEK_CUR);
    printf("SUCCESS\n");
	printf("version=%d\n", version);
	printf("nr_sections=%d\n", no_of_sections);

    for (int i = 0; i < no_of_sections; i++)
	{   
	    read (fd, &name, 14);
	    name[14]='\0';
        read (fd, &type, 1);
        read (fd, &offset, 4);
        read (fd, &size, 4);
		printf("section%d: %s %d %d\n", i + 1, name, type, size);
	}
	
}

int verifparse(char* path) {

	int fd=-1;
    fd= open(path, O_RDONLY);
    if (fd==-1){
        perror("Could not open input file");
        return -1;
    }
    char magic[2]; 
    int header_size=0, no_of_sections=0, version=0;
    char name[14];
    int type=0, offset=0, size=0;
    int nr=0;
    
    read(fd, magic, 2);
	read(fd, &header_size, 2);
    if(strcmp(magic, "2E") != 0) {
        //printf("ERROR\nwrong magic");
        return -1;
    }

	read(fd, &version, 4);
    if(version < 24 || version > 101) {
        //printf("ERROR\nwrong version");
        return -1;
    }

    read(fd, &no_of_sections, 1);
	if (no_of_sections < 8 || no_of_sections > 10) {
	    //printf("ERROR\nwrong sect_nr");
		return -1;
	}

    for (int i = 0; i < no_of_sections; i++) {  
        read (fd, name, 14);
        read (fd, &type, 1);
        read (fd, &offset, 4);
        read (fd, &size, 4); 
        
		if (type != 32 && type != 18 && type != 67) {
		    //printf("ERROR\nwrong sect_types");
			return -1;
		}
		if (type == 67) {
		    nr++;
		}
	}
	return nr;
	
	
}

int findall(char* path, int ok)
{

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return 0;
    }
    if (ok==0){
        printf("SUCCESS\n");
        ok=1;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                if (S_ISREG(statbuf.st_mode) && verifparse(fullPath)>=3)
                    printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode)) {
                    findall(fullPath, ok);
                }
            }
        }
    }
    closedir(dir);
    return 1;
}

void extract (char*path, char* section, char* linie)
{
    int fd=-1;
    fd= open(path, O_RDONLY);
    if (fd==-1){
        perror("Could not open input file");
        return;
    }
    char magic[2]; 
    int header_size=0, no_of_sections=0, version=0;
    char name[14];
    int type=0, offset=0, size=0;
    //int nr=0;
    
    int ok_f=1, ok_s=0;
    read(fd, magic, 2);
	read(fd, &header_size, 2);
    if(strcmp(magic, "2E") != 0) {
        //printf("ERROR\nwrong magic");
        ok_f=0;
    }

	read(fd, &version, 4);
    if(version < 24 || version > 101) {
        //printf("ERROR\nwrong version");
        ok_f=0;
    }

    read(fd, &no_of_sections, 1);
	if (no_of_sections < 8 || no_of_sections > 10) {
	    //printf("ERROR\nwrong sect_nr");
		ok_f=0;
	}
    int sectie=0, line=0;
    sscanf(section, "%d", &sectie);
    sscanf(linie, "%d", &line); 
    for (int i = 0; i < no_of_sections; i++) {  
        read (fd, name, 14);
        read (fd, &type, 1);
        read (fd, &offset, 4);
        read (fd, &size, 4); 
        
        if (type != 32 && type != 18 && type != 67) {
		    //printf("ERROR\nwrong sect_types");
			ok_f=0;
		}
        
        if (i==sectie-1){
            ok_s=1;
            int nr=0, j=0;
            ssize_t read_size=0;
            unsigned char* buff=(unsigned char*)malloc(sizeof(unsigned char)*size);
            
            lseek(fd, offset, SEEK_SET);
            read_size=read(fd, buff, size);

            //printf("SUCCESS\n");
            while(1){
                //int j=0, nr=0; 
                if (size<0){
                //read error
                break;
                } else if (size==0){
                //end of file
                printf("ERROR\ninvalid line\n");
                break;
                } else {
                    for (int i=0; i<read_size; i++){
                        if (buff[i]=='\n'){
                            j=i;
                            nr++; i++;
                        }
                        if (nr==line-1) {
                            while (i<read_size){
                                //printf("%c", buff[i]);
                                    if(buff[i]!='\n')
                                        i++;
                                    else{
                                        buff[i]='\0';
                                    break;    
                                    }   
                            }
                            printf("SUCCESS\n");
                            for (int k=i-1; k>=j; k--)
                                printf("%c", buff[k]);
                            printf("\n");
                            //ok=1;
                            //break;
                            free(buff);
                            close(fd);
                            return;
                            }
   
                        }
                    size=read(fd, buff, read_size);
                    }
            }
            free(buff);
            //break;
        }
        
		
		
	}
	if (ok_f==0)
	    printf("ERROR\ninvalid file\n");
	else
	    if (ok_s==0)
	        printf("ERROR\ninvalid section\n");
	        
	close(fd);
	
	//close(dir);
}
int main(int argc, char **argv){
    if(argc >= 2){
    
        //2.2
        
        if(strcmp(argv[1], "variant") == 0){
            printf("97699\n");
            }
            
        //2.3
        
        if (strcmp(argv[1], "list")==0){
        
            //afisarea normala
            
            char*path= (char*)malloc(sizeof(char)*strlen(argv[2])-5);
            if (strstr(argv[2], "path=")){
                strcpy(path, argv[2]+5);
                if (afisareDirector(path)==-1){
                    printf("ERROR\ninvalid directory path");
                    //return 0;
                }
            }
            
            //afisare recursiva
            
            if ((strcmp(argv[2], "recursive")==0) && (strstr(argv[3], "path="))){
                char*path= (char*)malloc(sizeof(char)*strlen(argv[3])-5);
                strcpy(path, argv[3]+5);
                afisareRecursivaDirector(path, 0);
                free(path);
            }
            
            
            //afisare cu name_ends_with
            
            if ((strstr(argv[2], "name_ends_with=")) && (strstr(argv[3], "path="))){
                char*path= (char*)malloc(sizeof(char)*strlen(argv[3])-5);
                strcpy(path, argv[3]+5);
                char*end= (char*)malloc(sizeof(char)*strlen(argv[2])-15);
                strcpy(end, argv[2]+15);
                if(afisareEndsWith(path, end)==-1){
                    printf("ERROR\ninvalid directory path");
                }
                free(path);
                free(end);
            }
 
            //afisare cu permissions
            
            if ((strstr(argv[2], "permissions=")) && (strstr(argv[3], "path="))){
                char*path= (char*)malloc(sizeof(char)*strlen(argv[3])-5);
                char permissions[9];
                strcpy(permissions, argv[2]+12);
                strcpy(path, argv[3]+5);
                //permissions acum contine permisiunile citite de la tastatura, transformate sub forma rwx---
                if(afisarePermisiuni(path, permissions)==-1){
                    printf("ERROR\ninvalid directory path");
                }
                free(path); 
            }
            free(path);
        }
        
        //2.4
        
        if (strcmp(argv[1], "parse")==0){   
            char*path= (char*)malloc(sizeof(char)*strlen(argv[2])-5);
            if (strstr(argv[2], "path=")){
                strcpy(path, argv[2]+5);
                parse(path);
            }
            free(path);
            //return 0;
        }
        
        //2.5
        
        if (strcmp(argv[1], "extract")==0){   
            char*path= (char*)malloc(sizeof(char)*strlen(argv[2])-5);
            if (strstr(argv[2], "path=")){
                strcpy(path, argv[2]+5);
                if(strstr(argv[3], "section=")){
                    if(strstr(argv[4], "line=")){
                        extract(path, argv[3]+8, argv[4]+5);
                        //printf("1");
                    }
                }
            }
            free(path);
        }
        
        //2.6
        
        if (strcmp(argv[1], "findall")==0){   
            char*path= (char*)malloc(sizeof(char)*strlen(argv[2])-5);
            if (strstr(argv[2], "path=")){
                strcpy(path, argv[2]+5);
                if (findall(path, 0)==0){
                    printf("ERROR\ninvalid dyrectory path\n");
                }
            }
            free(path);
        }
        
    
    }
    return 0;
}
