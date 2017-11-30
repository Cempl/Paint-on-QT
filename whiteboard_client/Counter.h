#ifndef COUNTER_H
#define COUNTER_H


class Counter
{
	private:

		Counter()
		:
			mCommandCounter(0)
		{}

	public:

		static Counter& getInstance() 
		{
			static Counter  instance;
			return instance;
		}

	int mCommandCounter;
};

#endif // COUNTER_H