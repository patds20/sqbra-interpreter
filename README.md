<img src="images/squarebracket.png" alt="Logo" width="50%">
<h1>SQBRA Interpreter <i>(Version 1.3.1)</i></h1>
<h2>Interpreter for the SquareBracket Programming Language</h2>
<p>SquareBracket is a <b>simple programming language</b> designed to make the execution of small algorithms and math scripts easy. It has a syntax that is <b>easy to learn</b> and uses intuitive commands to perform mathematical operations, create variables, and control program flow. It is designed to be extensible and many <b>more functions will be added in future versions</b> to make it more powerful. With its focus on simplicity and ease of use, SquareBracket is a good choice for anyone looking to quickly and easily create and execute mathematical algorithms and scripts. Small bugs may still occur. The interpreter is lightweight and completely <b>implemented in C++</b>.</p>
</br>
<h3>Command:</h3>
<ul>
  <li><b><span style="color:blue">cvar</span> &lt;variable_name&gt; &lt;initial_value&gt;</b>: Creates a new variable with the given name and assigns it the given initial value.</li>
  <li><b><span style="color:blue">mvar</span> &lt;variable1&gt; &lt;variable2&gt; ... &lt;variableN&gt; &lt;initial_value&gt;</b>: Creates multiple variables at once with the same initial value.</li>
  <li><b><span style="color:blue">set</span> &lt;variable_name&gt; (&lt;expression&gt;)</b>: Assigns the value of the given expression to the variable with the given name.</li>
  <li><b><span style="color:blue">if</span> (&lt;statement&gt;) [</b><i>code block</i><b>]</b>: Executes the code block if the given statement is true.</li>
  <li><b><span style="color:blue">while</span> (&lt;statement&gt;) [</b><i>code block</i><b>]</b>: Repeatedly executes the code block as long as the given statement is true.</li>
  <li><b><span style="color:blue">loop</span> &lt;variable_name&gt; &lt;iterations&gt; do [</b><i>code block</i><b>]</b>: Repeatedly executes the code block the specified number of times, decrementing the given variable each time.</li>
  <li><b><span style="color:blue">sloop</span> &lt;variable_name&gt; &lt;iterations&gt; do [</b><i>code block</i><b>]</b>: Same as <span style="color:blue">loop</span>, but changes to the variable inside the code block do not affect the loop.</li>
  <li><b><span style="color:blue">sin</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the sine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">cos</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the cosine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">tan</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the tangent of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">asin</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the inverse sine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">acos</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the inverse cosine of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">atan</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the inverse tangent of the source and assigns the result to the target.</li>
  <li><b><span style="color:blue">sec</span> &lt;target&gt; &lt;source&gt;</b>: Calculates the secans of the source and assigns the result to the target.</li>
  </ul>
