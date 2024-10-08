#include <iostream>
#include <cstdlib>
#include <vector>
#include <bits/stdc++.h>
#include <cassert>
#include <cstdio>
#include "kol.h"
using namespace std;

typedef struct lista{
    interesant *glowa, *ogon;
}lista;

lista stworz()
{
    lista wyn;
    wyn.glowa = (interesant*)malloc(sizeof(interesant));
    wyn.ogon = (interesant*)malloc(sizeof(interesant));
    wyn.glowa->l1 = NULL;
    wyn.glowa->l2 = wyn.ogon;
    wyn.ogon->l1 = NULL;
    wyn.ogon->l2 = wyn.glowa;
    return wyn;
}

bool pusta(const lista l)
{
    return l.glowa->l2 == l.ogon;
}

interesant *przod(const lista l)
{
    return l.glowa->l2;
}

interesant *koniec(const lista l)
{
    return l.ogon->l2;
}

void zamien(interesant *a, interesant *stary, interesant *nowy)
{
    if(a)
    {
        if(a->l1 == stary)
            a->l1 = nowy;
        else
            a->l2 = nowy;
    }

}

void dodaj_na_koniec(lista &l, int x)
{
    interesant *el = (interesant*)malloc(sizeof(interesant));
    el->wartosc = x;
    el->l1 = l.ogon;
    el->l2 = l.ogon->l2;
    zamien(l.ogon->l2, l.ogon, el);
    l.ogon->l2 = el;
}

void usun(interesant *x)
{
    zamien(x->l1, x, x->l2);
    zamien(x->l2, x, x->l1);
}

void usun_pierwszy(lista &l)
{
    if(!pusta(l))
        usun(l.glowa->l2);
}

void odwroc(lista &l)
{
    interesant *x = l.glowa;
    l.glowa = l.ogon;
    l.ogon = x;
}

//l2 jest przypinana do l1
void scal(lista &l1, lista &l2)
{
    interesant *b = l1.ogon;
    interesant *f = l2.glowa;
    zamien(b->l2, b, f->l2);
    zamien(f->l2, f, b->l2);
    b->l2 = f;
    f->l2 = b;
    l1.ogon = l2.ogon;
    l2.glowa = f;
    l2.ogon = b;
}

vector <lista> kolejki;
int akt_numerek = 0;

void otwarcie_urzedu(int m)
{
    kolejki.resize(m);
    for(int i = 0; i < m; i++)
        kolejki[i] = stworz();
}

interesant *nowy_interesant(int k)
{
    dodaj_na_koniec(kolejki[k], akt_numerek++);
    return koniec(kolejki[k]);
}

int numerek(interesant* i)
{
    return i->wartosc;
}

interesant *obsluz(int k)
{
    if(pusta(kolejki[k]))
        return NULL;
    interesant *x = przod(kolejki[k]);
    usun_pierwszy(kolejki[k]);
    return x;
}

void zmiana_okienka(interesant *i, int k)
{
    zamien(i->l1, i, i->l2);
    zamien(i->l2, i, i->l1);
    i->l1 = kolejki[k].ogon;
    i->l2 = kolejki[k].ogon->l2;
    zamien(kolejki[k].ogon->l2, kolejki[k].ogon, i);
    kolejki[k].ogon->l2 = i;
}

void zamkniecie_okienka(int k1, int k2)
{
    scal(kolejki[k2], kolejki[k1]);
}

pair <interesant*, interesant*> przesun(interesant *poprzedni, interesant *aktualny)
{
    pair <interesant*, interesant*> wyn;
    if(aktualny->l1 != poprzedni)
    {
        poprzedni = aktualny;
        aktualny = aktualny->l1;
    }
    else
    {
        poprzedni = aktualny;
        aktualny = aktualny->l2;
    }
    wyn = {poprzedni, aktualny};
    return wyn;
}

std::vector<interesant*> fast_track(interesant *i1, interesant *i2)
{
    pair <interesant*, interesant*> przesunieci;
    vector <interesant*> lewy;
    vector <interesant*> prawy;
    interesant *poprzedni_l = i1;
    interesant *l = i1->l1;
    interesant *poprzedni_p = i1;
    interesant *p = i1->l2;
    lewy.push_back(poprzedni_l);
    prawy.push_back(poprzedni_p);
    if(i1 == i2)
    {
        usun(i1);
        return lewy;
    }
    while((l != i2) && (p != i2))
    {
        if(l != NULL)
        {
            przesunieci = przesun(poprzedni_l, l);
            poprzedni_l = przesunieci.first;
            l = przesunieci.second;
            lewy.push_back(poprzedni_l);
        }
        if(p != NULL)
        {
            przesunieci = przesun(poprzedni_p, p);
            poprzedni_p = przesunieci.first;
            p = przesunieci.second;
            prawy.push_back(poprzedni_p);
        }
    }
    if(l == i2)
    {
        lewy.push_back(l);
        for(long unsigned int i = 0; i < lewy.size(); i++)
            usun(lewy[i]);
        return lewy;
    }
    else {
        prawy.push_back(p);
        for(long unsigned int i = 0; i < prawy.size(); i++)
            usun(prawy[i]);
        return prawy;
    }
}

void naczelnik(int k)
{
    odwroc(kolejki[k]);
}

std::vector<interesant *> zamkniecie_urzedu()
{
    long unsigned int m = kolejki.size();
    vector <interesant*> wyn;
    for(int i = 0; i < (int)m; i++)
    {
        while(!pusta(kolejki[i]))
           wyn.push_back(obsluz(i));
        free(kolejki[i].glowa);
        free(kolejki[i].ogon);
    }
    return wyn;
}
