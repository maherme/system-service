# Coding Style

## Formatting

  - Don't use tabs, use 4 character to indent.
  - Use comments in this way: /* comment */
  - For functions:
    ```C
    bool foo_function(int arg1, char arg2){
    
        bool a = 0;
        
        return a;
    }
    ```
  - For loop or conditional sentences:
    ```C
    if((a < b) && (c >= d)){
        foo1();
    }
    else{
        foo2();
    }
    ```
    Use braces for a single if condition:
    ```C
    if(a < b){
        foo();
    }
    ```
## Doxygen
  - Information about functions is placed in .c files, please, follow the format below:
    ```C
    /**
     * @brief bla bla bla.
     *
     * @param[in] param1 bla bla bla.
     * @param[in] param2 bla bla bla.
     * @param[out] param3 bla bla bla.
     *
     * @return bla bla bla.
     */
    ```
