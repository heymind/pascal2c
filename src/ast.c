//
// Created by heymind on 2020/3/1.
//

#include "ast.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <utlist.h>


void ast_node_destroy(ASTNode *node) {
    assert(node != NULL);
    assert(node->prev == NULL && "can not destroy a node white it still attached");
    assert(node->next == NULL && "can not destroy a node white it still attached");
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
    assert(attr->next == NULL && "can not destroy an attr white it still attached");
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

