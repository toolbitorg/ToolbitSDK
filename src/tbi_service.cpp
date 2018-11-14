#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <mutex>
#include "tbi_service.h"

std::mutex mtx;

void emptyEventHandler(tbiPacket pckt)
{
	;
}


TbiService::TbiService(TbiDevice *p) :
	resque(4)
{
	tdev = p;
	thAbort = false;
	th = new thread(&TbiService::worker, this);
	eventHandler = emptyEventHandler;
}

TbiService::~TbiService()
{
	thAbort = true;
	th->join();
	delete th;
}

bool TbiService::readAttribute(Attribute *att)
{
	tbiPacket pckt;

	pckt.dat[0] = PROTOCOL_VERSION + 4;  // The first byte is Version(7-6bit) + Length(5-0bit).
	pckt.dat[1] = OP_ATT_VALUE_GET;     // Operation Code
	pckt.dat[2] = (att->getAttid() & 0xFF00) >> 8;
	pckt.dat[3] = att->getAttid() & 0xFF;

	mtx.lock();
	if (tdev->isOpen()) {
		tdev->write(pckt.dat, 4);
	}
	else {
		return false;
	}
	mtx.unlock();

	pckt = resque.dequeue();
	if ((pckt.dat[0] & 0xC0) == PROTOCOL_VERSION  // Check received packet
		&& pckt.dat[1] == OP_ATT_VALUE_GET
		&& pckt.dat[2] == RC_OK)
	{
		int len = (pckt.dat[0] & 0x3F) - 3;
		return att->setValue(&pckt.dat[3], len);
	}
	return false;
}

bool TbiService::writeAttribute(Attribute att)
{
	tbiPacket pckt;
	uint8_t len = att.getValueLength();
	uint16_t id = att.getAttid();

	pckt.dat[0] = PROTOCOL_VERSION + 4 + len;  // The first byte is Version(7-6bit) + Length(5-0bit).
	pckt.dat[1] = OP_ATT_VALUE_SET;           // Operation Code
	pckt.dat[2] = (id & 0xFF00) >> 8;
	pckt.dat[3] = id & 0xFF;
	char *p = att.getValueStr();
	for (int i = 0; i < len; i++) {
		pckt.dat[4 + i] = *p++;                // Set value
	}
	mtx.lock();
	if (tdev->isOpen()) {
		tdev->write(pckt.dat, 4+len);
	}
	else {
		return false;
	}
	mtx.unlock();

	tbiPacket rcvp = resque.dequeue();
	if (rcvp.dat[0] == PROTOCOL_VERSION + 3  // Check received packet
		&& rcvp.dat[1] == OP_ATT_VALUE_SET
		&& rcvp.dat[2] == RC_OK)
		return true;

	return false;
}

void TbiService::bindEventHandler(std::function<void(tbiPacket pckt)> handler)
{
	eventHandler = handler;
}

struct thread_aborted {};
void TbiService::worker()
{
	tbiPacket pckt;

	while (1) {

		mtx.lock();
		if (tdev->isOpen()) {

			if (tdev->read(pckt.dat) > 0)
				if (pckt.dat[1] == EVT_NOTIFY) {
					eventHandler(pckt);
				}
				else {
					resque.enqueue(pckt);
				}
		}
		mtx.unlock();
		this_thread::sleep_for(chrono::milliseconds(1));
		if (thAbort)
			break;
	}
}
