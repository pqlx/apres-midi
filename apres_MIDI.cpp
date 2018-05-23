// apres_MIDI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Decoder.h"
#include "HeaderChunk.h"
#include "MIDIChunk.h"

#include <memory>

int main()
{
	auto d = Decoder();
	d.SetMIDIBuffer("C:\\Users\\david\\Downloads\\Fagning.mid");

	MIDI a = d.GetMIDI();

	std::cout << dynamic_cast<HeaderChunk*>(a.chunks[0])->tracks << std::endl;

	system("pause");
	
    return 0;
}

