# Proc-Filesystem-Project
Filesystem project for Systems Programming & Operating Systems




# Compile and Run

In order to execute the program, you must call the ./MYps program, along with and of the following tags: -p, -U, -v, -c, -s, -S. Depending on what tags are passed/not passed, it will accordingly print out information. If you do end up calling the -p tag, you must pass ONLY 1 pid with the -p tag for the tag to work properly, and the -p tag must be the first tag passed in outside of the executable call. 















# Design Section

# Main function:
The main() is used to parse through the command line by using the getopt() function, and storing in a tags struct, whether the corresponding tag was passed or not (specify 0 or 1). After this, I initialize a char ** ids, which holds a pointers to all the pid's I will be working with in the program. If the pid was specified in the command line, I call the is_valid_pid(). Otherwise we call the getallpids() function. After when either of these functions are done, we iteratively call the move on to the printouts() function for each pid pointer in char ** ids, which is the last thing in our whole program






# is_valid_pid() 
will pass in the pointer to that pid. The function checks if the passed in pid actually exists, or not, and will exit out of the program if it doesn't exist. If it does exist, then it will simply add a pointer to the pid to char **ids, then exit out of the function once checking, and continue back to the main function and execute the printouts() function






# getallpids():
This function is called in when there is no -p tag passed in the command line, so the main purpose of this function is to parse through /proc and identify all pid directories that exist. I accomplish this by taking the first char of each file/directory in /proc, and converting it to it's ASCII value, and seeing if it's in the ASCII value range for numbers 1-9. If it's in that range, then that means the corresponding directory is a pid directory, then I call the uid_match() function from there





# uidmatch(): 
This function is called when we are parsing through /proc, and we find a pid directory. In this function, we start parsing further inside by going into the /proc/pid/status pathfile, and with the help of strtok and ASCII, we are able to extract the uid of our pid. Then we take the program uid, the specific processes's uid, the pid, and our char ** ids, to pass all the information to the comparestrings() function





# comparestrings():
The function is simply used to parse through the program's uid and processes uid, to see if they end up matching or not. If it's a match, then we append the char * of the pid to char ** ids. Otherwise, we simply exit the function and continue finding the next pid directory in the getallpid() function, and start the whole process over again







# printouts(): 
This function is executed for the end of the program when we have all the pid's we're working with in char **ids,and we're passing in each pid from char ** ids to this function. We also pass in the tags struct. We first start by initializing a new struct (struct info), where for each pid, we can store all the possible information for printing out. Then regardless of the tags, we extract all the information and store it in this struct, by calling the getstatinfo(), statminfo(), and cmdline() functions. After we call these functions and extract the info into the struct, we run through a bunch of conditionals to see what tags are 0/1 in the struct tags, and accordingly print out stuff from struct info.







# getstatinfo():
This function is just to parse through the /proc/pid/stat, to extract information that can be printed out. I also pass in a number, that can either be 1,2, or 3. That is because 3 of the tag information is actually found in this stat file for each pid. So instead of making 3 different functions for each tag, I pass in a number to see accordingly what info from stat I'm trying to pass in. If the number is 1, then I need the 2nd item from stat, if it's 2, then I need the 13th item, and 14th item if it's a 3. I extract these items by storing the whole file content in a char * with getline(), and tokenize the whole pointer by " ", so then I can easily iterate through each item.







# statminfo(): 
This function extracts the info needed from the statm file from proc/pid. Since in our scenario we could only possibly need 1 thing from statm and that's the first item, I store the whole file content in a char *, then tokenize it " ", then just simply return the token, since the first token is the first content of the file that is needed





# cmdline():
This function just parses through /proc/pid/cmdline, so we can get the cmdline of the pid directory. A catch with this function, is that if there is no cmdline, then it will just be a null terminated string. So by looking at external resources (source link shared within function), I used the getline(), and created a size_t characters, which returns the number of chars that are read from the getline() function. Since the minimum number it can be is 2 ("\0\0"), if it's equal to 2, then that means we return " ". Else it means that there is a cmdline and so then we simply return the cmdline







# initial_print_formatting(): 
This function is simply ran once throughout the whole entire program. It serves to print out an initial heading of what information is being printed out of each process(pid, vmem, cmdline, e.t.c), by compaing the struct tags and seeing what is 0/1. It helps to see what each column represents



















# Testing Plan

When it came to testing, I really just tested the code per function that I created. I knew from the beginning that there are multiple aspects in this program, and so breaking up this huge project into smaller tasks (functions) would help out a lot in ensuring I can test each function and ensure that it works before adding more and more components to it. Regarding coding bugs, I wouldn't say there any major bugs that I encounted with this lab. I had a few instances when I forgot to free a memory allocated varaible, which resulted in errors, and also the whole null terminating string aspect of the cmdline was a little tricky at first to approach. But after testing that function continously and researching, I was able to figure out how to bypass that issue.
