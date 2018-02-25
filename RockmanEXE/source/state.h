#pragma once

#ifndef NULL
#define NULL (0)
#endif // !NULL


class StateBase{
public:
	virtual ~StateBase(){}

	virtual void Draw(){}
	virtual void Process() = 0;
};

class StateMgr{
	StateBase *now, *before;
public:
	StateMgr():now(NULL), before(NULL){}
	StateMgr(StateBase *initState):now(initState), before(NULL){}
	~StateMgr(){
		if( before )
			delete before;
		if( now )
			delete now;
	}

	void Draw(){
		if( now )
			now->Draw();
	}

	void Process(){
		if( now )
			now->Process();
	}

	void ChangeNext(StateBase *next){
		if( now )
			delete now;
		now = next;
	}

	void GotoBack(){
		if( before ){
			delete now;
			now = before;
			before = NULL;
		}
	}

	void SetBack(){
		if( before )
			delete before;
		before = now;
		now = NULL;
	}
};
