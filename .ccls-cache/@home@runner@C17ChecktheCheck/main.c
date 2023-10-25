#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/errno.h>
#include <ctype.h>

// Declare variables
FILE *gInputFile;
const int gChessboardSize = 8;
char gChessboard [gChessboardSize][gChessboardSize];
int gKingHorizontalPosition = 0;
int gKingVerticalPosition = 0;

// Declare Functions
void usage (char *);
void initializeChessboard (void);
void findKing (int*, int*);
void rotateKing (void);
bool offBoardLower (int);
bool offBoardUpper (int);
bool kingPawn (int, int);
bool knight (int, int);
bool rookBishopQueen (int, int);
bool inCheck (void);
bool checkWrapper (char*, int);
void mirrorH (void);
void mirrorV (void);
void rotateBoard (void);
void swapCaps (void);
void swapColours (void);
bool checkTerminationCondition (void);
void printChessboard (void);

int main(int argc, char *argv[]) {

  // File Initialization
  if (argc != 2) usage (argv[0]);

  if (NULL == (gInputFile = fopen(argv[1], "r"))) {
    fprintf (stderr, "inputFile: %s: %s\n", argv[1], strerror(errno));
    exit (EXIT_FAILURE);
  }
  else {
//    fprintf (stderr, "%s opened for reading.\n" , argv[1]);
  }
    

  int gameNum = 1;
  while (true) {
      
    initializeChessboard (); 

    if (checkTerminationCondition())
      break;

    bool black = checkWrapper ("BLACK", gameNum);
    bool white = checkWrapper ("WHITE", gameNum);
    if (!black && !white) {
      checkWrapper ("NO",gameNum);
    }
    
    
  gameNum++;
  }



  // Close File
  fclose (gInputFile);
  
  return 0;
}

// Define Functions
void usage (char *cmd) {
  fprintf (stderr, "usage: %s inputFileName\n", cmd);
  exit (EXIT_SUCCESS);
}

void initializeChessboard (void) {
  for (int i = 0; i < gChessboardSize; i++) { // Height/Y
    for (int j = 0; j <= gChessboardSize; j++) { // Width/X '\n' => +1
      char c = fgetc(gInputFile);
      if (c != '\n') {
        gChessboard[i][j] = c;
      }
      else {
        // Do nothing
      }
    }
  }
  fgetc (gInputFile); // Strip '\n'
  findKing ( &gKingHorizontalPosition, &gKingVerticalPosition);
}

void findKing (int* h, int* v) {
  for (int i = 0; i < gChessboardSize; i++) { // Height/Y
    for (int j = 0; j < gChessboardSize; j++) { // Width/X
      if (gChessboard[i][j] == 'K') {
        *h = i;
        *v = j;
      }
    }
  }
}

bool offBoardLower (int p) {
  if (p < 0) {
    return true;
  }
  else {
    return false;
  }
}

bool offBoardUpper (int p) {
  if (p >= gChessboardSize) {
    return true;
  }
  else {
    return false;
  }
}

bool kingPawn (int Horizontal, int Vertical) {
  // Diagonals
  if (!offBoardLower(Vertical - 1) && !offBoardLower(Horizontal - 1)) {
    if (gChessboard[Vertical - 1][Horizontal - 1] == 'K') {
      return true;
    }
  }
  if (!offBoardLower(Vertical - 1) && !offBoardUpper(Horizontal + 1)) {
    if (gChessboard[Vertical - 1][Horizontal + 1] == 'K' || gChessboard[Vertical - 1][Horizontal + 1] == 'P') {
      return true;
    }
  }
  if (!offBoardUpper(Vertical + 1) && !offBoardLower(Horizontal - 1)) {
    if (gChessboard[Vertical + 1][Horizontal - 1] == 'K') {
      return true;
    }
  }
  if (!offBoardUpper(Vertical + 1) && !offBoardUpper(Horizontal + 1)) {
    if (gChessboard[Vertical + 1][Horizontal + 1] == 'K' || gChessboard[Vertical + 1][Horizontal + 1] == 'P') {
      return true;
    }
  }

  // Orthogonals
  if (!offBoardLower(Vertical)) {
    if (gChessboard[Vertical - 1][Horizontal] == 'K') {
      return true;
    }
  }
  if (!offBoardUpper(Vertical)) {
    if (gChessboard[Vertical + 1][Horizontal] == 'K') {
      return true;
    }
  }
  if (!offBoardLower(Horizontal)) {
    if (gChessboard[Vertical][Horizontal - 1] == 'K') {
      return true;
    }
  }
  if (!offBoardUpper(Horizontal)) {
    if (gChessboard[Vertical][Horizontal + 1] == 'K') {
      return true;
    }
  }
  
  return false;
}

bool knight (int Horizontal, int Vertical) {
  // Top right (1)
  if (!offBoardLower(Vertical - 2) && !offBoardLower(Horizontal + 1)) {
    if (gChessboard[Vertical - 2][Horizontal + 1] == 'N') {
      return true;
    }
  }
  // Top right (2)
  else if (!offBoardLower(Vertical - 1) && !offBoardLower(Horizontal + 2)) {
    if (gChessboard[Vertical - 1][Horizontal + 2] == 'N') {
      return true;
    }
  }
  // Bottom right (1)
  else if (!offBoardLower(Vertical - 1) && !offBoardLower(Horizontal + 2)) {
    if (gChessboard[Vertical + 1][Horizontal + 2] == 'N') {
      return true;
    }
  }
  // Bottom right (2)
  else if (!offBoardLower(Vertical + 2) && !offBoardLower(Horizontal + 1)) {
    if (gChessboard[Vertical + 2][Horizontal + 1] == 'N') {
      return true;
    }
  }
  // Bottom left (1)
  else if (!offBoardLower(Vertical + 2) && !offBoardLower(Horizontal - 1)) {
    if (gChessboard[Vertical + 2][Horizontal - 1] == 'N') {
      return true;
    }
  }
  // Bottom left (2)
  else if (!offBoardLower(Vertical + 1) && !offBoardLower(Horizontal - 2)) {
    if (gChessboard[Vertical + 1][Horizontal - 2] == 'N') {
      return true;
    }
  }
  // Top left (1)
  else if (!offBoardLower(Vertical - 1) && !offBoardLower(Horizontal - 2)) {
    if (gChessboard[Vertical - 1][Horizontal - 2] == 'N') {
      return true;
    }
  }
  // Top left (2)
  else if (!offBoardLower(Vertical - 2) && !offBoardLower(Horizontal - 1)) {
    if (gChessboard[Vertical - 2][Horizontal - 1] == 'N') {
      return true;
    }
  }
  return false;
}

bool rookBishopQueen (int X, int Y) {
  // Orthogonals
  // Up
  for (int i = Y - 1; i >= 0; i--) {
    if (gChessboard[X][i] == 'R' || gChessboard[X][i] == 'Q') {
      return true;
    }
    if (gChessboard[X][i] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  // Down
  for (int i = Y + 1; i < gChessboardSize; i++) {
    if (gChessboard[X][i] == 'R' || gChessboard[X][i] == 'Q') {
      return true;
    }
    if (gChessboard[X][i] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  // Left
  for (int i = X - 1; i >= 0; i--) {
    if (gChessboard[i][Y] == 'R' || gChessboard[i][Y] == 'Q') {
      return true;
    }
    if (gChessboard[X][i] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  // Right
  for (int i = X + 1; i < gChessboardSize; i++) {
    if (gChessboard[i][Y] == 'R' || gChessboard[i][Y] == 'Q') {
      return true;
    }
    if (gChessboard[X][i] == '.') {
      continue;
    }
    else {
      break;
    }
  }

  // Diagonals
  // Top right
  for (int i = X + 1, j = Y - 1; i < gChessboardSize; i++, j--) {
    if (gChessboard[i][j] == 'B' || gChessboard[i][j] == 'Q') {
      return true;
    }
    if (gChessboard[i][j] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  // Top left
  for (int i = X - 1, j = Y - 1; i < gChessboardSize; i--, j--) {
    if (gChessboard[i][j] == 'B' || gChessboard[i][j] == 'Q') {
      return true;
    }
    if (gChessboard[i][j] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  // Bottom left
  for (int i = X - 1, j = Y + 1; i < gChessboardSize; i--, j++) {
    if (gChessboard[i][j] == 'B' || gChessboard[i][j] == 'Q') {
      return true;
    }
    if (gChessboard[i][j] == '.') {
      continue;
    }
    else {
      break;
    }
  }
  // Bottom right
  for (int i = X + 1, j = Y + 1; i < gChessboardSize; i++, j++) {
    if (gChessboard[i][j] == 'B' || gChessboard[i][j] == 'Q') {
      return true;
    }
    if (gChessboard[i][j] == '.') {
      continue;
    }
    else {
      break;
    }
  }

  return true;
}

bool inCheck (void) {
  if (!kingPawn(gKingHorizontalPosition, gKingVerticalPosition)) {
    if (!knight(gKingHorizontalPosition, gKingVerticalPosition)) {
      if (!rookBishopQueen(gKingHorizontalPosition, gKingVerticalPosition)) {
        return false;
      }
      else {
        return true;
      }
    }
    else {
      return true;
    }
  }
  else {
    return true;
  }
  return true;
}

bool checkWrapper (char* colour, int game) {
  if (strcmp(colour, "BLACK")) {
    if (inCheck()) {
      printf ("Game #%d: black king is in check.\n", game);
      return true;
    }
  }
  else if (strcmp(colour, "WHITE")) {
    swapColours();
    if (inCheck()) {
      printf ("Game #%d: white king is in check.\n", game);
      return true;
    }
  }
  else if (strcmp(colour, "NO")) {
    printf ("Game #%d: no king is in check.\n", game);
  } 
  return true;
}

void mirrorH (void) {
  char temp;
  for (int i = 0; i < gChessboardSize / 2; i++) {
    for (int j = 0; j < gChessboardSize; j++) {
      temp = gChessboard[7 - i][j];
      gChessboard[7 - i][j] = gChessboard[i][j];
      gChessboard[i][j] = temp;
    }
  }
}

void mirrorV (void) {
  char temp;
  for (int i = 0; i < gChessboardSize; i++) {
    for (int j = 0; j < gChessboardSize / 2; j++) {
      temp = gChessboard[i][7 - j];
      gChessboard[i][7 - j] = gChessboard[i][j];
      gChessboard[i][j] = temp;
    }
  }
}

void rotateBoard (void) {
  mirrorH ();
  mirrorV ();
}

void swapCaps (void) {
  for (int i = 0; i < gChessboardSize; i++) { // Height/Y
    for (int j = 0; j < gChessboardSize; j++) { // Width/X
      if (isupper(gChessboard[i][j])) {
        gChessboard[i][j] = tolower(gChessboard[i][j]);
      }
      else if (islower(gChessboard[i][j])) {
        gChessboard[i][j] = toupper(gChessboard[i][j]);
      }
    }
  }
}

void swapColours (void) {
  rotateBoard ();
  swapCaps ();
  findKing(&gKingHorizontalPosition, &gKingVerticalPosition);
}

bool checkTerminationCondition (void) {
  for (int i = 0; i < gChessboardSize; i++) { // Height/Y
    for (int j = 0; j < gChessboardSize; j++) { // Width/X
      if (gChessboard[i][j] != '.') {
        return false;
      }
    }
  }
  return true;
}

void printChessboard (void) {
  for (int i = 0; i < gChessboardSize; i++) { // Height/Y
    for (int j = 0; j < gChessboardSize; j++) { // Width/X
      printf (" %c", gChessboard[i][j]);
    }
    printf ("\n");
  }
  printf("\n");
}