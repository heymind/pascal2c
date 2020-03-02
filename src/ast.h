//
// Created by heymind on 2020/3/1.
//

#ifndef PASCAL2C_AST_H
#define PASCAL2C_AST_H

#include "sds.h"
#include <stdio.h>
#include "string.h"

enum ast_node_attr_kind {
    CONST_STRING = 1,
    STRING = 2,
    INTEGER = 3,
    AST_NODE = 4
};

struct ast_node_attr_t {
    char *key;
    enum ast_node_attr_kind kind;
    void *value;
    struct ast_node_attr_t *next;
};

struct ast_node_pos_t {
    int start, end;
    int col, row;
};

struct ast_node_t {
    char *type;
    struct ast_node_pos_t *pos;
    struct ast_node_attr_t *first_attr;
    struct ast_node_t *next;
    struct ast_node_t *prev;
};


typedef struct ast_node_attr_t ASTNodeAttr;
typedef struct ast_node_t ASTNode;
typedef struct ast_node_pos_t ASTNodePos;

/**
 * Create a Position Object representing the location of the current statement
 * @param start stmt location offset to the beginning of the current file
 * @param end
 * @param col stmt location offset to the beginning of the current line
 * @param row line number of the current stmt
 * @return ASTNodePos Object
 */
ASTNodePos *ast_node_pos_create(int start, int end, int col, int row);

/**
 * Create an Abstract syntax tree (AST) Object
 * @param type const string (char* without malloc)
 * @param pos ASTNodePos Object
 * @return ASTNode Object
 */
ASTNode *ast_node_create(char *type, ASTNodePos *pos);

/**
 * Destroy an AST Object and free the heap memory allocated for it ( including sub-objs, attr objs ... )
 * @param node
 */
void ast_node_destroy(ASTNode *node);

/**
 * Create an integer attribute object
 * @param key the attribute name ( const str required )
 * @param val the attribute value
 * @return ASTNodeAttr Object
 */
ASTNodeAttr *ast_node_attr_create_integer(char *key, int val);

/**
 * Create a string attribute object from the const str
 * @param key the attribute name ( const str required )
 * @param val the attribute value
 * @return ASTNodeAttr Object
 */
ASTNodeAttr *ast_node_attr_create_const_str(char *key, char *val);

/**
 * Create a string attribute object
 * @param key the attribute name ( const str required )
 * @param val the attribute value
 * @return ASTNodeAttr Object
 */
ASTNodeAttr *ast_node_attr_create_str(char *key, sds str);

/**
 * Create an ASTNodes attribute object ( the attr contains ast node(s) )
 * @param key the attribute name ( const str required )
 * @param val the attribute value
 * @return ASTNodeAttr Object
 */
ASTNodeAttr *ast_node_attr_create_node(char *key, ASTNode *node);

/**
 * Destroy an ASTNodes attribute object and free the heap memory allocated for it ( including sub-objs, string allocated on heap )
 * @param key the attribute name
 * @param val the attribute value
 * @return ASTNodeAttr Object
 */
void ast_node_attr_destroy(ASTNodeAttr *attr);

/**
 * Attach an attribute object to the AST node ( panic if name ( key ) duplicates )
 * @param node ASTNode Object
 * @param attr ASTNodeAttr Object
 */
void ast_node_attach_attr(ASTNode *node, ASTNodeAttr *attr);

/**
 * Get an attribute object from the AST node
 * @param node ASTNode Object
 * @param key
 * @return ASTNodeAttr Object, null if it doesn't exist.
 */
ASTNodeAttr *ast_node_get_attr(ASTNode *node, char *key);

/**
 * Get an integer attribute value from the AST node, panic if it doesn't exist.
 * @param node ASTNode Object
 * @param key
 * @return
 */
int ast_node_get_attr_integer_value(ASTNode *node, char *key);

/**
 * Get an string attribute value from the AST node, panic if it doesn't exist.
 * @param node ASTNode Object
 * @param key
 * @return
 */
char *ast_node_get_attr_str_value(ASTNode *node, char *key);

/**
 * Get an ASTNodes attribute value from the AST node, panic if it doesn't exist.
 * @param node ASTNode Object
 * @param key
 * @return
 */
ASTNode *ast_node_get_attr_node_value(ASTNode *node, char *key);

#endif //PASCAL2C_AST_H
