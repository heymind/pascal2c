## 依赖
为了统一 unix 系统和非 unix 系统对 lex, yacc 的依赖，故使用 bison 和 flex
```sh
# yacc -> bison 
# lex  -> flex
brew install bison flex 
```
## 程序编译：
```sh
mkdir build
cd build
cmake ..
make
./pascal2c
```
或者直接点击 Clion 中运行按钮，代码生成过程已集成到编译过程之中，无需手动调用生成代码。


## 程序运行
```
    >$ ./pascal2c <FilePath>
```    
