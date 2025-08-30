package dev.muhings.lox;

import java.util.ArrayList;
import java.util.List;

import static dev.muhings.lox.TokenType.BANG;
import static dev.muhings.lox.TokenType.BANG_EQUAL;
import static dev.muhings.lox.TokenType.ELSE;
import static dev.muhings.lox.TokenType.EOF;
import static dev.muhings.lox.TokenType.EQUAL;
import static dev.muhings.lox.TokenType.EQUAL_EQUAL;
import static dev.muhings.lox.TokenType.FALSE;
import static dev.muhings.lox.TokenType.GREATER;
import static dev.muhings.lox.TokenType.GREATER_EQUAL;
import static dev.muhings.lox.TokenType.IDENTIFIER;
import static dev.muhings.lox.TokenType.IF;
import static dev.muhings.lox.TokenType.LEFT_BRACE;
import static dev.muhings.lox.TokenType.LEFT_PAREN;
import static dev.muhings.lox.TokenType.LESS;
import static dev.muhings.lox.TokenType.LESS_EQUAL;
import static dev.muhings.lox.TokenType.MINUS;
import static dev.muhings.lox.TokenType.NIL;
import static dev.muhings.lox.TokenType.NUMBER;
import static dev.muhings.lox.TokenType.PLUS;
import static dev.muhings.lox.TokenType.PRINT;
import static dev.muhings.lox.TokenType.RIGHT_BRACE;
import static dev.muhings.lox.TokenType.RIGHT_PAREN;
import static dev.muhings.lox.TokenType.SEMICOLON;
import static dev.muhings.lox.TokenType.SLASH;
import static dev.muhings.lox.TokenType.STAR;
import static dev.muhings.lox.TokenType.STRING;
import static dev.muhings.lox.TokenType.TRUE;
import static dev.muhings.lox.TokenType.VAR;

/* Lox GRAMMAR (lowest to highest precedence)
--------------------------------------------------------------
program        → declaration* EOF 
declaration    → classDecl | funcDecl | varDecl | statement
classDecl      → "class" IDENTIFIER ( "<" IDENTIFIER )? "{" function* "}"
funDecl        → "fun" function
function       → IDENTIFIER "(" parameters? ")" block
parameters     → IDENTIFIER ( "," IDENTIFIER )*
varDecl        → "var" IDENTIFIER ( "=" expression )? ";"
statement      → exprStmt | forStmt | ifStmt | printStmt | returnStmt | whileStmt | block
returnStmt     → "return" expression? ";"
exprStmt       → expression ";"?
forStmt        → "for" "(" ( varDecl | exprStmt | ";" ) expression? ";" expression? ")" statement 
ifStmt         → "if" "(" expression ")" statement ( "else" statement )?
printStmt      → "print" expression ";"? 
whileStmt      → "while" "(" expression ")" statement
block          → "{" declaration* "}"
expression     → assignment
assignment     → ( call "." )? IDENTIFIER "=" assignment | logic_or
logic_or       → logic_and ( "or" logic_and )*
logic_and      → equality ( "and" equality )*
equality       → comparison ( ( "!=" | "==" ) comparison )* 
comparison     → shift ( ( ">" | ">=" | "<" | "<=" ) shift )* 
shift          → term ( ( ">>" | "<<" | ">>>" ) term )*
term           → factor ( ( "-" | "+" ) factor )* 
factor         → unary ( ( "/" | "*" ) unary )* 
unary          → ( "!" | "-" ) unary | call 
call           → primary ( "(" arguments? ")" | "." IDENTIFIER )*
arguments      → expression ( "," expression )*
primary        → "true" | "false" | "nil" | "this" | NUMBER | STRING | IDENTIFIER | "(" expression ")" | "super" "." IDENTIFIER
*/

public class Parser {
	 private static class ParseError extends RuntimeException {}

	private final List<Token> tokens;
	private int current = 0;

	Parser(List<Token> tokens) {
		this.tokens = tokens;
	}

	List<Stmt> parse() {
    List<Stmt> statements = new ArrayList<>();
    while (!isAtEnd()) {
      statements.add(declaration());
    }

    return statements; 
  }

	private Stmt declaration() {
    try {
      if (match(VAR)) return varDeclaration();

      return statement();
    } catch (ParseError error) {
      synchronize();
      return null;
    }
  }

	private Stmt varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");

    Expr initializer = null;
    if (match(EQUAL)) {
      initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Stmt.Var(name, initializer);
  }

	private Stmt statement() {
    if (match(PRINT)) return printStatement();
		if (match(LEFT_BRACE)) return new Stmt.Block(block());
		if (match(IF)) return ifStatement();

    return expressionStatement();
  }

	private List<Stmt> block() {
    List<Stmt> statements = new ArrayList<>();

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      statements.add(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
  }

	private Stmt ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition."); 

    Stmt thenBranch = statement();
    Stmt elseBranch = null;
    if (match(ELSE)) {
      elseBranch = statement();
    }

    return new Stmt.If(condition, thenBranch, elseBranch);
  }

	private Stmt printStatement() {
    Expr value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Stmt.Print(value);
  }

	private Stmt expressionStatement() {
    Expr expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return new Stmt.Expression(expr);
  }

	private Expr expression(){
		return assignment();
	}

	private Expr assignment() {
    Expr expr = equality();

    if (match(EQUAL)) {
      Token equals = previous();
      Expr value = assignment();

      if (expr instanceof Expr.Variable variable) {
        Token name = variable.name;
        return new Expr.Assign(name, value);
      }

      error(equals, "Invalid assignment target."); 
    }

    return expr;
  }

	private Expr equality(){
		Expr expr = comparison();

		while (match(BANG_EQUAL, EQUAL_EQUAL)) {
      Token operator = previous();
      Expr right = comparison();
      expr = new Expr.Binary(expr, operator, right);
    }

    return expr;
	}

	private Expr comparison() {
    Expr expr = term();

    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
      Token operator = previous();
      Expr right = term();
      expr = new Expr.Binary(expr, operator, right);
    }

    return expr;
  }

	private Expr term() {
    Expr expr = factor();

    while (match(MINUS, PLUS)) {
      Token operator = previous();
      Expr right = factor();
      expr = new Expr.Binary(expr, operator, right);
    }

    return expr;
  }

	private Expr factor() {
    Expr expr = unary();

    while (match(SLASH, STAR)) {
      Token operator = previous();
      Expr right = unary();
      expr = new Expr.Binary(expr, operator, right);
    }

    return expr;
  }

	private Expr unary() {
    if (match(BANG, MINUS)) {
      Token operator = previous();
      Expr right = unary();
      return new Expr.Unary(operator, right);
    }

    return primary();
  }

	private Expr primary() {
    if (match(FALSE)) return new Expr.Literal(false);
    if (match(TRUE)) return new Expr.Literal(true);
    if (match(NIL)) return new Expr.Literal(null);

    if (match(NUMBER, STRING)) {
      return new Expr.Literal(previous().literal);
    }

		if (match(IDENTIFIER)) {
      return new Expr.Variable(previous());
    }

    if (match(LEFT_PAREN)) {
      Expr expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return new Expr.Grouping(expr);
    }

		throw error(peek(), "Expect expression.");
  }

	private Token consume(TokenType type, String message) {
    if (check(type)) return advance();

    throw error(peek(), message);
  }

	private boolean match(TokenType... types) {
		for (TokenType type : types) {
			 if (check(type)) {
        advance();
        return true;
      }
		}

		return false;
	}

	private boolean check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
  }

	private Token advance() {
    if (!isAtEnd()) current++;
    return previous();
  }

	private boolean isAtEnd() {
    return peek().type == EOF;
  }

  private Token peek() {
    return tokens.get(current);
  }

  private Token previous() {
    return tokens.get(current - 1);
  }

	private ParseError error(Token token, String message) {
    Lox.error(token, message);
    return new ParseError();
  }

	private  void  synchronize () {
     advance (); while (! isAtEnd ()) {
       if ( previous (). type == SEMICOLON ) return ; switch ( peek (). type ) {
         case CLASS :
         case FUN :
         case VAR :
         case FOR :
         case IF :
         case WHILE :
         case PRINT :
         case RETURN :
           return ; 
      } advance (); 
    } 
  }
}
