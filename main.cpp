#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

class Mancare
{
    virtual double calcul_pret()=0;
    virtual void afisare()=0;
};

class Ingredient
{
    char den[200];
    int cant;
    float pret;
public:
    Ingredient (char den[200]="", int cant=0,float pret=0)
    {
        strcpy(this->den,den);
        this->cant=cant;
        this->pret=pret;
    }
    ~Ingredient()
    {
        strcpy(den,"");
        cant=0;
        pret=0;
    }
    Ingredient(Ingredient& ingr)
    {
        strcpy(den,ingr.den);
        cant=ingr.cant;
        pret=ingr.pret;
    }
    Ingredient& operator=(Ingredient& ingr)
    {
        if(this==&ingr)
        {
            return ingr;
        }
        strcpy(den,ingr.den);
        cant=ingr.cant;
        pret=ingr.pret;
        return *this;
    }
    friend class Pizza;
    friend istream& operator>>(istream&,Ingredient&);
    friend ostream& operator<<(ostream& g,Ingredient &);
};

istream& operator>>(istream& f,Ingredient& ingr)
{
    cout<<"Denumirea ingredientului: ";
    f>>ingr.den;
    cout<<"Cantitatea ingredientului: ";
    f>>ingr.cant;
    cout<<"Pretul ingredientului: ";
    f>>ingr.pret;
    return f;
}
ostream& operator<<(ostream& g,Ingredient& ingr)
{
    g<<"Denumirea: ";
    g<<ingr.den<<endl;
    g<<"Cantitatea: ";
    g<<ingr.cant<<endl;
    g<<"Pretul: ";
    g<<ingr.pret<<endl;
    return g;
}

class Pizza:public Mancare
{
    Ingredient *v;
    int nr_ingrediente;
    const double manopera=4.5;
    double pret_pizza;
    int veg;
    int online;
    float distanta;
public:
    Pizza()
    {
        cout<<"Numarul de ingrediente:";
        cin>>nr_ingrediente;
        cout<<"Pizza este vegetariana? 1-Da 0-Nu "<<endl;
        cin>>veg;
        cout<<"Comandati online? 1-Da 0-Nu "<<endl;
        cin>>online;
        if(online==1)
        {
            cout<<"Introduceti distanta ";
            cin>>distanta;
        }
        v=new Ingredient[nr_ingrediente];
        for(int i=0; i<nr_ingrediente; i++)
        {
            cout<<"Ingredientul cu numarul "<<i+1<<endl;
            cin>>v[i];
        }
        this->nr_ingrediente=nr_ingrediente;
    }
    ~Pizza()
    {
        for(int i=0; i<nr_ingrediente; i++)
            delete v;
        nr_ingrediente=0;
    }
    int getVeg()
    {
        return this->veg;
    }
    double getPretPizza()
    {
        pret_pizza=calcul_pret();
        return this->pret_pizza;
    }
    Pizza(Pizza& nr)
    {
        nr_ingrediente=nr.nr_ingrediente;
    }
    Pizza& operator=(Pizza& nr)
    {
        if(this==&nr)
            return nr;
        nr_ingrediente=nr.nr_ingrediente;
        return *this;
    }
    friend ostream& operator<<(ostream&,Pizza&);
    double calcul_pret()
    {
        pret_pizza=0;
        for(int i=0; i<nr_ingrediente; i++)
        {
            pret_pizza=pret_pizza+v[i].pret;
        }
        pret_pizza=pret_pizza+manopera;
        if(online==1)
        {
            double procent=5.0/100*pret_pizza;
            while(distanta>=10)
            {
                distanta=distanta-10;
                pret_pizza=pret_pizza+procent;
            }
        }
        return pret_pizza;
    }
    void afisare()
    {
        cout<<*this;
    }
};

ostream& operator<<(ostream& g,Pizza& p)
{
    g<<endl;
    g<<"Pizza este vegetariana/nu (1/0): ";
    g<<p.getVeg()<<endl;
    g<<"Numarul de ingrediente: ";
    g<<p.nr_ingrediente<<endl;
    for(int i=0; i<p.nr_ingrediente; i++)
        g<<p.v[i]<<endl;
    g<<"Pretul final este: ";
    g<<p.getPretPizza();
    return g;
}

template <class M>
class Meniu
{
    vector<M*> m;
    int nr;
    double pret_veg;
public:
    Meniu(int nr=0, double pret_veg=0)
    {
        this->nr=nr;
        this->pret_veg=pret_veg;
    }
    template <class R> Meniu& operator+=(R &p)
    {
        nr++;
        if(p->getVeg()==1)
            pret_veg=pret_veg+p->getPretPizza();
        m.push_back(p);
        return *this;
    }
    ~Meniu()
    {
        pret_veg=0;
        m.clear();
        nr=0;
    }
    void afisare()
    {
        try
        {
            if(m.size() == 0)
                throw 0;
            for(unsigned int i = 0; i < m.size(); i++)
            {
                m[i]->afisare();
            }
            cout<<"Pret total pizza vegetariana este: "<<pret_veg;
        }
        catch(int x)
        {
            cout << "Vectorul are lungimea " << x << endl;
        }
    }
};
template <> class Meniu<int>
{
    vector<Pizza*> m;
    int nr;
    double pret_veg;
public:
    Meniu(int nr=0, double pret_veg=0)
    {
        try
        {
            if(nr < 0 || pret_veg < 0)
                throw -1;
            this->nr=nr;
            this->pret_veg=pret_veg;
        }
        catch(int x)
        {
            cout << "O valoare este negativa";
        }

    }
    template <class R> Meniu& operator+=(R &p)
    {
        nr++;
        if(p->getVeg()==1)
            pret_veg=pret_veg+p->getPretPizza();
        m.push_back(p);
        return *this;
    }
    ~Meniu()
    {
        pret_veg=0;
        m.clear();
        nr=0;
    }
    void afisare()
    {
        try
        {
            if(m.size() == 0)
                throw 0;
            for(unsigned int i = 0; i < m.size(); i++)
            {
                m[i]->afisare();
            }
            cout<<"Pret total pizza vegetariana este: "<<pret_veg;
        }
        catch(int x)
        {
            cout << "Vectorul are lungimea " << x << endl;
        }
    }
};


int main()
{
    Meniu<Pizza> m;
    for(int i = 0; i < 2; i++)
    {
        Pizza *p = new Pizza();
        m += p;
    }

    m.afisare();

    return 0;
}
