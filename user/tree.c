#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"        
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *lastname(char *path)
{
    char *p;
    for(p = path + strlen(path); p >= path; p--){
        if(*p == '/'){
            return p + 1;
        }
    }
    return path;
}

void tree(char *path, int depth)
{
    int fd = open(path, O_RDONLY);
    if(fd < 0){
        fprintf(2, "tree: cannot open %s\n", path);
        exit(1);
    }

    struct stat st;
    if(fstat(fd, &st) < 0){
        fprintf(2, "tree: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    char *name = lastname(path);
    for(int i = 0; i < depth; i++) printf("  ");
    if(st.type == T_DIR)
    {
        if(strlen(name) == 0)
            printf("//\n");   
        else
            printf("%s/\n", name);
    }
    else printf("%s\n", name);

    if(st.type != T_DIR){
        close(fd);
        return;
    }

    struct dirent de;
    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0) continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

        char fullpath[512];
        int len = strlen(path);
        strcpy(fullpath, path);
        fullpath[len] = '/';
        strcpy(fullpath + len + 1, de.name);

        struct stat st2;
        if(stat(fullpath, &st2) < 0){
            fprintf(2, "tree: cannot stat %s\n", fullpath);
            continue;  
        }

        tree(fullpath, depth + 1);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        tree(".", 0);
        exit(0);
    }
    else if(argc > 2){
        fprintf(2, "Only accept one argument: tree <directory>\n");
        exit(1);
    }

    tree(argv[1], 0);
    exit(0);
}