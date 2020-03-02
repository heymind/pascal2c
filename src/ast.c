//
// Created by heymind on 2020/3/1.
//

#include "ast.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <utlist.h>

ASTNodePos *ast_node_pos_create(int start, int end, int col, int row) {
    ASTNodePos *pos = malloc(sizeof(ASTNodePos));
    pos->start = start;
    pos->end = end;
    pos->col = col;
    pos->row = row;
    return pos;
}

void ast_node_destroy(ASTNode *node) {
    assert(node != NULL);
    assert(node->prev == NULL && "can not destroy a node while it still attached");
    assert(node->next == NULL && "can not destroy a node while it still attached");
    ASTNodeAttr *attr = NULL, *tmp = NULL;
    LL_FOREACH_SAFE(node->first_attr, attr, tmp) {
        LL_DELETE(node->first_attr, attr);
        ast_node_attr_destroy(attr);
    }
    if (node->pos != NULL) free(node->pos);
    free(node);
}

ASTNodeAttr *ast_node_attr_create_node(char *key, ASTNode *node) {
    assert(key != NULL && "attr key should not be null");
    assert(node != NULL && "ast node in attr should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    attr->key = key;
    attr->kind = AST_NODE;
    attr->value = (void *) node;
    attr->next = NULL;
    return attr;
}

void ast_node_attr_destroy(ASTNodeAttr *attr) {
    assert(attr != NULL && "attr should not be null");
    assert(attr->next == NULL && "can not destroy an attr while it still attached");
    switch (attr->kind) {
        case AST_NODE: {
            ASTNode *node = NULL, *tmp = NULL, *head = attr->value;
            DL_FOREACH_SAFE(head, node, tmp) {
                DL_DELETE(head, node);
                ast_node_destroy(node);
            }
            break;
        }
    }
    free(attr);
}

ASTNodeAttr *ast_node_attr_create_integer(char *key, int val) {
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    static int value;
    value = val;
    attr->key = key;
    attr->kind = INTEGER;
    attr->value = (int *) &value;
    attr->next = NULL;
    return attr;
}

ASTNodeAttr *ast_node_attr_create_const_str(char *key, char *val) {
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    attr->key = key;
    attr->kind = CONST_STRING;
    attr->value = val;
    attr->next = NULL;
    return attr;
}

ASTNodeAttr *ast_node_attr_create_str(char *key, sds str) {
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    attr->key = key;
    attr->kind = STRING;
    attr->value = str;
    attr->next = NULL;
    return attr;
}

ASTNode *ast_node_create(char *type, ASTNodePos *pos) {
    assert(type != NULL && "node type should not be null");
//    assert(pos != NULL && "node pos should not be null");
    ASTNode *node = malloc(sizeof(ASTNode));
    node->pos = pos;
    node->type = type;
    node->first_attr = NULL;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void ast_node_attach_attr(ASTNode *node, ASTNodeAttr *attr) {
    assert(node != NULL && "ast node in attr should not be null");
    assert(attr != NULL && "attr node should not be null");

    ASTNodeAttr *current_attr;
    current_attr = node->first_attr;

    if (current_attr == NULL){
        node->first_attr = attr;
    } else {
        while(current_attr->next != NULL){
            current_attr = current_attr->next;
        }
        current_attr->next = attr;
    }
}

ASTNodeAttr *ast_node_get_attr(ASTNode *node, char *key){
    assert(node != NULL && "ast node in attr should not be null");
    assert(key != NULL && "attr key should not be null");

    ASTNodeAttr *current_attr, *target_attr;
    current_attr = node->first_attr;

    if(current_attr == NULL){
        target_attr = NULL;
    } else {
        while(current_attr != NULL &&  strcmp(current_attr->key, key)){
            current_attr = current_attr->next;
        }
        target_attr = current_attr;
    }
    assert(target_attr != NULL && "ast node doesn't have attr named key");
    return target_attr;
}

int ast_node_get_attr_integer_value(ASTNode *node, char *key){
    assert(node != NULL && "ast node in attr should not be null");
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *current_attr, *target_attr;
    current_attr = node->first_attr;

    if(current_attr == NULL){
        target_attr = NULL;
    } else {
        while(current_attr != NULL && strcmp(current_attr->key, key)){
            current_attr = current_attr->next;
        }
        target_attr = current_attr;
    }
    assert(target_attr != NULL && "ast node doesn't have attr named key");
    assert(target_attr->kind == INTEGER && "attr node named key isn't integer node");

    return *(int *)target_attr->value;
}

char *ast_node_get_attr_str_value(ASTNode *node, char *key){
    assert(node != NULL && "ast node in attr should not be null");
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *current_attr, *target_attr;
    current_attr = node->first_attr;

    if(current_attr == NULL){
        target_attr = NULL;
    } else {
        while(current_attr != NULL && strcmp(current_attr->key, key)){
            current_attr = current_attr->next;
        }
        target_attr = current_attr;
    }
    assert(target_attr != NULL && "ast node doesn't have attr named key");
    assert(target_attr->kind == STRING && "attr node named key isn't string node");

    return (char *)target_attr->value;
}

ASTNode *ast_node_get_attr_node_value(ASTNode *node, char *key){
    assert(node != NULL && "ast node in attr should not be null");
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *current_attr, *target_attr;
    current_attr = node->first_attr;

    if(current_attr == NULL){
        target_attr = NULL;
    } else {
        while(current_attr != NULL && strcmp(current_attr->key, key)){
            current_attr = current_attr->next;
        }
        target_attr = current_attr;
    }
    assert(target_attr != NULL && "ast node doesn't have attr named key");
    assert(target_attr->kind == AST_NODE && "attr node named key isn't ast node");

    return (ASTNode *)target_attr->value;
}