#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/wait.h>
#include <sys/stat.h>

/*---------------------------------------------------------------------------------------------------------------------------------
Algo:
    code starts with main where we take contain of ls -ltr into lsResult. The contains could be files, link, Directory. giveSize give size of files and Directory added together.
    For directory contains we use for loop which directory to take and use function dirSize which create new
    process and give size for all contain inside it. Last loop is for link and its contents calculation
------------------------------------------------------------------------------------------------------------------------------------------------
    */

char **lsResult;
char *executableFileName;
unsigned long result=4096;   //initilized because we want to add parent directory size
//below function add size of files inside parent directory
unsigned long giveSize(int columLen,char *path) {
    unsigned long value = 0;
    char *delimiter1 = " ";
    char *fileNameBuff = (char *)malloc(5000*sizeof(char));
    char *templsResult = (char *)malloc(5000*sizeof(char));
    for(int i=0;i<columLen;++i){
        memset(templsResult,0,strlen(templsResult));
        strncpy(templsResult,lsResult[i],strlen(lsResult[i])+1);
        char *portion = strtok(templsResult,delimiter1); 
        if(portion[0] == 'l' || portion[1] == 'p'|| portion[0] == 'd')
                continue;
        int whiteSpace = 1;
        while (portion != NULL)
        {
            /* code */
            
            memset(fileNameBuff,0,strlen(fileNameBuff));
            strncpy(fileNameBuff,portion,strlen(portion)+1);
            portion = strtok(NULL,delimiter1);
            whiteSpace++;
            while (whiteSpace > 9)
            {
                /* code */
                if(portion == NULL) 
                    break;
                
                char *temp = "";
                strcat(fileNameBuff,temp);
                strcat(portion,temp);
                temp = " ";
                strcat(fileNameBuff,temp);
                strcat(fileNameBuff,portion);
                whiteSpace++;
                portion = strtok(NULL,delimiter1);
            }
        }
        struct stat sbuf1;
        char *tempPath = malloc(5000*sizeof(char));
        strncpy(tempPath,path,strlen(path)+1);
        char *tmp = "/";
        if(tempPath[strlen(tempPath)-1] != '/')
            strcat(tempPath,tmp);
        fileNameBuff = strcat(tempPath,fileNameBuff);
        stat(fileNameBuff,&sbuf1);
        if(sbuf1.st_ino != 0)
            value += sbuf1.st_size;
        fileNameBuff[0] = '\0';
        
    }
    free(fileNameBuff);
    free(templsResult);
    return value;
}
//bwlow function add size of file inside link 
unsigned long linkgiveSize(char *filePath,int columLen) {
    unsigned long value = 0;
    char *delimiter1 = " ";
    char *fileNameBuff = (char *)malloc(5000*sizeof(char));
    char *templsResult = (char *)malloc(5000*sizeof(char));
    for(int i=0;i<columLen;++i){
        memset(templsResult,0,strlen(templsResult));
        strncpy(templsResult,lsResult[i],strlen(lsResult[i])+1);
        char *portion = strtok(templsResult,delimiter1); 
        if(portion[0] == 'l' || portion[1] != 'p'|| portion[0] == 'd')
                continue;
        int whiteSpace = 1;
        while (portion != NULL)
        {
            /* code */
            
            memset(fileNameBuff,0,strlen(fileNameBuff));
            strncpy(fileNameBuff,portion,strlen(portion)+1);
            portion = strtok(NULL,delimiter1);
            whiteSpace++;
            while (whiteSpace > 9)
            {
                /* code */
                if(portion == NULL) 
                    break;
                
                char *temp = "";
                strcat(fileNameBuff,temp);
                strcat(portion,temp);
                temp = " ";
                strcat(fileNameBuff,temp);
                strcat(fileNameBuff,portion);
                temp="";
                strcat(fileNameBuff,temp);
                whiteSpace++;
                portion = strtok(NULL,delimiter1);
            }
        }
        char *tempFilePath;
        tempFilePath = (char *)malloc(5000*sizeof(char));
        strncpy(tempFilePath,filePath,strlen(filePath)+1);
        tempFilePath[strlen(filePath)+1] = '\0';
        strcat(tempFilePath,fileNameBuff);
        struct stat sbuf1;
        stat(tempFilePath,&sbuf1);
        if(sbuf1.st_ino != 0)
            value += sbuf1.st_size;
        fileNameBuff[0] = '\0';
        free(tempFilePath);
        
    }
    free(fileNameBuff);
    free(templsResult);
    
    return value;
}

//below function code is to get directory names 
char * dirfileName(char *dirdiscription){

    char *delimiter1 = " ";
    char *dirNameBuff = (char *)malloc(5000*sizeof(char));
    char *tempPortion = (char *)malloc(5000*sizeof(char));

    strncpy(tempPortion,dirdiscription,strlen(dirdiscription)+1);
    char *portion = strtok(tempPortion,delimiter1);
    int whiteSpace = 1;
    while (portion != NULL)
    {
        /* code */
        memset(dirNameBuff,0,strlen(dirNameBuff));
        strncpy(dirNameBuff,portion,strlen(portion)+1);
        portion = strtok(NULL,delimiter1);
        whiteSpace++;
        while (whiteSpace > 9)
        {
            /* code */
            if(portion == NULL)
                break;

            char *temp = "";
            strcat(dirNameBuff,temp);
            strcat(portion,temp);
            temp = " ";
            strcat(dirNameBuff,temp);
            strcat(dirNameBuff,portion);
            temp="";
            strcat(dirNameBuff,temp);
            whiteSpace++;
            portion = strtok(NULL,delimiter1);
            
        }
        
    }
    free(tempPortion);
    return dirNameBuff;      
}
//below function code is to get link path 
char * fileName(char *containListDiscription){
    char *delimiter1 = " ";
    char *fileNameBuff = (char *)malloc(5000*sizeof(char));
    char *portion = strtok(containListDiscription,delimiter1); 
    int whiteSpace = 1;
    while (portion != NULL)
    {
        /* code */
        
        memset(fileNameBuff,0,strlen(fileNameBuff));
        strncpy(fileNameBuff,portion,strlen(portion));
        portion = strtok(NULL,delimiter1);
    }
    memset(containListDiscription,0,strlen(containListDiscription));
    char *tmp = "lp";
    strncpy(containListDiscription,tmp,strlen(tmp)+1);
    containListDiscription[2] = '\0';
    return fileNameBuff;
}

unsigned long dirSize(char *path, char *dirName){
    unsigned long value = 0;
    int temp;
    if(path[strlen(path)-1] != '/' ){
        char *tmp = "/";
        strcat(path,tmp);
    }
    int fd1[2];
    strcat(path,dirName);

    if(pipe(fd1) < 0){
        // printf("%s : %d pipe. Unable to execute.",__FILE__,__LINE__);
        exit(-1);
    } 
    temp = fork();
    if(temp < 0){
        // printf("%s : %d fork unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    if(!temp){
        close(fd1[0]);
        close(1);
        dup(fd1[1]);
        execl(executableFileName, executableFileName,path, NULL);
        // printf("%s : %d exec unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    int childPid = waitpid(temp, NULL, 0);

    close(fd1[1]); 
    char readBuffer1[5000];
    int nbytes = read(fd1[0],readBuffer1, sizeof(readBuffer1)+1);
    close(fd1[0]);
    value = atoi(readBuffer1);
    return value;
}

unsigned long giveLinkSize(char **linklsResult, int lenLinkColumn, char *path){
    unsigned long value = 0;
    char *delimiter1 = " ";
    char *fileNameBuff = (char *)malloc(5000*sizeof(char));
    char *templsResult = (char *)malloc(5000*sizeof(char));
    for(int i=0;i<lenLinkColumn;++i){
        memset(templsResult,0,strlen(templsResult));
        strncpy(templsResult,linklsResult[i],strlen(linklsResult[i])+1);
        char *portion = strtok(templsResult,delimiter1); 
        if(portion[0] == 'l' || portion[0] == 'd')
                continue;
        int whiteSpace = 1;
        while (portion != NULL)
        {
            /* code */
            
            memset(fileNameBuff,0,strlen(fileNameBuff));
            strncpy(fileNameBuff,portion,strlen(portion)+1);
            portion = strtok(NULL,delimiter1);
            whiteSpace++;
            while (whiteSpace > 9)
            {
                /* code */
                if(portion == NULL) 
                    break;
                
                char *temp = "";
                strcat(fileNameBuff,temp);
                strcat(portion,temp);
                temp = " ";
                strcat(fileNameBuff,temp);
                strcat(fileNameBuff,portion);
                whiteSpace++;
                portion = strtok(NULL,delimiter1);
            }
        }
        struct stat sbuf1;
        char *tempPath = malloc(5000*sizeof(char));
        strncpy(tempPath,path,strlen(path)+1);
        char *tmp = "/";
        if(tempPath[strlen(tempPath)-1] != '/')
            strcat(tempPath,tmp);
        fileNameBuff = strcat(tempPath,fileNameBuff);
        stat(fileNameBuff,&sbuf1);
        if(sbuf1.st_ino != 0)
            value += sbuf1.st_size;
        fileNameBuff[0] = '\0';
        
    }
    return value;
}
//below function does the same as main function execpt its content inside the  link and stored in linklsResult
void linkDirFileNameAdd(char *linkDir, int *columLen){
    int fd[2];
    int readbufferSize = 4000;
    int temp;
    unsigned long value = 0;
    if(pipe(fd) < 0){
        // printf("%s : %d pipe. Unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    temp = fork();

    if(temp < 0){
        // printf("%s : %d fork unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    if(!temp){
        close(fd[0]);
        close(1);
        dup(fd[1]);
        execl("/bin/ls", "ls","-ltr", linkDir, NULL);
        // printf("%s : %d exec unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }
    int childPid = wait(&temp);

    close(fd[1]);   //file descriptor for writing to pipe line
    char readbuffer[readbufferSize];
    int nbytes;         //does nothing
    nbytes = read(fd[0],readbuffer, sizeof(readbuffer)+1);
    close(fd[0]);
    char *delimiter1 = "\n";
    char *portion = (char *)malloc(5000*sizeof(char));
    portion = strtok(readbuffer, delimiter1);
    
    portion = strtok(NULL, delimiter1);
    char **linklsResult;
    linklsResult = (char **)malloc(1*sizeof(char *));
    int linkcolumnLen=0;
    char *dirNameBuff;
    while (portion != NULL)
    {
        /* code */
        if(portion[0] != 'd'){
            *(linklsResult+linkcolumnLen) = (char*)malloc((strlen(portion)+1)*sizeof(char));
            strncpy(linklsResult[linkcolumnLen],portion,strlen(portion)+1);
            ++linkcolumnLen;
            portion = strtok(NULL, delimiter1);    
            linklsResult = realloc(linklsResult,(linkcolumnLen+1)*sizeof(char *));          //realloc increase whole array by given amount
            continue;
        }
        portion[1] = 'p';

        if(portion[0] == 'd' && portion[1]== 'p'){
            dirNameBuff = dirfileName(portion);
            result += dirSize(linkDir,dirNameBuff);
        }
        
    }
    //below block is for other links in the folder
    char *fileNameBuff;
    for(int i=0;i<linkcolumnLen;++i){
        char *tempNew = malloc(5000*sizeof(char));
        strncpy(tempNew,linklsResult[i],strlen(linklsResult[i])+1);
        if(tempNew[0] == 'l' && tempNew[1] != 'p'){
            fileNameBuff = fileName(tempNew);
            linkDirFileNameAdd(fileNameBuff,columLen);
            result += linkgiveSize(fileNameBuff,(*columLen));
        }
        linklsResult[i][1] = 'p';
    }

    result += giveLinkSize(linklsResult,linkcolumnLen,linkDir);

    free(portion);
    free(linklsResult);
}

unsigned long main(int argc, char* argv[]){
    int fd[2];
    int readbufferSize = 5000;
    int temp;
    executableFileName = (char *)malloc(5000*sizeof(char));
    memset(executableFileName,0,strlen(executableFileName));
    strncpy(executableFileName,argv[0],strlen(argv[0])+1);
    if(pipe(fd) < 0){
        // printf("%s : %d pipe. Unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    temp = fork();

    if(temp < 0){
        // printf("%s : %d fork unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    if(!temp){
        close(fd[0]);
        close(1);
        dup(fd[1]);
        execl("/bin/ls", "ls","-ltr", argv[1], NULL);
        // printf("%s : %d exec unable to execute.",__FILE__,__LINE__);
        exit(-1);
    }

    int childPid = wait(&temp);

    close(fd[1]);   //file descriptor for writing to pipe line
    char readbuffer[readbufferSize];
    int nbytes;         //does nothing
    nbytes = read(fd[0],readbuffer, sizeof(readbuffer)+1);
    close(fd[0]);
    char *delimiter1 = "\n";
    char *portion = (char *)malloc(5000*sizeof(char));
    portion = strtok(readbuffer, delimiter1);
    portion = strtok(NULL, delimiter1);

    int colLen = 0;
    lsResult = (char **)malloc(1*sizeof(char *));

    while (portion != NULL)
    {
        /* code */
        char *tmp = "\n";
        if(!strcmp(portion,tmp)){
            continue;
        }
        *(lsResult+colLen) = (char*)malloc((strlen(portion)+1)*sizeof(char));
        strncpy(lsResult[colLen],portion,strlen(portion)+1);
        ++colLen;
        portion = strtok(NULL, delimiter1);
        lsResult = realloc(lsResult,(colLen+1)*sizeof(char *));        //realloc increase whole array by given amount       
    }

    result += giveSize(colLen,argv[1]);

    //size of files inside directory
    char *dirNameBuff;
    
    for(int i=0;i<colLen;++i){
        int fd1[2];
        char readBuffer1[readbufferSize];
        if(lsResult[i][0] != 'd'){
            continue;
        }
        if(lsResult[i][0] == 'd' && lsResult[i][1] == 'p'){
            continue;
        }
        char *tempNew1 = malloc(5000*sizeof(char));
        strncpy(tempNew1,lsResult[i],strlen(lsResult[i])+1);
        lsResult[i][1] = 'p';
        dirNameBuff = dirfileName(tempNew1);
        char *path = malloc(5000*sizeof(char));
        strncpy(path,argv[1],strlen(argv[1])+1);
        result += dirSize(path,dirNameBuff);

    }
        // for link size calculation
    char *pathForFilesInLink;
    for(int i=0;i<colLen;++i){
        char *tempNew = malloc(5000*sizeof(char));
        strncpy(tempNew,lsResult[i],strlen(lsResult[i])+1);
        if(tempNew[0] == 'l' && tempNew[1] != 'p'){
            pathForFilesInLink = fileName(tempNew);
            linkDirFileNameAdd(pathForFilesInLink,&colLen);
            result += linkgiveSize(pathForFilesInLink,colLen);
            lsResult[i][1] = 'p';
        }
        free(tempNew);
    }
    printf("%ld\n",result);
    return result;
}

