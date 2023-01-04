//The libraries i included
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<stdlib.h>
#include<math.h>

using namespace std;

//A function that helps me copy the input of a CSV into another CSV - used in create() function
void copyCsv(const std::string &inputFilename, const std::string &outputFilename)
{
    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);

    std::string line;
    while (std::getline(inputFile, line))
    {
        outputFile << line << '\n';
    }

    inputFile.close();
    outputFile.close();
}

//The template which helps me in the calculation of the rooms needed. If the number of people dont divide by the max number of people per room we need an extra room. I added exception handlers as well regarding dividin by 0.
template <typename X>
int div(X x, X y)
{
    try
    {
        if (y == 0)
        {
            throw invalid_argument("Division by zero.");
        }

        if (x % y != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    catch (const invalid_argument &e)
    {
        cerr << e.what() << '\n';
        return -1;
    }
    catch (const exception &e)
    {
        cerr << "An error occurred: " << e.what() << '\n';
        return -1;
    }
}

//The function that creatts the CSV with the cast in English. I initially had a CSV without the extras. I filled it with extras by adding randomly names from the names.csv.Then after that I managed to make the function to not modify the full cast CSV(wednesdayCast.csv) by adding commands as the one i will point with comments att
void create()
{
    fstream fout, fin, gout;

    int count = 1, i;
    string line, word;

    vector<string> row;
    //the commands that help at mentaining the CSV still
    remove("wednesdayCast.csv");
    copyCsv("wednesday_fara_extra.csv", "wednesdayCast.csv");
    copyCsv("names.csv","names_copy.csv");

    int numar_random, nr_elem=132;
    while(nr_elem!=0)
    {
        gout.open("wednesdayCast.csv", ios::out | ios::app);
        fin.open("names.csv", ios::in);

        if (fin.is_open())   /* ok, proceed with output */
        {

            fout.open("namesnew.csv", ios::out);
            numar_random=rand() % nr_elem;

            count=0;
            while (!fin.eof())
            {

                row.clear();
                getline(fin, line);
                stringstream s(line);

                while (getline(s, word, ','))
                {
                    row.push_back(word);
                }
                int row_size = row.size();

                if(count!=numar_random)
                {
                    if (!fin.eof())
                    {
                        fout << row[0] << "\n";
                    }
                }
                else
                {
                    gout<< row[0]<<",extra"<<"\n";
                }
                count++;
                if (fin.eof())
                    break;
            }

            fin.close();
            fout.close();
            gout.close();
            remove("names.csv");
            // renaming the new file with the existing file name
            rename("namesnew.csv", "names.csv");
            nr_elem--;

        }
        else
        {
            cout<<"Procesul a fost deja facut sau fisierul cu nume nu exista"<<"\n";
            break;
        }
    }
    remove("names.csv");
    rename("names_copy.csv","names.csv");
}

//Prima clasa Producer care contine atributele protected name, job and eater(the type of regime they have). I used this class to declare in main all the backstage(technicians, writers,etc) as an array of objects
class Producer
{
public:
//default constructor
    Producer()
    {

    }
//default destructor
    ~Producer() {};
//a function that creates the backstage_array of objects
    friend int backstage_array(Producer *backstage);
    friend class Personage;
//another constructor
    Producer(string name, string job, string eater)
    {
        this->name = name;
        this->job = job;
        this->eater = eater;
    }
//setters
    void setName(string n)
    {
        this->name=n;
    }
    void setJob(string j)
    {
        this->job=j;
    }
    void setEater(string e)
    {
        this->eater=e;
    }
    //getters
    string getName()
    {
        return this->name;
    }

    string getJob()
    {
        return this->job;
    }
    string getEater()
    {
        return this->eater;
    }
    void getAll()
    {
        cout<<this->name<<" | "<<this->job<<" | "<<this->eater<<endl;
    }

private:
    string name, job, eater;
};

//the next class is Personage, basically the people that appear on the screen, actors and extras. I made them as a child class because they have an extra attribute, the role they are playing
class Personage : public Producer
{
private:
    string role;
public:
    friend int players_array(Personage *players, int *v, int *w, int *s, int *p, int *h);
    friend class Cost;
    Personage() {};
    ~Personage() {};
    void setRole(string r)
    {
        this->role=r;
    }
    Personage(string nume, string job, string eater, string r)
    {
        setName(nume);
        setJob(job);
        setEater(eater);
        this->role=r;
    }

//getter
    string getRole()
    {
        return this->role;
    }
    void getAll()
    {
        cout<<this->name<<" | "<<this->job<<" | "<<this->eater<<" | "<<this->role<<endl;
    }
//setter

};
//functie cu scopul de a crea vectorul de obiecte preluand fiecare linie din csv-ul cu castul cu conditia sa nu fie figuranti sau actori
//De asemenea transmite mai departe numarul de oameni din backstage care sunt normali, vegetarieni si pescarieni, fiecare in parte
int backstage_array(Producer *backstage, int *b_n, int *b_v, int *b_p)
{
    int bn=0, bv=0, bp=0;
    int i,count=0, random_value;
    string eating[3]= {"Normal","Vegetarian","Pescarian"};

    ifstream inputFile;
    inputFile.open("wednesdayCast.csv");
    string line = "";

    while (getline(inputFile, line))
    {

        stringstream inputString(line);

        string n;
        string j;
        getline(inputString, n, ',');
        getline(inputString, j, ',');

        line = "";
        if(j!="extra" && j!="actor/actress")
        {
            backstage[count].setJob(j);
            backstage[count].setName(n);

            random_value=rand() % 3;
            switch(random_value)
            {
            case 0:
                bn++;
                break;
            case 1:
                bv++;
                break;
            case 2:
                bp++;
                break;
            }

            backstage[count].setEater(eating[random_value]);
            count++;
        }
    }
    *b_n=bn;
    *b_v=bv;
    *b_p=bp;
    return count;
}

//functie care creeaza vectorul de obiecte Personage, adica actorii si figurantii, si trimite totodata numarul de vampiri,
//varcolaci,sirene, clarvazatori, oameni, oameni cu regim normal, oameni cu regim vegetarian, oameni cu regim pescarian
int players_array(Personage *players, int *v, int *w, int *s, int *p, int *h, int *p_n, int *p_v, int *p_p)
{
    int v1=0, w1=0, s1=0, p1=0,h1=0, pn=0, pv=0, pp=0;

    int i,count=0, random_value;
    string eating[3]= {"Normal","Vegetarian","Pescarian"};
    string makeup[5]= {"Vampire","Werewolf","Siren","Psychic","Human"};
    ifstream inputFile;
    inputFile.open("wednesdayCast.csv");
    string line = "";

    while (getline(inputFile, line))
    {

        stringstream inputString(line);

        string n;
        string j;
        getline(inputString, n, ',');
        getline(inputString, j, ',');

        line = "";
        if(j=="extra" || j=="actor/actress")
        {
            players[count].setJob(j);
            players[count].setName(n);
            random_value=rand() % 5;
            players[count].setRole(makeup[random_value]);
            switch(random_value)
            {
            case 0:
                v1++;
                break;
            case 1:
                w1++;
                break;
            case 2:
                s1++;
                break;
            case 3:
                p1++;
                break;
            case 4:
                h1++;
                break;
            }
            random_value=rand() % 3;
            switch(random_value)
            {
            case 0:
                pn++;
                break;
            case 1:
                pv++;
                break;
            case 2:
                pp++;
                break;
            }
            players[count].setEater(eating[random_value]);
            count++;
        }
    }
    *v=v1;
    *w=w1;
    *s=s1;
    *p=p1;
    *h=h1;
    *p_n=pn;
    *p_v=pv;
    *p_p=pp;
    return count;
}

//clasa Cost cu fiecare cost in parte ce trebuie trecut in tabel in relatie cu numarul de zile respectiv
class Cost
{
private:
    double f_d;
    double transport;
    double hotel;
    double makeup;
    double rent;
    int time;

public:
//constructor default
    Cost() {};
//constructor
    Cost(double f, double t, double h, double m, double r)
    {
        this->f_d=f;
        this->transport=t;
        this->hotel=h;
        this->makeup=m;
        this->rent=r;
    }

    //getters
    double getTrans()
    {
        return this->transport;
    }
    int getTime()
    {
        return this->time;
    }
    double getMakeup()
    {
        return this->makeup;
    }

    double getRent()
    {
        return this->rent;
    }
    double getHotel()
    {
        return this->hotel;
    }
    double getF()
    {
        return this->f_d;
    }
//functie care creeaza vectorul de costuri pentru fiecare interval de timp
    friend void cost_array(Cost *price, int v, int w, int s, int p, int h, int n_a, int n_b, int n_f, int e_n, int e_v, int e_p)
    {
        //intervalele de timp initializate
        price[0].time=30;
        price[1].time=45;
        price[2].time=60;
        price[3].time=100;
        int i;
        //calculam numaul total de oameni prin adunarea numarul persoanelor din fiecare categorie
        int total_people=n_a+n_b+n_f;


        for(i=0; i<4; i++)
        {
            //numarul total de persoane impartit la numarul maxim de locuri per autocar si *2 din cauza la intors
            price[i].transport=((int)(total_people/50)+1)*2*5680;

            //Conversie lei in dolari | 1 dolar = 4.62 lei
            price[i].transport=(double)(price[i].transport/4.62*price[i].time);

            //inmultim pretul machiajului cu numarul de oameni pentru fiecare categorie in parte, adunam si rezultatul il inmultim cu numarul de zile
            price[i].makeup=(v*230+w*555+s*345+p*157+h*55)*price[i].time;
            //conversie lei in dolari | 1 dolar = 4.62 lei
            price[i].makeup=price[i].makeup/(double)(4.62);

            //De vreme ce proprietarul conacului ofera o reducere de 2% la fiecare 10 zile => se vor plati 98000 lei la fiecare 10 zile
            //si pretul normal de 100000/zi in rest
            price[i].rent=(price[i].time/10)*98000+10000*(price[i].time%10);
            //Conversie lei in dolari | 1 dolar = 4.62 lei
            price[i].rent=price[i].rent/(double)(4.62);

            //Pentru inchirierea camerei de hotel m-am folosit de template pentru a-mi da seama daca numarul de actori adunat cu backstage se imparte exact sau
            //nu la 2 pentru a determina daca se inchiriaza o camera in plus sau nu pentru persoana ramasa pe dinafara daca exista
            //Astfel functia daca exista rest returneaza 1 si se va adauga 1 la numarul de camere si 0 in schimb.
            //Acelasi lucru se intampla si pentru figuranti, iar rezultatele se aduna dupa ce au fost inmultite cu pretul per camera fiecare
            price[i].hotel=(((n_a+n_b)/2)+div<int>(n_a+n_b, 2))*350+((n_f/3)+div<int>(n_f,3))*420;
            //Conversie lei in dolari | 1 dolar = 4.62 lei
            price[i].hotel=price[i].hotel*price[i].time/(double)(4.62);

            //2l apa =6 lei => O persoana consuma 1 litru => 3 lei/persoana
            //1l cafea= 30lei => o persoana consuma 0.5 litri => 15 lei/persoana
            //2l suc=8 lei => o persoana consuma 0.8 litri => 3.2 lei/persoana
            //Inmultim numarul de persoane total cu suma preturile pentru fiecare persoana
            //Pentru mancare se aduna produsele dintre numarul de persoane ce apartin fiecarui regim in parte cu costul meniului respectiv
            price[i].f_d=((3+15+3.2)*total_people)+(e_n*40)+(e_v*33)+(e_p*46);
            //Se inmulteste cu numarul de zile
            //Conversie lei in dolari | 1 dolar = 4.62 lei
            price[i].f_d=price[i].f_d*price[i].time/(double)(4.62);
        }


    }

    //functie ce creaza csv-urile cu costurile in dolari in engleza cat si romana
    friend void cost_csv(Cost *price)
    {
        fstream fout, gout;
        int i;
        double total=0;
        fout.open("cost.csv", ios::out);
        gout.open("cost_ro.csv",ios::out);
        fout<<"Days,Transport($),Hotel($),Makeup($),Food & Drinks($),Rent($),Total($)\n";
        gout<<"Zile,Transport($),Hotel($),Makeup($),Mancare & Bauturi($),Chirie($),Total($)\n";
        for(i=0; i<4; i++)
        {
            total=price[i].getF()+price[i].getMakeup()+price[i].getTrans()+price[i].getHotel()+price[i].getRent();
            fout<<price[i].getTime()<<","<<price[i].getTrans()<<","<<price[i].getHotel()<<","<<price[i].getMakeup()<<","<<price[i].getF()<<","<<price[i].getRent()<<","<<total<<"\n";
            gout<<price[i].getTime()<<","<<price[i].getTrans()<<","<<price[i].getHotel()<<","<<price[i].getMakeup()<<","<<price[i].getF()<<","<<price[i].getRent()<<","<<total<<"\n";
        }

        fout.close();
        gout.close();
    }
};

//o clasa abstracta Menu cu scopul de a reprezenta caracteristicile meniului principal
class Menu
{
public:

    string soup;
    string main;
    string dessert;
    //instructiunea ce ofera calitatea abstracta
    virtual void abstract()=0;

};
//clasa copil clasei Menu la care am adaugat ziua de servire, tipul regimului careia i se aplica si pretul respectiv
class Submenu : public Menu
{
public:
    int day;
    string type;
    double price;
//constructor default
    Submenu() {};
//functie cu rol de constructor
    void add(int d, string t, string s, string m, string di, double p)
    {
        this->type=t;
        this->day=d;
        this->soup=s;
        this->main=m;
        this->dessert=di;
        this->price=p;
    }
//crearea csv-ului in engleza in urma initializarii vectorului de obiecte in main
    friend void add_submenu(Submenu *submenu)
    {
        fstream fout;
        fout.open("menu.csv", ios::out);
        fout<<"Day,Type,Soup,Main Course,Dessert,Price($)\n";
        int i;
        for(i=0; i<9; i++)
            fout<<submenu[i].day<<","<<submenu[i].type<<","<<submenu[i].soup<<","<<submenu[i].main<<","<<submenu[i].dessert<<","<<submenu[i].price<<"\n";
        fout.close();
    }
    //o functie care sa demonstreze polimorfismul prin overriding functie abstract definita in clasa parinte
    void abstract()
    {
        cout<<"Not abstract anymore";
    }
};


int main()
{
    int nr_players, nr_backstage,v,w,s,p,h,b_n,b_v,b_p,p_n,p_v,p_p;
//pun comanda care a creat tabelul cu tot cu figuranti
    create();

    //Initializam vectorul de backstage(sunetisti, scriitori, etc)
    Producer backstage[800];
    //alam numarul de oameni din backstage prin apelarea functiei de mai jos, totodata si numarul de oameni din cadrul fiecarui regim din backstage
    nr_backstage=backstage_array(backstage, &b_n, &b_v, &b_p);

    //Initializam vectorul de jucatori(actori si figuranti)
    Personage players[800];
    //aflam numarul de players prin apelarea functiei de mai jos impreuna cu numarul de vampiri, etc; numarul jucatorilor pentru fiecare regim
    nr_players=players_array(players,&v,&w,&s,&p,&h,&p_n,&p_v,&p_p);


//Nr de fig este cunoscut din cerinta problemei
    int nr_fig=132,nr_act;
//Numarul de actori se calculeaza din diferenta numarului de players - cel de figuranti
    nr_act=nr_players-nr_fig;
cout<<"Exemplu de element din vectorul de backstage:"<<endl;
    backstage[1].getAll();
cout<<"Exemplu de element din vectorul de players:"<<endl;
    players[1].getAll();

cout<<endl;

cout<<"Numarul de actori: "<<nr_act<<endl;
cout<<"Numarul de figuranti: "<<nr_fig<<endl;
cout<<"Numarul de players: "<<nr_players<<endl;
cout<<"Numarul de oameni din backstage: "<<nr_backstage<<endl;
cout<<"Numarul total de oameni: "<<nr_players+nr_backstage<<endl<<endl;

cout<<"Numarul de vampiri: "<<v<<endl;
cout<<"numarul de varcolaci: "<<w<<endl;
cout<<"Numarul de sirene: "<<s<<endl;
cout<<"Numarul de clarvazatori: "<<p<<endl;
cout<<"Numarul de oameni: "<<h<<endl<<endl;

cout<<"Numarul total de oameni cu regim normal: "<<p_n+b_n<<endl;
cout<<"Numarul total de oameni cu regim vegetarian: "<<p_v+b_v<<endl;
cout<<"Numarul total de oameni cu regim pescarian: "<<p_p+b_p<<endl;

//Facem tabelul cu costuri intai definim un vector de obiecte gol
    Cost price[7];

//bagam in vector elementele necesare
//v=nr vampiri
//w=nr varcolaci
//s=nr sirene
//p=nr clarvazatori
//h=nr oameni
//p_n+b_n=numarul de actori, figuranti si backstage normali
//p_v+b_v=numarul de actori, figuranti si backstage vegetarieni
//p_p+b_v=numarul de actori, figuranti si backstage pescarieni
    cost_array(price, v, w, s, p, h, nr_players-nr_fig, nr_backstage, nr_fig,p_n+b_n, p_v+b_v, p_p+b_p);

//transpunerea vectorului de obiecte in CSV
    cost_csv(price);

//Facem meniul de mancare
    Submenu menu[10];
    menu[0].add(1,"Normal","Split Pea Soup","Burger","Tiramisu",40/4.62);

    menu[1].add(1,"Vegetarian", "Asparagus Soup", "Vegan Burger", "Blueberry Pie", 33/4.62);

    menu[2].add(1, "Pescarian", "Clam Chowder", "Fish & Chips", "Cheesecake",46/4.62);

    menu[3].add(2, "Normal", "Chicken Noodle Soup", "Lasagna", "Apple Pie",40/4.62);

    menu[4].add(2, "Vegetarian", "Pumpkin Soup", "Falafel", "Cinnamon Rolls",33/4.62);

    menu[5].add(2, "Pescarian", "Lobster Bisque", "Baked Gnocchi with Broccoli", "Frozen Yogurt",46/4.62);

    menu[6].add(3, "Normal", "Italian Sausage and Bean Soup", "Pizza Margherita", "Ice Cream Cake",40/4.62);

    menu[7].add(3, "Vegetarian", "Tomato Soup", "Pasta with Tomato Cream Sauce", "Lemon Pie",33/4.62);

    menu[8].add(3, "Pescarian", "Mediterranean Style Fish Soup", "Tuna Pizza", "Cherry Pie",46/4.62);

    //punem vectorul menu in csv
    add_submenu(menu);

    return 0;
}
