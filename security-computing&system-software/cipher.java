//Encrypting a plaintext file using the Vigenere cipher

import java.util.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
public class pa01 {

	public static void main(String[] args) {
	
	File file1 = new File(args[0]);
    File file2 = new File(args[1]);
    String key = "";
    String plain = "";
		
   // open the key file
		try {
			BufferedReader br = new BufferedReader(new FileReader(file1));
       String line = null;
        while ((line = br.readLine()) != null) {
            //System.out.println(line);
            key = key.concat(line);
        }
			br.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
   
   // open the plain-text file
   try {
			BufferedReader br = new BufferedReader(new FileReader(file2));
      String line = null;
        while ((line = br.readLine()) != null) {
           // System.out.println(line);
             plain = plain.concat(line);
        }
			br.close();
		}catch (Exception e) {
			e.printStackTrace();
		}
   
   
   		key = format(key);
		plain = format(plain);
		
		// pad with x if plain is too short
		if(plain.length() <512) {
			int numX = 512 - plain.length();
			for(int i=0; i<numX; i++) plain = plain.concat("x");
		}
		
		char[] keyA = key.toCharArray();
		char[] plainA = plain.toCharArray();
		
		// print out the key
		int count =1;
		System.out.print("\n\nVigenere Key:\n\n");
		for(int i=0; i<keyA.length;i++) {
			System.out.print(keyA[i]);
			if(count%80 == 0 && count!=0) System.out.println();
			count++;
		}
		
		// print out the plain-text
		count = 1;
		System.out.print("\n\n\nPlaintext:\n\n");
		for(int i=0; i<512;i++) {
			System.out.print(plainA[i]);
			if(count%80 == 0 && count!=0) System.out.println();
			count++;
		}
		
		char[] cipher = vigenere(keyA, plainA);
		
		// print out the cipher text
		count =1;
		System.out.print("\n\n\nCiphertext:\n\n");
		for(int i=0; i<512;i++) {
			System.out.print(cipher[i]);
			if(count%80 == 0 && count!=0) System.out.println();
			count++;
		}
     System.out.println();
					
		
	}
			
      // return a string of only lower case letters included in the passed string
	public static String format(String line) {
		String a = "abcdefghijklmnopqrstuvwxyz";
		
		String ret = "";
		char[] lineArray = line.toCharArray();
		for(int i =0; i< lineArray.length; i++) {
			
			// exclude any numbers
			if(Character.isDigit(lineArray[i])) continue;
			
			// add letter, change capital to lower case
			if(Character.isLetter(lineArray[i])) {
				if(lineArray[i] - 0 >= 65 && lineArray[i] - 0 <= 90) {
					int index = lineArray[i] - 'A';
					String add = Character.toString(a.charAt(index));
					ret = ret.concat(add);
					continue;
				}
				String add = Character.toString(lineArray[i]);
				ret = ret.concat(add);
			}	
		}
		return ret;
	}
	
	
	// perform the encryption using vigenere cipher
	public static char[] vigenere(char[] key, char[] plain) {
		char[] c = new char[512];
 
		for(int i=0;i<plain.length && i < 512;i++) {
			int index = i % key.length;
			c[i] = newLet(plain[i],key[index]);
		}
		return c;
	}
	
	// find what letter should be added
	public static char newLet(char p, char k) {
		String a = "abcdefghijklmnopqrstuvwxyz";
		int index = k - 'a';
		int increment = p -'a';		
		index = (index + increment) %26;
		return a.charAt(index);
	}
}
