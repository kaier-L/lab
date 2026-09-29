#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h> 

typedef struct {
    char id[20]; 
    char name[21];
    double score1;
    double score2;
    double score3;
    double sum;
    double avr;
} student;

int main() 
{
    int n;
    printf("请输入要管理的人数：");
    scanf("%d", &n);

    if (n < 1 || n > 10) 
    {
        printf("人数超出范围\n");
        return 0;
    }

    student stu[10];
    student* p = stu;

    for (int j = 0; j < n; j++) 
    {
        
        scanf("%s %s %lf %lf %lf", (p + j)->id, (p + j)->name, &(p + j)->score1, &(p + j)->score2, &(p + j)->score3);
        (p + j)->sum = (p + j)->score1 + (p + j)->score2 + (p + j)->score3;
        (p + j)->avr = (p + j)->sum / 3.0;
    }
    for (int i = 0; i < n - 1; i++)
    {
        for (int k = 0; k < n - 1 - i; k++)
        {
            if ((p + k)->sum < (p + k + 1)->sum) 
            {
                student temp = *(p + k);
                *(p + k) = *(p + k + 1);
                *(p + k + 1) = temp;
            }
        }
    }

    printf("这是排序后\n");
    for (int l = 0; l < n; l++) 
    {
        printf("%s %s %.2lf %.2lf %.2lf %.2lf %.2lf\n", (p + l)->id, (p + l)->name, (p + l)->score1, (p + l)->score2, (p + l)->score3, (p + l)->sum, (p + l)->avr);
    }
    
    while (1)
    {
        printf("\n请输入要查找的学号");
        char search[20];
        scanf("%s", search);
        if (strcmp(search, "!") == 0)
        {
            printf("查找结束。\n");
            break;
        }

        int found = 0;
        for (int t = 0; t < n; t++) 
        {
            if (strcmp((p + t)->id, search) == 0) 
            {
                printf("%s %s %.2lf %.2lf %.2lf %.2lf %.2lf\n", (p + t)->id, (p + t)->name, (p + t)->score1, (p + t)->score2, (p + t)->score3, (p + t)->sum, (p + t)->avr);
                found = 1;
                break;
            }
        }

        if (!found) 
        {
            printf("未找到该学生\n");
        }
        
    }

    return 0;
}
