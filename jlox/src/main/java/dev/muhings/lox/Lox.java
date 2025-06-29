package dev.muhings.lox;

import java.io.IOException;

public class Lox {
	 public static void main(String[] args) throws IOException {
    if (args.length > 1) {
      System.out.println("Usage: jlox [script]");
      System.exit(64); 
    } else if (args.length == 1) {
      // runFile(args[0]);
			System.out.println("Not yet implemented - runFile");
    } else {
			// runPrompt();
      System.out.println("Not yet implemented - runPrompt");
    }
  }
}
