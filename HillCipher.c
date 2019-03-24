#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MatrixSize = 0;
int rowsPlain = 0;
int ** inverseMatrixValues;
char* finalText;
int Fi = 0;
void Encryption();
void Decryption();
void inverseMatrix(int key[MatrixSize][MatrixSize], int, int);
void printMatrix(int key[MatrixSize][MatrixSize], int);
int determinant(int key[MatrixSize][MatrixSize], int);
void transpose(int num[MatrixSize][MatrixSize], int fac[MatrixSize][MatrixSize], int, int);
int** splitPlainTextUp(char* plaintext);
void matrixMulti(int first[MatrixSize][MatrixSize], int ** second, int);
int gcdExtended(int a, int b, int *x, int *y);
int power(int x, int y);


int main(){
  int input = -1;
  printf("Enter 1 to ENCRYPT or 2 to DECRYPT: ");
  scanf("%d", &input);
  if(input == 1){
    Encryption();
  }else{
    Decryption();
  }
}

void Encryption(){
  char* matrixBuffer = malloc(sizeof(char) * 1024);
  char* nums = malloc(sizeof(char) * 1024);
  printf("\nEnter plain-text:\n");
  scanf("%s", matrixBuffer);
  //THISISAHILLCIPHER
  printf("\nEnter block size of matrix:\n");
  scanf("%d", &MatrixSize);
  int keyInt[MatrixSize][MatrixSize];
  printf("\nEnter Key Matrix:\n");
  int row = -1;
  for(int i = 0; i < MatrixSize*MatrixSize; i++){
    scanf("%s", nums);
    if(i%MatrixSize == 0)
      row++;
    keyInt[row][i%MatrixSize] = atoi(nums
    );
  }
  int plaintextR[MatrixSize][MatrixSize];
  int ** plaintext = splitPlainTextUp(matrixBuffer);
  finalText = malloc(sizeof(char) * (strlen(matrixBuffer)+1));
  for(int i = 0; i < rowsPlain; i++){
    matrixMulti(keyInt, plaintext, i);
  }
  printf("\nEncrypted Text is:\n%s\n", finalText);
}

void Decryption(){
  // char buffer[100];
  // char matrixBuffer[20];
  char* matrixBuffer = malloc(sizeof(char) * 1024);
  char* nums = malloc(sizeof(char) * 1024);

  printf("\nEnter cipher-text:\n");
  scanf("%s", matrixBuffer);

  printf("\nEnter block size of matrix:\n");
  scanf("%d", &MatrixSize);
  int keyInt[MatrixSize][MatrixSize];
  // inverseMatrixValues[MatrixSize][MatrixSize];
  printf("\nEnter Key Matrix:\n");
  int row = -1;
  for(int i = 0; i < MatrixSize*MatrixSize; i++){
    scanf("%s", nums);
    if(i%MatrixSize == 0)
      row++;
    keyInt[row][i%MatrixSize] = atoi(nums);
  }

  // printf("You entered %s\n", buffer);

  int det = determinant(keyInt, MatrixSize);
  int x, y;
  if(det < 0){
    det += 26;
  }
  int a = det;
  int b = 26;
  int g = gcdExtended(a, b, &x, &y);
  if(x < 0){
    x += 26;
  }
  // printf("det = %d\n", det);
  // printf("x = %d\n", x);
  // printf("y = %d\n", y);

  if(det == 0 || g != 1){
    printf("\nKey Matrix is not invertible mod 26. Exiting Program.\n");
    exit(1);
  }else{
    inverseMatrix(keyInt, MatrixSize, x);
  }

  printf("\nInverse Key Matrix is:\n");
  // printf("done %d %d %d %d\n", inverseMatrixValues[0][0], inverseMatrixValues[0][1], inverseMatrixValues[1][0],inverseMatrixValues[1][1]);
  for(int i = 0; i < MatrixSize; i++){
    for(int j = 0; j < MatrixSize; j++){
      keyInt[i][j] = inverseMatrixValues[i][j];
      printf("%d ", keyInt[i][j]);
      // printf("%6f\n", keyInt[i][j]);
    }
    printf("\n");
  }
  int ** plaintext = splitPlainTextUp(matrixBuffer);
  finalText = malloc(sizeof(char) * (strlen(matrixBuffer)+1));
  for(int i = 0; i < rowsPlain; i++){
    matrixMulti(keyInt, plaintext, i);
  }
  printf("\nDecrypted Text is:\n%s\n", finalText);
}

void matrixMulti(int first[MatrixSize][MatrixSize], int ** second, int row){
  // printf("first\n");
  // printMatrix(first, MatrixSize);
  // printf("second %d %d %d %d\n", second[0][0], second[0][1], second[1][0], second[1][1]);

  // printMatrix(second, MatrixSize);
  int sum = 0;
  int multiply[1][MatrixSize];
  int q = MatrixSize;
  int p = MatrixSize;

  for (int j = 0; j < q; j++) {
    for (int k = 0; k < p; k++) {
      sum +=first[j][k]*second[row][k];
    }
    sum = sum%26;
    multiply[0][j] = sum;
    sum = 0;
  }
  // printf("%c", (multiply[0][0] + 'A'));
  // printf("%c", (multiply[0][1] + 'A'));
  for(int i = 0; i < MatrixSize; i++){
    // printf("HERE%c\n", (multiply[0][i] + 'A'));
    finalText[Fi++] = multiply[0][i] + 'A';
  }
  // finalText[Fi++] = multiply[0][1] + 'A';
  // printf("\n");
}

int** splitPlainTextUp(char* plaintext){
  int **x = (int **)malloc(((strlen(plaintext)/MatrixSize)+1) * sizeof(int *));
  int row = 0;
  for(int i = 0; i < strlen(plaintext); i+=MatrixSize){
    x[row] = (int *)malloc(MatrixSize * sizeof(int));
    for (int j = 0; j < MatrixSize; j++) {
      if(i+j >= strlen(plaintext)){
        x[row][j] = 23;
      }else{
        x[row][j] = (plaintext[i+j] - 'A');
      }
      // printf("%d, %d - %c -- y[%d]\n", row, j, plaintext[i+j], x[row][j]);
    }
    // printf("\n");
    row++;
  }
  rowsPlain = row;
  return x;
}

int determinant(int key[MatrixSize][MatrixSize], int numFac){
  int flip = 1;
  int det = 0;
  int det2[MatrixSize][MatrixSize];
  // int m, n;
  if (numFac == 1){
    return (key[0][0]);
  }
  else{
    det = 0;
    for (int a = 0; a < numFac; a++){
      int det2X = 0;
      int det2Y = 0;
      for (int i = 0;i < numFac; i++){
        for (int j = 0 ;j < numFac; j++){
          det2[i][j] = 0;
          if (i != 0 && j != a){
            det2[det2X][det2Y] = key[i][j];
            if (det2Y < (numFac - 2)){
              det2Y++;
            }
            else{
              det2Y = 0;
              det2X++;
            }
          }
        }
      }
      det = det + flip * (key[0][a] * determinant(det2, numFac - 1));
      flip *= -1;
    }
  }
  return (det);
}

void inverseMatrix(int key[MatrixSize][MatrixSize], int numFac, int x){
  int det2[MatrixSize][MatrixSize], factor[MatrixSize][MatrixSize];
  for (int k = 0; k < numFac; k++){
    for (int l = 0; l < numFac; l++){
        int det2X = 0;
        int det2Y = 0;
        for (int i = 0; i < numFac; i++){
          for (int j = 0; j < numFac; j++){
            if (i != k && j != l){
              det2[det2X][det2Y] = key[i][j];
              if (det2Y < (numFac - 2)){
                det2Y++;
              }
              else{
                det2Y = 0;
                det2X++;
              }
            }
          }
      }
      factor[k][l] = power(-1, k + l) * determinant(det2, numFac - 1);
    }
  }
  transpose(key, factor, numFac, x);
}

void transpose(int key[MatrixSize][MatrixSize], int factor[MatrixSize][MatrixSize], int numFac, int x){
  // printf("???1\n");
  int inverse[MatrixSize][MatrixSize];
  inverseMatrixValues = (int **)malloc((MatrixSize * sizeof(int *)) + 1);
  for(int i = 0; i < MatrixSize; i++){
    inverseMatrixValues[i] = (int *)malloc((MatrixSize * sizeof(int)) + 1);
  }
  // printf("size %d\n", MatrixSize + 1);
  for (int i = 0; i < numFac; i++){
    // printf("???3 %d\n", i);

    // printf("???4\n");
    for (int j = 0;j < numFac; j++){
      // inverseMatrixValues[i] = (int *)malloc((MatrixSize * sizeof(int)) + 1);
      // printf("j: %d, i: %d\n", j, i);
      inverseMatrixValues[j][i] = ((int)factor[i][j]*x)%26;
      // printf("XX: %d\n",inverseMatrixValues[j][i] );
      if(inverseMatrixValues[j][i] < 0){
        inverseMatrixValues[j][i] += 26;
      }
    }
  }

  // inverseMa

  // printf("done %d %d %d %d\n", inverseMatrixValues[0][0], inverseMatrixValues[0][1], inverseMatrixValues[1][0],inverseMatrixValues[1][1]);
  // printMatrix(inverseMatrixValues, numFac);
}

void printMatrix(int key[MatrixSize][MatrixSize], int numFac){
  for(int i = 0; i < numFac; i++){
    for(int j = 0; j < numFac; j++){
      printf("%d ", key[i][j]);
    }
    printf("\n");
  }
}

// C function for extended Euclidean Algorithm
int gcdExtended(int a, int b, int *x, int *y) {
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b%a, a, &x1, &y1);
    *x = y1 - (b/a) * x1;
    *y = x1;
    return gcd;
}

int power(int x, int y){
  if(y % 2 == 0){
    return 1;
  }
  return -1;
}
