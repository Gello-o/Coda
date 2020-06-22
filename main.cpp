#include <iostream>
#include "coda.h"
#include <cassert> // assert

/**
	Funtore per l'uguaglianza tra numeri interi.

	@brief Funtore per il confronto tra numeri interi.
*/
struct equal_int {
	bool operator()(int a, int b) const {
		return a==b;
	}
};

/**
	Funtore per l'uguaglianza tra stringhe. 

	@brief Funtore per il confronto tra stringhe.
*/
struct equal_string {
	bool operator()(const std::string &a, const std::string &b) const {
		return (a==b);
	} 
};

/**
	Struct point che implementa un punto 2D.

	@brief Struct point che implementa un punto 2D.
*/
struct point {
	int x; //Cordinata punto x
	int y; //Cordinata punto y

	point(int xx, int yy) : x(xx), y(yy) {}
};

/**
	Funtore per il confronto sull'uguaglianza tra due punti.
    Ritorna TRUE se p1.x = p2.x e p1.y = p2.y

	@brief Funtore per il confronto di due punti.
*/
struct equal_point {
	bool operator()(const point &p1, const point &p2) const {
		return (p1.x==p2.x) && (p1.y==p2.y);
	} 
};

/**
	Ridefinizione dell'operatore di stream << per un point.
	Necessario per l'operatore di stream della classe coda.
*/
std::ostream &operator<<(std::ostream &os, const point &p) {
	std::cout<<"("<<p.x<<","<<p.y<<")";
	return os;
}


/**
	Predicato utilizzato per la funzione globale transformif.
    Ritorna TRUE se il valore a (intero) è minore di 20 e maggiore di 10.

	@brief Funtore per il confronto di due interi.
*/
struct funct {
	bool operator()(int a) const {
		return ((a<20) && (a>10));
	}
};

/**
	Operatore utilizzato per la funzione globale transformif.
    Ritorna il valore a (intero) raddoppiato.

	@brief Funtore per raddoppiare un intero.
*/
struct doubling {
	int operator()(int a) const {
		return 2*a;
	}
};

typedef coda<int, equal_int> qint;
typedef coda<std::string, equal_string> qstring;
typedef coda<point, equal_point> qpoint;

void test_constructor() {
	std::cout << "****** Test costruttori e stampa di valori interi ******" << std::endl;

	qint q1;
	std::cout<<"Inserimento dei valori 5, 15, 10, 27, 34, 2, 9";
    q1.enqueue(5);
    q1.enqueue(15);
    q1.enqueue(10);
    q1.enqueue(27);
    q1.enqueue(34);
    q1.enqueue(2);
    q1.enqueue(9);

	std::cout << std::endl << "PRINT (q1)" << std::endl;
    q1.print();
    assert(q1.size()==7);
	
	qint q2(q1);
	std::cout << "PRINT (q2)" << std::endl;
    q2.print();
    assert(q2.size()==q1.size());
	
	qint q3;
	q3 = q1;	
	std::cout << "PRINT (q3)" << std::endl;
    q3.print();
    assert(q3.size()==q1.size());
	
	std::cout << "PRINT (q4)" << std::endl;
	int a[7] = {5, 15, 10, 27, 34, 2, 9};
	qint q4(a, a+7);
	q4.print();
	
};

void test_gestione_coda() {
	std::cout << std::endl << "****** Test sulla gestione della coda di valori interi ******"<<std::endl;

	qint q1;
	std::cout<<"Inserimento dei valori 20, 15, 10, 17, 25, 30, 23";
    q1.enqueue(20);
    q1.enqueue(15);
    q1.enqueue(10);
    q1.enqueue(17);
    q1.enqueue(25);
    q1.enqueue(30);
    q1.enqueue(23);
	
	bool flag1 = q1.search(20);
	assert(flag1 == true);
	std::cout << std::endl << "Ricerca del nodo con valore 20: " << flag1 << std::endl;

	bool flag2 = q1.search(-20);
	assert(flag2 == false);
	std::cout << "Ricerca del nodo con valore -20: " << flag2 << std::endl;

	qint::iterator i,ie;
	std::cout<<"Stampa con iteratori: "<<std::endl;
	for(i=q1.begin(),ie=q1.end(); i!=ie; ++i)
		std::cout << *i << " ";
	
	std::cout << std::endl << "Testa: " << q1.getHead();
	std::cout << std::endl << "Coda: " << q1.getTail();
	
	std::cout << std::endl << "Modifica (testa = 70, coda = 50)" << std::endl;
	q1.setHead(70);
	q1.setTail(50);

	std::cout << "Stampa con operator<<" << std::endl;
	std::cout << q1 << std::endl;
	
	std::cout << "Rimozione dalla coda e aggiunta elemento di valore 100" << std::endl;
	assert(q1.size()==7);
	q1.dequeue();
	assert(q1.size()==6);
	q1.enqueue(100);
	assert(q1.size()==7);

	std::cout << "Stampa con operator<<" << std::endl;
	std::cout << q1 << std::endl;
}


void test_predicate() {
	std::cout << std::endl << "****** Test sul predicato transformif per valori interi ******" << std::endl;

	qint q1;
	std::cout<<"Inserimento dei valori 23, 15, 9, 17, 16, 21, 19" << std::endl;
    q1.enqueue(23);
    q1.enqueue(15);
    q1.enqueue(9);
    q1.enqueue(17);
    q1.enqueue(16);
    q1.enqueue(21);
    q1.enqueue(19);
    
    std::cout << "Stampa di valori (con <20 e >10 raddoppiati): " << std::endl;
    transformif(q1, funct(), doubling());
	q1.print();
}


void test_string() {
	std::cout << std::endl << "****** Test su una coda di stringhe ******"<<std::endl;

	qstring q1;
	std::cout<<"Inserimento dei valori mia, marsellus, jules, vincent";
    q1.enqueue("mia");
    q1.enqueue("marsellus");
    q1.enqueue("jules");
    q1.enqueue("vincent");
	
	bool flag1 = q1.search("jules");
	assert(flag1 == true);
	std::cout << std::endl << "Ricerca del nodo con valore jules: " << flag1 << std::endl;

	bool flag2 = q1.search("jimmie");
	assert(flag2 == false);
	std::cout << "Ricerca del nodo con valore jimmie: " << flag2 << std::endl;
	
	std::cout << "Rimozione dalla coda" << std::endl;
	q1.dequeue();

	qstring::iterator i,ie;
	std::cout<<"Stampa con iteratori: "<<std::endl;
	for(i=q1.begin(),ie=q1.end(); i!=ie; ++i)
		std::cout << *i << " ";
	std::cout << std::endl;
}

void test_point() {
	std::cout << std::endl << "****** Test su una coda di valori point ******"<<std::endl;

	qpoint q1;
	std::cout<<"Inserimento dei valori (1,1), (1,2), (2,7), (0,0), (5,4), (2,4), (1,0)"<<std::endl;
	q1.enqueue(point(1,1));
	q1.enqueue(point(1,2));
	q1.enqueue(point(2,7));
	q1.enqueue(point(0,0));
	q1.enqueue(point(5,4));
	q1.enqueue(point(2,4));
	q1.enqueue(point(1,0));
	
	std::cout<<"Stampa con iteratori: "<<std::endl;
	qpoint::iterator i,ie;
	for(i=q1.begin(),ie=q1.end(); i!=ie; ++i)
		std::cout<<*i<< " ";

	std::cout << std::endl << "Dimensione coda: " << q1.size() << std::endl;
	
	std::cout << "Testa: " << q1.getHead() << std::endl;
	std::cout << "Coda: " << q1.getTail() << std::endl;
	
	std::cout << "Modifica (testa = (9,4), coda = (7,-6))" << std::endl;
	q1.setHead(point(9,4));
	q1.setTail(point(7,-6));
	std::cout << "Stampa con operator<<" << std::endl;
	std::cout << q1 << std::endl;
}

int main() {
    qint q1;
	
	test_constructor();
	test_gestione_coda();
	test_predicate();
	test_string();
	test_point();
	return 0;
}