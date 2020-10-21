# C# Interactive bash-like Shell


## Architecture and Program Design

## Shell Scripting

### Environment variables

- [ ] Add functionality to create, edit and remove environment variables, and use them in commands.
  - [ ] Create environment variables via assignment (e.g. a=1)
  - [ ] Use variables with $ escaping
  - [ ] List shell-local variables and environment variables with `set`
  - [ ] List environment variables with `env`

---

## Basic bash commands

### [echo - display a line of text](https://linux.die.net/man/1/echo "man echo")

#### `echo` (No args)

- [ ] Print nothing

#### `echo [string]`

- [ ] Print out the given string

#### `echo "[string]"`

- [ ] Print the string without the quotes

#### `echo $VAR`

- [ ] Correctly print the value of VAR, or print nothing if VAR is not defined.

#### `echo "$VAR"`

- [ ] Correctly print the value of VAR without quotes, or print nothing if VAR is not defined.

#### `echo [string]$VAR`

- [ ] Correctly substitute the value of the VAR variable into the string and print it out on the screen.

#### `echo "[string]$VAR"`

- [ ] Correctly substitute the value of the VAR variable into the string and print it out on the screen, without quotes.

### [cd - change the working directory](https://man7.org/linux/man-pages/man1/cd.1p.html "man cd")

#### `cd` (No args)

- [ ] Change to the home directory

#### `cd [directory]`

- [x] Change to the given directory
- [x] Fail and give error message if directory doesn't exist

#### `cd -`

- [ ] Change to the previous directory, and also print it on the screen.

---

### [cat - concatenate files and print on the standard output](https://man7.org/linux/man-pages/man1/cat.1.html "man cat")

#### `cat` (No args)

- [x] Read from stdin (as with bash)

#### `cat` [file]

- [x] Print contents of file.
- [x] Show error message if file doesn't exist

#### `cat` [directory]

- [x] Show error message saying that cat doesn't work on directories.

---

### [ls - List directory contents](https://man7.org/linux/man-pages/man1/ls.1.html "man ls")

#### `ls` (No args)

- [ ] Print all files and folders in current directory (i.e. exactly like ls in bash).

#### `ls [directory]`

- [ ] Print all files and folders in the given directory.
- [ ] Give error if directory doesn't exist.

---

### [pwd - print name of current/working directory](https://linux.die.net/man/1/pwd "man pwd")

#### `pwd` (No args)

- [x] Print name of current working directory.
