#pragma once

#include "conceptions.h"
#include <functional>
#include <cstdint>

#ifdef CMPP_EXPORTS
#define CMPP_API __declspec(dllexport)
#else
#define CMPP_API __declspec(dllimport)
#endif

namespace comm {
	class Communicator;
}

using std::function;
using comm::Communicator;

namespace cmpp {

	class SPID;
	class MessageTask;
	struct DeliveryAcceptors;
	class Protocol20;

	typedef function<void(bool success, const char* failure)> CommonAction;
	typedef function<void(const char*)> ErrorReporter;
	typedef function<void(bool success, const char* failure, uint64_t)> SubmitAction;
	typedef function<void(const char*, const char*, const char*)> SMSAcceptor;
	typedef function<void(uint64_t, DeliverCode)> ReportAcceptor;

	class CMPP_API MessageGateway {
	public:
		MessageGateway(Protocol20* protocol, SMSAcceptor smsAcceptor, ReportAcceptor reportAcceptor, ErrorReporter errorReporter, float heartbeatInterval = 180.0);
		~MessageGateway();

		void open(const SPID *spid, CommonAction action);
		void close();
		void send(const MessageTask* task, SubmitAction action);

	private:
		// ��ֹ����MessageGateway����
		MessageGateway(const MessageGateway&);
		static unsigned long __stdcall heartbeatThread(void* self);
		void keepActive();
		void stopHeartbeat();
		void authenticate( const SPID* account, CommonAction& action );
		void registerDeliveriesHandler();
		void registerActiveHandler();

	private:
		Protocol20* protocol;
		Communicator* communicator;
		const char* spid;
		DeliveryAcceptors* acceptors;
		void* heartbeatThreadHandle;
		void* heartbeatStopEvent;
		float heartbeatInterval;
	};
}
