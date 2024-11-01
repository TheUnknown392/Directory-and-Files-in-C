#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define char_buffer 360




//TODO:


// void createDir();
// void deleteDir();
// void createFile();
// void deleteFile();

void viewFile_and_Dir(DIR *dir,int mode,char inputDir[]);



int isDots(char dirName[]);
void options(char inputDir[]);
DIR * inputDirectory(char inputDir[]);
void exit_if_null(void *ptr, char *message);

int main(){
    int menuChoice;
    DIR *dir=NULL;
    char inputDir[char_buffer]={'\000'};
    
    dir=inputDirectory(inputDir);

    options(inputDir);

    while(1){
        do{
            printf("\nNext: ");
            scanf("%d",&menuChoice);
            if(!(menuChoice>=1&&menuChoice<=11)){
                printf("Please input 1-11: ");
            }
            char ch;
            while(ch=getchar()!='\n'&&ch!=EOF){} // clean up input buffer for fgets
        }while(!(menuChoice>=1&&menuChoice<=11));

        switch(menuChoice){
            case 1:
            dir=inputDirectory(inputDir);
            break;
            case 2:
            //createDir();
            break;
            case 3:
            //deleteDir();
            break;
            case 4:
            //createFile();
            break;
            case 5:
            //deleteFile();
            break;
            case 6:
            case 7:
            case 8:
            case 9:
            viewFile_and_Dir(dir,menuChoice,inputDir);
            break;
            case 10:
            options(inputDir);
            break;
            case 11:
            exit(1);
            break;
            default:
            printf("Unknown Error");
            break;
        }
    }
}

void exit_if_null(void *ptr, char *message){
    if(ptr==NULL){
        printf("%s",message);
        exit(0);
    }
}

DIR* inputDirectory(char inputDir[]){
    DIR *dir=NULL;
    printf("Input Directory: ");
    fgets(inputDir, char_buffer, stdin);
    inputDir[strcspn(inputDir,"\n")]='\0'; //removes the ending \n with \0. \n is there when we press enter. fgets scans the new line too
    dir=opendir(inputDir);
    exit_if_null(dir,"no directory found");
    return dir;
}

void viewFile_and_Dir(DIR *dir,int mode,char inputDir[]){
    struct dirent *entry;
    struct stat pathTypeStruct; // struct for stats for whatever is gonna get cheaked
    char tempPath[char_buffer];
    
    while( (entry=readdir(dir)) != NULL ){
        snprintf(tempPath, char_buffer, "%s/%s", inputDir, entry->d_name);
        if(stat(tempPath,&pathTypeStruct)==1){ // save info of the given path (dir) into the structure (pathType)
            exit_if_null(NULL,"Problem finding some path");
        } 
        if(!isDots(entry->d_name)){
            if(mode==6){
                printf("%s\n",entry->d_name);
            }else if(mode==7){
                if(S_ISREG(pathTypeStruct.st_mode)){ // returns 1 if it's a file
                    printf("%s\n",entry->d_name);
                }
            }else if(mode==8){
                if(S_ISDIR(pathTypeStruct.st_mode)){ // returns if it's a directory
                    printf("%s\n",entry->d_name);
                }
            }else if(mode==9){
                if(!(S_ISREG(pathTypeStruct.st_mode)||S_ISDIR(pathTypeStruct.st_mode))){ // returns if it's a not directory or file
                    printf("%s\n",entry->d_name);
                }
            }else{
                printf("Error in viewFile_and_Dir");
                exit(0);
            }
        }
    }
    rewinddir(dir);
    printf("\n");
}
void options(char inputDir[]){
    printf("\nCurrent Directory: %s\n",inputDir);
    printf("\n1. Change the directory\n2. Create directory\n3. Delete directory\n4. Create file\n5. Delete file\n6. View files and directory\n7. View files only\n8. View directory only\n9. View special directory and Files\n10. Show Main Options again\n11. Exit\n");
}
int isDots(char dirName[]){
    int index=strcspn(dirName,"\0");
    if(dirName[index-1]=='.'){
        return 1;
    }
    return 0; 
}