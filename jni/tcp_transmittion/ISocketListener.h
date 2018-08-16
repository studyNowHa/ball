#ifndef ISCK_LISTENER
#define ISCK_LISTENER

class ISocketListener
{
public:
		virtual ~ISocketListener() { };
		virtual void notify(std::string msg) = 0;
};

class iiSocketListener : public ISocketListener
{
public:
	virtual void notify(std::string msg);
};

#endif
