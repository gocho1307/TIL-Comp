#ifndef __TIL_TARGETS_TYPE_CHECKER_H__
#define __TIL_TARGETS_TYPE_CHECKER_H__

#include "targets/basic_ast_visitor.h"

namespace til {

/**
 * Prints a type as a string.
 */
class type_checker : public basic_ast_visitor {
    cdk::symbol_table<til::symbol> &_symtab;

    basic_ast_visitor *_parent;

  public:
    type_checker(std::shared_ptr<cdk::compiler> compiler,
                 cdk::symbol_table<til::symbol> &symtab,
                 basic_ast_visitor *parent)
        : basic_ast_visitor(compiler), _symtab(symtab), _parent(parent) {}

  public:
    ~type_checker() { os().flush(); }

  protected:
    bool check_compatible_ptr_types(std::shared_ptr<cdk::basic_type> t1,
                                    std::shared_ptr<cdk::basic_type> t2);
    void processUnaryExpression(cdk::unary_operation_node *const node, int lvl,
                                bool acceptDouble);
    bool processBinaryExpression(cdk::binary_operation_node *const node,
                                 int lvl);
    void processAdditiveBinaryExpression(cdk::binary_operation_node *const node,
                                         int lvl, bool isSubtraction);
    void processMultiplicativeBinaryExpression(
        cdk::binary_operation_node *const node, int lvl);
    void
    processComparisonBinaryExpression(cdk::binary_operation_node *const node,
                                      int lvl);
    void processEqualityBinaryExpression(cdk::binary_operation_node *const node,
                                         int lvl);
    void processLogicalBinaryExpression(cdk::binary_operation_node *const node,
                                        int lvl);
    template <typename T>
    void process_literal(cdk::literal_node<T> *const node, int lvl) {}

  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include ".auto/visitor_decls.h" // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end
};

} // til

//---------------------------------------------------------------------------
//     HELPER MACRO FOR TYPE CHECKING
//---------------------------------------------------------------------------

#define CHECK_TYPES(compiler, symtab, node)                                    \
    {                                                                          \
        try {                                                                  \
            til::type_checker checker(compiler, symtab, this);                 \
            (node)->accept(&checker, 0);                                       \
        } catch (const std::string &problem) {                                 \
            std::cerr << (node)->lineno() << ": " << problem << std::endl;     \
            return;                                                            \
        }                                                                      \
    }

#define ASSERT_SAFE_EXPRESSIONS CHECK_TYPES(_compiler, _symtab, node)

#endif
