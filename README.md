<img src="images/squarebracket.png" alt="Logo" width="50%">
<h1>SQBRA Interpreter <i>(Version 2.2.3 - Rapid Red Panda)</i></h1>
<h2>Interpreter for the SquareBracket Programming Language</h2>
<p>SquareBracket is a <b>simple programming language</b> designed to make the execution of small algorithms and math scripts easy. It has a syntax that is <b>easy to learn</b> and intuitive commands to perform mathematical operations, create variables, and control program flow. It is designed to be extensible and many <b>more functions will be added in future versions</b> to make it more powerful. With its focus on simplicity and ease of use, SquareBracket is a good choice for anyone looking to quickly and easily create and execute mathematical algorithms and scripts. Small bugs may still occur. The interpreter is lightweight and completely <b>implemented in C++</b>.</p>
<h2>Usage</h2>
<p>The SquareBracket Interpreter can be used with the following syntax:</p>
<pre>sqbra.exe [flags] [path to the code file] [input values (separated with spaces)]</pre>

<p>Here's what each part of the syntax means:</p>
<ul>
  <li><code>[flags]</code>: Optional flags that can be used to modify the behavior of the interpreter. See the "Flags" section below for more information.</li>
  <li><code>&lt;path to the code file&gt;</code>: The path to the code file that contains your code.</li>
  <li><code>&lt;input values&gt;</code>: Optional input values that can be passed to the program. These values will be available in the <code>args</code> list when the program runs.</li>
</ul>
<h3>Flags</h3>
<p>The following flags are available:</p>
<ul>
  <li><code>-v</code>: Outputs the current version of the interpreter.</li>
  <li><code>-c</code>: Only checks the code for syntax errors.</li>
  <li><code>-pt</code>: Checks for syntax errors and prints the parsed tree.</li>
  <li><code>-I</code>: Indicates that input values are given and writes them to the <code>args</code> list.</li>
</ul>
<h3>Examples</h3>
<p>Here are some examples of how to use the SquareBracket Interpreter:</p>
<h3>Example 1: Running a SquareBracket program with input values</h3>
<p>If you have a SquareBracket program saved in a file called <code>program.sqbr</code> and you want to pass the input values <code>1.32</code> and <code>54.624</code> to the program, you can use the following command:</p>
<pre>sqbra.exe -I program.sqbr 1.32 54.624</pre>

<p>This will run the <code>program.sqbr</code> file with the input values passed in as arguments.</p>
<h3>Example 2: Checking a SquareBracket program for syntax errors</h3>
<p>If you want to check a SquareBracket program called <code>program.sqbr</code> for syntax errors, you can use the following command:</p>
<pre>sqbra.exe -c program.sqbr</pre>

<p>This will check the <code>program.sqb</code> file for syntax errors and output any errors that are found.</p>
<h3>Example 3: Printing the parsed tree of a SquareBracket program</h3>
<p>If you want to check a SquareBracket program called <code>program.sqbr</code> for syntax errors and print out the parsed tree, you can use the following command:</p>
<pre>sqbra.exe -pt program.sqbr</pre>
<p>This will check the <code>program.sqbr</code> file for syntax errors, print out the parsed tree, and output any errors that are found.</p>
<h2>Syntax of the Language:</h2>
<h3>Variables and Assignments</h3>
<ul>
  <li><b><span style="color:blue">cvar</span> &lt;variable_name&gt; &lt;initial_value&gt;</b>: Create a new variable with the given name and assign it the given initial value.</li>
  <li><b><span style="color:blue">mvar</span> &lt;variable1&gt; &lt;variable2&gt; ... &lt;variableN&gt; &lt;initial_value&gt;</b>: Create multiple variables at once with the same initial value.</li>
  <li><b><span style="color:blue">set</span> &lt;variable_name&gt; (&lt;expression&gt;)</b>: Assign the value of the given expression to the variable with the given name.</li>
   <li><b><span style="color:blue">clist</span> name length</b>: create a list with the given name and length. Example: clist my_list 5.</li>
   <li><b><span style="color:blue">cmat</span> name length1 length2</b>: create a matrix with the given name and size. Example: cmat my_matrix 5 5.</li>
   <li><b><span style="color:blue">ldef</span> name [1.23,1.65,1.87,23.4]</b>: create a predefined list.</li>
   <li><b><span style="color:blue">mdef</span> name [[1,2],[3,4]]</b>: create a predefined matrix.</li>
 </ul>
 <h3>Control Flow Statements</h3>
 <ul>
    <li><b> <span style="color:blue">loop</span> n do [ ... ]</b>: loop n times and execute the code in the indented block. The variable n is decremented each time.</li>
  <li><b><span style="color:blue">sloop</span> n do [ ... ]</b>: same as loop, but changes to the loop variable inside the code block do not affect the loop.</li>
  <li><b> <span style="color:blue">autoloop</span> n expr do [ ... ]</b>: assign variable n the value of expression expr at the beginning and then loop n times and execute the code in the indented block. The variable n is decremented each time.</li>
  <li><b><span style="color:blue">if</span> (statement) [ ... ]</b>: execute the code in the indented block if the statement is true. The statement can use the following operators: = (equal), != (not equal), >= (greater or equal), <= (smaller or equal), > (greater), < (smaller). Example: if (x = 0) [set y 1].</li>
   <li><b><span style="color:blue">elif</span> (statement) [ ... ]</b>: execute the code in the indented block if the previous if statement is false and the current statement is true. The statement can use the following operators: = (equal), != (not equal), >= (greater or equal), <= (smaller or equal), > (greater), < (smaller).</li>
    <li><b><span style="color:blue">else</span> [ ... ]</b>: execute the code in the indented block if the previous if statement is false.</li>
  <li><b><span style="color:blue">while</span> (statement) [ ... ]</b>: execute the code in the indented block as long as the statement is true. The statement has the same syntax as in the if command.</li>
  <li><b><span style="color:blue">funct</span> identifier [ ... ]</b>: declare a void function without return value. The end of the corresponding code must be marked with a closing squarebracket in a single line.</li>
  <li><b><span style="color:blue">call</span> identifier</b>: execute a specific function without return value.</li>
</ul>
<h3>Trigonometric Functions</h3>
<ul>
  <li><b><span style="color:blue">sin</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the sine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">cos</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the cosine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">tan</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the tangent of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">asin</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the inverse sine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">acos</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the inverse cosine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">atan</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the inverse tangent of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">sec</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the secant of the source and assigns the result to the target.</li>
  </ul>
  <h3>I/O Statements</h3>
  <ul>
  <li><b><span style="color:blue">print</span> ["string"]</b>: print a string and break a new line. Example: print ["Hello, world!"].</li>
  <li><b><span style="color:blue">printb</span> ["string"]</b>: same as print, but does not break a new line. Example: printb ["Hello, world!"].</li>
  <li><b><span style="color:blue">printv</span> variable</b>: prints the value of a variable and does not break a new line. Example: printv x.</li>
  <li><b><span style="color:blue">printm</span> matrix</b>: prints the cells of a matrix and does not break a new line. Example: prints mymat.</li>
  <li><b><span style="color:blue">newl</span></b>: break a new line. Example: newl.</li>
  <li><b><span style="color:blue">input</span> variable ["string"]</b>: ask for user input and save it in the variable. The optional string is used as a prompt. Example: input x ["Please enter a number: "].</li>
  <li><b><span style="color:blue">readf</span> listname ["file.csv"]</b>: read a csv-file containing only a list. Example: readf list ["file.csv"]</li>
  <li><b><span style="color:blue">writef</span> listname ["file.csv"]</b>: write a list in a csv file. Example: writef list ["file.csv"]</li>
  </ul>
  <h3>List Operators</h3>
  <ul>
  <li><b><span style="color:blue">push</span> var list</b>: append the value of var at the end of the list.</li>
  <li><b><span style="color:blue">pop</span> var list</b>: remove the last value of the list and save it in var.</li>
  <li><b><span style="color:blue">chsl</span> list length</b>: change the size of an existing list.</li>
  <li><b><span style="color:blue">getl</span> variable name</b>: get the length of a list and save it in the variable. Example: getl len mylist.</li>
  <li><b><span style="color:blue">getdim</span> rows columns name</b>: get the dimensions of a matrix and save them in the variables. Example: getdim x y mymatrix.</li>
  <li><b><span style="color:blue">list</span>[index]</b>: access an element of a list. Example: set my_list[0] 42.</li>
  </ul>
  <h3>Basic Math Functions</h3>
  <ul>
  <li><b><span style="color:blue">floor</span> x</b>: round down the number x. Example: floor var1.</li>
  <li><b><span style="color:blue">ceil</span> x</b>: round up the number x. Example: ceil var1.</li>
  <li><b><span style="color:blue">round</span> x</b> precision: round the number x to the given number of precision digits. Example: round 3.14159 3.</li>
  <li><b><span style="color:blue">xroot</span> target source rt</b>: calculate var^(1/rt). Example: xroot var1 (2+x) 2 calculates the square root of (2+x) and saves the result in var1.</li>
  <li><b><span style="color:blue">random</span> target max min</b>: set target to a random double with interval [max,min]. Example: random x 0 10.</li>
  <li><b><span style="color:blue">log</span> target source exp</b>: calculate the logarithm of source with exponent exp and save the result in target</li>
  </ul>
  <h3>Syntax & Structure</h3>
<p>All commands must be in a single line. Indentation is optional but renders the code more structured and clear.</p></br>
<p><b>Example:</b></p>
<code>funct printHello [
    while (var1 != 0) [
        print ["Hello!"]
        set var1 (var1-2)
    ]
]
</code>
<p>Please take a look at the example programs in the corresponding directory.</p>

<h2>Data Structures:</h2>
<p>In SquareBracket, the only data structures available are doubles, lists and matrices of doubles. You can create a new list with the "clist" command followed by the name and the length of the list. Matrices can be created with the "cmat" command, the name and two expressions for the length in both dimensions. You can access elements of the list by indexing the list with square brackets, for example: "my_list[index]" or matrices with "my_matrix[i1][i2]". You can also use the "getl" command to get the length of a list and assign it to a variable.</p>
  
