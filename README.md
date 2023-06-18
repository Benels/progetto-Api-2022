# Final Project For 'Algorithms and Data Structures' (API) 2021-2022 at Polimi


The Goal of the project was to create a game in which the player can guess a word contained in a given vocabulary.
The player knows the lenght of the word that he has to guess and has a limited number of attempts. 
In each attempt the player provides a word and the game gives the player some hints about the similarities between the word he provided and the main word to be guessed: for each letter, the game prints '+' if the letter is in the right position, '|' if the letter is present in the word but is in the wrong position, and '/' if the letter isn't contained in the word. The hints also respect the number of occurrencies of a letter, printing '|' for the first occurrencies of that letter in the wrong positions, but then '/' if the occurrencies of that letter in the word given by the player are more that of the hidden word.

The project is fully implemented in C language, using linked lists and red & black trees as data structures.

Finale Grade: 30 Cum Laude
