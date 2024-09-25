// This program is firmware developed for a personal USB keyboard project. The keyboard only contains 9 keys--
// currently, these are 7 Binary keys, a Shift key, and a "begin/end typing" key (now referred to as the Type key).
// To type a character on this keyboard, one must begin by pressing the Type key. Next, the user may press the
// remaining 8 keys to select a desired keystroke. Each of the 8 keys (7 Binaries and 1 Shift) correspond to a
// single binary digit, and the combined byte of digits corresponds to a unique keystroke. As a hypothetical
// example, 0b0000 0001 might correspond to typing the letter 'a', 0b0000 0010 might correspond to the letter 'b',
// and so on. Bit 7 of this byte is the shift key, which augments the keystroke in the same manner as a
// conventional keyboard. Building on the previous example, 0b1000 0001 would correspond to typing the letter 'A', etc.
// Once the desired combination has been input, the user must press the Type key once more to transmit the letter to the computer.
