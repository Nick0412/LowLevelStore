## Building
To build this project, you can run the following
``` 
make all
```

This will create a directory called `bin/main-programs/` in which the executable targets will be placed. As of now it creates two executables called `client` and `server`.

## Convetions

### Preprocessor Macros
All preprocessor macro definitions should be prefixed with `PP_`, short for preprocessor, and be capital snake case. 

```c
#define PP_MAX(a, b) a > b ? a : b
```

### Function Names
- Function names should be lower snake case (in the process of trying to convert current function names to this.)

    ```c
    extern void my_cool_function();
    ```

- Function names that act on specific structs or types should be prefixed with that type's name using lower snake case.
```c
typedef struct MyCoolStruct
{

} MyCoolStruct;

void my_cool_struct_initialize(MyCoolStruct* data);
```

### Variables Names
Variable names should be lower snake case.

```c
int some_fun_variable = 5;
```

### Struct Names
Struct names should be upper camel case and always `typedef`'ed to have their simple name.
```c
typedef struct MyImportantStruct
{
    // ...
} MyImportantStruct;
```
