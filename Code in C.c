#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct splnum
{
    int a, b, c, d;
}
splnum;

splnum addspl(splnum u, splnum v); /* Returns u+v */
splnum subspl(splnum u, splnum v); /* Returns u-v */
splnum mulspl(splnum u, splnum v); /* Returns uv */
splnum invspl(splnum u); /* Returns 1/u provided that u is not zero */
splnum divspl(splnum u, splnum v); /* Returns u/v provided that v is not zero */
void prnspl (splnum u); /* Print u as in the sample outputs */
splnum splnumify(splnum u); //Takes care of constraints
int gcd(int a, int b); //Computes the GCD of 2 numbers
void findroots(int r, int s, splnum roots[2]); //Find roots of the the quadratic equation
void solvehomogeneous(int r, int s, int a0, int a1); //Solves a homogeneous recursion
int equals (splnum u, splnum v); //Calculates if two splnums are equal or not
void solvenonhomogeneous(int r, int s, int t, int a0, int a1); //Solves a non homogeneous recursion

splnum addspl(splnum u, splnum v)
{
    splnum sum;
    sum.a = (u.a)*(v.c) + (v.a)*(u.c);
    sum.b = (u.b)*(v.c) + (v.b)*(u.c);
    sum.c = (u.c)*(v.c);
    if (u.d == 0)
        sum.d = v.d;
    else
        sum.d = u.d;

    return splnumify(sum);
}

splnum subspl(splnum u, splnum v)
{
    splnum sub;
    sub.a = (u.a)*(v.c) - (v.a)*(u.c);
    sub.b = (u.b)*(v.c) - (v.b)*(u.c);
    sub.c = (u.c)*(v.c);
    if (u.d == 0)
        sub.d = v.d;
    else
        sub.d = u.d;

    return splnumify(sub);
}

splnum mulspl(splnum u, splnum v)
{
    splnum mul;
    mul.a = (u.a)*(v.a) + (v.b)*(u.b)*(u.d);
    mul.b = (u.a)*(v.b) + (v.a)*(u.b);
    mul.c = (u.c)*(v.c);
    if (u.d == 0)
        mul.d = v.d;
    else
        mul.d = u.d;
    return splnumify(mul);
}

splnum invspl(splnum u)
{
    if (u.a != 0 || (u.b != 0 && u.d != 0))
    {
        splnum inv;
        inv.a = (u.a)*(u.c);
        inv.b = -(u.b)*(u.c);
        inv.c = (u.a)*(u.a)-(u.b)*(u.b)*(u.d);
        inv.d = u.d;
        return splnumify(inv);
    }

    else
        printf("Error: Division by zero");
}

splnum divspl(splnum u, splnum v)
{
    return splnumify(mulspl(u, invspl(v)));
}

void prnspl(splnum u)
{
    if (u.d != 0 && u.b != 0)
    {
        if (u.b >= 0){
            if (u.a == 0){
                if (u.b == 1){
                    if (u.c == 1)
                        printf("(sqrt(%d))",u.d);
                    else if (u.c == -1)
                        printf("(-sqrt(%d))",u.d);
                    else
                        printf("(sqrt(%d)) / %d", u.d, u.c);}
                else{
                    if (u.c == 1 || u.c == -1)
                        printf("(%d sqrt(%d))", u.c*u.b, u.d);
                    else
                        printf("(%d sqrt(%d)) / %d", u.b, u.d, u.c);}}
            else{
                if (u.b == 1){
                    if (u.c == 1)
                        printf("(%d + sqrt(%d))", u.a, u.d);
                    else if(u.c == -1)
                        printf("(%d - sqrt(%d))", u.c*u.a, u.d);
                    else
                        printf("(%d + sqrt(%d)) / %d", u.a, u.d, u.c);}
                else{
                    if (u.c == 1 || u.c == -1)
                        printf("(%d + %d sqrt(%d))", u.c*u.a, u.c*u.b, u.d);
                    else
                        printf("(%d + %d sqrt(%d)) / %d", u.a, u.b, u.d, u.c);}}}
        else{
            if (u.a == 0){
                if (u.b == -1){
                    if (u.c == 1)
                        printf("(- sqrt(%d)) / %d", u.d);
                    else if (u.c == -1)
                        printf("(sqrt(%d)) / %d", u.d);
                    else
                        printf("(- sqrt(%d)) / %d", u.d, u.c);}
                else{
                    if (u.c == 1 || u.c == -1)
                        printf("(- %d sqrt(%d))", -1*(u.b)*u.c, u.d);
                    else
                        printf("(- %d sqrt(%d)) / %d", -1*(u.b), u.d, u.c);}}
            else{
                if (u.b == -1){
                    if(u.c == 1)
                        printf("(%d - sqrt(%d))", u.a, u.d);
                    else if (u.c == -1)
                        printf("(%d + sqrt(%d))", u.c*u.a, u.d);
                    else
                        printf("(%d - sqrt(%d)) / %d", u.a, u.d, u.c);}
                else{
                    if (u.c == 1 || u.c == -1)
                        printf("(%d - %d sqrt(%d)) / %d", u.c*u.a, -1*(u.b)*u.c, u.d);
                    else
                        printf("(%d - %d sqrt(%d)) / %d", u.a, -1*(u.b), u.d, u.c);}}}
    }

    else if (u.c != 1 && u.c != -1)
    {
        if (u.c > 0)
            printf("(%d / %d)", u.a, u.c);
        else if(u.c < 0)
            printf("(%d / %d)", -1*u.a, -1*u.c);
    }
    else
        printf("%d", u.c*u.a);
}

splnum splnumify(splnum u)
{
    int a = u.a;
    int b = u.b;
    int c = u.c;
    int d = u.d;

    int gcd_abc = gcd(a, gcd(b, c));
    a/=gcd_abc;
    b/=gcd_abc;
    c/=gcd_abc;

    float sqrt_d_in_float = sqrt(d);
    int sqrt_d_in_int = (int) sqrt_d_in_float;
    if (d >= 0 && sqrt_d_in_int == sqrt_d_in_float)
    {
        //Perfect square
        a = a + b*sqrt_d_in_int;
        b = 0;
        d = 0;

    }
    else if (d != 0)
    {
        //Not a perfect square
        if (a%2 == 0 && d%4 == 0 && c%2 == 0)
        {
            d/=4;
            a/=2;
            c/=2;
        }
    }

    gcd_abc = gcd(a, gcd(b, c));
    a/=gcd_abc;
    b/=gcd_abc;
    c/=gcd_abc;

    if (c > 0)
    {
        u.a = a;
        u.b = b;
        u.c = c;
        u.d = d;
    }

    else if (c < 0)
    {
        u.a = -a;
        u.b = -b;
        u.c = -c;
        u.d = d;
    }

    return u;
}

int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

void findroots(int r, int s, splnum roots[2])
{
    roots[0].a = r;
    roots[0].b = 1;
    roots[0].c = 2;
    roots[0].d = r*r + 4*s;

    roots[1].a = r;
    roots[1].b = -1;
    roots[1].c = 2;
    roots[1].d = r*r + 4*s;
    roots[0] = splnumify(roots[0]);
    roots[1] = splnumify(roots[1]);
}

void solvehomogeneous(int r, int s, int a0, int a1)
{
    printf("\n+++ Solving the homogeneous recurrence\n");
    splnum roots[2];
    printf("    Characteristic equation: x^2 + (%d)x + (%d) = 0\n", -r, -s);
    findroots(r, s, roots);
    printf("    Root 1 = ");
    prnspl(roots[0]);
    printf("\n    Root 2 = ");
    prnspl(roots[1]);
    printf("\n");
    printf("    Homogeneous Solution :\n    ");
    splnum U, V;
    if (equals(roots[0], roots[1]))
    {
        //Equal roots
        U.a = 2*a1 - r*a0;
        U.b = 0;
        U.c = r;
        U.d = 0;
        U = splnumify(U);

        V.a = a0;
        V.b = 0;
        V.c = 1;
        V.d = 0;
        V = splnumify(V);

        printf("[");
        prnspl(U);
        printf("n + ");
        prnspl(V);
        printf("] ");
        prnspl(roots[0]);
        printf("^n\n");
    }

    else
    {
        //Unequal roots
        U.a = a0*(r*r + 4*s);
        U.b = 2*a1 - r*a0;
        U.c = 2*(r*r + 4*s);
        U.d = (r*r + 4*s);
        U = splnumify(U);

        V.a = a0*(r*r + 4*s);
        V.b = -2*a1 + r*a0;
        V.c = 2*(r*r + 4*s);
        V.d = (r*r + 4*s);
        V = splnumify(V);

        printf("[");
        prnspl(U);
        printf("] [");
        prnspl(roots[0]);
        printf("]^n + [");
        prnspl(V);
        printf("] [");
        prnspl(roots[1]);
        printf("]^n\n");
    }
}

int equals (splnum u, splnum v)
{
    u = splnumify(u);
    v = splnumify(v);
    if (u.a == v.a && u.b == v.b && u.c == v.c && u.d == v.d)
        return 1;
    return 0;
}

void solvenonhomogeneous(int r, int s, int t, int a0, int a1)
{
    printf("\n+++ Solving the homogeneous recurrence\n");
    splnum roots[2];
    printf("    Characteristic equation: x^2 + (%d)x + (%d) = 0\n", -r, -s);
    findroots(r, s, roots);
    printf("    Root 1 = ");
    prnspl(roots[0]);
    printf("\n    Root 2 = ");
    prnspl(roots[1]);
    printf("\n");
    printf("    Particular solution : ");
    splnum A1, A2, A3;
    splnum one;
    one.a = 1;
    one.b = 0;
    one.c = 1;
    one.d = 0;

    if (equals(roots[0], roots[1]))
    {
        //Equal Roots
        if (equals(roots[0], one))
        {
            //B=R
            A3.a = 1;
            A3.b = 0;
            A3.c = 2;
            A3.d = 0;
            printf("[");
            prnspl(A3);
            printf("] n^2");
            printf("\n    Homogeneous solution :\n    ");

            A1.a = a0;
            A1.b = 0;
            A1.c = 1;
            A1.d = 0;

            A2.a = 2*a1-2*a0-1;
            A2.b = 0;
            A2.c = 2;
            A2.d = 0;

            printf("[");
            prnspl(A2);
            printf("n + ");
            prnspl(A1);
            printf("] 1^n");
        }
        else
        {
            //B!=R
            splnum temp;

            A3.a = t;
            A3.b = 0;
            A3.c = 1-r-s;
            A3.d = 0;

            prnspl(A3);
            printf("\n    Homogeneous solution :\n    ");

            temp.a = a0;
            temp.b = 0;
            temp.c = 1;
            temp.d = 0;

            A1 = subspl(temp, A3);

            temp.a = a1;

            A2 = subspl(divspl(subspl(temp, A3), roots[0]), A1);

            printf("[");
            prnspl(A2);
            printf("n + ");
            prnspl(A1);
            printf("] [");
            prnspl(roots[0]);
            printf("]^n");;
        }
    }
    else
    {
        //Distinct Roots
        if (equals(roots[0], one) || equals(roots[1], one))
        {
            //B=R1 or B=R2
            A3.a = t;
            A3.b = 0;
            A3.c = r+2*s;
            A3.d = 0;
            printf("[");
            prnspl(A3);
            printf("] n");

            printf("\n    Homogeneous solution :\n    ");

            splnum a_0, a_1;
            a_0.a = a0;
            a_0.b = 0;
            a_0.c = 1;
            a_0.d = 0;

            a_1.a = a1;
            a_1.b = 0;
            a_1.c = 1;
            a_1.d = 0;

            A2 = divspl(subspl(subspl(a_1, A3), mulspl(a_0, roots[0])), subspl(roots[1], roots[0]));
            A1 = subspl(a_0, A2);

            printf("[");
            prnspl(A1);
            printf("] [");
            prnspl(roots[0]);
            printf("]^n + [");
            prnspl(A2);
            printf("] [");
            prnspl(roots[1]);
            printf("]^n");

        }
        else
        {
            //B!=R1 and B!=R2
            A3.a = t;
            A3.b = 0;
            A3.c = 1-r-s;
            A3.d = 0;

            prnspl(A3);
            printf("\n    Homogeneous solution :\n    ");

            splnum a_0, a_1;
            a_0.a = a0;
            a_0.b = 0;
            a_0.c = 1;
            a_0.d = 0;

            a_1.a = a1;
            a_1.b = 0;
            a_1.c = 1;
            a_1.d = 0;

            //A2 = divspl(addspl(subspl(subspl(a_1, A3), mulspl(a_0, roots[0])), mulspl(A3, roots[0])), subspl(roots[1], roots[0]));
            A2 = divspl(addspl(subspl(subspl(mulspl(A3, roots[0]), mulspl(a_0, roots[0])), A3), a_1), subspl(roots[1], roots[0]));
            A1 = subspl(subspl(a_0, A2), A3);

            printf("[");
            prnspl(A1);
            printf("] [");
            prnspl(roots[0]);
            printf("]^n + [");
            prnspl(A2);
            printf("] [");
            prnspl(roots[1]);
            printf("]^n");
        }
    }

}
void main()
{
    int r, s, t, a0, a1;

    printf("r = ");
    scanf("%d", &r);
    printf("s = ");
    scanf("%d", &s);
    printf("t = ");
    scanf("%d", &t);
    printf("a0 = ");
    scanf("%d", &a0);
    printf("a1 = ");
    scanf("%d", &a1);

    solvehomogeneous(r, s, a0, a1);
    solvenonhomogeneous(r, s, t, a0, a1);
}
