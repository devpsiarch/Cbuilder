#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


void printcmd(const unsigned int count,...){
    char*next;
    va_list args;
    va_start(args,count);
    for(unsigned int i = 0 ; i < count ; i ++){
        next = va_arg(args,char *);
        printf("%s ",next);
    }
    va_end(args);
}

void cmd(const char*target,...){
    char buffer[256];
    size_t current = 0;
    char*next = NULL;
    va_list args;
    va_start(args,target);
    while(next != target){
        next = va_arg(args,char *);
        size_t len = strlen(next); 
        for(size_t i = 0 ; i < len ; i++){
            buffer[current+i] = next[i];
        }
        buffer[current+ len] = ' ';
        current+= len+1;
    }
    va_end(args);
    buffer[current] = '\0';
    printf("%s\n",buffer);
    system(buffer);
}
// recommeded to add
#define CFLAGS "-Wall -Wextra"
#define QUICK_MANUAL "Write the build.c builder file then execute it to compile your code\n"
// additional implimentation for conviniance ... this is C
typedef struct string_vec string_vec;
struct string_vec {
    char**vec;
    size_t current;
    size_t capacity;
    int (*append)(string_vec*,char*);
    void (*show)(string_vec*);
    void (*destroy)(string_vec*);
};
int append(string_vec*self,char*str){
    if(self->current >= self->capacity){
        size_t new_capacity = self->capacity*2;
        char**new_vec = realloc(self->vec,sizeof(char*)*new_capacity);
        if(new_vec == NULL){
            // this way we keep the original vec if appending failed.
            perror("realloc failed while appending to string_vec.\n");
            return -1;
        }
        self->vec = new_vec;
        self->capacity = new_capacity;
    }
    // creates a new memory and copy the str
    self->vec[self->current] = strdup(str);
    if(!self->vec[self->current]){
        perror("Failed to copy new element while appending to vec.\n");
        return -1;
    }
    self->current++;
    return 0;
}
void show(string_vec*self){
    for(size_t i = 0 ; i < self->current ; i++){
        printf("%s ",self->vec[i]);
    }
    printf("\n");
}
void destroy(string_vec*self){
    for(size_t i = 0 ; i < self->capacity ; i++){
        free(self->vec[i]);
    }
    self->vec = NULL;
    self->current = 0;
    self->capacity = 0;
}
void initstring_vec(string_vec*self){
    self->append = append;
    self->destroy = destroy;
    self->show = show;
    self->current = 0;
    self->capacity = 4;
    self->vec = malloc(sizeof(char*)*4);
}
// build command looks like this ; 
// gcc [CFLAGS] -o [OUTPUT_NAME] [SOURCE_FILES] [LIBS]
typedef struct Builder Builder;
struct Builder {
    char*cc;
    char*target;
    char**flags;
    char**libs;
    char**src;
    char*bdir;
    char build_command_buffer[256];
    void (*appendcc)(Builder*,char*);
    void (*showcc)(Builder*);
    void (*appendtarget)(Builder*,char*);
    void (*showtarget)(Builder*);
    void (*appendbdir)(Builder*,char*);
    void (*showbdir)(Builder*);
};
void appendcc(Builder*self,char*new_cc){
    self->cc = new_cc;
}
void showcc(Builder*self){
    printf("using compiler: %s.\n",self->cc);
}
void appendtarget(Builder*self,char*new_target){
    self->target = new_target;
}
void showtarget(Builder*self){
    printf("building : %s.\n",self->target);
}
void appendbdir(Builder*self,char*new_bdir){
    self->bdir = new_bdir;
}
void showbdir(Builder*self){
    if(self->bdir == NULL){
        printf("building in current directory.\n");
    }
    else{ 
        printf("building to: %s.\n",self->bdir);
    }
}
void initbuilder(Builder*self){
    self->appendcc = appendcc;
    self->showcc = showcc;
    self->appendtarget = appendtarget;
    self->showtarget = showtarget;
    self->appendbdir = appendbdir;
    self->showbdir = showbdir;
}

int main(int argc,char*argv[]){
    if(argc >= 2){
        if(strcmp(argv[1],"-h") == 0){
            printf(QUICK_MANUAL);
            return 0;
        }
    }
    string_vec test;
    initstring_vec(&test);
    test.append(&test,"helo");

    
    test.show(&test);

    test.destroy(&test);
    return 0;
    // char*cc = "clang";
    // char*flags = "-lm";
    // char*file = "test.c";
    // cmd("test",cc,flags,CFLAGS,file,"-o","test");
    
    // well use a macro do to all the ugly code job
    Builder tester = {.bdir = NULL};
    initbuilder(&tester);
    tester.appendcc(&tester,"clang");
    tester.appendtarget(&tester,"test");
    tester.showcc(&tester);
    return 0;
    // Build builder = {};
    // builder.appendcc("clang");
    // builder.appendtarget("main");
    // builder.appendsrc("...",...);
    // builder.appendflags("...",...);
    // builder.appendbuilddir("...",...);
    // builder.build();
}
