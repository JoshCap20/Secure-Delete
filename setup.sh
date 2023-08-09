
# Compile program
gcc secure_delete.c -o erase

if [ -n "$ZSH_VERSION" ]; then
    # Zsh
    echo "alias erase='$PWD/erase'" >> ~/.zshrc
    source ~/.zshrc
elif [ -n "$BASH_VERSION" ]; then
    # Bash
    echo "alias erase='$PWD/erase'" >> ~/.bashrc
    source ~/.bashrc
else 
    echo "Unknown shell, you will need to manually add the alias to your shell's rc file."
fi
