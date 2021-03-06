#ifndef MATLABINTERFACE_H_DEFINED
#define MATLABINTERFACE_H_DEFINED

#include <ProcessorHeaders.h>
#include "MatlabInterfaceEditor.h"

#include "DataQueue.h"
#include "SocketThread.h"

#include <iostream>
#include <chrono>
#include <functional>

#include <algorithm>
#include <random>
#include <vector>

#define MAX_MESSAGE_SIZE 		2000

#define WRITE_BLOCK_LENGTH		1024	
#define DATA_BUFFER_NBLOCKS		300
#define EVENT_BUFFER_NEVENTS	512
#define SPIKE_BUFFER_NSPIKES	512

#define NIDAQ_BIT_VOLTS			0.001221f
#define NPX_BIT_VOLTS			0.195f
#define MAX_BUFFER_SIZE			40960
#define CHANNELS_PER_THREAD		384

template <typename Time = std::chrono::microseconds, typename Clock = std::chrono::high_resolution_clock>
struct func_timer
{
	template <typename F, typename... Args>
	static Time duration(F&& f, Args... args)
	{
		auto start = Clock::now();
		std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
		auto end = Clock::now();

		return std::chrono::duration_cast<Time>(end-start);
	} 
};

//
class MatlabInterface : public GenericProcessor
{
public:
	MatlabInterface();
	~MatlabInterface();

	bool hasEditor() const { return true; }
	AudioProcessorEditor* createEditor() override;

	void setSelectedChannel(int channel);
	int getSelectedChannel() { return selectedChannel; };

	int connect(String port, String host);

	void process(AudioSampleBuffer& buffer) override;

	void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition) override;

	void setParameter(int parameterIndex, float newValue) override;

	/** Saving custom settings to XML. */
	//void saveCustomParametersToXml(XmlElement* parentElement) override;

	/** Load custom settings from XML*/
	//void loadCustomParametersFromXml() override;

	void updateSettings() override;
	void start();
	void runTest();

private:

	int selectedChannel;

	ScopedPointer<DataQueue> dataQueue;
	ScopedPointer<SocketThread> socketThread;

	std::vector<float> matlab_data;

	int count;

};

#endif