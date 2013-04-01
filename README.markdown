#Reading and Writing Electronic Text: Examples in C

I teach a class at [ITP](http://itp.nyu.edu/) called [Reading and Writing
Electronic Text](http://rwet.decontextualize.com). It's an introductory
programming class, taught in Python, that focuses on text analysis and creative
text generation. (Python example code for the class can be found
[here](http://github.com/aparrish/rwet-examples).) I'm trying to teach myself
C, and in pursuit of that goal, I've undertaken the task of translating the
example code from RWET into C. Those examples can be found in this repository.

#Contents

Here's a list of what's included so far. Check the source code for more
information on what these programs do and how to run them. Many of these
programs are direct translations of the Python examples; others are slight
elaborations/generalizations.

* `cat`: a simple clone of UNIX `cat`.
* `concordance`: counts the frequency of each unique word in a given input.
* `count_word_pairs`: counts the frequency of each unique word pair in a given input.
* `forfinder`: given a string on the command line, prints all lines matching that string in the input from the beginning of that string until the end of the line.
* `markov`: a Markov chain text generator
* `ngram_count`: a more generalized version of `count_word_pairs`. Counts unique n-grams of a given order.
* `randlines`: reads in lines from standard input, then prints them back in random order.
* `randomize_words`: splits each line from standard input into words, then prints those words out in random order.
* `ut_randlines`: a version of `randlines` that uses `ut_array`s instead of a home-grown linked list.

# Caveats

* This is purely for my own education, and I make no guarantees about the
safety or functionality of the resulting code. (I would love to get feedback if
you notice I've done something horribly wrong.)

* I'm using Troy Hanson's excellent
[uthash](http://troydhanson.github.com/uthash/) for arrays and hashes.

* I'm using plain old C string functions, so it works with Unicode only on
accident.

* My Makefile is pretty dumb, because I am still learning how to do make files.

* All code herein is released under the MIT license (see `LICENSE` in the
repository), unless otherwise specified. (Especially uthash, which I've
included in this repository for ease of use, but which obviously was not
created by me!)

#Compiling

Download the code and type `make all`. The code was developed with gcc on OSX,
but afaik it should compile pretty much anywhere.

