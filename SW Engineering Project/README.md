**Markdown**
=======================
Markdown is intended to be as easy-to-read and easy-to-write as is feasible.  
It helps you to make html file using your '.md'file.  
It also show your file with different style that you choose.  

**[Execution]**
-------------------------

To use this program, download all the '.java' files and compile.bat file and execute compile.bat.  
Then, using console, get to the directory and type in
 > $java md2html [options]

since main() method is in md2html.java. Descriptions about the possible options are specified below.


**[options]**
-----------------------

 * Options :
 > **--help**: displays this information  
   
 > **(necessary) -i [md_file]+**: specifies [md_file] to read  
 .md extension is needed in the file name  
   
 > **(optional) -o [html_file]+**: places the output as [html_file]   
 default setting is [input_file_name.html]  
 .html extension is needed in the file name  
   
 > **(optional) -style [plain|fancy|slide]+**: sets HTML style as one of plan/fancy/slide  
 default setting is plain  
 
 Multiple input files can be processed at once.  
 Different output file name ane style can be assigned for each input file,  
 but there should be no duplicates in the output file names.  
 The number of output files and styles cannot exceed that of input files.  
 
 
**[specification]**
------------------------

### **--help** 
 * This command displays the help message that describes the command options.
 * no matter where --help is in the command line arguments, the help message is displayed.

 * Usage
 > $java md2html --help


### **-i <md_file>**
 * This commands reads <md_file> which needs to be converted to html.  
 * The file name has to finish with '.md'.  
 the program does not run without this option specified.  
 
 * Usage
 > $java md2html -i a.md


### **-o <html_file>**
 * This command specifies the output file names as <html_file>.
 * Default output file name is the same with that of input file with different extension.
 * The file name has to finish with '.html'.

 * Usage
 > $java md2html -o a.html
    
    
### **-style [plain|fancy|slide]**
 * This command sets the style of the output html file as one of plain, fancy, or slide.
 * Default style is 'plain'.
 
 * Usage
 > $java md2html -style fancy
