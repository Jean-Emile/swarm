#ifndef OBSERVATEUR_H_
#define OBSERVATEUR_H_

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

class Observable;

typedef int Info;

class Observateur
{
protected:
	std::list<Observable*> m_list;
	typedef std::list<Observable*>::iterator iterator; 
	typedef std::list<Observable*>::const_iterator const_iterator;
	virtual ~Observateur() = 0;
public:
	virtual void Update(const Observable* observable) const ;

	void AddObs(Observable* obs);
	void DelObs(Observable* obs);
};

class Observable
{
	std::list<Observateur*> m_list;

	typedef std::list<Observateur*>::iterator iterator; 
	typedef std::list<Observateur*>::const_iterator const_iterator;

public:
	void AddObs( Observateur* obs);
	void DelObs(Observateur* obs);

	virtual ~Observable();
protected:
	void Notify(void);

};

#endif /*OBSERVATEUR_H_*/
