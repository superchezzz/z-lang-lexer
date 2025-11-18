# Z Lang: No Cap, Just Code ⚡

## 🧢 What is Z Lang?

**Z Language** is a programming language made for the younger generation of programmers. It re-imagines the foundations of C through the lens of contemporary digital culture, slang, and memes.

Our motto, **"Code With Vibes,"** embodies the idea that programming doesn't have to be rigid or intimidating. Z Lang makes coding approachable, expressive, and culturally relevant for students who grew up in the era of social media and digital self-expression.

### 🎯 Target Audience
Aspiring Gen Z programmers who are tech-savvy but want a learning environment that is interactive, visually appealing, and relatable.

---

## 🚀 Key Features

### 1. Gen Z Syntax (No Cap)
We've rebranded technical terminology with a unique flavor:
* **`SPILL`** instead of `printf` (Output)
* **`GRAB`** instead of `scanf` (Input)
* **`NUMBS`** instead of `int`
* **`TEXT`** instead of `string`
* **`CYCLE`** instead of `while`

### 2. EZPRINT (Type-Agnostic Output)
Say goodbye to complex format specifiers like `%d` or `%s`. Z Lang automatically detects data types.

```c
// No format specifiers needed!
NUMBS HighScore = 420;
TEXT PlayerName = "GenZCoder";
SPILL("Player: ", PlayerName, " Score: ", HighScore);
```

### 3. CHAIN-Z (Fluent Syntax)
Perform multiple consecutive operations with a single function call to reduce redundancy.

```c
// Stacking calls for a clean flow
SPILL("This")("is")("CHAIN-Z!");
```
### **4. ZAVED (Reusable Blocks)**

A simplified way to save and reuse blocks of code without complex function declarations.

```c
ZAVED GreetUser {
    SPILL("Hi Z Coder! Welcome back!");
}

// Call it anywhere
GreetUser();
```
## 🔤 Token Support

The lexer supports the full Z Lang character set, including:

### **Keywords**
`IF`, `ELSE`, `FOR`, `WHILE` (mapped to `CYCLE`), `DO`

### **Operators**
Arithmetic, Assignment, Comparison, Logical, and Unary

### **Literals**
- Integers (`NUMBS`)
- Floating-point (`DECI`)
- Strings (`TEXT`)
- Characters (`EMOJI`)

### **Comments**
- Single-line: `//`
- Multi-line: `/* ... */`


## 🛠️ Technical Details

This repository contains the **Lexical Analyzer (Scanner)** for Z Lang, built from scratch in C.
- **Language:** C  
- **Input:** `.zlang` source files  


## 💻 How to Run

### **1. Compile the Lexer**

```bash
gcc z-lexer.c -o z-lexer
```

### **2. Run with a .zlang File**
Create a file named test.zlang and run:
```terminal
./z-lexer test.zlang
```
