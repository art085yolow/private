# private

linki : 
- https://www.3blue1brown.com/topics/neural-networks
- http://yann.lecun.com/exdb/mnist/


# Attempt 1
Description:
Class construction based on lists of pointers and code functions corresponding to data.
Results:
Large memory leak, misinterpretation of operation and handling of pointers.

# Attempt 2
Description:
Attempt to put code and functions in one class.
Results:
One big mess. Which very quickly causes problems and is incomprehensible.

# Attempt 3
Description:
Use of standard libraries, MAP, PAIR and TUPLE, to simplify the amount of code. Limited use of pointers or replacement by reference.
