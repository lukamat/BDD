#ifndef _DEFS_H
#define _DEFS_H

#include <iostream>
#include <memory>
#include <set>
#include <map>

using namespace std;
const int TABLE_SIZE = 128;

class BDD_class;
typedef shared_ptr<BDD_class> BDD;

#define PAIR( a , b )( ( unsigned int ) ( ( ( ( unsigned int ) a ) +((unsigned int ) b ) ) ∗ ( ( ( unsigned int ) a ) +(( unsigned int ) b ) +((unsigned int ) 1 ) ) / ( ( unsigned int ) 2 ) +(( unsigned int ) a ) ) )

#define TRIPLE( a , b , c ) ( ( unsigned int ) (PAIR ( ( unsigned int ) c , PAIR(a , b) ) ) )

#define NODEHASH( lvl , l , h )(TRIPLE ( ( lvl ) , ( l ) , ( h ) ) %bddnodesize ) ;
//--------------------------------------------------
class HashEntry {

private:
      int key;
      BDD value;
public:

      HashEntry(int key, BDD value) {
            this->key = key;
            this->value = value;
      }

       int getKey() {
            return key;
      }

      BDD getValue() {
            return value;
      }
};
//--------------------------------------------------

class HashMap {

private:
      HashEntry **table;

public:
      HashMap() {
            table = new HashEntry*[TABLE_SIZE];
            for (int i = 0; i < TABLE_SIZE; i++)
                  table[i] = NULL;
      }

      BDD get(int key) {
            int hash = (key % TABLE_SIZE);
            while (table[hash] != NULL && table[hash]->getKey() != key)
                  hash = (hash + 1) % TABLE_SIZE;
            if (table[hash] == NULL)
                  return NULL;
            else
                  return table[hash]->getValue();
      }

 

      void put(int key, BDD & value) {
            int hash = (key % TABLE_SIZE);
            while (table[hash] != NULL && table[hash]->getKey() != key)
                  hash = (hash + 1) % TABLE_SIZE;
            if (table[hash] == NULL)
                  table[hash] = new HashEntry(key, value);
      }     

       ~HashMap() {
            for (int i = 0; i < TABLE_SIZE; i++)
                  if (table[i] != NULL)
                        delete table[i];
            delete[] table;
      }

};

//--------------------------------------------------

//HashMap BDDTable;

//--------------------------------------------------
class BaseFormula;
typedef shared_ptr<BaseFormula> Formula;

//--------------------------------------------------
class BaseFormula : public enable_shared_from_this<BaseFormula> {

public:
  enum Type { T_TRUE, T_FALSE, T_ATOM, T_NOT, T_AND, T_OR, T_IMP, T_IFF };
  
  virtual Type getType() const = 0;
  virtual void printFormula(ostream & ostr) const = 0;
};
//--------------------------------------------------

class AtomicFormula : public BaseFormula {};

//--------------------------------------------------

class LogicConstant : public AtomicFormula {};
//--------------------------------------------------

class True  : public LogicConstant {

public:
	True(){};

virtual Type getType() const {return T_TRUE;}

virtual void printFormula(ostream & ostr) const 
  {
    ostr << "TRUE";
  }
};

//--------------------------------------------------

class False  : public LogicConstant {
public:
	False(){};

virtual Type getType() const {return T_FALSE;}

virtual void printFormula(ostream & ostr) const 
  {
    ostr << "FALSE";
  }

};

//--------------------------------------------------

class Atom : public AtomicFormula {
private:
  string var;
public:
   Atom(string n)
    :var(n)
  {}

virtual Type getType() const {return T_ATOM;}

virtual void printFormula(ostream & ostr) const 
  {
    ostr << var;
  }

};
//--------------------------------------------------

class UnaryConjective : public BaseFormula {
protected:
  Formula _op;
public:
  UnaryConjective(const Formula & op)
    :_op(op)
  {}

};
//--------------------------------------------------


class Not : public UnaryConjective {
public:
 Not(const Formula & op)
    :UnaryConjective(op)
  {}


virtual Type getType() const {return T_NOT;}

 virtual void printFormula(ostream & ostr) const 
  {
   ostr << "~(";
    _op->printFormula(ostr);
    ostr << ")";
  }
};

//--------------------------------------------------

class BinaryConjective : public BaseFormula {
protected:
  Formula _op1;
  Formula _op2;
public:
 BinaryConjective(const Formula & op1, 
		   const Formula & op2)
    :_op1(op1),
     _op2(op2)
  {}



};

//--------------------------------------------------

class And : public BinaryConjective {
public:
  And(const Formula & op1, 
      const Formula & op2)
    :BinaryConjective(op1, op2)
  {}


virtual Type getType() const {return T_AND;}

 virtual void printFormula(ostream & ostr) const 
  {
    ostr << "(";
    _op1->printFormula(ostr);
    ostr << " /\\ ";
    _op2->printFormula(ostr);
    ostr << ")";
  }



};

//--------------------------------------------------

class Or : public BinaryConjective {
public:
  Or(const Formula & op1, 
      const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

virtual Type getType() const {return T_OR;}

virtual void printFormula(ostream & ostr) const 
  {
    ostr << "(";
    _op1->printFormula(ostr);
    ostr << " \\/ ";
    _op2->printFormula(ostr);
    ostr << ")";
  }

};
//--------------------------------------------------

class Imp : public BinaryConjective {
public:
  Imp(const Formula & op1, 
      const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

virtual Type getType() const {return T_IMP;}

virtual void printFormula(ostream & ostr) const 
  {
    ostr << "(";
    _op1->printFormula(ostr);
    ostr << " => ";
    _op2->printFormula(ostr);
    ostr << ")";
  }

};

//--------------------------------------------------

class Iff : public BinaryConjective {
public:
  Iff(const Formula & op1, 
      const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

virtual Type getType() const {return T_IFF;}

virtual void printFormula(ostream & ostr) const 
  {
    ostr << "(";
    _op1->printFormula(ostr);
    ostr << " <=> ";
    _op2->printFormula(ostr);
    ostr << ")";
  }

};

//--------------------------------------------------
// If-Then-Else normal form
//--------------------------------------------------
/*
class ITE_NF;
typedef shared_ptr<ITE_NF> ITE;

class ITE_NF : public enable_shared_from_this<ITE_NF>{
private:
ITE IF;
ITE THEN;
ITE ELSE;

public:

ITE_NF(ITE & _IF, ITE & _THEN, ITE & _ELSE){
IF = _IF;
THEN = _THEN;
ELSE = _ELSE;

}

void print() const{
	cout << "ITE(" ;
	IF->print() ;
	cout << ",";
	THEN->print();
	cout << ",";
	ELSE->print();
	cout << ")";
 
	};


};

//--------------------------------------------------


class ITE_VAR : public ITE_NF{

}; */

//--------------------------------------------------
// BDD
//--------------------------------------------------


class BDD_class : public enable_shared_from_this<BDD_class>{
//zameniti sa private !!!
public: 
int index;
BDD high;
BDD low;
BDD next;

public:

BDD_class (int _index, BDD & _high, BDD & _low, BDD & _next){
index = _index;
high = _high;
low = _low;
next = _next;

}

BDD_class (int _index, BDD & _high, BDD & _low){
index = _index;
high = _high;
low = _low;
next = NULL;

}


};
//--------------------------------------------------
// funkcije
//--------------------------------------------------
/*
BDD bdd_makenode ( int var , BDD & high , BDD & low ) {

unsigned int hash = NODEHASH( var , high , low ) ;

if ( BDDTable.get(hash) == NULL ) {

BDDTable.put(hash, new bdd ( var , high , low ) );
return BDDTable.get(hash);

} else {

BDD node = BDDTable.get(hash) ;

while ( (node−>next != NULL) && ( node−>index != var || node−>high != high || node−>low != low ) )

	node = node−>next ;


if ( node−>index != var || node−>high != high || node−>low != low ) {

// make new node and add t o b u c k e t

BDD newNode = BDD(new BDD_class ( var , high , low )) ;

//node−>next = newNode ;

return newNode ;

} else {

// found matching node i n b u c k e t
return node ;

}

}
 }
*/
//--------------------------------------------------
inline
ostream & operator << (ostream & ostr, const Formula & f)
{
  f->printFormula(ostr);
}

extern Formula parsed_formula;


#endif
