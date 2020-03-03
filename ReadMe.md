## 程序编译：
若无修改ast_lex.l, ast_yacc.y，直接编译
若有修改: 
```
    >$ lex ast_lex.l
    >$ yacc -d ast_yacc.y
    CLion运行
    编译成功后，生成可执行文件在CMakeFiles，现文件读入从运行参数中读入，需要从命令行中运行
```
注：win下同理，在此不再赘述
## 程序运行
```
    >$ ./pascal2c <FilePath>
```    
