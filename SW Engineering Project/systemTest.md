#header
 * Setext Header specification is "Any number of underlining = ’s or - ’s will work."
 * AtxHeader specification is "you may 'close' atx-style headers."

SetextHeader_2=
==
SetextHeader_1=
=
SetextHeader_2-
--
SetextHeader_1-
-

#AtxHeader_1
##AtextHeader_2
###AtextHeader_3
####AtextHeader_4
#####AtextHeader_5
######AtextHeader_6
#AtxHeader_1#
##AtextHeader_2##
###AtextHeader_3###
####AtextHeader_4####
#####AtextHeader_5#####
######AtextHeader_6######

------------------

#hr
 * you may use spaces betseen the hyphens or asterisks.
 * placing three or more hyphens or asterisks.

3*and space
* * *
3*
***
5*
*****
3-and space
- - -

3- -> this can be understand with header. we have to specify this.

---

---------------------
#Block

this is paragraph maybeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
enter
did
not 
act.
we need br tag  we can use this with double space end of statement.  
like  
this  
threespace   
ahahaha

---------------------------------------------------

#Blockquotes
 * use '>'characters for blockquoting.

test case 1 -> all line '>andspace'
> all lines
> are '>_'
> test
> test
> test

test case 2 -> all line just '>'
>all lines
>are '>'
>test
>test
>test

test case 3 -> lazy, first line of paragraph '>_'
> this is lazy test
i'm so lazyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
lazyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
testtttttttttttttttttttttttttttttttttttttttttttttttttttttt
test

> thisis next blockqhotesssssssssssssssssssssssssssssssssss
lazb blockquotessssssssssssssssssssssssssssssssssss
lazy
test

test case 4 -> bloquotes nested adding additional levels of '>_'
> this is first level of quoting.
>
> > this is nested blockquotes.
> > > this is three level of quoting.
>
> back to the first level

test case 5 -> blockquotes nested / lazy can do this? XXXXXXXXXXXXXXXXXXXXXX
> this is first level of quoting.
	> indented quoted block. it can do this>
what level of quoted block?
it have to first level.

test case 6 -> contain other markdown elements. header, list, code blocks.
> ## this is atx header 2
>
> 1. this is first list item.
> 2. this is second list item.
>
> Here's some example code: -> XXXXXXXXXXXXXXXX where is code?
> 	return shell_exec (" echo $input | $markdown_script "); one tab
>     return shell_exec (" echo $input | $markdown_script "); four space.

---------------------------------------------------
# List
 * can use asterisks, pluses, and hyphens.  
 * you can lazy.  
 * If you do use lazy list numbering, however, you should still start the list with the number 1. => lazy should start with 1.
 * At some point in the future, Markdown may support starting ordered lists at an arbitrary number.  => future..?
 * List markers typically start at the left margin, but may be indented by up to three spaces. List markers must be followed by one or more spaces or a tab.  => front : maximum three space, back : one or more space.
 * If list items are separated by blank lines, Markdown will wrap the items in <p> tags in the HTML output

test case 1 -> normal
 * *test
 * b
 * c

 + +test
 + b
 + c

 - -test
 - b
 - c

test case 2 -> two or more space at front
* no
* b
* c

  * two -> indent
  * b
  * c

   + three -> indent
   + b
   + c

    - four -> indent
    - b
    - c

     - five -> more indent
     - b
     - c

 -no back space. XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 -b
 -c

test case 3 -> can mix expression? / how can we specify with emphasis or strong's *.
 * start with *
 + second is +
 - final is -

test case 4 -> ordered.
 1. ordered.
 2. bbbb
 3. cccc

test case 5 -> ugly ordered. // how can we specify first of number and end of number.
 3. start with 3.
 8. second is 8.
 6. end with 10.

 1. all is 1 => continue!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 1. bbbbbbbb
 1. ccccccc

test case 6 -> lazy. //separated by blank lines with p tag.//
 * lazzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzyyyy
 testttttttttttttttttttttttttttttttttttttttttttttttttttt
 lollllllllllllllllllllllllllllllllllllllllllllllllllll
 * second lazyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
 is starteddddddddddddddddddddddddddddddddddddddddddd

 * lazy and intend!!!haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
   aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
   ccccccccccccccccccccccccccccccccccccccccccccccccccccc
  * intend!!!!!!!!!ddddddddddddddddddddddddddddddddddddddddd
    qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq
    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww

test case 7 -> To put a blockquote within a list item, the blockquote’s > delimiters need to be indented
 * a list item with a blockquotes

     > This is a blockquote
     > inside a list item.

test case 8 -> a code block within a list item, the code block needs to be indented twice – 8 spaces or two tabs
 * A list item with a code block:

 		<two tabs>

         <eight spaces>

 10. not escape
 10/. escape

-------------------------------------------------------------
# CodeBlocks.
 *  indent every line of the block by at least 4 spaces or 1 tab. // pre and code tag
 * 4 spaces or 1 tab is removed from each line

test case 1 - normal
This is normal paragraph
	this is a code block. 1 tab.
???how can we divide?
    this is a code block. 4 spaces.

test case 2 - Within a code block, ampersands ( & ) and angle brackets ( < and > ) are automatically converted into HTML entities. => XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    <div class = "hi">
    	&copy; 2016
    </div>

---------------------------------------------------
# Links
 * two style of links: inline and reference.
 * title is option

test cases 1

This is [example](http://www.naver.com/ "NaVeR") inline link.
emmm.... [This](http://www.daum.net/) has no title attribute.

test cases 2
Reference-style link
 * optionally use a space to separate the sets of brackets.
 * link definition three type.',",(.

[naver.yahoo][id] reference-style link.
[google.GGGGG] [id1] reference-style link.
[facebook.333333] [id2] reference-style link.


[id]: http://www.naver.com/ 'Yahoo'
[id1]: http://www.google.com/ (GGGGG)
[id2]: http://www.facebook.com/ "333333"

test cases 3
 * The link URL may, optionally, be surrounded by angle brackets:
 * put the title attribute on the next line and use extra spaces or tabs for padding.

[daum] [id3] reference-style link.
[naver] [id4] reference-style link.

[id3]: <http://www.daum.com/> "d"
[id4]: <http://www.naver.com/>
		 "n"

test cases 4
 * Link definition names may constist of letters, numbers, spaces, and punctuation – but they are not case sensitive. 

[link tExt 1, 2, 3][id3]
[Link Text 3, 2, 1][ID3]
equal.

test case 5
 * The implicit link name shortcut allows you to omit the name of the link, in which case the link text itself is used as the name. Just use an empty set of square brackets. => name and id -> same case.

 [facebook][]

 [facebook]: http://www.facebook.com/

--------------------------------------------
#Emphasis
 * use *and _ // em

test case_1
*single asterisk*
_single underscores_
**double asterisk**
__double underscores__

test case_2 -> space back of this. => XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
* single space *
_ single underscore _
** double space **
__ double underscore __

-------------------------------------------

#Code
 * use _‘_.
 * To include a literal backtick character within a code span, you can backslash escape it.

test case_1 => XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxfail
Hey ' printf() ' like this.

test case_2 -> escape backtick character. -> XXXXXXXXXXXXXXXXXXXXXXXXXX
 ' there is a literal backtick \\' here.'
' there is a literal backtick \\' here.'

test case_3 -> we can use multiple backtick opening and closing delimiter.=> XXXXXXXXXXXXXXXXXXXXXXX
 '' there is a literal backtick ' here.''
'' there is a literal backtick ' here.''

test case_4 -> with in code, &amp; and &lt; or &gt; automatically encoded. =>XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

 ' & and < and > ' 

-------------------------------------------

#Image

![ Alt text ](/ path / to / img . jpg )
![ Alt text ](/ path / to / img . jpg " Optional title ")

Reference-style image

![ Alt text ][id5]

[id5]: url/to/image " Optional title attribute "

---------------------------------------------
#Backslash escapes

\\ backslash
\‘ backtick
\* asterisk
\_ underscore
\{\} curly braces
\[\] square brackets
\(\) parentheses
\# hash mark
\. dot
\! exclamation mark