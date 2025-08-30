package dev.muhings.lox;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Stack;

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

class Resolver implements Expr.Visitor<Void>, Stmt.Visitor<Void> {
  private final Interpreter interpreter;
	private final Stack<Map<String, Boolean>> scopes = new Stack<>();
	private FunctionType currentFunction = FunctionType.NONE;

	private enum FunctionType {
    NONE,
    FUNCTION
  }

  Resolver(Interpreter interpreter) {
    this.interpreter = interpreter;
  }

	@Override
	public Void visitExpressionStmt(Expression stmt) {
		resolve(stmt.expression);
    return null;
	}

	@Override
	public Void visitFunctionStmt(Function stmt) {
		declare(stmt.name);
    define(stmt.name);

    resolveFunction(stmt, FunctionType.FUNCTION);
    return null;
	}

	@Override
	public Void visitIfStmt(If stmt) {
		resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != null) resolve(stmt.elseBranch);
    return null;
	}

	@Override
	public Void visitBlockStmt(Block stmt) {
		beginScope();
    resolve(stmt.statements);
    endScope();
    return null;
	}

	@Override
	public Void visitClassStmt(Class stmt) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitClassStmt'");
	}

	@Override
	public Void visitPrintStmt(Print stmt) {
		resolve(stmt.expression);
    return null;
	}

	@Override
	public Void visitReturnStmt(Return stmt) {
		if (currentFunction == FunctionType.NONE) {
      Lox.error(stmt.keyword, "Can't return from top-level code.");
    }

		if (stmt.value != null) {
      resolve(stmt.value);
    }

    return null;
	}

	@Override
	public Void visitVarStmt(Var stmt) {
		declare(stmt.name);
    if (stmt.initializer != null) {
      resolve(stmt.initializer);
    }
    define(stmt.name);
    return null;
	}

	@Override
	public Void visitWhileStmt(While stmt) {
		resolve(stmt.condition);
    resolve(stmt.body);
    return null;
	}

	@Override
	public Void visitLiteralExpr(Literal expr) {
		return null;
	}

	@Override
	public Void visitLogicalExpr(Logical expr) {
		resolve(expr.left);
    resolve(expr.right);
    return null;
	}

	@Override
	public Void visitSetExpr(Set expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitSetExpr'");
	}

	@Override
	public Void visitSuperExpr(Super expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitSuperExpr'");
	}

	@Override
	public Void visitThisExpr(This expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitThisExpr'");
	}

	@Override
	public Void visitUnaryExpr(Unary expr) {
		resolve(expr.right);
    return null;
	}

	@Override
	public Void visitBinaryExpr(Binary expr) {
		resolve(expr.left);
    resolve(expr.right);
    return null;
	}

	@Override
	public Void visitGetExpr(Get expr) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException("Unimplemented method 'visitGetExpr'");
	}

	@Override
	public Void visitCallExpr(Call expr) {
		resolve(expr.callee);

    for (Expr argument : expr.arguments) {
      resolve(argument);
    }

    return null;
	}

	@Override
	public Void visitGroupingExpr(Grouping expr) {
		resolve(expr.expression);
    return null;
	}

	@Override
	public Void visitVariableExpr(Variable expr) {
		if (!scopes.isEmpty() &&   
				Objects
					.equals(
						scopes.peek().get(expr.name.lexeme), 
						Boolean.FALSE
					)
				) {
      Lox.error(expr.name,
          "Can't read local variable in its own initializer.");
    }

    resolveLocal(expr, expr.name);
    return null;
	}

	@Override
	public Void visitAssignExpr(Assign expr) {
		resolve(expr.value);
    resolveLocal(expr, expr.name);
    return null;
	}

	void resolve(List<Stmt> statements) {
    for (Stmt statement : statements) {
      resolve(statement);
    }
  }

	private void resolve(Stmt stmt) {
    stmt.accept(this);
  }

	private void resolve(Expr expr) {
		expr.accept(this);
  }

	private void beginScope() {
    scopes.push(new HashMap<>());
  }

	private void endScope() {
    scopes.pop();
  }

	private void declare(Token name) {
    if (scopes.isEmpty()) return;

    Map<String, Boolean> scope = scopes.peek();
    scope.put(name.lexeme, false);
  }

	private void define(Token name) {
    if (scopes.isEmpty()) return;
    scopes.peek().put(name.lexeme, true);
  }

	private void resolveLocal(Expr expr, Token name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
      if (scopes.get(i).containsKey(name.lexeme)) {
        interpreter.resolve(expr, scopes.size() - 1 - i);
        return;
      }
    }
  }

	private void resolveFunction(Stmt.Function function, FunctionType type) {
		FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : function.params) {
      declare(param);
      define(param);
    }
    resolve(function.body);
    endScope();

		currentFunction = enclosingFunction;
  }
}
