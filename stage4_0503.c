#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char note;   
    int octave;  
} Note;

typedef struct {
    Note root;
    Note third;
    Note fifth;
} Chord;

int noteToPitch(char note) {
    switch (toupper(note)) {
        case 'C': return 0;
        case 'D': return 2;
        case 'E': return 4;
        case 'F': return 5;
        case 'G': return 7;
        case 'A': return 9;
        case 'B': return 11;
        default: return -1; 
    }
}

int checkSameOctave(const Chord *chord) {
    const Note *p = &chord->root;
    int octave = p->octave;
    for (int i = 0; i < 3; i++, p++) {
        if (p->octave != octave)
            return 0; 
    }
    return 1;
}

int checkDuplicates(const Chord *chord) {
    const Note *notes[3] = {&chord->root, &chord->third, &chord->fifth};
    for (int i = 0; i < 3; i++) {
        for (int j = i+1; j < 3; j++) {
            if (toupper(notes[i]->note) == toupper(notes[j]->note))
                return 0; 
        }
    }
    return 1;
}

int intervalBetweenNotes(Note n1, Note n2) {
    int pitch1 = noteToPitch(n1.note) + n1.octave * 12;
    int pitch2 = noteToPitch(n2.note) + n2.octave * 12;
    if (pitch1 < 0 || pitch2 < 0)
        return -1; 
    int diff = pitch2 - pitch1;
    if (diff < 0) diff = -diff;
    return diff;
}

void checkHarmony(const Chord *chord) {
    int rootThird = intervalBetweenNotes(chord->root, chord->third);
    int rootFifth = intervalBetweenNotes(chord->root, chord->fifth);

    printf("Harmony Analysis:\n");
    printf("Interval between root and third: %d semitones\n", rootThird);
    printf("Interval between root and fifth: %d semitones\n", rootFifth);

    if ((rootThird == 3 || rootThird == 4) && (rootFifth == 7 || rootFifth == 8)) {
        printf("This is a harmonious chord (minor or major).\n");
    } else if (rootThird > 4 && rootFifth > 8) {
        printf("This chord is wide but may sound dissonant.\n");
    } else {
        printf("This chord may not be harmonious.\n");
    }
}

int readNoteAndOctave(Note *note) {
    char buffer[10];
    printf("Enter note (A-G): ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    if (strlen(buffer) < 2) return 0;
    note->note = toupper(buffer[0]);
    if (note->note < 'A' || note->note > 'G') return 0;

    printf("Enter octave (1-8): ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return 0;
    int oct = atoi(buffer);
    if (oct < 1 || oct > 8) return 0;
    note->octave = oct;
    return 1;
}

void learnMusicTheory() {
    Chord chord;

    printf("Input the root note:\n");
    if (!readNoteAndOctave(&chord.root)) {
        printf("Invalid input for root note.\n");
        return;
    }
    printf("Input the third note:\n");
    if (!readNoteAndOctave(&chord.third)) {
        printf("Invalid input for third note.\n");
        return;
    }
    printf("Input the fifth note:\n");
    if (!readNoteAndOctave(&chord.fifth)) {
        printf("Invalid input for fifth note.\n");
        return;
    }

    printf("\nYour chord:\n");
    printf("Root: %c%d\n", chord.root.note, chord.root.octave);
    printf("Third: %c%d\n", chord.third.note, chord.third.octave);
    printf("Fifth: %c%d\n", chord.fifth.note, chord.fifth.octave);

    if (!checkSameOctave(&chord)) {
        printf("Invalid chord: notes are not in the same octave.\n");
        return;
    }

    if (!checkDuplicates(&chord)) {
        printf("Invalid chord: duplicate notes detected.\n");
        return;
    }

    printf("Chord is valid.\n");

    checkHarmony(&chord);
}
