#ifndef CODA_H
#define CODA_H

#include <algorithm>  // std::swap
#include <iostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
	@file coda.h
	@brief Dichiarazione della classe templata coda
*/


/**
	Classe eccezione custom che deriva da std::logic_error
	Viene generata quando si tenta di rimuovere un elemento oppure si tenta la lettura/scrittura 
	di _head o _tail da una coda vuota.

	@brief empty queue exception
*/
class empty_queue_exception : public std::logic_error {
public:
	/**
		Costruttore di default 
	*/
    empty_queue_exception() : std::logic_error("Empty Queue.") {}
};

template <typename T, typename E>
class coda {

	/**
		Struttura di supporto interna che implementa un elemento di una coda.

		@brief Elemento della coda.
	*/
    struct element{
        T value; // Valore dell'elemento specifico di una coda.
        element *next; // Puntatore all'elemento successivo della coda. Per successivivo si intende per ordine d'inserimento.
		element *prev; // Puntatore all'elemento precedente della coda. Per precedente si intende per ordine d'inserimento.
        
		/**
			Costruttore che inizializza l'elemento.
			@param v valore del dato
		*/
		explicit element(const T &v) 
			: value(v), next(nullptr), prev(nullptr) { }

		/**
			Distruttore
		*/
		~element() {
            next = nullptr;
			prev = nullptr;
        }
    }; // struct element

	/**
		Funzione helper per copiare una coda in un'altra

		@param e elemento da cui partire per copiare la coda
		@throw eccezione di allocazione di memoria
	*/
    void copy_helper(element *e) {
		try {
            if(e) {
                enqueue(e->value);
                copy_helper(e->next);
            }
		}
		catch(...) {
			clear();
			throw;
		}
    }
    
    /**
		Funzione helper per la rimozione ricorsiva degli elementi.

		@param e elemento da rimuovere 
	*/
	void clear_helper(element *e) {
		if(e) {
            clear_helper(e->next);
			delete e;
			e = nullptr;	
		}
	}

	element *_head; // Puntatore all'inizio della coda.
	element *_tail; // Puntatore alla fine della coda.
	unsigned int _size;	// Numero di elementi in una coda.
	
    E _equal; // Funtore per l'uguaglianza.
	
public:

	/**
		Costruttore di default
	*/
	coda() : _head(nullptr), _tail(nullptr), _size(0) { }

	/**
		Costruttore di copia

		@param other coda da copiare
		@throw eccezione di allocazione di memoria
	*/
	coda(const coda &other) : _head(nullptr), _tail(nullptr), _size(0) {
        copy_helper(other._head);
	}

	/**
		Operatore di assegnamento
		
		@param other coda da copiare
		@throw eccezione di allocazione di memoria
		@return reference a this
	*/
	coda &operator=(const coda &other) {
		if(this != &other) {
			coda tmp(other);
			std::swap(_head,tmp._head);
			std::swap(_tail,tmp._tail);
			std::swap(_size,tmp._size);
		}
		return *this;
	}

	/**
		Distruttore
	*/
	~coda() {
		clear();
	}

	/**
		Inserisce un elemento in una coda. L'elemento sarà inserito alla fine della coda.

		@param value valore da inserire
		@throw eccezione di allocazione di memoria
	*/
    void enqueue(const T &value) {
		element *tmp;
		
		try {
			tmp = new element(value);
		} catch (...) {
			throw;
		}
		
		if(!_head) {
			_head = tmp;
			_tail = tmp;
		}
		else {
			_tail->next = tmp;
			tmp->prev = _tail;
			_tail = tmp;
		}
        
        _size++;
    }
	
    /**
		Funzione che elimina un nodo dalla coda. 
		Verrà rimosso il primo elemento inserito per ordine di tempo. 
		Se la coda è vuota, viene lanciata un'eccezione.
		Ritorna il valore dell'elemento rimosso.
        
        @throw empty_queue_exception.
		@return il valore dell'elemento rimosso.
	**/
	T dequeue() {
		T del;
		
		if(_head) {
			if(_head == _tail) {
				del = _head->value;
				delete _head;
				_head = nullptr;;
				_tail = nullptr;
			}
			else {
				del = _head->value;
				this->_head = _head->next;
				delete _head->prev;
				_head->prev = nullptr;
			}
		}
		else {
			throw new empty_queue_exception();
		}
		
		_size--;
		return del;
	}

	/**
		Cancella i nodi dalla coda
	*/
	void clear() {
		clear_helper(_head);
		_head = nullptr;
		_tail = nullptr;
        _size = 0;
	}

	/**
		Ritorna il numero di elementi nella coda

		@return numero di elementi inseriti
	*/
	unsigned int size() const {
		return _size;
	}
	
	/**
		Stampa il valore degli elementi presenti in una coda
		dal più recente al meno recente
	*/
	void print() const {
		element *tmp = _head;
		while(tmp) {
			std::cout << tmp->value << " ";
			tmp = tmp->next;
		}
		std::cout << std::endl;
	}
	
	/**
		Ritorna il valore dell'elemento in testa alla coda
		Se la coda è vuota, viene lanciata un'eccezione
        
        @throw empty_queue_exception
		@return valore T della testa della coda
	*/
	T getHead() const {
		if(_head)
			return _head->value;
		else
			throw new empty_queue_exception();
	}
	
	/**
		Ritorna il valore dell'ultimo elemento inserito nella coda
		Se la coda è vuota, viene lanciata un'eccezione
        
        @throw empty_queue_exception
		@return valore T dell'ultimo elemento inserito nella coda
	*/
	T getTail() const {
		if(_tail)
			return _tail->value;
		else
			throw new empty_queue_exception();
	}
	
	/**
		Setta il il valore della testa con un nuovo valore
		Se la coda è vuota, viene lanciata un'eccezione
        
        @throw empty_queue_exception
		@param value con cui sostituire il valore della testa
	*/
	void setHead(const T &value) {
		if(_head)
			_head->value = value;
		else
			throw new empty_queue_exception();
	}
	
	/**
		Setta il il valore della coda con un nuovo valore
		Se la coda è vuota, viene lanciata un'eccezione
        
        @throw empty_queue_exception
		@param value con cui sostituire il valore della coda
	*/
	void setTail(const T &value) {
		if(_tail)
			_tail->value = value;
		else
			throw new empty_queue_exception();
	}
	
	/**
		Determina se esiste un elemento nella coda. L'uguaglianza e' definita dal funtore di uguaglianza

		@param value valore da cercare

		@return TRUE se esiste almeno un elemento con valore value
	*/
	bool search(const T &value) const {
		element *tmp = _head;
		
		while(tmp) {
			if(_equal(tmp->value, value))
				return true;
			tmp = tmp->next;
		}
		return false;
	}

	/**
		Costruttore secondario che costruisce la coda a partire da una sequenza generica di dati identificata da due iteratori.

		@param begin iteratore di inizio della sequenza
		@param end iteratore di fine della sequenza

		@throw eccezione di allocazione di memoria
	*/
	template <typename IterT>
	coda(IterT begin, IterT end) 
		: _head(nullptr), _tail(nullptr), _size(0) {

		try {
			while(begin != end) {
				enqueue(static_cast<T>(*begin));
				++begin;
			}
		}
		catch(...) {
			clear();
			throw;
		}
	}
	

	class iterator {
		element *_e;
		
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef T*                        pointer;
		typedef T&                        reference;

	
		iterator() : _e(nullptr) { }
		
		iterator(const iterator &other) : _e(other._e) { }

		iterator& operator=(const iterator &other) {
			_e = other._e;
			return *this;
		}

		~iterator() { }

		// Ritorna il dato riferito dall'iteratore (dereferenziamento)
		reference operator*() const {
			return _e->value;
		}

		// Ritorna il puntatore al dato riferito dall'iteratore
		pointer operator->() const {
			return &(_e->value);
		}

		// Operatore di iterazione post-incremento
		iterator operator++(int) {
			iterator tmp(*this);
			_e = _e->next;
			return tmp;
		}

		// Operatore di iterazione pre-incremento
		iterator& operator++() {
            _e = _e->next;
            return *this;
		}

		// Uguaglianza
		bool operator==(const iterator &other) const {
			return (_e == other._e);
		}

		// Diversita'
		bool operator!=(const iterator &other) const {
			return (_e != other._e);
		}

	private:
		//Dati membro

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class coda;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		iterator(element *e) : _e(e) { }
		
	}; // classe iterator
	
	
	// Ritorna l'iteratore all'inizio della sequenza dati
	iterator begin() {
		return iterator(_head);
	}
	
	// Ritorna l'iteratore alla fine della sequenza dati
	iterator end() {
		return iterator(nullptr);
	}
	
}; // classe coda


/**
	Ridefinizione dell'operatore di stream per la stampa del contenuto della coda

	@param os oggetto stream di output
	@param q1 coda da stampare

	@return reference allo stream di output
*/
template <typename T, typename E>
std::ostream &operator<<(std::ostream &os, 
	coda<T,E> &q1) {
	
	typename coda<T,E>::iterator i,ie;
	
	i = q1.begin();
	ie = q1.end();

	while(i!=ie) {
		os << *i << " ";
		++i;
	}

	return os;
}

/**
    @brief modifica di elementi di una coda che soddisfano un predicato con una funzione determinata da un operatore

	Modifica gli elementi che soddisfano il predicato pred

	@param q1 coda su cui verificare il predicato
	@param pred predicato da soddisfare
	@param oper operatore con cui si andrà eventualmente a modificare la coda
*/
template <typename T, typename E, typename P, typename F>
void transformif(coda<T,E> &q1, P pred, F oper) {
	
	typename coda<T,E>::iterator i,ie;

	i = q1.begin();
	ie = q1.end();
	
	while(i!=ie) {
		if(pred(*i)) {
			*i = oper(*i);
		}
		++i;
	}
}


#endif