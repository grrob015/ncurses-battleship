# ncurses-battleship
A battleship game using the ncurses library.

<h2>Environments and Technologies Used</h2>

- KDE Plasma (Desktop environment for Linux, X11 and Wayland compatible)
- GCC, the GNU Compiler Collection
- Konsole (Graphical Terminal Command Shell)
- nucurses, a programming C library used for making text-based UIs in terminals
- vim, a free and open-source text editor that comes with Linux
<!-- vimtutor is your best friend -->

<h2>Operating Systems Used </h2>

- Arch Linux (Kernel Version 6.12.1)

<h2> Purpose of the project </h2>

This project was meant to practice writing functions and using the ncurses library in the C programming language. 

If I was to make changes and improve this program now, (aside from troubleshooting the hard mode of the game), I would try to make the menus even more visual and intuitive for the player to navigate, and add a two palyer mode where only one board displays the visible ships at a time so you could play with a friend instead of only against an AI.

From a coding perspective, my use of variables (especially in the hard mode section of the `generate_ai_shots` function is sloppy, and probably contributed to the complexity and confusion of things. From further studies, it would behoove me to define more things locally to reuse names like "i" for iterating for loops, rather than limiting myself to a collection of variables that I need to shuffle around to fit the purposes of the algorithm at any given stage.

<h2> Video demonstration of compiling and running </h2>
<a href="http://www.youtube.com/watch?feature=player_embedded&v=2C_AqzyvAhM
" target="_blank"><img src="http://img.youtube.com/vi/2C_AqzyvAhM/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>
