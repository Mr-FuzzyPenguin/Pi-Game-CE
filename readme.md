# Pi-Memorization Game

### Motivation:
5 years ago during COVID, on March 31st, I made [this post](https://www.cemetech.net/forum/viewtopic.php?t=16673).

Today, with some experience with AVR-Assembly, very terrible (and nearly nonexistent, or purely fabricated) skills in C-programming, a ton of hubris and incompetence, I decided to take upon the challenge that I set off to achieve 5 years ago. This, is the result. The project was made during Spring Break of 2025 with a lot of help from wonderful folks at [Cemetech]("https://www.cemetech.net") and the brilliant documentation for the CE C/C++ toolchain which could be found [here]("https://ce-programming.github.io/toolchain/index.html").

### Goal:
Speed up the game with C to make it magnitudes *faster*, and also store 100k digits instead of the measly (and pathetic) 1k digits that the |L PI list(s) types on the calculator's Ti-BASIC program limits you to storing.

### Known limitations:
Sometimes the Calculator will throw an, "Error: Memory", which states that it has ran out of memory, or does nothave enough memory to continue. The reason why this happens is because, the 50kb size array of `uint8_t` hex values storing each individual digit of Pi in a nibble is... well... there's no easy way to put it... it's absolutely gigantic. So much so, that sometimes the calculator will require a reset. You can do so by hitting the following keys in sequence: [2nd] + [+] + [7] + [1] + [2] and it may resolve the "Error: Memory" issue.

### Dedication:
This program was made for my sister ~~who probably should be paying attention in math class instead of playing this game~~
