#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int code;
    int recNo;
} BinTreeElementType;

typedef struct BinTreeNode *BinTreePointer;

struct BinTreeNode{
    BinTreeElementType Data;
    BinTreePointer LChild, RChild;
};

typedef enum{ FALSE, TRUE } boolean;

typedef struct{
    int code;
    char surname[20];
    char name[20];
    char sex;
    int year;
    float grade;
} StudentT;

void CreateBST(BinTreePointer *Root) { *Root = NULL; }
boolean BSTEmpty(BinTreePointer Root) { return (Root == NULL); }

void RecBSTInsert(BinTreePointer *Root, BinTreeElementType Item) {
    if (BSTEmpty(*Root)) {
        (*Root) = (BinTreePointer)malloc(sizeof(struct BinTreeNode));
        (*Root)->Data = Item;
        (*Root)->LChild = (*Root)->RChild = NULL;
    }
    else if (Item.code < (*Root)->Data.code)
        RecBSTInsert(&(*Root)->LChild, Item);
    else if (Item.code > (*Root)->Data.code)
        RecBSTInsert(&(*Root)->RChild, Item);
}

void RecBSTSearch(BinTreePointer Root, BinTreeElementType KeyValue, boolean *Found, BinTreePointer *LocPtr) {
    if (BSTEmpty(Root)) *Found = FALSE;
    else if (KeyValue.code < Root->Data.code)
        RecBSTSearch(Root->LChild, KeyValue, Found, LocPtr);
    else if (KeyValue.code > Root->Data.code)
        RecBSTSearch(Root->RChild, KeyValue, Found, LocPtr);
    else {
        *Found = TRUE;
        *LocPtr = Root;
    }
}

void RecBSTInorder(BinTreePointer Root) {
    if (Root) {
        RecBSTInorder(Root->LChild);
        printf("(%d, %d)\n", Root->Data.code, Root->Data.recNo);
        RecBSTInorder(Root->RChild);
    }
}

int BuildBST(BinTreePointer *Root) {
    FILE *fp = fopen("students_Sample.dat", "r");
    StudentT student;
    BinTreeElementType indexRec;
    int size = 0;
    char termch;
    if (!fp) return 0;
    while (fscanf(fp, "%d, %[^,], %[^,], %c, %d, %f%c", &student.code, student.surname, student.name, &student.sex, &student.year, &student.grade, &termch) == 7) {
        indexRec.code = student.code;
        indexRec.recNo = size++;
        RecBSTInsert(Root, indexRec);
    }
    fclose(fp);
    return size;
}

void PrintStudent(int RecNum) {
    FILE *infile = fopen("students_Sample.dat", "r");
    StudentT student;
    char termch;
    if (!infile) return;
    for (int i = 0; i <= RecNum; i++)
        fscanf(infile, "%d, %[^,], %[^,], %c, %d, %f%c", &student.code, student.surname, student.name, &student.sex, &student.year, &student.grade, &termch);
    printf("%d, %s, %s, %c, %d, %.2f\n", student.code, student.surname, student.name, student.sex, student.year, student.grade);
    fclose(infile);
}

void PrintStudentsWithGrade() {
    FILE *infile = fopen("students_Sample.dat", "r");
    StudentT student;
    float theGrade;
    char termch;
    printf("Enter minimum grade: ");
    scanf("%f", &theGrade);
    if (!infile) return;
    while (fscanf(infile, "%d, %[^,], %[^,], %c, %d, %f%c", &student.code, student.surname, student.name, &student.sex, &student.year, &student.grade, &termch) == 7)
        if (student.grade >= theGrade)
            printf("%d, %s, %s, %c, %d, %.2f\n", student.code, student.surname, student.name, student.sex, student.year, student.grade);
    fclose(infile);
}

void Menu() {
    printf("\nMenu:\n");
    printf("1. Insert new student\n");
    printf("2. Search for a student\n");
    printf("3. Print all students\n");
    printf("4. Print students with grade >= given\n");
    printf("5. Quit\n");
    printf("Choice: ");
}

int main() {
    FILE *fp;
    boolean found;
    BinTreePointer Root = NULL, LocPtr;
    int choice, size;
    BinTreeElementType indexRec;
    StudentT student;
    size = BuildBST(&Root);
    do {
        Menu();
        scanf("%d", &choice);
        getchar();
        if (choice == 1) {
            fp = fopen("students_Sample.dat", "a");
            if (!fp) exit(1);
            printf("Enter Student Code: ");
            scanf("%d", &student.code);
            indexRec.code = student.code;
            RecBSTSearch(Root, indexRec, &found, &LocPtr);
            if (!found) {
                printf("Enter Surname, Name, Sex, Year, Grade: ");
                scanf("%s %s %c %d %f", student.surname, student.name, &student.sex, &student.year, &student.grade);
                indexRec.recNo = size++;
                RecBSTInsert(&Root, indexRec);
                fprintf(fp, "%d, %s, %s, %c, %d, %.2f\n", student.code, student.surname, student.name, student.sex, student.year, student.grade);
            }
            fclose(fp);
        }
        else if (choice == 2) {
            printf("Enter Student Code to Search: ");
            scanf("%d", &student.code);
            indexRec.code = student.code;
            RecBSTSearch(Root, indexRec, &found, &LocPtr);
            if (found) PrintStudent(LocPtr->Data.recNo);
            else printf("Student not found.\n");
        }
        else if (choice == 3) {
            RecBSTInorder(Root);
        }
        else if (choice == 4) {
            PrintStudentsWithGrade();
        }
    } while (choice != 5);
    return 0;
}
