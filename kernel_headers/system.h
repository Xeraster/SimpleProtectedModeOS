extern void main2();

extern "C" void systemCall(unsigned int functionCode, void *data);

extern "C" unsigned int kernelVersion()
{	
	//unsigned int parameter
	void* d = nullptr;
	systemCall(0x0001, d);
	
}

extern "C" void helloworldtest()
{
	void* d = nullptr;
	systemCall(0x0002, d);
	return;
}

extern "C" void begin()
{
	//whatever
	main2();
}
