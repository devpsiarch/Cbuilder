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

// ==============================================
// ============== Implementation: string_vec ==============
// ==============================================

#define SHOWVEC(subject) for(size_t i = 0 ; i < subject.current ; i++)\
                     {printf("%s ",subject.vec[i]);               \
                     }printf("\n");                              

typedef struct string_vec string_vec;
struct string_vec {
    char**vec;
    size_t current;
    size_t capacity;
    int (*append)(string_vec*,char*);
    int (*pop)(string_vec*);
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
int pop(string_vec*self){
    if(self->current == 0){
        perror("failed to pop since string_vec is already empty.\n");
        return -1;
    }
    self->current--;
    return 0;
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
    self->pop = pop;
    self->destroy = destroy;
    self->current = 0;
    self->capacity = 4;
    self->vec = malloc(sizeof(char*)*4);
}
// ==============================================
// ============== Implementation: Builder ==============
// ==============================================

// build command looks like this ; 
// gcc [CFLAGS] -o [OUTPUT_NAME] [SOURCE_FILES] [LIBS]
typedef struct Builder Builder;
#define SHOWCC(subject) printf("using compiler: %s.\n",subject.cc);
#define SHOWTARGET(subject) printf("building : %s.\n",subject.target);
#define SHOWBDIR(subject) if(subject.bdir == NULL)                     \
                            printf("building in current directory.\n");\
                          else                                         \
                            printf("building to: %s.\n",subject.bdir); \

struct Builder {
    char*      cc;
    char*      target;
    char*      bdir;
    string_vec flags;
    string_vec libs;
    string_vec srcs;
    char build_command_buffer[256];
    void (*appendcc)(Builder*,char*);
    void (*appendtarget)(Builder*,char*);
    void (*appendbdir)(Builder*,char*);
    void (*appendflags)(Builder*,char*);
    void (*appendlibs)(Builder*,char*);
    void (*appendsrcs)(Builder*,char*);
    void (*construct)(Builder*);
    void (*clean_up)(Builder*);
};
void appendcc(Builder*self,char*new_cc){self->cc = new_cc;}
void appendtarget(Builder*self,char*new_target){self->target = new_target;}
void appendbdir(Builder*self,char*new_bdir){self->bdir = new_bdir;}

void appendflags(Builder*self,char*new_flag){self->flags.append(&self->flags,new_flag);}
void appendlibs(Builder*self,char*new_lib){self->libs.append(&self->libs,new_lib);}
void appendsrcs(Builder*self,char*new_src){self->srcs.append(&self->srcs,new_src);}

// petetion to be added as <defer> as macro to call Builder.clean_up 
// for poor user who used this software...
void clean_up(Builder*self){
    self->flags.destroy(&self->flags);
    self->srcs.destroy(&self->srcs);
    self->libs.destroy(&self->libs);
}
void construct(Builder*self){
    // gcc [CFLAGS] -o [OUTPUT_NAME] [SOURCE_FILES] [LIBS]
    // placeholder for now .... we need to cat all the string we have and execute the command
    // add in some crossplateform stuff 
    // also change the buffer and the char pointer to the heap , its neeter.
    (void)(self);
}
void initbuilder(Builder*self){
    self->appendcc = appendcc;
    self->appendtarget = appendtarget;
    self->appendbdir = appendbdir;
    initstring_vec(&self->flags);
    initstring_vec(&self->libs);
    initstring_vec(&self->srcs);
    self->appendflags = appendflags;
    self->appendlibs = appendlibs;
    self->appendsrcs = appendsrcs;
    self->clean_up = clean_up;
    self->construct = construct;
}

int main(int argc,char*argv[]){
    if(argc >= 2){
        if(strcmp(argv[1],"-h") == 0){
            printf(QUICK_MANUAL);
            return 0;
        }
    }
    // these two lines should be expressed in a macro
    string_vec test;
    initstring_vec(&test);

    test.append(&test,"hello");
    test.append(&test,"elden");
    test.append(&test,"ring");
    SHOWVEC(test);
    
    test.destroy(&test);
    // char*cc = "clang";
    // char*flags = "-lm";
    // char*file = "test.c";
    // cmd("test",cc,flags,CFLAGS,file,"-o","test");
    
    // well use a macro do to all the ugly code job
    // the API will look something like this ...
    // very original i know
    // these two lines should be expressed in a macro
    Builder tester = {.bdir = NULL};
    initbuilder(&tester);
    
    tester.appendcc(&tester,"clang");
    tester.appendtarget(&tester,"test");
    tester.appendsrcs(&tester,"test.c");
    tester.appendflags(&tester,"-Wall");
    tester.appendflags(&tester,"-Wextra");
    tester.appendflags(&tester,"-lm");
    tester.appendflags(&tester,"-o");
    SHOWCC(tester);
    tester.clean_up(&tester);
    return 0;
    // Build builder = {};
    // builder.appendcc("clang");
    // builder.appendtarget("main");
    // builder.appendsrc("...",...);
    // builder.appendflags("...",...);
    // builder.appendbuilddir("...",...);
    // builder.build();
}
