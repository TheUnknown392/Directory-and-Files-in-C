#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#define char_buffer 360
#define file_Length 50
//TODO:


// void deleteDir();
// void createFile();
// void deleteFile();

void viewFile_and_Dir(DIR *dir,int mode,char inputDir[]);
void createDir_and_File(char inputDir[], int mode);

int isDots(char dirName[]);
void options(char inputDir[]);
DIR * inputDirectory(char inputDir[]);
void exit_if_null(void *ptr, char *message);
void clearInputBuffer();

int main(){
    int menuChoice;
    DIR *dir=NULL;
    char inputDir[char_buffer]={'\0'};
    dir=inputDirectory(inputDir);

    options(inputDir);

    while(1){
        do{
            printf("\nNext: ");
            scanf("%d",&menuChoice);
            if(!(menuChoice>=1&&menuChoice<=11)){
                printf("Please input 1-11: ");
            }
            clearInputBuffer();
        }while(!(menuChoice>=1&&menuChoice<=11));

        switch(menuChoice){
            case 1:
            closedir(dir);
            dir=inputDirectory(inputDir);
            break;
            case 2:
            createDir_and_File(inputDir,menuChoice);
            break;
            case 3:
            //deleteDir();
            break;
            case 4:
            createDir_and_File(inputDir,menuChoice);
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
            exit(0);
            break;
            default:
            printf("Unknown Error");
            break;
        }
        menuChoice=0;
    }
    closedir(dir);
    return 0;
}

void exit_if_null(void *ptr, char *message){
    if(ptr==NULL){
        perror(message);
        exit(1);
    }
}

DIR* inputDirectory(char inputDir[]){
    DIR *dir=NULL;
    while(1){
        printf("Input Directory: ");
        fgets(inputDir, char_buffer, stdin);
        inputDir[strcspn(inputDir,"\n")]='\0'; //removes the ending \n with \0. \n is there when we press enter. fgets scans the new line too
        dir=opendir(inputDir);
        if(dir!=NULL){
            break;
        }
        perror("Cheak your directory and try again");
    }
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
            switch(mode){
                case 6:
                printf("%s\n",entry->d_name);
                break;
                case 7:
                if(S_ISREG(pathTypeStruct.st_mode)){ // returns 1 if it's a file
                    printf("%s\n",entry->d_name);
                }
                break;
                case 8:
                if(S_ISDIR(pathTypeStruct.st_mode)){ // returns if it's a directory
                    printf("%s\n",entry->d_name);
                }
                break;
                case 9:
                if(!(S_ISREG(pathTypeStruct.st_mode)||S_ISDIR(pathTypeStruct.st_mode))){ // returns if it's a not directory or file
                    printf("%s\n",entry->d_name);
                }
                break;
                default:
                exit_if_null(NULL,"Error: ");
                break;
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
void clearInputBuffer(){
    char ch;
    while(ch=getchar()!='\n'&&ch!=EOF){} // clean up input buffer for fgets
}
void createDir_and_File(char inputDir[],int mode){
    char tempDirOrFile[char_buffer];
    char file_folder_name[file_Length]={'\0'};
    FILE *file;
    
    (mode==2)?
    printf("What do you want to name your directory? You must follow directory naming convention: "):
    printf("What do you want to name your file? You must follow file naming convention: ");

    fgets(file_folder_name,file_Length,stdin);
    
    int len=strlen(file_folder_name);//fgets also inputs the enter '\n' into the array.
    file_folder_name[len-1]='\0'; // removing '\n'

    snprintf(tempDirOrFile,char_buffer,"%s/%s",inputDir,file_folder_name);
    switch(mode){
        case 2:
        if (mkdir(tempDirOrFile)){
            perror("\nAttempt at creating folder has been unsucessfull");
        }else{
            printf(" \nFolder creation sucessfull\n");
        }
        break;
        case 4:
        file=fopen(tempDirOrFile,"w");
        if (file==NULL){
            perror("\nAttempt at creating file has been unsucessfull");
        }else{
            fclose(file);
            printf(" \nfile creation sucessfull\n");
        }
        break;
        default:
        printf("\nunknown Error");
        exit(1);
        break;
    }

}