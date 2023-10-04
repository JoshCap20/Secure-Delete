# Secure-Delete

Simple deletion script for files and directories with three passes in accordance with most security standards.  

Pass 1 rewrites with 0s.  
Pass 2 rewrites with binary 1s.  
Pass 3 rewrites with random characters.   

Verification occurs after each pass.  

## Installation
`
git clone https://github.com/JoshCap20/Secure-Delete.git && cd Secure-Delete && source setup.sh
`
* This will add an alias for `erase` to the shell's global scope.


## Usage
`erase <path-to-file-or-directory>`
