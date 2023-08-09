# Secure-Delete

Simple deletion script for files and directories with three passes in accordance with most security standards.  

Pass 1 rewrites with 0s.  
Pass 2 rewrites with binary 1s.  
Pass 3 rewrites with random characters.   

Verification occurs after each pass.  

## Installation
`
git clone https://github.com/JoshCap20/Secure-Delete.git && cd Secure-Delete && gcc -o erase secure_delete.c
`


## Usage
`./erase <path-to-file-or-directory>`

* Will only work from directory with erase, see suggestion for global usage.

## Suggestion
Create an alias in your shell configuration file to 'erase' and then just use `erase <relative-path-to-file-or-directory>`
* For ZSH shells, add `alias erase="ABSOLUTE_FILE_PATH/erase"` to ~/.zshrc
* For Bash shells, add above to ~/.bashrc

* Not sure what shell? Run `echo $0` or `echo $SHELL`

## Military and Security Standards Met:
- DoD 5220.22-M
- NIST SP 800-88 (Clear method)
- NCSC-TG-025 (Technically since 0's complement is 1)
- Air Force System Security Instruction 5020
- Aperiodic random overwrite/Random

(Note: Always validate with the respective standards documentation to ensure compliance for your specific needs.)

## Potential Standards to Meet with Tweaks:
- BSI
- HMG Infosec Standard 5 (First write 1s then 0s)
