# OuroC
Build your C project with nothing other then pure C !

![logo](assets/logo.png)
<img src="assets/logo.png" alt="logo" width="200"/>
# How to use
1- get the ```ouro.h``` into your home directory.\
2- create a ```build.c``` file.\
3- Write you build instructions.\
```c
#include <stdio.h>
#include "./ouroc.h"
#define OUROC_IMPLI
int main(int argc,char*argv[]){
    // Marco for prettier code soon ... sorry am lazy
    Builder tester = {
        .bdir = NULL,
        .buffer_used = 0,
        .buffer_max = 256
    };  
    initbuilder(&tester);

    tester.appendcc(&tester,"clang");
    tester.appendtarget(&tester,"test");
    tester.appendsrcs(&tester,"test.c");
    
    // you can wrap these to there own functions 
    tester.appendflags(&tester,"-Wall");
    tester.appendflags(&tester,"-Wextra");
    tester.appendflags(&tester,"-lm");
    tester.appendflags(&tester,"-o");
 
    tester.construct(&tester);
    SHOWCOMMAND(tester);
    tester.execute(&tester);
    tester.clean_up(&tester);
	return 0;
}
 ```
Or you can construct the build command in real time.
```c
##include "./ouroc.h"
#define OUROC_IMPLIinclude <stdio.h>
int main(void){
	Builder admin = {
        .bdir = NULL,
        .buffer_used = 0,
        .buffer_max = 256
    };
    initbuilder(&admin);
    admin.appendcmd(&admin,"gcc ");
    admin.appendcmd(&admin,"-Wall ");
    admin.appendcmd(&admin,"-Wextra ");
    admin.appendcmd(&admin,"-lm  ");
    admin.appendcmd(&admin,"-o admin_test ");
    admin.appendcmd(&admin,"test.c");
    SHOWCOMMAND(admin);
    admin.execute(&admin);
	admin.clean_up(&admin);
	return 0;
}
```
3- compile your builder only once then run it to build your project.\

# Why ? 
Only real developers know , if you build your C projects with anything other then C itself , then you are not a real programmer ... 
