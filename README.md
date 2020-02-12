# iona - easy to use interpreted programming language

## Content

* [About the project](#about-the-project)
* [Syntax preview](#syntax-preview)
* [Visual Studio Code extension](#visual-studio-code-extension)
* [What is supported](#what-is-supported)
  * [built-in data types](#built-in-data-types)
  * [string interpolation](#string-interpolation)
  * [if](#if)
  * [for each](#for-each)
  * [while and do while](#while-and-do-while)
  * [internal functions](#internal-functions)
  * [custom functions](#custom-functions)
    * [function return](#function-return)
  * [internal variables](#internal-variables)
  * [increment and decrement operator](#increment-and-decrement-operator)
  * [compound assignment](#compound-assignment)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Helpful links](#helpful-links)

## About the project

I wanted to learn more about how programming languages actually work and create something at least a little bit useful after understanding the basic concept behind it.

So iona is written in c++17 and uses smart pointers (because I haven't used them a lot) for heap objects.
The architecture/handling is very basic:
- Source file reading
- Lexing into tokens
- Parsing and creating an abstract syntax tree
- Interpreting ("executing") the AST with the visitor pattern

There is a "compiler" cmake project, you might think if I am crazy to build a compiler?
No, thats really too much work and a hell more difficult. I have thought that, if iona supports multiple source files, it would be really annoying to run/distribute the programs.
So I want to try an idea where I "compile" all source files into a single container like format which the iona interpreter can then execute all together.
In the end you would only have one file to run a program with the interpreter.

Another idea is to have a CLI utility program to create projects from templates (eg. for visual studio code) 
and provide a way to automatically re-run the program if the source file changes.

## Syntax preview

The variable and function naming is inspired by C#. Variable types are dynamically assigned during runtime.
There are global variables, local variables, variable assignments and custom functions with parameters.
String interpolation with variables and arrays of built-in types are also supported just like simple if statements.

```javascript
var helloWorld = "Hello, World!"
var arrayOfInts = [2, 4, 54]
var arrayOfFloats = [2.3, 4.5, 43.0]
var value = 5
var myBool = false

func Main()
{
    WriteLine("helloWorld = {helloWorld}")

    WriteLine(arrayOfInts[2])

    if value == 5
        SecondHelloWorld(value)

    if myBool
    {
        SecondHelloWorld(100)
    }
    else
    {
        WriteLine("myBool is false")
    }

    for x in Range(5)
        value++
}

func SecondHelloWorld(x)
{
    WriteLine("Hello, World! x = {x}")
}
```

## Visual Studio Code extension

There is an official vs code extension available [here](https://github.com/zixoan/vscode-iona-lang) to add syntax highlighting and code snippets for iona.

## What is supported

### built-in data types

Int, Float, String, Bool

### string interpolation

You can use string interpolation to output a formatted string more readable.

```javascript
var x = 0
var y = 5

func Main()
{
    WriteLine("x={x}, y={y}")
    // Outputs: x=0, y=5
}
```

> Currently there are only simple variables usages supported, but in the future full expressions will be supported.
> For example: WriteLine("5 + 5 = {5 + 5}") will then output "5 + 5 = 10"

### if

```javascript
var myBool = false
var value = 100

func Main()
{
    if myBool
    {
        WriteLine("myBool is true")
    }
    else
    {
        WriteLine("myBool is false")
    }

    if value == 100
    {
        WriteLine("value is equal to 100")
    }

    // Outputs:
    // myBool is false
    // value is equal to 100
}
```

### for each

```javascript
var array = [0, 1, 2]

func Main()
{
    for num in array
        WriteLine("Number = {num}")
    
    // Outputs:
    // Number = 0
    // Number = 1
    // Number = 2

    for i in Range(3)
        WriteLine("i = {i}")

    // Outputs:
    // i = 0
    // i = 1
    // i = 2
}
```

### while and do while

```javascript
var n = 5

func Main()
{
    while n > 0
    {
        WriteLine(n)

        n--
    }
    
    // Outputs:
    // 5
    // 4
    // 3
    // 2
    // 1

    do
    {
        WriteLine(n)
        n--
    }
    while n > 0

    // Outputs:
    // 0
}
```

### internal functions

- WriteLine(string|bool|int|float|array) : void
  - Outputs the data as a line to the standard output
  - WriteLine("Hello, World!")
- ReadLine() : string
  - Reads a line from the standard input
  - var input = ReadLine()
- ReadInt() : string
  - Reads an int from the standard input
  - var number = ReadInt()
- ReadFloat() : string
  - Reads an float from the standard input
  - var floatingPointNum = ReadFloat()
- Size(string|array) : int
  - Returns the size of a given string or number of elements in an array
  - var size = Size("Hello")
- Empty(string|array) : bool
  - Returns true if the string or array size is zero, otherwise false
- Random(int lowerBound, int upperBound) : int
  - Returns an int between the lower and upper bound (both inclusive)
  - var rand = Random(0, 3)
- ToLowerCase(string) : string
  - Returns the lower case version of the input string
- ToUpperCase(string) : string
  - Returns the upper case version of the input string
- StartsWith(string value, string prefix) : bool
  - Returns true if the value string starts with the prefix string (case sensitive).
- EndsWith(string value, string suffix) : bool
  - Returns true if the value string ends with the suffix string (case sensitive).
- Range(int upperBound) : int_array
  - Returns an int array from zero to the given upper bound
- FileExists(string path) : bool
  - Returns true if the given file/directory exists, otherwise false
- FileRead(string path) : string
  - Returns the complete content of the file from the given path. If the path is a directory an empty string is returned.
- FileWrite(string path, string data) : bool
  - Writes the data to the file path and returns true if successfull, otherwise false

### custom functions

You can write your own functions with parameters and call them from eg. Main:

```javascript
func Main()
{
    MyFunc(100, 4)
    // Outputs: x=100, y=4
}

func MyFunc(x, y)
{
    WriteLine("x={x}, y={y}")
}
```

> The parameter declaration will probably change to remove eg. the var keyword.

#### function return

Custom functions can also have a simple return statement.

```javascript
func Main()
{
    WriteLine(Add(100, 4))
    // Outputs: 104
}

func Add(a, b)
{
    // Some other logic..    

    return a + b
}
```

### internal variables

The following internal variables are pre declared:
   - PI (3.14159)
   - INT_MIN (-2147483648)
   - INT_MAX (2147483647)
   - FLOAT_MIN (1.17549e-38)
   - FLOAT_MAX (3.40282e+38)

They can be used just like normal variables:

```javascript
func Main()
{
    WriteLine(PI)
    // Outputs: 3.14159
}
```

### increment and decrement operator

The short increment and decrement operator is also supported.

```javascript
var x = 0
var y = 0.0

func Main()
{
    x++
    y++
    WriteLine("x={x} y={y}")
    // Outputs: x=1 y=1.0

    x--
    y--
    // Outputs: x=0 y=0.0

    WriteLine(x++)
    // Outputs: 1
}
```

### compound assignment

Compound assignment is supported with the operator +,-,* and /.

```javascript
var x = 2
var y = 5.0

func Main()
{
    x *= 2
    WriteLine(x)
    // Outputs: 4

    y += 2.5
    WriteLine(x)
    // Outputs: 7.5
}
```

### Usage

TODO (provide binary downloads for the iona interpreter)

TODO for future (provide an online interpreter as a playground)

### Roadmap

- [X] array index variable assignment
- [X] improved and standardized error messages (with file name etc.)
  - [X] All parser messages
  - [X] All interpreter messages
- [X] more boolean operators
- [X] more binary operator handling
- [X] if-else statements
  - [ ] complex if-else-if statements
- [X] while loop
- [X] do while loop
- [ ] for i
- [X] one line statement blocks without curly brackets
- [X] simple return statement (not nested)
- [ ] a lot more internal functions (Size/Length, ReadFile, Min, Max, Random, ToUpperCase, ToLowerCase, ...)
  - [X] Size
  - [X] Empty
  - [X] ReadInt
  - [X] ReadFloat
  - [X] Random
  - [ ] Min
  - [ ] Max
  - [ ] Reverse
  - [X] Range
  - [X] ToUpperCase
  - [X] ToLowerCase
  - [X] EndsWith
  - [X] StartsWith
  - [ ] Contains
  - [ ] Split
  - [ ] Trim
  - [X] FileExists
  - [X] FileRead
  - [X] FileWrite
  - [ ] FileReadLines
  - [ ] FileWriteLines
  - [ ] FileCopy
  - [ ] FileList
  - [ ] ToString
  - [ ] ToInt/IsInt
  - [ ] ToFloat/IsFloat
  - [ ] ToBool/IsBool
- [X] internal pre-defined variables
- [ ] switch statements
- [ ] expressions inside an interpolated string
- [X] CLI utility to create projects and watch code
  - [X] create projects from templates (eg. for visual studio code with tasks)
    - iona new project-name
  - [ ] watch directory/source file for changes and re-run program automatically
    - iona watch
- [X] Visual Studio Code extensions
  - [X] Core language syntax highlighting etc.
  - [X] Code snippets
- [ ] Tests

### Contributing

Any contibutions are greatly appreciated. 
Just fork the project, create a new feature branch, commit and push your changes and open a pull request.

### License

Distributed under the GNU General Public License v3.0. See LICENSE for more information.

### Helpful links

Here is a list of useful links if you want to learn more about interpreted programming languages like I did:

- [Let's Build A Simple Interpreter (by Ruslan's Blog)](https://ruslanspivak.com/lsbasi-part1/)
- [Crafting Interpreters](https://craftinginterpreters.com/)