#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
char img_path[2][100];
int img_num=0;
int readFileList(char *basePath)
{
    
    DIR *dir;
    struct dirent *ptr;

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8)    ///file
        {
	       //strcpy(img_path[img_num],basePath);
               strcpy(img_path[img_num++],ptr->d_name);
               
            if(img_num > 1)
            {
                printf("More than two files in %s\n", basePath);
                closedir(dir);
                return -1;
            } 
	    }

        else 
        {
	    continue;
        }
    }
    
        
    closedir(dir);
    return 1;
}


int main()
{
   
    fflush(stdout);
    
    char basePath[100]="cfg/";
    
    readFileList(basePath);
    printf("name1:%s\n", img_path[0]);
    printf("name2:%s\n", img_path[1]);
}
