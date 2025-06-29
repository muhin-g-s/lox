package dev.muhings.lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static dev.muhings.lox.TokenType.AND; 
import static dev.muhings.lox.TokenType.BANG;
import static dev.muhings.lox.TokenType.BANG_EQUAL;
import static dev.muhings.lox.TokenType.CLASS;
import static dev.muhings.lox.TokenType.COMMA;
import static dev.muhings.lox.TokenType.DOT;
import static dev.muhings.lox.TokenType.ELSE;
import static dev.muhings.lox.TokenType.EOF;
import static dev.muhings.lox.TokenType.EQUAL;
import static dev.muhings.lox.TokenType.EQUAL_EQUAL;
import static dev.muhings.lox.TokenType.FALSE;
import static dev.muhings.lox.TokenType.FOR;
import static dev.muhings.lox.TokenType.FUN;
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
import static dev.muhings.lox.TokenType.OR;
import static dev.muhings.lox.TokenType.PLUS;
import static dev.muhings.lox.TokenType.PRINT;
import static dev.muhings.lox.TokenType.RETURN;
import static dev.muhings.lox.TokenType.RIGHT_BRACE;
import static dev.muhings.lox.TokenType.RIGHT_PAREN;
import static dev.muhings.lox.TokenType.SEMICOLON;
import static dev.muhings.lox.TokenType.SLASH;
import static dev.muhings.lox.TokenType.STAR;
import static dev.muhings.lox.TokenType.STRING;
import static dev.muhings.lox.TokenType.SUPER;
import static dev.muhings.lox.TokenType.THIS;
import static dev.muhings.lox.TokenType.TRUE;
import static dev.muhings.lox.TokenType.VAR;
import static dev.muhings.lox.TokenType.WHILE;


class Scanner {
  private final String source;
  private final List<Token> tokens = new ArrayList<>();
	private int start = 0;
  private int current = 0;
  private int line = 1;

	private static final Map<String, TokenType> keywords;

  static {
    keywords = new HashMap<>();
    keywords.put("and",    AND);
    keywords.put("class",  CLASS);
    keywords.put("else",   ELSE);
    keywords.put("false",  FALSE);
    keywords.put("for",    FOR);
    keywords.put("fun",    FUN);
    keywords.put("if",     IF);
    keywords.put("nil",    NIL);
    keywords.put("or",     OR);
    keywords.put("print",  PRINT);
    keywords.put("return", RETURN);
    keywords.put("super",  SUPER);
    keywords.put("this",   THIS);
    keywords.put("true",   TRUE);
    keywords.put("var",    VAR);
    keywords.put("while",  WHILE);
  }

  Scanner(String source) {
    this.source = source;
  }

	List<Token> scanTokens() {
    while (!isAtEnd()) {
      start = current;
      scanToken();
    }

    tokens.add(new Token(EOF, "", null, line));
    return tokens;
  }

	private boolean isAtEnd() {
    return current >= source.length();
  }

	private void scanToken() {
    char c = advance();
    switch (c) {
			case '(' -> addToken(LEFT_PAREN);
			case ')' -> addToken(RIGHT_PAREN);
			case '{' -> addToken(LEFT_BRACE);
			case '}' -> addToken(RIGHT_BRACE);
			case ',' -> addToken(COMMA);
			case '.' -> addToken(DOT);
			case '-' -> addToken(MINUS);
			case '+' -> addToken(PLUS);
			case ';' -> addToken(SEMICOLON);
			case '*' -> addToken(STAR);

			case '!' -> addToken(match('=') ? BANG_EQUAL : BANG);
			case '=' -> addToken(match('=') ? EQUAL_EQUAL : EQUAL);
			case '<' -> addToken(match('=') ? LESS_EQUAL : LESS);
			case '>' -> addToken(match('=') ? GREATER_EQUAL : GREATER);

			case '/' -> {
				if (match('/')) {
						while (peek() != '\n' && !isAtEnd()) advance();
				} else {
						addToken(SLASH);
				}
			}

			case '"' -> string();

			case ' ', '\r', '\t' -> {}

			case '\n' -> line++;

   		default -> {
				if (isDigit(c)) {
					number();
				} else if (isAlpha(c)) {
					identifier();
				} else {
					Lox.error(line, "Unexpected character.");
				}
			}
		}
  }

	private boolean isDigit(char c) {
    return c >= '0' && c <= '9';
  } 

	private void number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();

      while (isDigit(peek())) advance();
    }

    CharSequence numberText = source.subSequence(start, current);
		double value = Double.parseDouble(numberText.toString());
		addToken(NUMBER, value);
  }

	private void identifier() {
    while (isAlphaNumeric(peek()))
      advance();

    String text = source.substring(start, current);
    TokenType type = keywords.get(text);

		if (type == null)
      type = IDENTIFIER;
    addToken(type);
  }


	private boolean isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
  }

  private boolean isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
  }

	private void string() {
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') line++;
      advance();
    }

    if (isAtEnd()) {
      Lox.error(line, "Unterminated string.");
      return;
    }

    advance();

    String value = source.substring(start + 1, current - 1);
    addToken(STRING, value);
  }

	private char peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source.charAt(current + 1);
  } 

	private boolean match(char expected) {
    if (isAtEnd()) return false;
    if (source.charAt(current) != expected) return false;

    current++;
    return true;
  }

	private char peek() {
    if (isAtEnd()) return '\0';
    return source.charAt(current);
  }

	private char advance() {
    return source.charAt(current++);
  }

  private void addToken(TokenType type) {
    addToken(type, null);
  }

  private void addToken(TokenType type, Object literal) {
    String text = source.substring(start, current);
    tokens.add(new Token(type, text, literal, line));
  }
}
