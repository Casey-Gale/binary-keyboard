// c code for funny keyboard
// uses variables as a stand in for io ports

// This program is firmware developed for a personal USB keyboard project. The keyboard only contains 9 keys--
// currently, these are 7 Binary keys, a Shift key, and a "begin/end typing" key (now referred to as the Type key).
// To type a character on this keyboard, one must begin by pressing the Type key. Next, the user may press the
// remaining 8 keys to select a desired keystroke. Each of the 8 keys (7 Binaries and 1 Shift) correspond to a
// single binary digit, and the combined byte of digits corresponds to a unique keystroke. As a hypothetical
// example, 0b0000 0001 might correspond to typing the letter 'a', 0b0000 0010 might correspond to the letter 'b',
// and so on. Bit 7 of this byte is the shift key, which augments the keystroke in the same manner as a
// conventional keyboard. Building on the previous example, 0b1000 0001 would correspond to typing the letter 'A', etc.
// Once the desired combination has been input, the user must press the Type key once more to transmit the letter to the computer.

// A keystroke is sent only for an instant. (By this, I mean each keystroke "pressed" scancode is immediately followed by the corresponding keystroke "released" scancode. In the future, this may be lengthened depending on related bugs.

// Data is initially stored in an array on the stack, which contains the string data necessary to transmit to the host computer to register a keystroke ("scancode"). The array is one dimensional, with the index directly corresponding to the input binary code from the keys. On startup, the information from this array is transferred to a dynamic binary search tree on the heap. Due to the strict nature of organization by binary string, the tree will be incredibly fast to traverse (a maximum of 8 iterations is required to find any element)

// In this prototype program, I have no access to microcontroller ports and the like. As such, ports will be represented by internal variables. Ports will have their variable names begin with a capital letter, whereas internal variables will have their names in lowercase.

#include <stdio.h>

// MAX_SCANCODE_LEN is 16 chars for now. this is an assumption that any scancode will not be greater than 16 chars long,
// which is very safe. Additionally, I dont think this does anything right now.
#define MAX_SCANCODE_LEN 16


// sorry god for what is about to follow
// an array of 256 strings
// each entry will either be null terminated or NULL
char* table[256] = {
	"\x1c", // enter
	NULL, // a
	NULL, // b  
	NULL, // c  
	NULL, // d
	NULL, // e
	NULL, // f
	NULL, // g
	NULL, // h
	NULL, // i
	NULL, // j
	NULL, // k
	NULL, // l
	NULL, // m
	NULL, // n
	NULL, // o
	NULL, // p
	NULL, // q
	NULL, // r
	NULL, // s
	NULL, // t
	NULL, // u
	NULL, // v
	NULL, // w
	NULL, // x
	NULL, // y
	NULL, // z
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL, //
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
    
};

// capitalized variables are inputs, lowercase variables are internal variables

char Key_pressed = 0,
char key_light = 0;
int Type = 0;
int last_type = 0;
int do_type = 0;
char* no_key = "temp_code";
char* keystroke_buffer = no_key;
char key_byte = 0;
char last_read = 0;
char key_light = 0;
char toggle_bits = 0;

// Key_pressed is an input which shows if a given key is pressed. if the nth bit (from the right) is 1, then then nth key is pressed
// key_light is an output which triggers an LED backlight.

// each node of the BST contains the keystroke typed on the keyboard, and a corresponding scancode represented by a c string.
// BST is organized by bit. given an n-deep level of BST, the left child will have a 0 in the n-th bit and the right a 1 in the n-th bit.
struct node {
	char keystroke; // binary combo typed on keyboard
	char* scancode; // scancode transmitted to computer
	struct node* left; // left node
	struct node* right; // right node
};

// dynamically allocates and returns pointer to new node
struct node* init_node(char new_keystroke, char* new_scancode, struct node* new_left, struct node* new_right) {
	struct node* new_node = malloc(sizeof(struct node));
	if(new_node == NULL)
    	return NULL;
	new_node->keystroke = new_keystroke;
	new_node->scancode = new_scancode;
	new_node->left = new_left;
	new_node->right = new_right;
	return new_node;
}

struct node* finish_node(struct node* current, char n_keystroke, char* n_scancode) {
	current->keystroke = n_keystroke;
	current->scancode = n_scancode;
	return current;
}

// init head
struct node* head = init_node('\0', NULL, NULL, NULL);

// recursive function for creating tree. initally call with depth == 0
struct node* construct_bst(int depth) {
	if(depth == 8)
    	return NULL;
	return init_node('\0', NULL, construct_bst(depth+1), construct_bst(depth+1));
}

// searches BST to find the node corresponding to the given keystroke
// new version of function allows traversal purely by sorting assumption as opposed to reading node values
char* find_node(char keystroke) {
	struct node* current = head;
	char it_keystroke = 0;
	int depth = 0;
	while(1) {
    	if(keystroke >> depth == 0) // if keystroke is nonzero after right shifting depth digits, increment depth
        	break;
    	depth++;
	}
	if(depth >= 8)
    	// panic
    
	int n = 0;
	for(n = 0; n <= depth; n++) {
    	// if the n-th digit is a 1, go right. else, go left
    	if(keystroke & (0b00000001 << n)) {
        	current = current->right;
    	}
    	else {
        	current = current->left;
    	}
   	 
    	n++;
	}
	return current;
}

// populates the tree from hardcoded table
struct node* populate_bst() {
	head = construct_bst(0); // recursively generate the full tree with no values. always begin this call with parameter 0.
	char key;
	for(key = 0; key < 256; key++) {
    	struct node* temp = find_node(key); // a little inefficient because we possibly traverse entire tree for each character
                                        	// however, we only do this on startup. after tree is populated, search will be fast
    	finish_node(temp, key, table[key]); // we can index table directly with key because keystrokes are
                                        	// ascending from 0 - 255, like the table index
	}
}

void set_flag(int& flag) {flag = 1;}
void clear_flag(int& flag) {flag = 0;}



/* -----------------------------------------------------------
// interrupt service routine:
// send keystroke_buffer over USB, then
keystroke_buffer = no_key; // clear buffer
// then return to where interrupt was called
// ---------------------------------------------------------*/

// using atmega32u4
int main() {
    
	populate_bst(); // allocate and populate binary search tree using global variable 'head'
    
	// initialize timer/counter to interrupt every 1ms    
    
	// send keyboard confirmation over USB
    
	// enable interrupts

	// typing loop
	while(1) {
    	// 2 states:
    	// 	1. !do_type (not typing anything)
    	// 	2. do_type (typing a keystroke)
    	// toggle states with rising edge of Type
    	do_type = do_type^(!last_type&Type);
    	// only toggle do_type if last_type is low (meaning Type was low last cycle)
    	// and Type is high (meaning Type is high now) i.e. rising edge
    	last_type = Type; // after logic with last_type, update last_type.
   	 
    	if(do_type) {
        	while(do_type) {
            	key_byte = Key_pressed;
            	// the following logic determines if a key has been newly pressed, versus simply held down. nothing should happen from being held down.
            	// truth table:
            	// last_read | 	key_byte | 	toggle_bits
            	// 0        	0        	0
            	// 0        	1        	1
            	// 1        	0        	0
            	// 1        	1        	0
            	// Binary equation: toggle_bits = (!last_read) * key_byte
            	toggle_bits = ~last_read&key_byte; // for all bits, only set if last_read is low and key_byte is high
            	last_read = key_byte; // after logic with last_read, update last_read.
            	key_light = toggle_bits^key_light; // XOR key_light by toggle_bits to toggle all keys which have been pressed
           	 
            	do_type = do_type^(!last_type&Type); // once again toggle do_type based on rising edge input from Type
            	last_type = Type; // after logic with last_type, update last_type.
           	 
        	}
        	struct node* desired_node = find_node(key_light); // look up key_light in BST
        	keystroke_buffer = desired_node->scancode; // point keystroke_buffer to scancode of corresponding node
        	key_light = 0; // clear register
        	last_read = 0; // clear register
        	key_byte = 0; // clear register
       	 
    	}
	}
}
