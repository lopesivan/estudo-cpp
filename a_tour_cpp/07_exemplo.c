/*
 *                     __ __       ___
 *                    /\ \\ \    /'___`\
 *                    \ \ \\ \  /\_\ /\ \
 *                     \ \ \\ \_\/_/// /__
 *                      \ \__ ,__\ // /_\ \
 *                       \/_/\_\_//\______/
 *                          \/_/  \/_____/
 *                                        Algoritmos
 *
 *
 *      Author: Ivan Lopes
 *        Mail: lopesivan.ufrj@gmail.com
 *        Site: ivanlopes.eng.br
 *     License: gpl
 *    Language: C ansi
 *        File: 07_exemplo.c
 *        Date: ter 23 mai 2023 23:36:52 -03
 * Description:
 */

#include <stdio.h>

void print (int* p)
{
    int i = 0;
    printf ("--1-------\n");
    for (i = 0; i<10; i++)
    {
        printf ("valor = %d\n", * (p + i));
    }
}

void print2 (int* p)
{
    int i = 0;
    printf ("--2-------\n");
    for (i = 0; i<5; i++)
    {
        printf ("valor = %d\n", * (p + i));
    }
}

void print3 (int* p)
{
    int i = 0;
    printf ("--3-------\n");
    for (i = 0; i<10; i++)
    {
        printf ("valor = %d\n",   p[i]);
    }
}

int main (int argc, char* argv[])
{

    int v[10] = {77, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int* p = v;

    printf ("valor = %d\n", *p);
    printf ("valor = %d\n", *v);
    print (p);
    print (v);
    p = &v[3];
    print2 (p);
    print2 (&v[3]);

    /* print3 (&v[0]); */
    print3 (v);
    return 0;
}
