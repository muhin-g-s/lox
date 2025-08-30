package dev.muhings.lox;

import java.util.List;

import dev.muhings.lox.Expr.Assign;
import dev.muhings.lox.Expr.Binary;
import dev.muhings.lox.Expr.Call;
import dev.muhings.lox.Expr.Get;
import dev.muhings.lox.Expr.Grouping;
import dev.muhings.lox.Expr.Literal;
import dev.muhings.lox.Expr.Logical;
import dev.muhings.lox.Expr.Set;
import dev.muhings.lox.Expr.Super;
import dev.muhings.lox.Expr.This;
import dev.muhings.lox.Expr.Unary;
import dev.muhings.lox.Expr.Variable;
import dev.muhings.lox.Stmt.Block;
import dev.muhings.lox.Stmt.Class;
import dev.muhings.lox.Stmt.Expression;
import dev.muhings.lox.Stmt.Function;
import dev.muhings.lox.Stmt.If;
import dev.muhings.lox.Stmt.Print;
import dev.muhings.lox.Stmt.Return;
import dev.muhings.lox.Stmt.Var;
import dev.muhings.lox.Stmt.While;

class Interpreter implements Expr.Visitor<Object>,
                             Stmt.Visitor<Void> {
  private Environment environment = new Environment();
	
	void interpret(List<Stmt> statements) {
    try {
      for (Stmt statement : statements) {
        execute(statement);
      }
    } catch (RuntimeError error) {
      Lox.runtimeError(error);
    }
  }

	private void execute(Stmt stmt) {
    stmt.accept(this);
  }

	@Override
	public Object visitLiteralExpr(Literal expr) {
		return expr.value;
	}

	@Override
	public Object visitLogicalExpr(Logical expr) {
		Object left = evaluate(expr.left);

    if (expr.operator.type == TokenType.OR) {
      if (isTruthy(left)) return left;
    } else {
      if (!isTruthy(left)) return left;
    }

    return evaluate(expr.right);
	}

	@Override
	public Object visitSetExpr(Set expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitSetExpr'");
	}

	@Override
	public Object visitSuperExpr(Super expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitSuperExpr'");
	}

	@Override
	public Object visitThisExpr(This expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitThisExpr'");
	}

	@Override
	public Object visitUnaryExpr(Unary expr) {
		Object right = evaluate(expr.right);

      return switch (expr.operator.type) {
          case MINUS -> -(double)right;
          case BANG -> ! isTruthy( right );
          default -> null;
      };
      // return null;
	}

	@Override
	public Object visitBinaryExpr(Binary expr) {
		Object left = evaluate(expr.left);
    Object right = evaluate(expr.right); 

    switch (expr.operator.type) {
      case MINUS -> {
          checkNumberOperands(expr.operator, left, right);
          return (double)left - (double)right;
          }
      case SLASH -> {
          checkNumberOperands(expr.operator, left, right);
          return (double)left / (double)right;
          }
      case STAR -> {
          checkNumberOperands(expr.operator, left, right);
          return (double)left * (double)right;
          }
			case PLUS -> { 
                            if (left instanceof Double && right instanceof Double) {
                                return (double)left + (double)right;
                            }
                            
                            if (left instanceof String && right instanceof String) {
                                return (String)left + (String)right;
                            }
                            
                            throw new RuntimeError(expr.operator,
                                    "Operands must be two numbers or two strings.");
          }
			case GREATER -> {
                            checkNumberOperands(expr.operator, left, right);
                            return (double)left > (double)right;
          }
      case GREATER_EQUAL -> {
          checkNumberOperands(expr.operator, left, right);
          return (double)left >= (double)right;
          }
      case LESS -> {
          checkNumberOperands(expr.operator, left, right);
          return (double)left < (double)right;
          }
      case LESS_EQUAL -> {
          checkNumberOperands(expr.operator, left, right);
          return (double)left <= (double)right;
          }
			case BANG_EQUAL -> {
                            return !isEqual(left, right);
          }
      case EQUAL_EQUAL -> {
          return isEqual(left, right);
          }

			default -> {
                            return null;
          }
    }
	}

	@Override
	public Object visitGetExpr(Get expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitGetExpr'");
	}

	@Override
	public Object visitCallExpr(Call expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitCallExpr'");
	}

	@Override
	public Object visitGroupingExpr(Grouping expr) {
		return evaluate(expr.expression);
	}

	@Override
	public Object visitVariableExpr(Variable expr) {
		return environment.get ( expr.name );
	}

	@Override
	public Object visitAssignExpr(Assign expr) {
		Object value = evaluate(expr.value);
    environment.assign(expr.name, value);
    return value;
	}

	private String stringify(Object object) {
    if (object == null) return "nil";

    if (object instanceof Double) {
      String text = object.toString();
      if (text.endsWith(".0")) {
        text = text.substring(0, text.length() - 2);
      }
      return text;
    }

    return object.toString();
  }

	private Object evaluate(Expr expr) {
    return expr.accept(this);
  }

	private  boolean  isTruthy ( Object  object ) {
     if ( object == null ) return  false ;
     if ( object  instanceof  Boolean ) return ( boolean ) object ;
     return  true ; 
  }

	private boolean isEqual(Object a, Object b) {
    if (a == null && b == null) return true;
    if (a == null) return false;

    return a.equals(b);
  }

	private void checkNumberOperands(Token operator, Object left, Object right) {
    if (left instanceof Double && right instanceof Double) return;
    
    throw new RuntimeError(operator, "Operands must be numbers.");
  }

	@Override
	public Void visitExpressionStmt(Expression stmt) {
		 evaluate(stmt.expression);
    return null;
	}

	@Override
	public Void visitFunctionStmt(Function stmt) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitFunctionStmt'");
	}

	@Override
	public Void visitIfStmt(If stmt) {
		if (isTruthy(evaluate(stmt.condition))) {
      execute(stmt.thenBranch);
    } else if (stmt.elseBranch != null) {
      execute(stmt.elseBranch);
    }
    return null;
	}

	@Override
	public Void visitBlockStmt(Block stmt) {
		executeBlock(stmt.statements, new Environment(environment));
    return null;
	}

	void executeBlock(List<Stmt> statements,
                    Environment environment) {
    Environment previous = this.environment;
    try {
      this.environment = environment;

      for (Stmt statement : statements) {
        execute(statement);
      }
    } finally {
      this.environment = previous;
    }
  }

	@Override
	public Void visitClassStmt(Class stmt) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitClassStmt'");
	}

	@Override
	public Void visitPrintStmt(Print stmt) {
		Object value = evaluate(stmt.expression);
    System.out.println(stringify(value));
    return null;
	}

	@Override
	public Void visitReturnStmt(Return stmt) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitReturnStmt'");
	}

	@Override
	public Void visitVarStmt(Var stmt) {
		Object value = null;
    if (stmt.initializer != null) {
      value = evaluate(stmt.initializer);
    }

    environment.define(stmt.name.lexeme, value);
    return null;
	}

	@Override
	public Void visitWhileStmt(While stmt) {
		 while (isTruthy(evaluate(stmt.condition))) {
      execute(stmt.body);
    }
    return null;
	}
}
