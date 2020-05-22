//
// Created by heymind on 2020/3/1.
//

#include "ast.h"

ASTNodePos *ast_node_pos_create(int start_row, int start_column, int end_row, int end_column) {
    ASTNodePos *pos = malloc(sizeof(ASTNodePos));
    pos->start_row = start_row;
    pos->start_column = start_column;
    pos->end_row = end_row;
    pos->end_column = end_column;
    return pos;
}

void ast_node_destroy(ASTNode *node) {
    if (node == NULL) {
        error_with_pos("node is NULL");
    }
    if (node->prev != NULL || node->next != NULL) {
        error_with_pos("can not destroy a node while it still attached");
    }
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

ASTNodeAttr *ast_node_attr_create_node(const char *key, ASTNode *node) {
    if (key == NULL) {
        error_with_pos("attr key should not be null");
    }
    if (node == NULL) {
        error_with_pos("ast node in attr should not be null");
    }
    assert(key != NULL && "attr key should not be null");
    assert(node != NULL && "ast node in attr should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    attr->key = key;
    attr->kind = ATTR_KIND_AST_NODE;
    attr->value = (void *) node;
    attr->next = NULL;
    return attr;
}

void ast_node_attr_destroy(ASTNodeAttr *attr) {
    if (attr == NULL) {
        error_with_pos("attr should not be null");
    }
    if (attr->next != NULL) {
        error_with_pos("can not destroy an attr while it still attached");
    }
    assert(attr != NULL && "attr should not be null");
    assert(attr->next == NULL && "can not destroy an attr while it still attached");
    switch (attr->kind) {
        case ATTR_KIND_AST_NODE: {
            ASTNode *node = NULL, *tmp = NULL, *head = attr->value;
            DL_FOREACH_SAFE(head, node, tmp) {
                DL_DELETE(head, node);
                ast_node_destroy(node);
            }
            break;
        }
        case ATTR_KIND_STRING:
            sdsfree((sds) attr);
            break;
        default:
            break;
    }
    free(attr);
}

ASTNodeAttr *ast_node_attr_create_integer(const char *key, int val) {
    if (key == NULL) {
        error_with_pos("attr key should not be null");
    }
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    static int value;
    value = val;
    attr->key = key;
    attr->kind = ATTR_KIND_INTEGER;
    attr->value = (int *) &value;
    attr->next = NULL;
    return attr;
}

ASTNodeAttr *ast_node_attr_create_const_str(const char *key, char *val) {
    if (key == NULL) {
        error_with_pos("attr key should not be null");
    }
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    attr->key = key;
    attr->kind = ATTR_KIND_CONST_STRING;
    attr->value = val;
    attr->next = NULL;
    return attr;
}

ASTNodeAttr *ast_node_attr_create_str(const char *key, sds str) {
    if (key == NULL) {
        error_with_pos("attr key should not be null");
    }
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *attr = malloc(sizeof(ASTNodeAttr));
    attr->key = key;
    attr->kind = ATTR_KIND_STRING;
    attr->value = str;
    attr->next = NULL;
    return attr;
}

ASTNode *ast_node_create(const char *type, ASTNodePos *pos) {
    if (type == NULL) {
        error_with_pos("node type should not be null");
    }
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

ASTNode *ast_node_create_without_pos(const char *type) {
    if (type == NULL) {
        error_with_pos("node type should not be null");
    }
    assert(type != NULL && "node type should not be null");
//    assert(pos != NULL && "node pos should not be null");
    ASTNode *node = malloc(sizeof(ASTNode));
    node->pos = NULL;
    node->type = type;
    node->first_attr = NULL;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void ast_node_set_attr_integer(ASTNode *node, const char *key, int value) {
    ASTNodeAttr *attr = ast_node_get_attr(node, key);
    if (attr == NULL) {
        attr = ast_node_attr_create_integer(key, value);
        ast_node_attach_attr(node, attr);
    } else {
        assert(attr->kind == ATTR_KIND_INTEGER);
        attr->value = (void *) value;
    }
}

void ast_node_set_attr_str(ASTNode *node, const char *key, sds value) {
    ASTNodeAttr *attr = ast_node_get_attr(node, key);
    if (attr == NULL) {
        attr = ast_node_attr_create_str(key, value);
        ast_node_attach_attr(node, attr);
    } else {
        assert(attr->kind == ATTR_KIND_STRING);
        sds_free(attr->value);
        attr->value = (void *) value;
    }
}

void ast_node_set_attr_const_str(ASTNode *node, const char *key, const char *value) {
    ASTNodeAttr *attr = ast_node_get_attr(node, key);
    if (attr == NULL) {
        attr = ast_node_attr_create_const_str(key, value);
        ast_node_attach_attr(node, attr);
    } else {
        assert(attr->kind == ATTR_KIND_CONST_STRING);
        attr->value = (void *) value;
    }
}

void ast_node_attr_node_append(ASTNode *node, const char *key, ASTNode *sub_node) {
    if (sub_node == NULL)
        return;
    if (sub_node->next != NULL || sub_node->prev != NULL) {
        error_with_pos("sub_node should not be attached");
    }
    assert(sub_node->next == NULL && sub_node->prev == NULL && "sub_node should not be attached");

    ASTNodeAttr *attr = ast_node_get_attr(node, key);
    if (attr == NULL) {
        attr = ast_node_attr_create_node(key, sub_node);
        ast_node_attach_attr(node, attr);
    } else {
        assert(attr->kind == ATTR_KIND_AST_NODE);
        ASTNode *head = (ASTNode *) attr->value;
        LL_APPEND(head, sub_node);
    }

}

void ast_node_extend(ASTNode *origin, ASTNode *extended) {
    LL_CONCAT(origin->first_attr, extended->first_attr);
    extended->first_attr = NULL;
    ast_node_destroy(extended);
}

void ast_node_attr_append(ASTNodeAttr *attr, ASTNodeAttr *sub_attr) {
    if (attr == NULL) {
        error_with_pos("attr node should not be null");
    }
    assert(attr != NULL && "attr node should not be null");
    attr->next = sub_attr;
}

void ast_node_create_attr_list_append(ASTNode *list_main_node, const char *key, ASTNode *list_node) {

}

UT_array *append_array_type(UT_array *types, char *string) {
    if (types == NULL) {
        utarray_new(types, &ut_str_icd);
        utarray_push_back(types, &string);
    } else {
        utarray_push_back(types, &string);
    }
    return types;
}

int find_array_type(UT_array *types, char *string) {
    char **p = NULL;
    while ((p = (char **) utarray_next(types, p))) {
        if (strcmp(*p, string) == 0)
            return 1;
    }
    return 0;
}

void ast_node_attach_attr(ASTNode *node, ASTNodeAttr *attr) {
    if (node == NULL) {
        error_with_pos("ast node in attr should not be null");
    }
    if (attr == NULL) {
        error_with_pos("attr node should not be null");
    }
    assert(node != NULL && "ast node in attr should not be null");
    assert(attr != NULL && "attr node should not be null");
    LL_APPEND(node->first_attr, attr);

//    ASTNodeAttr *current_attr;
//    current_attr = node->first_attr;
//
//    if (current_attr == NULL){
//        node->first_attr = attr;
//    } else {
//        while(current_attr->next != NULL){
//            current_attr = current_attr->next;
//        }
//        current_attr->next = attr;
//    }
}

ASTNodeAttr *ast_node_get_attr(ASTNode *node, const char *key) {
    if (node == NULL) {
        error_with_pos("ast node in attr should not be null");
    }
    if (key == NULL) {
        error_with_pos("attr key should not be null");
    }
    assert(node != NULL && "ast node in attr should not be null");
    assert(key != NULL && "attr key should not be null");
    ASTNodeAttr *cur;
//    ASTNodeAttr *current_attr, *target_attr;
//    current_attr = node->first_attr;
//
//    if(current_attr == NULL){
//        target_attr = NULL;
//    } else {
//        while(current_attr != NULL &&  strcmp(current_attr->key, key)){
//            current_attr = current_attr->next;
//        }
//        target_attr = current_attr;
//    }

    LL_FOREACH(node->first_attr, cur) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
    };
//    assert(target_attr != NULL && "ast node doesn't have attr named key");
    return NULL;
}

int ast_node_get_attr_integer_value(ASTNode *node, const char *key) {
//    assert(node != NULL && "ast node in attr should not be null");
//    assert(key != NULL && "attr key should not be null");
//    ASTNodeAttr *current_attr, *target_attr;
//    current_attr = node->first_attr;
//
//    if(current_attr == NULL){
//        target_attr = NULL;
//    } else {
//        while(current_attr != NULL && strcmp(current_attr->key, key)){
//            current_attr = current_attr->next;
//        }
//        target_attr = current_attr;
//    }
    ASTNodeAttr *target_attr = ast_node_get_attr(node, key);
    if (target_attr == NULL) {
        error_with_pos("ast node doesn't have attr named key");
    }
    if (target_attr->kind != ATTR_KIND_INTEGER) {
        error_with_pos("attr node named key isn't integer node");
    }
    assert(target_attr != NULL && "ast node doesn't have attr named key");
    assert(target_attr->kind == ATTR_KIND_INTEGER && "attr node named key isn't integer node");

    return *(int *) target_attr->value;
}

char *ast_node_get_attr_str_value(ASTNode *node, const char *key) {
//    assert(node != NULL && "ast node in attr should not be null");
//    assert(key != NULL && "attr key should not be null");
//    ASTNodeAttr *current_attr, *target_attr;
//    current_attr = node->first_attr;
//
//    if(current_attr == NULL){
//        target_attr = NULL;
//    } else {
//        while(current_attr != NULL && strcmp(current_attr->key, key) != 0){
//            current_attr = current_attr->next;
//        }
//        target_attr = current_attr;
//    }
    ASTNodeAttr *target_attr = ast_node_get_attr(node, key);
    //assert(target_attr != NULL && "ast node doesn't have attr named key");
    if (target_attr == NULL)
        return NULL;
    if (target_attr->kind != ATTR_KIND_STRING) {
        error_with_pos("attr node named key isn't string node");
    }
    assert(target_attr->kind == ATTR_KIND_STRING && "attr node named key isn't string node");

    return (char *) target_attr->value;
}

ASTNode *ast_node_get_attr_node_value(ASTNode *node, const char *key) {
//    assert(node != NULL && "ast node in attr should not be null");
//    assert(key != NULL && "attr key should not be null");
//    ASTNodeAttr *current_attr, *target_attr;
//    current_attr = node->first_attr;
//
//    if(current_attr == NULL){
//        target_att`r = NULL;
//    } else {
//        while(current_attr != NULL && strcmp(current_attr->key, key)){
//            current_attr = current_attr->next;
//        }
//        target_attr = current_attr;
//    }
    ASTNodeAttr *target_attr = ast_node_get_attr(node, key);
    if (target_attr == NULL)
        return NULL;
    if (target_attr->kind != ATTR_KIND_AST_NODE) {
        error_with_pos("attr node named key isn't ast node");
    }
    assert(target_attr->kind == ATTR_KIND_AST_NODE && "attr node named key isn't ast node");

    return (ASTNode *) target_attr->value;
}


void _ast_node_attr_dump_fp(ASTNodeAttr *head, FILE *fp) {
    if (head == NULL) {
        fprintf(fp, "null");
        return;
    }
    ASTNodeAttr *attr = NULL;
    fprintf(fp, "{");
    DL_FOREACH(head, attr) {
        if (attr != head) fprintf(fp, ",");
        fprintf(fp, "\"%s\":", attr->key);
        switch (attr->kind) {
            case ATTR_KIND_INTEGER:
                fprintf(fp, "%d", (int) attr->value);
                break;
            case ATTR_KIND_CONST_STRING:
            case ATTR_KIND_STRING:
                fprintf(fp, "\"%s\"", (char *) attr->value);
                break;
            case ATTR_KIND_AST_NODE:
                assert(attr->value != NULL);
                _ast_node_dump_fp((ASTNode *) (attr->value), fp);
                break;
        }
    }
    fprintf(fp, "}");
}

void _ast_node_dump_fp(ASTNode *head, FILE *fp) {
    if (head == NULL) {
        error_with_pos("head should not be NULL");
    }
    assert(head != NULL);
    ASTNode *node = NULL;
    //fprintf(fp, "[");
    DL_FOREACH(head, node) {
        if (node != head) fprintf(fp, ",");
        if (strstr(node->type, "LIST")) {
            fprintf(fp, "[");
            ASTNodeAttr *attr = NULL;
            DL_FOREACH(node->first_attr, attr) {
                if (attr != node->first_attr) fprintf(fp, ",");
                fprintf(fp, "{\"type\":\"%s\",", node->type);
                if (node->pos != NULL) {
                    fprintf(fp, "\"pos\":\"(%d,%d)->(%d,%d)\",", node->pos->start_row, node->pos->start_column,
                            node->pos->end_row, node->pos->end_column);
                }
                fprintf(fp, "\"attrs\":");
                ASTNodeAttr *attr_ = malloc(sizeof(ASTNodeAttr));
                *attr_ = *attr;
                attr_->next = NULL;
                _ast_node_attr_dump_fp(attr_, fp);
                free(attr_);
                fprintf(fp, "}");
            }
            fprintf(fp, "]");
        } else {
            fprintf(fp, "{\"type\":\"%s\",", node->type);
            if (node->pos != NULL) {
                fprintf(fp, "\"pos\":\"(%d,%d)->(%d,%d)\",", node->pos->start_row, node->pos->start_column,
                        node->pos->end_row, node->pos->end_column);
            }
            fprintf(fp, "\"attrs\":");
            _ast_node_attr_dump_fp(node->first_attr, fp);
            fprintf(fp, "}");
        }
//        fprintf(fp, "{\"type\":\"%s\",\"start\":%d,\"end\":%d,\"row\":%d,\"col\":%d,\"attrs\":",
//                node->type,
//                node->pos->start,
//                node->pos->end,
//                node->pos->row,
//                node->pos->col
//        );
    }
    //fprintf(fp, "]");
}

void ast_node_dump_json(ASTNode *head, char *filename) {
    if (head == NULL || filename == NULL) {
        error_with_pos("head or filename should not be NULL");
    }
    assert(head != NULL && filename != NULL);
    FILE *fp = fopen(filename, "w+");
    _ast_node_dump_fp(head, fp);
    fclose(fp);
}