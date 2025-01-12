#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>


#define Char_Length 360
#define File_Folder_Length Char_Length 

void delete_subDir(char inputDir[]);
void delete_subFiles(char inputDir[]);
void deleteDir(char inpitDir[]);
void deleteFile(char inputDir[]);

void viewFile_and_Dir(DIR *dir,int mode,char inputDir[]);
void createDir_and_File(char inputDir[], int mode);

int isDots(char dirName[]);
void options(char inputDir[]);
DIR * inputDirectory(char inputDir[]);
void exit_if_null(void *ptr, char *message, char ch[]);
void clearInputBuffer();

int main(int argc,char* argv[]){
    int menuChoice;
    DIR *dir=NULL;
    char *temp=NULL;
    char inputDir[Char_Length]={'\0'};
    
    if(argc==1){
        dir=inputDirectory(inputDir);
        options(inputDir);
    }else if(argc==2){
        temp=argv[1];
        dir=opendir(temp);
        exit_if_null(dir,"Cheak your directory and try again",temp);
        options(temp);
    }else{
        printf("Too many arguments.");
        exit(1);
    }


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
            case 4:
            createDir_and_File(inputDir,menuChoice);
            break;

            case 3:
            deleteDir(inputDir);
            break;

            case 5:
            deleteFile(inputDir);
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

void exit_if_null(void *ptr, char *message, char ch[]){


    if(ptr==NULL){
        perror(message);
        if (ch!=NULL){
            printf("%s",ch);
        }
        exit(1);
    }
}

DIR* inputDirectory(char inputDir[]){
    DIR *dir=NULL;
    while(1){
        printf("Input Directory: ");
        fgets(inputDir, Char_Length, stdin);
        //clearInputBuffer();
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
    char tempPath[Char_Length];
    
    while( (entry=readdir(dir)) != NULL ){
        snprintf(tempPath, Char_Length, "%s/%s", inputDir, entry->d_name);
        if(stat(tempPath,&pathTypeStruct)==1){ // save info of the given path (dir) into the structure (pathType)
            exit_if_null(NULL,"Problem finding some path",tempPath);
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
                exit_if_null(NULL,"Error: ",NULL);
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
    size_t index=strcspn(dirName,"\0");
    if(dirName[index-1]=='.'){
        return 1;
    }
    return 0; 
}

void clearInputBuffer(){
    int ch;
    while((ch=getchar())!='\n'&&ch!=EOF); // clean up input buffer for fgets
}

void createDir_and_File(char inputDir[],int mode){
    char tempDirOrFile[Char_Length];
    char file_folder_name[File_Folder_Length]={'\0'};

    
    (mode==2)?
    printf("What do you want to name your directory? You must follow directory naming convention: "):
    printf("What do you want to name your file? You must follow file naming convention: ");

    fgets(file_folder_name,File_Folder_Length,stdin);
    //clearInputBuffer();
    
    size_t len=strlen(file_folder_name);//fgets also inputs the enter '\n' into the array.
    file_folder_name[len-1]='\0'; // removing '\n'
    FILE *file;

    snprintf(tempDirOrFile,Char_Length+2,"%s/%s",inputDir,file_folder_name);
    switch(mode){
        case 2:
        if (
            mkdir(
                tempDirOrFile

                // #ifndef _WIN32 //don't think it makes sense to do this
                // ,S_IRWXU
                // #endif

            )

            )
        {
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

void deleteFile(char inputDir[]){
    char ch[Char_Length],tempPath[Char_Length];
    printf("input file you want to delete: ");
    fgets(ch,Char_Length,stdin);
    ch[strcspn(ch,"\n")]='\0';
    snprintf(tempPath, Char_Length+2, "%s/%s", inputDir, ch);
    
    if((remove(tempPath))!=0){
        printf("Could not delete the file. cheak and try again.");
        return;
    }
    printf("The file was removed sucessfully");
}

void deleteDir(char inputDir[]){
    char ch[Char_Length],tempPath[Char_Length],c;
    printf("input folder you want to delete: ");
    fgets(ch,Char_Length,stdin);
    ch[strcspn(ch,"\n")]='\0';
    snprintf(tempPath, Char_Length+2, "%s/%s", inputDir, ch);
    
    if((rmdir(tempPath))!=0){
        printf("Could not delete the folder because \n Error code: %d.",errno);
        if(errno==ENOTEMPTY){
            printf("\nSubfolders and/or files found. Do you want to delete all subfolders and files? y/n: ");
            scanf("%c",&c);
            if(c=='n'||c=='N'){
                return;
            }else if(c=='y'||c=='Y'){
                delete_subFiles(tempPath);
                delete_subDir(tempPath); // deletes everything inside
                rmdir(tempPath);
            }
        }
        return;
    }
    printf("The folder was removed sucessfully");
}

void delete_subFiles(char tempPath[]){
    DIR *dir=opendir(tempPath);
    exit_if_null(dir,"Couldn't open folder",tempPath);
    struct dirent *entry;
    struct stat pathTypeStruct; // struct for stats for whatever is gonna get cheaked
    char tempPathII[Char_Length];
    
    while((entry=readdir(dir)) != NULL){
        snprintf(tempPathII, Char_Length, "%s/%s", tempPath, entry->d_name);
        
        if(stat(tempPathII,&pathTypeStruct)==1){ // save info of the given path (dir) into the structure (pathType)
            exit_if_null(NULL,"Problem finding some path",tempPathII);
        }

        if(!isDots(entry->d_name)){

            if(S_ISREG(pathTypeStruct.st_mode)){ // returns 1 if it's a file
                if((remove(tempPathII))!=0){
                    printf("Could not delete the file. cheak and try again.");
                    return;
                }
            }

            if(S_ISDIR(pathTypeStruct.st_mode)){ // returns if it's a directory
                delete_subFiles(tempPathII);
            }
        }

    }
    closedir(dir);
}

void delete_subDir(char tempPath[]){
    DIR *dir=opendir(tempPath);
    exit_if_null(dir,"Couldn't delete folder: %s",tempPath);


    struct dirent *entry;
    struct stat pathTypeStruct; 
    char tempPathII[Char_Length];

    while((entry=readdir(dir)) != NULL){
        snprintf(tempPathII, Char_Length, "%s/%s",tempPath, entry->d_name);
        if(stat(tempPathII,&pathTypeStruct)==1){
            exit_if_null(NULL,"Problem finding some path",tempPathII);
        } 
        if(!isDots(entry->d_name)){
            if(S_ISREG(pathTypeStruct.st_mode)){ 
                exit_if_null(NULL,"Some error occured",NULL);
            }
            if(S_ISDIR(pathTypeStruct.st_mode)){
                if((rmdir(tempPathII))!=0){
                    delete_subDir(tempPathII);
                }
            }
        }
    }
    rmdir(tempPath);
    closedir(dir);
}