#include "formula/ast.h"

namespace formula {

Value BinaryOpNode::accept(AstVisitor* visitor) {
    return visitor->visitBinaryOp(this);
}

Value UnaryOpNode::accept(AstVisitor* visitor) {
    return visitor->visitUnaryOp(this);
}

Value TernaryOpNode::accept(AstVisitor* visitor) {
    return visitor->visitTernaryOp(this);
}

Value NumberNode::accept(AstVisitor* visitor) {
    return visitor->visitNumber(this);
}

Value VariableNode::accept(AstVisitor* visitor) {
    return visitor->visitVariable(this);
}

Value FunctionCallNode::accept(AstVisitor* visitor) {
    return visitor->visitFunctionCall(this);
}

} // namespace formula
