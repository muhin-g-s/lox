package dev.muhings.lox;

import java.util.List;

import static dev.muhings.lox.TokenType.BANG;
import static dev.muhings.lox.TokenType.BANG_EQUAL;
import static dev.muhings.lox.TokenType.CLASS;
import static dev.muhings.lox.TokenType.EOF;
import static dev.muhings.lox.TokenType.EQUAL_EQUAL;
import static dev.muhings.lox.TokenType.FALSE;
import static dev.muhings.lox.TokenType.FOR;
import static dev.muhings.lox.TokenType.FUN;
import static dev.muhings.lox.TokenType.GREATER;
import static dev.muhings.lox.TokenType.GREATER_EQUAL;
import static dev.muhings.lox.TokenType.IF;
import static dev.muhings.lox.TokenType.LEFT_PAREN;
import static dev.muhings.lox.TokenType.LESS;
import static dev.muhings.lox.TokenType.LESS_EQUAL;
import static dev.muhings.lox.TokenType.MINUS;
import static dev.muhings.lox.TokenType.NIL;
import static dev.muhings.lox.TokenType.NUMBER;
import static dev.muhings.lox.TokenType.PLUS;
import static dev.muhings.lox.TokenType.PRINT;
import static dev.muhings.lox.TokenType.RETURN;
import static dev.muhings.lox.TokenType.RIGHT_PAREN;
import static dev.muhings.lox.TokenType.SEMICOLON;
import static dev.muhings.lox.TokenType.SLASH;
import static dev.muhings.lox.TokenType.STAR;
import static dev.muhings.lox.TokenType.STRING;
import static dev.muhings.lox.TokenType.TRUE;
import static dev.muhings.lox.TokenType.VAR;
import static dev.muhings.lox.TokenType.WHILE;

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

	Expr parse() {
    try {
      return expression();
    } catch (ParseError error) {
      return null;
    }
	}

	private Expr expression(){
		return equality();
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
