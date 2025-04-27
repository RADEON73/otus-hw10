#include "async.h"
#include "BulkProcessor.h"
#include <string>
#include <iostream>
#include "MultiThreadOutputter.h"
#include <thread>

namespace async {
	HANDLE connect(size_t packSize) {
		return new BulkProcessor(packSize);
	}

	void receive(HANDLE handle, const char* data, size_t size) {
		if (!handle || !data || size == 0)
			return;
		auto processor = static_cast<BulkProcessor*>(handle);
		std::string input(data, size);
		processor->parse(input);
	}

	void disconnect(async::HANDLE handle) {
		if (!handle)
			return;
		auto processor = static_cast<BulkProcessor*>(handle);
		processor->finalize();
		delete processor;
	}
	void flush(HANDLE handle)
	{
		auto processor = static_cast<BulkProcessor*>(handle);
		processor->finalize();
	}
}